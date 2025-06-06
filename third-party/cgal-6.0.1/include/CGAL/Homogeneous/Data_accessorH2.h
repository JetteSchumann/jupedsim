// Copyright (c) 1999,2001
// Utrecht University (The Netherlands),
// ETH Zurich (Switzerland),
// INRIA Sophia-Antipolis (France),
// Max-Planck-Institute Saarbruecken (Germany),
// and Tel-Aviv University (Israel).  All rights reserved.
//
// This file is part of CGAL (www.cgal.org)
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Homogeneous_kernel/include/CGAL/Homogeneous/Data_accessorH2.h $
// $Id: include/CGAL/Homogeneous/Data_accessorH2.h 50cfbde3b84 $
// SPDX-License-Identifier: LGPL-3.0-or-later OR LicenseRef-Commercial
//
//
// Author(s)     : Stefan Schirra


#ifndef CGAL_DATA_ACCESSORH2_H
#define CGAL_DATA_ACCESSORH2_H

namespace CGAL {

template < class R >
class Data_accessorH2
{
public:
    typedef typename R::FT FT;
    typedef typename R::RT RT;
    typedef typename R::Point_2      Point_2;

    RT  get_hx( const Point_2 & p) const { return( p.hx()); }
    RT  get_hy( const Point_2 & p) const { return( p.hy()); }
    RT  get_hw( const Point_2 & p) const { return( p.hw()); }

    void
    get( const Point_2 & p, RT& hx, RT& hy, RT& hw) const
    {
        hx = get_hx( p);
        hy = get_hy( p);
        hw = get_hw( p);
    }

    void
    set( Point_2& p, const RT & hx, const RT & hy, const RT & hw) const
    {
        p = Point_2( hx, hy, hw);
    }
};

} //namespace CGAL

#endif // CGAL_DATA_ACCESSORH2_H
