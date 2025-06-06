// Copyright (c) 2000
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Cartesian_kernel/include/CGAL/Cartesian/Point_2.h $
// $Id: include/CGAL/Cartesian/Point_2.h 50cfbde3b84 $
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Andreas Fabri, Herve Bronnimann

#ifndef CGAL_CARTESIAN_POINT_2_H
#define CGAL_CARTESIAN_POINT_2_H

#include <CGAL/Origin.h>

namespace CGAL {

template < class R_ >
class PointC2
{
  typedef PointC2<R_>                       Self;
  typedef typename R_::FT                   FT;
// https://doc.cgal.org/latest/Manual/devman_code_format.html#secprogramming_conventions
  typedef typename R_::Vector_2             Vector_2_;
  typedef typename R_::Point_2              Point_2;

  // We do not use reference counting here as it is done at the Vector_2 level.
  Vector_2_ base;

public:

  typedef typename Vector_2_::Cartesian_const_iterator Cartesian_const_iterator;

  typedef R_                                R;

  PointC2() {}

  PointC2(const Origin &)
    : base(NULL_VECTOR) {}

  template <class T1, class T2>
  PointC2(T1 &&x, T2 &&y)
    : base(std::forward<T1>(x), std::forward<T2>(y))
  {}

  template <class T1, class T2, class T3>
  PointC2(T1 &&hx, T2 &&hy, T3 &&hw)
    : base(std::forward<T1>(hx), std::forward<T2>(hy), std::forward<T3>(hw)) {}

  friend void swap(Self& a, Self& b)
#if !defined(__INTEL_COMPILER) && defined(__cpp_lib_is_swappable)
    noexcept(std::is_nothrow_swappable_v<Vector_2_>)
#endif
  {
    using std::swap;
    swap(a.base, b.base);
  }

  const FT& x() const
  {
      return base.x();
  }

  const FT& y() const
  {
      return base.y();
  }

  const FT& hx() const
  {
      return base.hx();
  }
  const FT& hy() const
  {
      return base.hy();
  }
  const FT& hw() const
  {
      return base.hw();
  }

  Cartesian_const_iterator cartesian_begin() const
  {
    return base.cartesian_begin();
  }

  Cartesian_const_iterator cartesian_end() const
  {
    return base.cartesian_end();
  }

  typename R_::Boolean   operator==(const PointC2 &p) const
  {
      return base == p.base;
  }
  typename R_::Boolean   operator!=(const PointC2 &p) const
  {
      return !(*this == p);
  }

};

} //namespace CGAL

#endif // CGAL_CARTESIAN_POINT_2_H
