// Copyright (c) 2000
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Intersections_2/include/CGAL/Intersections_2/Line_2_Triangle_2.h $
// $Id: include/CGAL/Intersections_2/Line_2_Triangle_2.h 50cfbde3b84 $
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Geert-Jan Giezeman


#ifndef CGAL_INTERSECTIONS_2_LINE_2_TRIANGLE_2_H
#define CGAL_INTERSECTIONS_2_LINE_2_TRIANGLE_2_H

#include <CGAL/Line_2.h>
#include <CGAL/Segment_2.h>
#include <CGAL/Triangle_2.h>
#include <CGAL/Point_2.h>
#include <CGAL/Intersections_2/internal/Straight_2.h>
#include <CGAL/kernel_assertions.h>
#include <CGAL/number_utils.h>
#include <CGAL/Intersection_traits_2.h>

namespace CGAL {

namespace Intersections {

namespace internal {

template <class K>
class Line_2_Triangle_2_pair {
public:
    enum Intersection_results {NO_INTERSECTION, POINT, SEGMENT, UNKNOWN};
    Line_2_Triangle_2_pair(typename K::Line_2 const *line,
                           typename K::Triangle_2 const *trian)
        : _line(line), _trian(trian) {}

    Intersection_results intersection_type() const;

    typename K::Point_2    intersection_point() const;
    typename K::Segment_2  intersection_segment() const;
protected:
    typename K::Line_2 const*_line;
    typename K::Triangle_2 const *  _trian;
    mutable Intersection_results     _result = UNKNOWN;
    mutable typename K::Point_2         _intersection_point;
    mutable typename K::Point_2        _other_point;
};

template <class K>
inline
typename K::Boolean
do_intersect(const typename K::Line_2& l,
             const typename K::Triangle_2& tr,
             const K&)
{
  typedef Line_2_Triangle_2_pair<K> pair_t;
  pair_t pair(&l, &tr);
  return pair.intersection_type() != pair_t::NO_INTERSECTION;
}

template <class K>
inline
typename K::Boolean
do_intersect(const typename K::Triangle_2& tr,
             const typename K::Line_2& l,
             const K& k)
{
  return internal::do_intersect(l, tr, k);
}

template <class K>
typename Line_2_Triangle_2_pair<K>::Intersection_results
Line_2_Triangle_2_pair<K>::intersection_type() const
{
  typedef typename K::Line_2 Line_2;
    if (_result!=UNKNOWN)
        return _result;
// The non const this pointer is used to cast away const.
    Straight_2_<K> straight(*_line);
    Line_2 l(_trian->vertex(0), _trian->vertex(1));
if (l.oriented_side(_trian->vertex(2)) == ON_POSITIVE_SIDE) {
//    if (_trian->is_counterclockwise()) {
        straight.cut_right_off(
            Line_2(_trian->vertex(0), _trian->vertex(1)));
        straight.cut_right_off(
            Line_2(_trian->vertex(1), _trian->vertex(2)));
        straight.cut_right_off(
            Line_2(_trian->vertex(2), _trian->vertex(0)));
    } else {
        straight.cut_right_off(
            Line_2(_trian->vertex(2), _trian->vertex(1)));
        straight.cut_right_off(
            Line_2(_trian->vertex(1), _trian->vertex(0)));
        straight.cut_right_off(
            Line_2(_trian->vertex(0), _trian->vertex(2)));
    }
    switch (straight.current_state()) {
    case Straight_2_<K>::EMPTY:
        _result = NO_INTERSECTION;
        return _result;
    case Straight_2_<K>::POINT: {
        straight.current(_intersection_point);
        _result = POINT;
        return _result;
        }
    case Straight_2_<K>::SEGMENT: {
        typename K::Segment_2 seg;
        straight.current(seg);
        _intersection_point = seg.source();
        _other_point = seg.target();
        _result = SEGMENT;
        return _result;
        }
    default:  // should not happen.
        CGAL_kernel_assertion_msg(false, "Internal CGAL error.");
        _result = NO_INTERSECTION;
        return _result;
    }
}


template <class K>
typename K::Point_2
Line_2_Triangle_2_pair<K>::
intersection_point() const
{
    if (_result==UNKNOWN)
        intersection_type();
    CGAL_kernel_assertion(_result == POINT);
    return _intersection_point;
}

template <class K>
typename K::Segment_2
Line_2_Triangle_2_pair<K>::
intersection_segment() const
{
  typedef typename K::Segment_2 Segment_2;
    if (_result==UNKNOWN)
        intersection_type();
    CGAL_kernel_assertion(_result == SEGMENT);
    return Segment_2(_intersection_point, _other_point);
}




template <class K>
typename CGAL::Intersection_traits
<K, typename K::Line_2, typename K::Triangle_2>::result_type
intersection(const typename K::Line_2 &line,
             const typename K::Triangle_2 &tr,
             const K&)
{
    typedef Line_2_Triangle_2_pair<K> is_t;
    is_t ispair(&line, &tr);
    switch (ispair.intersection_type()) {
    case is_t::NO_INTERSECTION:
    default:
        return intersection_return<typename K::Intersect_2, typename K::Line_2, typename K::Triangle_2>();
    case is_t::POINT:
        return intersection_return<typename K::Intersect_2, typename K::Line_2, typename K::Triangle_2>(ispair.intersection_point());
    case is_t::SEGMENT:
        return intersection_return<typename K::Intersect_2, typename K::Line_2, typename K::Triangle_2>(ispair.intersection_segment());
    }
}


template <class K>
inline
typename CGAL::Intersection_traits
<K, typename K::Line_2, typename K::Triangle_2>::result_type
intersection(const typename K::Triangle_2 &tr,
             const typename K::Line_2 &line,
             const K& k)
{
  return internal::intersection(line, tr, k);
}

} // namespace internal
} // namespace Intersections

CGAL_INTERSECTION_FUNCTION(Line_2, Triangle_2, 2)
CGAL_DO_INTERSECT_FUNCTION(Line_2, Triangle_2, 2)

} // namespace CGAL

#endif // CGAL_INTERSECTIONS_2_LINE_2_TRIANGLE_2_H
