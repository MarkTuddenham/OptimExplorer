
#include "torch/torch.hpp"

#include "core/events.hpp"
#include "torch/networks.hpp"

#include <memory>
#include <mutex>
#include <string>
#include <torch/torch.h>
#include <utility>

namespace OptimExplorer
{

TorchSystem::TorchSystem()
{

  // TODO: do we need a way to cancel these events?
  // e.g. if we set to train for 200 epochs then decide we want to change the hparams
  // we'll want to cancel the training -> keep track of threads & then cancel all button?

  event_dispatcher.appendListener(EventType::TrainModel, [&](const Event& ev) {
    std::thread t(&TorchSystem::train, this);
    t.detach();
  });

  event_dispatcher.appendListener(EventType::TestModel, [&](const Event& ev) {
    std::thread t(&TorchSystem::do_test, this);
    t.detach();
  });

  // TODO need to add m_optimiser
  // event_dispatcher.appendListener(EventType::DoEpoch, [&](const Event& ev) {
  //   std::thread t(&TorchSystem::do_epoch, this);
  //   t.detach();
  // });
}

void TorchSystem::set_model()
{
  const std::lock_guard<std::recursive_mutex> lock(m_mutex);

  m_model = torch::nn::AnyModule(std::make_shared<networks::mnist::CNNNet>());
  m_epochs_trained = 0;
  event_dispatcher.dispatch(Event(EventType::ClearMetrics));
}

void TorchSystem::do_epoch(torch::optim::Optimizer& optim)
{

  const std::lock_guard<std::recursive_mutex> lock(m_mutex);

  const size_t train_dataset_size = m_train_dataset.size().value();
  const double num_train_batches = train_dataset_size / m_hparams.batch_size;

  // TODO: change if the dataset changes
  static auto data_loader = torch::data::make_data_loader(
      m_train_dataset.map(torch::data::transforms::Normalize<>(0.1307, 0.3081))
          .map(torch::data::transforms::Stack<>()),
      m_hparams.batch_size);

  auto model = m_model.ptr();
  model->train();

  size_t batch_index = 0;
  float sum_loss = 0;
  // Iterate the data loader to yield batches from the dataset.
  for (auto& batch: *data_loader)
  {

    auto data = batch.data.to(m_device);
    auto target = batch.target.to(m_device);

    // Reset gradients.
    optim.zero_grad();

    // Execute the model on the input data.
    torch::Tensor prediction = m_model.forward<torch::Tensor>(data);

    // Compute a loss value to judge the prediction of our model.
    torch::Tensor loss = torch::nll_loss(prediction, target);
    sum_loss += loss.cpu().item<float>();

    auto pred_classes = prediction.argmax(1);
    int64_t correct = pred_classes.eq(target).sum().item<int64_t>();

    // Compute gradients of the loss w.r.t. the parameters of our model.
    loss.backward();
    // Update the parameters based on the calculated gradients.
    optim.step();

    // Dispatch Train Metrics
    event_dispatcher.dispatch(MetricEvent(
        EventType::TrainMetric,
        "loss",
        std::make_pair<>(m_epochs_trained + batch_index / num_train_batches, loss.item<double>())));

    // need since last batch is not a full batch.
    size_t num_items_in_batch = batch.data.sizes()[0];
    event_dispatcher.dispatch(MetricEvent(
        EventType::TrainMetric,
        "accuracy",
        std::make_pair<>(
            m_epochs_trained + batch_index / num_train_batches,
            100 * static_cast<double>(correct) / num_items_in_batch)));

    if (++batch_index % 100 == 0)
    {
      save_model("net");
    }
  }

  ++m_epochs_trained;
}

void TorchSystem::do_test()
{

  const std::lock_guard<std::recursive_mutex> lock(m_mutex);

  // TODO: change if the dataset changes
  static auto test_loader = torch::data::make_data_loader(
      m_test_dataset.map(torch::data::transforms::Normalize<>(0.1307, 0.3081))
          .map(torch::data::transforms::Stack<>()),
      m_hparams.batch_size);

  const size_t test_dataset_size = m_test_dataset.size().value();

  m_model.ptr()->eval();
  double test_loss = 0;
  int32_t correct = 0;

  for (const auto& batch: *test_loader)
  {
    auto data = batch.data.to(m_device);
    auto targets = batch.target.to(m_device);
    auto output = m_model.forward<torch::Tensor>(data);
    test_loss += torch::nll_loss(output, targets).item<double>();
    auto pred = output.argmax(1);
    correct += pred.eq(targets).sum().template item<int64_t>();
  }

  test_loss /= test_dataset_size;

  event_dispatcher.dispatch(MetricEvent(
      EventType::TestMetric,
      "loss",
      std::make_pair<>(static_cast<double>(m_epochs_trained), test_loss)));

  event_dispatcher.dispatch(MetricEvent(
      EventType::TestMetric,
      "accuracy",
      std::make_pair<>(
          static_cast<double>(m_epochs_trained),
          100 * static_cast<double>(correct) / test_dataset_size)));

  printf(
      "Epoch: %zu | Test loss: %.3f | Accuracy: %.1f%%\n",
      m_epochs_trained,
      test_loss,
      100 * static_cast<double>(correct) / test_dataset_size);
}

// TODO: this is just run do_epoch n times, do we need a func for this,
//  or just put it in the thread lambda?
void TorchSystem::train()
{

  const std::lock_guard<std::recursive_mutex> lock(m_mutex);

  if (m_model.is_empty())
  {
    set_model();
    m_model.ptr()->to(m_device);
  }

  // torch::optim::SGD optimizer(net->parameters(), /*lr=*/0.01);
  torch::optim::Adam optimizer(m_model.ptr()->parameters(), /*lr=*/0.001);

  const size_t target_epochs = m_epochs_trained + m_hparams.epochs;
  for (size_t epoch = m_epochs_trained; epoch < target_epochs; ++epoch)
  {
    do_epoch(optimizer);
    // TODO: do a valid pass here
  }
}

} // namespace OptimExplorer