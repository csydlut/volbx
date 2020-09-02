#pragma once

#include "ImportTab.h"

namespace Ui
{
class SpreadsheetsImportTab;
}  // namespace Ui

class DatasetSpreadsheet;

class SpreadsheetsImportTab : public ImportTab
{
    Q_OBJECT
public:
    explicit SpreadsheetsImportTab(QWidget* parent = nullptr);

    ~SpreadsheetsImportTab() override;

private Q_SLOTS:
    void openFileButtonClicked();

private:
    void analyzeFile(std::unique_ptr<DatasetSpreadsheet>& dataset);

    Ui::SpreadsheetsImportTab* ui;
};
