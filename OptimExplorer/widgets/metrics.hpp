#pragma once

#include "core/events.hpp"
#include "core/utils.hpp"
#include "widgets/utils.hpp"
#include "widgets/widget.hpp"

DISABLE_WARNING_PUSH_ALL
#include <Mahi/Gui.hpp>
DISABLE_WARNING_POP

#include <iostream>
#include <utility>
#include <vector>

namespace OptimExplorer
{

class MetricsWidget : public Widget
{
public:
  MetricsWidget() : Widget("Metrics")
  {

    event_dispatcher.appendListener(EventType::ClearMetrics, [&](const Event& ev) {
      m_valid_loss.clear();
      m_train_loss.clear();
      m_valid_acc.clear();
      m_train_acc.clear();
      m_scroll_train_loss.clear();
    });

    event_dispatcher.appendListener(EventType::TrainMetric, [&](const Event& ev) {
      auto me = dynamic_cast<const MetricEvent&>(ev);
      if (me.get_name() == "loss")
      {
        m_train_loss.emplace_back(me.get_metric());
        m_scroll_train_loss.add_point(me.get_metric());
      }
      else if (me.get_name() == "accuracy")
      {
        m_train_acc.emplace_back(me.get_metric());
      }
    });

    event_dispatcher.appendListener(EventType::ValidMetric, [&](const Event& ev) {
      auto me = dynamic_cast<const MetricEvent&>(ev);
      if (me.get_name() == "loss")
      {
        m_valid_loss.emplace_back(me.get_metric());
      }
      else if (me.get_name() == "accuracy")
      {
        m_valid_acc.emplace_back(me.get_metric());
      }
    });
  }

  void render() override;

private:
  void render_loss();
  void render_accuracy();
  void render_confusion_matrix();

  std::vector<std::pair<double, double>> m_valid_loss, m_train_loss;
  std::vector<std::pair<double, double>> m_valid_acc, m_train_acc;

  bool m_scrolling_paused = false;
  float m_scroll_history = 1.0f;
  ScrollingData m_scroll_train_loss;
};

} // namespace OptimExplorer