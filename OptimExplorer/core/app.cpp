#include "core/app.hpp"

#include "core/utils.hpp"

DISABLE_WARNING_PUSH_ALL
#include <Mahi/Gui.hpp>
DISABLE_WARNING_POP

namespace OptimExplorer
{

void App::update()
{

  ImGuiViewport* main_viewport = ImGui::GetMainViewport();

  ImGui::SetNextWindowPos(main_viewport->GetWorkPos(), ImGuiCond_FirstUseEver);
  ImGui::SetNextWindowSize(main_viewport->GetWorkSize(), ImGuiCond_FirstUseEver);

  constexpr ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus
      | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoDocking
      | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoCollapse;

  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::Begin("Root", NULL, window_flags);

  ImGui::DockSpace(
      ImGui::GetID("Root_DockSpace"),
      ImVec2(0.0f, 0.0f),
      ImGuiDockNodeFlags_PassthruCentralNode);

  ImGui::End();
  ImGui::PopStyleVar(3);

  // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

  for (auto& widget: m_widgets)
  {
    widget->update();
  }
}

} // namespace OptimExplorer