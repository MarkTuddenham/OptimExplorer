#pragma once
#include "core/utils.hpp"

DISABLE_WARNING_PUSH_ALL
#include <Mahi/Gui.hpp>
DISABLE_WARNING_POP

namespace OptimExplorer
{

class Widget
{
public:
  Widget(const char* name) : m_name(name) {}
  Widget() = delete;
  virtual ~Widget() = default;

  void update()
  {
    ImGui::Begin(m_name);
    render();
    ImGui::End();
  }
  virtual void render() = 0;

protected:
  const char* m_name;
};

} // namespace OptimExplorer