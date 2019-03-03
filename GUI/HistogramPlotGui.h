#ifndef HISTOGRAMPLOTGUI_H
#define HISTOGRAMPLOTGUI_H

#include "PlotDockWidget.h"
#include "Charts/HistogramPlot.h"

namespace Ui {
class HistogramPlotGui;
} // namespace Ui

/**
 * @brief Gui dock containing histogram.
 */
class HistogramPlotGui : public PlotDockWidget
{
    Q_OBJECT
public:
    explicit HistogramPlotGui(QWidget* parent = nullptr);

    ~HistogramPlotGui() override;

private:
    Q_DISABLE_COPY(HistogramPlotGui)

    Ui::HistogramPlotGui *ui;

    HistogramPlot histogramPlot_;

private slots:
    void dataChanged(PlotData plotData, Quantiles quantiles);
};

#endif // HISTOGRAMPLOTGUI_H
