
#pragma once
#include "core/utils.hpp"

DISABLE_WARNING_PUSH_ALL
#include <Mahi/Gui.hpp>
DISABLE_WARNING_POP

#include <utility>

namespace OptimExplorer
{

double random_range(double min, double max);

static const char* cmap_names[] =
    {"Default", "Dark", "Pastel", "Paired", "Viridis", "Plasma", "Hot", "Cool", "Pink", "Jet"};

// utility structure for realtime plots
class ScrollingData
{
public:
  ScrollingData();
  void add_point(double x, double y);
  void add_point(std::pair<double, double> point) { add_point(point.first, point.second); }
  void clear();
  int size() { return m_data.size(); }
  int offset() { return m_offset; }
  ImPlotPoint& first() { return m_data[0]; }

private:
  ImVector<ImPlotPoint> m_data;
  int m_max_size;
  int m_offset;
};

// utility structure for realtime plots
class RollingData
{
public:
  RollingData();
  void add_point(double x, double y);

private:
  double m_span;
  ImVector<ImPlotPoint> m_data;
};

} // namespace OptimExplorer