#include "core/events.hpp"

namespace OptimExplorer
{

eventpp::EventDispatcher<EventType, void(const Event&), EventPolicies> event_dispatcher;

}