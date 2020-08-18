#pragma once

#include "core/events.hpp"
#include "torch/networks.hpp"

#include <string>
#include <thread>
#include <torch/torch.h>
#include <utility>

namespace OptimExplorer
{

class TorchSystem
{
public:
  TorchSystem()
  {
    event_dispatcher.appendListener(EventType::TrainModel, [&](const Event& ev) {
      std::thread t(&TorchSystem::train, this);
      t.detach();
    });

    event_dispatcher.appendListener(EventType::TestModel, [&](const Event& ev) {
      // std::thread t(test);
      // t.detach();
      printf("Test the model");
    });
  }

private:
  void train();
};

} // namespace OptimExplorer