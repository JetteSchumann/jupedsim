// Copyright (c) 1999
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Kernel_23/include/CGAL/Ray_3.h $
// $Id: include/CGAL/Ray_3.h 50cfbde3b84 $
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Andreas Fabri, Stefan Schirra

#ifndef CGAL_RAY_3_H
#define CGAL_RAY_3_H

#include <CGAL/assertions.h>
#include <CGAL/Kernel/Return_base_tag.h>
#include <CGAL/representation_tags.h>
#include <CGAL/Dimension.h>
#include <CGAL/IO/io.h>

namespace CGAL {

template <class R_>
class Ray_3 : public R_::Kernel_base::Ray_3
{
  typedef typename R_::FT                    FT;
  typedef typename R_::Point_3               Point_3;
  typedef typename R_::Direction_3           Direction_3;
  typedef typename R_::Vector_3              Vector_3;
  typedef typename R_::Line_3                Line_3;
  typedef typename R_::Aff_transformation_3  Aff_transformation_3;

  typedef Ray_3                            Self;
  static_assert(std::is_same<Self, typename R_::Ray_3>::value);

public:

  typedef Dimension_tag<3>  Ambient_dimension;
  typedef Dimension_tag<1>  Feature_dimension;

  typedef typename R_::Kernel_base::Ray_3    Rep;

  const Rep& rep() const
  {
    return *this;
  }

  Rep& rep()
  {
    return *this;
  }

  typedef          R_                       R;

  Ray_3() {}

  Ray_3(const Rep& r)
    : Rep(r) {}

  Ray_3(Rep&& r)
    : Rep(std::move(r)) {}

  Ray_3(const Point_3& sp, const Point_3& secondp)
    : Rep(typename R::Construct_ray_3()(Return_base_tag(), sp, secondp)) {}

  Ray_3(const Point_3& sp, const Vector_3& v)
    : Rep(typename R::Construct_ray_3()(Return_base_tag(), sp, v)) {}

  Ray_3(const Point_3& sp, const Direction_3& d)
    : Rep(typename R::Construct_ray_3()(Return_base_tag(), sp, d)) {}

  Ray_3(const Point_3& sp, const Line_3& l)
    : Rep(typename R::Construct_ray_3()(Return_base_tag(), sp, l)) {}

  Ray_3 transform(const Aff_transformation_3 &t) const
  {
    return Ray_3(t.transform(this->source()),
                 t.transform(this->second_point()));
    // NB : Homogeneous used direction() instead of second_point().
  }

/*
  const Point_3 &   start() const;
  const Point_3 &   source() const
  {
      return get_pointee_or_identity(base).e0;
  }

  Direction_3 direction() const;
  Vector_3    to_vector() const;
  Line_3      supporting_line() const;
  Ray_3       opposite() const;

  bool        is_degenerate() const;
  bool        collinear_has_on(const Point_3 &p) const;
*/

  decltype(auto)
  point(const FT i) const
  {
    return R().construct_point_on_3_object()(*this, i);
  }

  decltype(auto)
  source() const
  {
    return R().construct_source_3_object()(*this);
  }

  decltype(auto)
  second_point() const
  {
    return R().construct_second_point_3_object()(*this);
  }

  decltype(auto)
  start() const
  {
    return source();
  }

  bool has_on(const Point_3 &p) const
  {
    return R().has_on_3_object()(*this, p);
  }

  Direction_3
  direction() const
  {
    typename R::Construct_vector_3 construct_vector;
    typename R::Construct_direction_3 construct_direction;
    return construct_direction( construct_vector(source(), second_point()) );
  }

  Ray_3
  opposite() const
  {
    return Ray_3( source(), - direction() );
  }

  Vector_3
  to_vector() const
  {
    typename R::Construct_vector_3 construct_vector;
    return construct_vector(source(), second_point());
  }

  Line_3
  supporting_line() const
  {
    return R().construct_line_3_object()(source(), second_point());
  }

  bool is_degenerate() const
  {
    return R().is_degenerate_3_object()(*this);
  }

};


template <class R >
std::ostream&
insert(std::ostream& os, const Ray_3<R>& r, const Cartesian_tag&)
{
    switch(IO::get_mode(os)) {
    case IO::ASCII :
        return os << r.start() << ' ' << r.direction();
    case IO::BINARY :
        return os<< r.start() << r.direction();
    default:
        return os << "RayC3(" << r.start() <<  ", " << r.direction() << ")";
    }
}

template <class R >
std::ostream&
insert(std::ostream& os, const Ray_3<R>& r, const Homogeneous_tag&)
{
  switch(IO::get_mode(os))
  {
      case IO::ASCII :
          return os << r.start() << ' ' << r.direction();
      case IO::BINARY :
          return os<< r.start() << r.direction();
      default:
          return os << "RayH3(" << r.start() <<  ", " << r.direction() << ")";
  }
}

template < class R >
std::ostream&
operator<<(std::ostream& os, const Ray_3<R>& r)
{
  return insert(os, r, typename R::Kernel_tag() );
}


template <class R >
std::istream&
extract(std::istream& is, Ray_3<R>& r, const Cartesian_tag&)
{
    typename R::Point_3 p;
    typename R::Direction_3 d;

    is >> p >> d;

    if (is)
        r = Ray_3<R>(p, d);
    return is;
}

template <class R >
std::istream&
extract(std::istream& is, Ray_3<R>& r, const Homogeneous_tag&)
{
  typename R::Point_3 p;
  typename R::Direction_3 d;
  is >> p >> d;
  if (is)
    r = Ray_3<R>(p, d);
  return is;
}

template < class R >
std::istream&
operator>>(std::istream& is, Ray_3<R>& r)
{
  return extract(is, r, typename R::Kernel_tag() );
}

} //namespace CGAL

#endif // CGAL_RAY_3_H
