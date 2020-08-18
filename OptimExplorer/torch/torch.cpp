
#include "torch/torch.hpp"

#include "core/events.hpp"
#include "torch/networks.hpp"

#include <string>
#include <torch/torch.h>
#include <utility>

namespace OptimExplorer
{

void TorchSystem::train()
// void train()
{
  const std::string base_path = "./";
  const std::string data_path = base_path + "data/mnist";
  const std::string model_path = base_path + "models/net.pt";

  torch::Device device(torch::kCPU);
  printf("CUDA DEVICE COUNT: %zu\n", torch::cuda::device_count());

  if (torch::cuda::is_available())
  {
    printf("CUDA is available! Inference on GPU.\n");
    device = torch::Device(torch::kCUDA);
  }
  else
  {
    printf("CUDA not available! Inference on CPU.\n");
  }

  // Create a new Net.
  auto net = std::make_shared<networks::mnist::CNNNet>();
  net->to(device);

  size_t batch_size = 128;
  size_t epochs = 5;

  // Create a multi-threaded data loader for the MNIST dataset.
  auto train_dataset =
      torch::data::datasets::MNIST(data_path, torch::data::datasets::MNIST::Mode::kTrain);

  auto data_loader = torch::data::make_data_loader(
      train_dataset.map(torch::data::transforms::Normalize<>(0.1307, 0.3081))
          .map(torch::data::transforms::Stack<>()),
      batch_size);

  auto test_dataset =
      torch::data::datasets::MNIST(data_path, torch::data::datasets::MNIST::Mode::kTest);

  auto test_loader = torch::data::make_data_loader(
      test_dataset.map(torch::data::transforms::Normalize<>(0.1307, 0.3081))
          .map(torch::data::transforms::Stack<>()),
      batch_size);

  const size_t train_dataset_size = train_dataset.size().value();
  const size_t test_dataset_size = test_dataset.size().value();

  // torch::optim::SGD optimizer(net->parameters(), /*lr=*/0.01);
  torch::optim::Adam optimizer(net->parameters(), /*lr=*/0.0001);

  const size_t num_train_batches = train_dataset_size / batch_size;

  event_dispatcher.dispatch(Event(EventType::ClearMetrics));

  for (size_t epoch = 0; epoch < epochs; ++epoch)
  {
    size_t batch_index = 0;
    float sum_loss = 0;

    // Iterate the data loader to yield batches from the dataset.
    for (auto& batch: *data_loader)
    {
      auto data = batch.data.to(device);
      auto target = batch.target.to(device);

      // Reset gradients.
      optimizer.zero_grad();

      // Execute the model on the input data.
      torch::Tensor prediction = net->forward(data);

      // Compute a loss value to judge the prediction of our model.
      torch::Tensor loss = torch::nll_loss(prediction, target);
      sum_loss += loss.cpu().item<float>();

      auto pred_classes = prediction.argmax(1);
      int64_t correct = pred_classes.eq(target).sum().item<int64_t>();

      // Compute gradients of the loss w.r.t. the parameters of our model.
      loss.backward();
      // Update the parameters based on the calculated gradients.
      optimizer.step();

      // Dispatch Train Metrics
      event_dispatcher.dispatch(MetricEvent(
          EventType::TrainMetric,
          "loss",
          std::make_pair<double, double>(
              epoch + static_cast<double>(batch_index) / num_train_batches,
              loss.item<double>())));

      event_dispatcher.dispatch(MetricEvent(
          EventType::TrainMetric,
          "accuracy",
          std::make_pair<double, double>(
              epoch + static_cast<double>(batch_index) / num_train_batches,
              100 * static_cast<double>(correct) / batch_size)));

      // Output the loss and checkpoint every 100 batches.
      if (++batch_index % 100 == 0)
      {
        // printf("Batch: %zu.%zu | Loss: %.3f\n", epoch, batch_index, loss.item<float>());
        // Serialize your model periodically as a checkpoint.
        torch::save(net, model_path);
      }

      // TODO: do a valid pass here
    }

    net->eval();
    double test_loss = 0;
    int32_t correct = 0;

    for (const auto& batch: *test_loader)
    {
      auto data = batch.data.to(device);
      auto targets = batch.target.to(device);
      auto output = net->forward(data);
      test_loss += torch::nll_loss(output, targets).item<float>();
      auto pred = output.argmax(1);
      correct += pred.eq(targets).sum().template item<int64_t>();
    }

    test_loss /= test_dataset_size;
    printf(
        "Epoch: %zu | Average Loss: %.3f | Test: Average loss: %.3f | Accuracy: %.1f%%\n",
        epoch,
        sum_loss / batch_index,
        test_loss,
        100 * static_cast<double>(correct) / test_dataset_size);
  }
}

}