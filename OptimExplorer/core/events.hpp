#pragma once

#include "core/utils.hpp"

DISABLE_WARNING_PUSH_ALL
#include "eventpp/eventdispatcher.h"

#include <Mahi/Gui.hpp>
DISABLE_WARNING_POP

#include <string>
#include <utility>

namespace OptimExplorer
{

enum EventType
{
  TrainMetric = 0,
  ValidMetric,
  TestMetric,
  ClearMetrics,

  // TorchSystem
  TrainModel,
  TestModel,
  DoEpoch,
  SetHparam,
};

class Event
{
public:
  Event() = delete;
  Event(EventType et) : m_et(et) {}

  virtual ~Event(){};

  const EventType& get_type() const { return m_et; }

private:
  const EventType m_et;
};

class MetricEvent : public Event
{
public:
  MetricEvent(EventType et, std::string name, std::pair<double, double> metric)
      : Event(et), m_name(name), m_metric(metric)
  {}

  const std::string& get_name() const { return m_name; }
  const std::pair<double, double>& get_metric() const { return m_metric; }

private:
  const std::string m_name;
  const std::pair<double, double> m_metric;
};

struct EventPolicies
{
  static EventType getEvent(const Event& ev) { return ev.get_type(); }
};

extern eventpp::EventDispatcher<EventType, void(const Event&), EventPolicies> event_dispatcher;

} // namespace OptimExplorer