/// Copyright © 2012-2022 Forschungszentrum Jülich GmbH
/// SPDX-License-Identifier: LGPL-3.0-or-later
#include "Stage.hpp"

#include "ConvexPolygon.hpp"
#include "GenericAgent.hpp"
#include "Point.hpp"
#include "Simulation.hpp"

////////////////////////////////////////////////////////////////////////////////
/// Waypoint
////////////////////////////////////////////////////////////////////////////////
Waypoint::Waypoint(Point position_, double distance_) : position(position_), distance(distance_)
{
}

bool Waypoint::IsCompleted(const GenericAgent& agent) const
{
    const auto actual_distance = (agent.pos - position).Norm();
    return actual_distance <= distance;
}

Point Waypoint::Target() const
{
    return position;
}

////////////////////////////////////////////////////////////////////////////////
/// Exit
////////////////////////////////////////////////////////////////////////////////
Exit::Exit(ConvexPolygon&& area_, std::vector<GenericAgent::ID>& toRemove_)
    : area(std::move(area_)), toRemove(toRemove_)
{
}

bool Exit::IsCompleted(const GenericAgent& agent) const
{
    const bool hasReachedExit = area.IsInside(agent.pos);
    if(hasReachedExit) {
        toRemove.push_back(agent.id);
    }
    return hasReachedExit;
}

Point Exit::Target() const
{
    return area.Centroid();
}
