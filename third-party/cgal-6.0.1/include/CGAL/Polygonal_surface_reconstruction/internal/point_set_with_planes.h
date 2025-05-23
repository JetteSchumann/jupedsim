// Copyright (c) 2018  Liangliang Nan. All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Polygonal_surface_reconstruction/include/CGAL/Polygonal_surface_reconstruction/internal/point_set_with_planes.h $
// $Id: include/CGAL/Polygonal_surface_reconstruction/internal/point_set_with_planes.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s) : Liangliang Nan

#ifndef CGAL_POLYGONAL_SURFACE_RECONSTRUCTION_POINT_SET_WITH_PLANES_H
#define CGAL_POLYGONAL_SURFACE_RECONSTRUCTION_POINT_SET_WITH_PLANES_H

#include <CGAL/license/Polygonal_surface_reconstruction.h>

#include <CGAL/Point_set_3.h>
#include <CGAL/property_map.h>
#include <CGAL/linear_least_squares_fitting_3.h>

#include <vector>


/*!
\file Point_set_with_planes.h
*/

namespace CGAL {


        namespace internal {

                // forward declaration
                template <typename Kernel>
                class Point_set_with_planes;


                /**
                *        A group of points (represented by their indices) belonging to a planar segment in a point set.
                */
                template <typename Kernel>
                class Planar_segment : public std::vector<std::size_t>
                {
                public:
                        typedef typename Kernel::Point_3                    Point;
                        typedef typename Kernel::Plane_3                Plane;
                        typedef internal::Point_set_with_planes<Kernel>        Point_set;

                public:

                        // \param point_set the point set that owns this planar segment.
                        Planar_segment(Point_set* point_set) : point_set_(point_set), supporting_plane_(nullptr) {}
                        ~Planar_segment() {}

                        Point_set* point_set() { return point_set_; }
                        void set_point_set(Point_set* point_set) { point_set_ = point_set; }

                        // Fits and returns the supporting plane of this planar segment
                        Plane* fit_supporting_plane() {
                                const typename Point_set::Point_map& points = point_set_->point_map();
                                std::list<Point> pts;
                                for (std::size_t i = 0; i < size(); ++i) {
                                        std::size_t idx = at(i);
                                        pts.push_back(points[idx]);
                                }

                                if (supporting_plane_)
                                        delete supporting_plane_;
                                supporting_plane_ = new Plane;
                                CGAL::linear_least_squares_fitting_3(pts.begin(), pts.end(), *supporting_plane_, CGAL::Dimension_tag<0>());

                                // Check orientation
                                int vote_for_opposite = 0;
                                for (std::size_t i = 0; i < size(); i++)
                                  if (supporting_plane_->orthogonal_vector() * point_set_->normal_map()[at(i)] < 0)
                                    vote_for_opposite++;
                                  else
                                    vote_for_opposite--;

                                if (vote_for_opposite > 0)
                                  *supporting_plane_ = supporting_plane_->opposite();

                                return supporting_plane_;
                        }

                        // Returns the supporting plane of this planar segment.
                        // \note Returned plane is valid only if fit_supporting_plane() has been called.
                        Plane* supporting_plane() const { return supporting_plane_; }

                private:
                        Point_set *         point_set_;
                        Plane *                supporting_plane_; // The hypothesis generator owns this plane and manages the memory
                };


                /**
                *        An enriched point set that stores the extracted planar segments
                */
                template <typename Kernel>
                class Point_set_with_planes : public Point_set_3<typename Kernel::Point_3>
                {
                public:

                        typedef Point_set_3<typename Kernel::Point_3>        Base_class;
                        typedef Point_set_with_planes<Kernel>                This_class;

                        typedef typename Kernel::FT                        FT;
                        typedef typename Kernel::Point_3                Point;
                        typedef typename Kernel::Vector_3                Vector;
                        typedef typename Kernel::Plane_3                Plane;
                        typedef internal::Planar_segment<Kernel>        Planar_segment;

                public:

                        /*!
                        \tparam PointRange The range of input points.

                        \tparam PointMap is a model of `ReadablePropertyMap` with value
                        type `Point_3<Kernel > `.

                        \tparam NormalMap is a model of `ReadablePropertyMap` with value
                        type `Vector_3<Kernel > `.

                        \tparam PlaneIndexMap is a model of `ReadablePropertyMap` with value
                        type `int `.

                        \param point_range the input point range.

                        \param point_map property map: value_type of `InputIterator` -> Point_3.

                        \param normal_map property map: value_type of `InputIterator` -> Vector_3.

                        \param plane_index_map property map: value_type of `InputIterator` -> int,
                        denoting the index of the plane it belongs to.
                        */
                        template <
                                typename PointRange,
                                typename PointMap,
                                typename NormalMap,
                                typename PlaneIndexMap
                        >
                                Point_set_with_planes(
                                        const PointRange& points,
                                        PointMap point_map,
                                        NormalMap normal_map,
                                        PlaneIndexMap plane_index_map
                                )
                        {
                                Base_class::resize(points.size());
                                Base_class::add_normal_map();

                                std::map<int, std::size_t> plane_index_remap;

                                // Gets to know the number of plane from the plane indices
                                std::size_t idx = 0;
                                for (typename PointRange::const_iterator it = points.begin(); it != points.end(); ++it) {
                                        Base_class::m_points[idx] = get(point_map, *it);
                                        Base_class::m_normals[idx] = get(normal_map, *it);
                                        int plane_index = get(plane_index_map, *it);
                                        if (plane_index != -1) {
                                          auto it_and_bool = plane_index_remap.emplace(plane_index, planar_segments_.size());
                                          if (it_and_bool.second) {
                                            planar_segments_.push_back(new Planar_segment(this));
                                            planar_segments_.back()->push_back(idx);
                                          }
                                          else planar_segments_[it_and_bool.first->second]->push_back(idx);
                                        }
                                        idx++;
                                }
                        }

                        ~Point_set_with_planes() {
                                for (std::size_t i = 0; i < planar_segments_.size(); ++i)
                                        delete planar_segments_[i];
                        }

                        std::vector< Planar_segment* >& planar_segments() { return planar_segments_; }
                        const std::vector< Planar_segment* >& planar_segments() const { return planar_segments_; }

                private:
                        std::vector< Planar_segment* > planar_segments_;
                };

        } //namespace internal

} //namespace CGAL


#endif // CGAL_POLYGONAL_SURFACE_RECONSTRUCTION_POINT_SET_WITH_PLANES_H
