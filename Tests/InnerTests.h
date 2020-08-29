#pragma once

#include <memory>

#include <QObject>

class DatasetInner;
class QTableView;

/**
 * @brief Test for inner format functionalities.
 */
class InnerTests : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();

    void testDatasets_data();
    void testDatasets();

    void testPartialData();

    void cleanupTestCase();

private:
    void generateDumpData();

    void checkDatasetDefinition(
        const QString& fileName,
        const std::unique_ptr<DatasetInner>& dataset) const;

    void checkDatasetData(const QString& fileName,
                          const QTableView& view) const;

    /**
     * @brief check export for given dataset name.
     * @param fileName name of dataset.
     */
    void checkExport(QString fileName);

    /**
     * @brief compare definition files
     * @param original original definition file.
     * @param generated generated definition file.
     */
    void compareDefinitionFiles(QByteArray& original, QByteArray& generated);

    void addTestCases(const QString& testNamePrefix);

    const QString tempFilename_{"temp"};

    static const QVector<QString> testFileNames_;
};
