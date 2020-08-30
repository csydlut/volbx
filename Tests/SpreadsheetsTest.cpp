#include "SpreadsheetsTest.h"

#include <QApplication>
#include <QTableView>
#include <QtTest/QtTest>

#include <Common/FileUtilities.h>
#include <Datasets/DatasetOds.h>
#include <Datasets/DatasetXlsx.h>
#include <ModelsAndViews/FilteringProxyModel.h>
#include <ModelsAndViews/TableModel.h>

#include "Common.h"
#include "DatasetCommon.h"

const QVector<QString> SpreadsheetsTest::testFileNames_{"excel",
                                                        "HistVsNormal",
                                                        "import1",
                                                        "import2",
                                                        "import3",
                                                        "smallDataSet",
                                                        "test2",
                                                        "testAccounts",
                                                        "testDataWithPlot",
                                                        "testDataWithPlot",
                                                        "test",
                                                        "testFileData",
                                                        "test01",
                                                        "test03",
                                                        "test04"};

void SpreadsheetsTest::initTestCase()
{
    // generateExpectedData();
}

void SpreadsheetsTest::testDefinition_data()
{
    addTestCasesForFileNames(testFileNames_);
}

void SpreadsheetsTest::testDefinition()
{
    QFETCH(QString, fileName);
    DatasetCommon::checkDefinition(fileName, Common::getSpreadsheetsDir());
}

void SpreadsheetsTest::testData_data()
{
    addTestCasesForFileNames(testFileNames_);
}

void SpreadsheetsTest::testData()
{
    QFETCH(QString, fileName);
    DatasetCommon::checkData(fileName, Common::getSpreadsheetsDir());
}

void SpreadsheetsTest::testDamagedFiles_data()
{
    addTestCasesForFileNames({"damaged"});
}

void SpreadsheetsTest::testDamagedFiles()
{
    QFETCH(QString, fileName);

    std::unique_ptr<Dataset> dataset{DatasetCommon::createDataset(
        fileName, Common::getSpreadsheetsDir() + fileName)};
    QVERIFY(!dataset->initialize());
}

void SpreadsheetsTest::compareExpectedDefinitionsOfOdsAndXlsx_data()
{
    addTestCaseForOdsAndXlsxComparison("Compare definition dumps");
}

void SpreadsheetsTest::compareExpectedDefinitionsOfOdsAndXlsx()
{
    compareOdsAndXlsxExpectedData(Common::getDefinitionDumpSuffix());
}

void SpreadsheetsTest::compareExpectedTsvDumpsOfOdsAndXlsx_data()
{
    addTestCaseForOdsAndXlsxComparison("Compare tsv dumps");
}

void SpreadsheetsTest::compareExpectedTsvDumpsOfOdsAndXlsx()
{
    compareOdsAndXlsxExpectedData(Common::getDataTsvDumpSuffix());
}

void SpreadsheetsTest::addTestCaseForOdsAndXlsxComparison(
    const QString& testNamePrefix)
{
    QTest::addColumn<QString>("fileName");

    for (const auto& fileName : testFileNames_)
    {
        QString testName{testNamePrefix + " for " + fileName};
        QTest::newRow(testName.toStdString().c_str()) << fileName;
    }
}

void SpreadsheetsTest::compareOdsAndXlsxExpectedData(const QString& fileSuffix)
{
    QFETCH(QString, fileName);

    QString filePath{Common::getSpreadsheetsDir() + fileName};
    auto [xlsxLoaded, xlsxDump] =
        FileUtilities::loadFile(filePath + ".xlsx" + fileSuffix);
    QVERIFY(xlsxLoaded);

    auto [odsLoaded, odsDump] =
        FileUtilities::loadFile(filePath + ".ods" + fileSuffix);
    QVERIFY(odsLoaded);

    QVector<QStringRef> xlsxLines{xlsxDump.splitRef('\n')};
    QVector<QStringRef> odsLines{odsDump.splitRef('\n')};
    QCOMPARE(xlsxLines.size(), odsLines.size());
    for (int i = 0; i < xlsxLines.size(); ++i)
        if (xlsxLines[i] != odsLines[i])
        {
            QString msg{"Difference in line " + QString::number(i + 1) +
                        "\nXlsx: " + xlsxLines[i] + "\nOds : " + odsLines[i]};
            QFAIL(msg.toStdString().c_str());
        }
}

void SpreadsheetsTest::addTestCasesForFileNames(
    const QVector<QString>& fileNames)
{
    QTest::addColumn<QString>("fileName");

    for (const auto& fileName : fileNames)
    {
        QString testFileName{fileName + ".xlsx"};
        QString testName{"Test " + testFileName};
        QTest::newRow(testName.toStdString().c_str()) << testFileName;
        testFileName = fileName + ".ods";
        testName = "Test " + testFileName;
        QTest::newRow(testName.toStdString().c_str()) << testFileName;
    }
}

void SpreadsheetsTest::generateExpectedData()
{
    QString generatedFilesDir{QApplication::applicationDirPath() +
                              "/generatedSpreadsheetsTestData/"};
    QDir().mkdir(generatedFilesDir);
    for (const auto& fileName : testFileNames_)
    {
        generateExpectedDataForFile(fileName + ".xlsx", generatedFilesDir);
        generateExpectedDataForFile(fileName + ".ods", generatedFilesDir);
    }
}

void SpreadsheetsTest::saveExpectedDefinition(
    const std::unique_ptr<Dataset>& dataset, const QString& filePath)
{
    QByteArray dumpedDefinition{dataset->definitionToXml(dataset->rowCount())};
    Common::saveFile(filePath + Common::getDefinitionDumpSuffix(),
                     dumpedDefinition);
}

void SpreadsheetsTest::saveExpectedTsv(const QTableView& view,
                                       const QString& filePath)
{
    QString tsvData{DatasetCommon::getExportedTsv(view)};
    Common::saveFile(filePath + Common::getDataTsvDumpSuffix(), tsvData);
}

void SpreadsheetsTest::generateExpectedDataForFile(const QString& fileName,
                                                   const QString& dir)
{
    std::unique_ptr<Dataset> dataset{DatasetCommon::createDataset(
        fileName, Common::getSpreadsheetsDir() + fileName)};
    dataset->initialize();

    QString filePath{dir + fileName};
    saveExpectedDefinition(dataset, filePath);

    DatasetCommon::activateAllDatasetColumns(*dataset);
    dataset->loadData();

    TableModel model(std::move(dataset));
    FilteringProxyModel proxyModel;
    proxyModel.setSourceModel(&model);
    QTableView view;
    view.setModel(&proxyModel);

    saveExpectedTsv(view, filePath);
}
