#include "metrics.hpp"

#include "core/utils.hpp"
#include "widgets/widget.hpp"

DISABLE_WARNING_PUSH_ALL
#include <Mahi/Gui.hpp>
#include <Mahi/Util.hpp>
DISABLE_WARNING_POP

#include <algorithm>
#include <vector>

namespace OptimExplorer
{

void MetricsWidget::render()
{

  if (ImGui::Button("Print Metrics"))
  {
    mahi::util::print("Metrics lol");
  }

  render_loss();
  render_accuracy();
  render_confusion_matrix();
}

void MetricsWidget::render_accuracy()
{
  if (ImGui::CollapsingHeader("Accuracy"))
  {

    ImPlot::SetNextPlotLimitsY(0, 100, ImGuiCond_Always);
    ImPlot::SetNextPlotLimitsX(0, 10, ImGuiCond_FirstUseEver);

    if (ImPlot::BeginPlot(
            "##Accuracy",
            "Epoch",
            NULL,
            ImVec2(-1, 200),
            ImPlotFlags_Default,
            ImPlotAxisFlags_Default | ImPlotAxisFlags_LockMin,
            ImPlotAxisFlags_Default | ImPlotAxisFlags_LockMin))
    {
      ImPlot::PlotLine(
          "Valid Accuracy",
          &m_valid_acc[0].first,
          &m_valid_acc[0].second,
          m_valid_acc.size(),
          0,
          2 * sizeof(double));

      ImPlot::PlotLine(
          "Train Accuracy",
          &m_train_acc[0].first,
          &m_train_acc[0].second,
          m_train_acc.size(),
          0,
          2 * sizeof(double));
      ImPlot::EndPlot();
    }
  }
}

void MetricsWidget::render_loss()
{
  if (ImGui::CollapsingHeader("Loss"))
  {

    ImPlot::SetNextPlotLimitsY(0, 3, ImGuiCond_FirstUseEver);
    ImPlot::SetNextPlotLimitsX(0, 10, ImGuiCond_FirstUseEver);

    if (ImPlot::BeginPlot(
            "##Loss",
            "Epoch",
            NULL,
            ImVec2(-1, 200),
            ImPlotFlags_Default,
            ImPlotAxisFlags_Default | ImPlotAxisFlags_LockMin,
            ImPlotAxisFlags_Default | ImPlotAxisFlags_LockMin))
    {
      ImPlot::PlotLine(
          "Valid Loss",
          &m_valid_loss[0].first,
          &m_valid_loss[0].second,
          m_valid_loss.size(),
          0,
          2 * sizeof(double));

      ImPlot::PlotLine(
          "Train Loss",
          &m_train_loss[0].first,
          &m_train_loss[0].second,
          m_train_loss.size(),
          0,
          2 * sizeof(double));
      ImPlot::EndPlot();
    }

    ImGui::NewLine();

    ImGui::SliderFloat("History", &m_scroll_history, 0, 5, "%.1f Epochs");

    double t = 0;
    if (!m_train_loss.empty())
    {
      t = m_train_loss.back().first;
    }

    ImPlot::SetNextPlotLimitsX(
        t - m_scroll_history,
        t,
        m_scrolling_paused ? ImGuiCond_Once : ImGuiCond_Always);

    if (ImPlot::BeginPlot(
            "##Scrolling",
            NULL,
            NULL,
            ImVec2(-1, 200),
            ImPlotFlags_Default,
            ImPlotAxisFlags_Default,
            ImPlotAxisFlags_Default | ImPlotAxisFlags_LockMin))
    {
      if (m_scroll_train_loss.size() > 0)
      {
        ImPlot::PlotLine(
            "Train Loss",
            &m_scroll_train_loss.first().x,
            &m_scroll_train_loss.first().y,
            m_scroll_train_loss.size(),
            m_scroll_train_loss.offset(),
            2 * sizeof(double));
      }
      ImPlot::EndPlot();
    }
  }
}

void MetricsWidget::render_confusion_matrix()
{

  if (ImGui::CollapsingHeader("Confusion Matrix"))
  {
    static float values1[7][7] = {
        {0.8f, 2.4f, 2.5f, 3.9f, 0.0f, 4.0f, 0.0f},
        {2.4f, 0.0f, 4.0f, 1.0f, 2.7f, 0.0f, 0.0f},
        {1.1f, 2.4f, 0.8f, 4.3f, 1.9f, 4.4f, 0.0f},
        {0.6f, 0.0f, 0.3f, 0.0f, 3.1f, 0.0f, 0.0f},
        {0.7f, 1.7f, 0.6f, 2.6f, 2.2f, 6.2f, 0.0f},
        {1.3f, 1.2f, 0.0f, 0.0f, 0.0f, 3.2f, 5.1f},
        {0.1f, 2.0f, 0.0f, 1.4f, 0.0f, 1.9f, 6.3f}};
    static float scale_min = 0;
    static float scale_max = 6.3f;
    static float values2[100 * 100];
    for (int i = 0; i < 100 * 100; ++i)
    {
      values2[i] = random_range(0, 1);
    }
    static ImPlotColormap map = ImPlotColormap_Viridis;
    if (ImGui::Button("Change Colormap", ImVec2(225, 0)))
      map = (map + 1) % ImPlotColormap_COUNT;
    ImPlot::SetColormap(map);
    ImGui::SameLine();
    ImGui::LabelText("##Colormap Index", "%s", cmap_names[map]);
    ImGui::SetNextItemWidth(225);
    // ImGui::DragFloat("Max", &scale_max, 0.01f, 0.1f, 20);
    static ImPlotAxisFlags axes_flags =
        ImPlotAxisFlags_LockMin | ImPlotAxisFlags_LockMax | ImPlotAxisFlags_TickLabels;
    static const char* xlabels[] = {"C1", "C2", "C3", "C4", "C5", "C6", "C7"};
    static const char* ylabels[] = {"R1", "R2", "R3", "R4", "R5", "R6", "R7"};

    ImPlot::SetNextPlotTicksX(0 + 1.0 / 14.0, 1 - 1.0 / 14.0, 7, xlabels);
    ImPlot::SetNextPlotTicksY(1 - 1.0 / 14.0, 0 + 1.0 / 14.0, 7, ylabels);

    if (ImPlot::BeginPlot("##Heatmap1", NULL, NULL, ImVec2(225, 225), 0, axes_flags, axes_flags))
    {
      ImPlot::PlotHeatmap("heat", values1[0], 7, 7, scale_min, scale_max);
      ImPlot::EndPlot();
    }
    ImGui::SameLine();
    ImPlot::ShowColormapScale(scale_min, scale_max, 225);
    ImPlot::SetColormap(ImPlotColormap_Default);
  }
}

} // namespace OptimExplorer