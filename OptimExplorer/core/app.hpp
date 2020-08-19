#pragma once

#include "core/utils.hpp"
#include "widgets/metrics.hpp"
#include "widgets/train.hpp"
#include "widgets/widget.hpp"

DISABLE_WARNING_PUSH_ALL
#include <Mahi/Gui.hpp>
#include <Mahi/Util.hpp>
DISABLE_WARNING_POP

#include "torch/torch.hpp"

#include <memory>
#include <thread>
#include <vector>

namespace OptimExplorer
{

// mahi::gui::Application has public non virtual dtor, ok since OptimExplorer::~App does nothing.
DISABLE_WARNING_NON_VIRTUAL_DTOR

class App : public mahi::gui::Application
{
public:
  App() : mahi::gui::Application(1920, 1080, "Optimisation Explorer")
  {

    ImGui::StyleColorsClassic();

    ImGuiStyle* imStyle = &ImGui::GetStyle();
    imStyle->WindowPadding = ImVec2(8, 8);
    imStyle->FramePadding = ImVec2(3, 2);
    imStyle->ItemSpacing = ImVec2(4, 4);
    imStyle->ItemInnerSpacing = ImVec2(4, 4);
    imStyle->IndentSpacing = 20.0f;
    imStyle->ScrollbarSize = 15.0f;
    imStyle->GrabMinSize = 5.0f;

    imStyle->WindowRounding = 4.0f;
    imStyle->ChildRounding = 4.0f;
    imStyle->FrameRounding = 4.0f;
    imStyle->PopupRounding = 4.0f;
    imStyle->ScrollbarRounding = 10.0f;
    imStyle->GrabRounding = 2.0f;
    imStyle->TabRounding = 4.0f;

    // ================ Add Widgets ==================
    m_widgets.emplace_back(std::make_unique<MetricsWidget>());
    m_widgets.emplace_back(std::make_unique<TrainWidget>());

    // ================ Create Torch System ==================
  }

  void update() override;

private:
  TorchSystem ts;
  std::vector<std::unique_ptr<Widget>> m_widgets;
};
DISABLE_WARNING_POP

} // namespace OptimExplorer