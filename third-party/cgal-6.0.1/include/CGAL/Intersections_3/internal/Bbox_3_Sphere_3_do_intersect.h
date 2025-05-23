// Copyright (c) 2008  INRIA Sophia-Antipolis (France), ETH Zurich (Switzerland).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Intersections_3/include/CGAL/Intersections_3/internal/Bbox_3_Sphere_3_do_intersect.h $
// $Id: include/CGAL/Intersections_3/internal/Bbox_3_Sphere_3_do_intersect.h 50cfbde3b84 $
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Camille Wormser, Jane Tournois, Pierre Alliez


#ifndef CGAL_INTERNAL_INTERSECTIONS_3_BBOX_3_SPHERE_3_DO_INTERSECT_H
#define CGAL_INTERNAL_INTERSECTIONS_3_BBOX_3_SPHERE_3_DO_INTERSECT_H

#include <CGAL/Intersections_3/internal/Iso_cuboid_3_Sphere_3_do_intersect.h>

#include <CGAL/Bbox_3.h>

namespace CGAL {
namespace Intersections {
namespace internal {

template <class K>
typename K::Boolean
do_intersect(const typename K::Sphere_3& sphere,
             const CGAL::Bbox_3& bbox,
             const K& k)
{
  return do_intersect_sphere_box_3(sphere,
                                   bbox.xmin(), bbox.ymin(), bbox.zmin(),
                                   bbox.xmax(), bbox.ymax(), bbox.zmax(),
                                   k);
}

template <class K>
typename K::Boolean
do_intersect(const CGAL::Bbox_3& bbox,
             const typename K::Sphere_3& sphere,
             const K& k)
{
  return do_intersect(sphere, bbox, k);
}

} // namespace internal
} // namespace Intersections
} // namespace CGAL

#endif // CGAL_INTERNAL_INTERSECTIONS_3_BBOX_3_SPHERE_3_DO_INTERSECT_H
