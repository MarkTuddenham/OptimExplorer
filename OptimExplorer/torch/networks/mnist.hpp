#pragma once

#include <torch/torch.h>

namespace OptimExplorer::networks
{

namespace mnist
{
struct Net : torch::nn::Module
{
  Net()
  {
    fc1 = register_module("fc1", torch::nn::Linear(784, 64));
    fc2 = register_module("fc2", torch::nn::Linear(64, 32));
    fc3 = register_module("fc3", torch::nn::Linear(32, 10));
  }

  torch::Tensor forward(torch::Tensor x)
  {
    x = torch::relu(fc1->forward(x.reshape({x.size(0), 784})));
    x = torch::dropout(x, /*p=*/0.5, /*train=*/is_training());
    x = torch::relu(fc2->forward(x));
    x = torch::log_softmax(fc3->forward(x), /*dim=*/1);
    return x;
  }

  torch::nn::Linear fc1{nullptr}, fc2{nullptr}, fc3{nullptr};
};

struct CNNNet : torch::nn::Module
{
  CNNNet()
      : fc1(4 * 4 * 20, 10),
        cv1(torch::nn::Conv2dOptions(1, 10, 5)),
        cv2(torch::nn::Conv2dOptions(10, 20, 5))

  {
    fc1 = register_module("fc1", fc1);
    cv1 = register_module("cv1", cv1);
    cv2 = register_module("cv2", cv2);
  }

  torch::Tensor forward(torch::Tensor x)
  {
    x = torch::relu(torch::max_pool2d(cv1->forward(x), 2));
    x = torch::relu(torch::max_pool2d(cv2->forward(x), 2));
    x = x.view({-1, 4 * 4 * 20});
    x = torch::dropout(x, /*p=*/0.5, /*training=*/is_training());
    x = torch::relu(fc1->forward(x));
    x = torch::log_softmax(x, /*dim=*/1);
    return x;
  }

  torch::nn::Linear fc1;
  torch::nn::Conv2d cv1, cv2;
};
} // namespace mnist

} // namespace OptimExplorer::networks