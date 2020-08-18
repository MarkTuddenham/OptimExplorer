#pragma once

#include "widgets/widget.hpp"

namespace OptimExplorer
{

class TrainWidget : public Widget
{
public:
  TrainWidget() : Widget("Train") {}

  void render() override;

private:
  float m_lr = 1e-3;
  float m_wd = 5e-4;
  int m_epochs = 10;
};

} // namespace OptimExplorer