#include "train.hpp"

#include "core/events.hpp"
#include "core/utils.hpp"

DISABLE_WARNING_PUSH_ALL
#include <Mahi/Gui.hpp>
DISABLE_WARNING_POP

namespace OptimExplorer
{

void TrainWidget::render()
{

  ImGui::Text("Hyperparameters");
  ImGui::Separator();
  ImGui::NewLine();
  ImGui::SliderFloat("Learning Rate", &m_lr, 0, 1, "%.2e", 6);
  ImGui::SliderFloat("Weight Decay", &m_wd, 0, 1, "%.2e", 6);
  ImGui::SliderInt("Epochs", &m_epochs, 0, 250, "%d");

  ImGui::Separator();
  ImGui::NewLine();

  if (ImGui::Button("Train"))
  {
    // TODO: add hparams to the event
    event_dispatcher.dispatch(Event(EventType::TrainModel));
  }

  ImGui::SameLine();
  if (ImGui::Button("Test"))
  {
    event_dispatcher.dispatch(Event(EventType::TestModel));
  }
}

} // namespace OptimExplorer