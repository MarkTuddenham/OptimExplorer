#include "utils.hpp"

#include "core/utils.hpp"

DISABLE_WARNING_PUSH_ALL
#include <Mahi/Gui.hpp>
DISABLE_WARNING_POP

#include <utility>

namespace OptimExplorer
{

double random_range(double min, double max)
{
  double scale = rand() / (double)RAND_MAX;
  return min + scale * (max - min);
}

ScrollingData::ScrollingData()
{
  m_max_size = 2000;
  m_offset = 0;
  m_data.reserve(m_max_size);
}

void ScrollingData::add_point(double x, double y)
{
  if (m_data.size() < m_max_size)
    m_data.push_back(ImPlotPoint(x, y));
  else
  {
    m_data[m_offset] = ImPlotPoint(x, y);
    m_offset = (m_offset + 1) % m_max_size;
  }
}

void ScrollingData::clear()
{
  if (m_data.size() > 0)
  {
    m_data.shrink(0);
    m_offset = 0;
  }
}

RollingData::RollingData()
{
  m_span = 10.0f;
  m_data.reserve(2000);
}

void RollingData::add_point(double x, double y)
{
  double xmod = fmod(x, m_span);
  if (!m_data.empty() && xmod < m_data.back().x)
    m_data.shrink(0);
  m_data.push_back(ImPlotPoint(xmod, y));
}

} // namespace OptimExplorer