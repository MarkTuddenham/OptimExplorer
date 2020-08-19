#pragma once

#include "torch/networks.hpp"

#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <torch/torch.h>

namespace OptimExplorer
{

struct Hparams
{
  size_t batch_size = 256;
  size_t epochs = 5;
};

class TorchSystem
{
public:
  TorchSystem();

private:
  void train();
  void set_model();
  void do_epoch(torch::optim::Optimizer& optim);
  void do_test();

  void attempt_to_use_cuda()
  {
    if (torch::cuda::is_available())
    {
      printf("CUDA DEVICE COUNT: %zu\n", torch::cuda::device_count());
      m_device = torch::Device(torch::kCUDA);
    }
  }
  void use_cpu() { m_device = torch::Device(torch::kCPU); }
  void set_hparams(const Hparams& hp) { m_hparams = hp; }
  void save_model(const std::string& name) const
  {
    torch::save(m_model.ptr(), m_model_path + name + ".pt");
  }

  std::string m_data_path = "./data/mnist";
  std::string m_model_path = "./models/";

  torch::Device m_device = torch::Device(torch::kCPU);
  torch::nn::AnyModule m_model;
  Hparams m_hparams;
  size_t m_epochs_trained = 0;

  // TODO: change to abstract DataSet
  torch::data::datasets::MNIST m_train_dataset =
      torch::data::datasets::MNIST(m_data_path, torch::data::datasets::MNIST::Mode::kTrain);
  torch::data::datasets::MNIST m_test_dataset =
      torch::data::datasets::MNIST(m_data_path, torch::data::datasets::MNIST::Mode::kTest);

  // Mutexes
  // We might be able to use shared mutexes & diff mutex for model/hparam/dataset
  // but lets keep it simple for now.
  // TODO: not using recursive mutex will allow other funcs to run in between each epoch of train
  std::recursive_mutex m_mutex;
};

} // namespace OptimExplorer