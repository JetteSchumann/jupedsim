// SPDX-License-Identifier: LGPL-3.0-or-later

#include "Point.hpp"
#include "jupedsim/jupedsim.h"
#include <tuple>

namespace jupedsim::detail
{
Point intoPoint(JPS_Point p);

JPS_Point intoJPS_Point(Point p);

JPS_Point intoJPS_Point(std::tuple<double, double> p);

std::tuple<double, double> intoTuple(JPS_Point p);
} // namespace jupedsim::detail
