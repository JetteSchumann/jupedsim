// Copyright (c) 2003-2008  INRIA Sophia-Antipolis (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Circular_kernel_2/include/CGAL/Filtered_bbox_circular_kernel_2/interface_macros.h $
// $Id: include/CGAL/Filtered_bbox_circular_kernel_2/interface_macros.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Monique Teillaud, Sylvain Pion, Pedro Machado

#include <CGAL/license/Circular_kernel_2.h>

// Partially supported by the IST Programme of the EU as a Shared-cost
// RTD (FET Open) Project under Contract No  IST-2000-26473
// (ECG - Effective Computational Geometry for Curves and Surfaces)
// and a STREP (FET Open) Project under Contract No  IST-006413
// (ACS -- Algorithms for Complex Shapes)



// This file is intentionally not protected against re-inclusion.
// It's aimed at being included from within a kernel traits class, this
// way we share more code.

// It is the responsibility of the including file to correctly set the 2
// macros CGAL_Filtered_Bbox_Circular_Kernel_pred and CGAL_Filtered_Bbox_Circular_Kernel_cons.
// And they are #undefed at the end of this file.

  CGAL_Filtered_Bbox_Circular_Kernel_pred(Compare_x_2, compare_x_2_object)
  CGAL_Filtered_Bbox_Circular_Kernel_pred(Compare_y_2, compare_y_2_object)
  CGAL_Filtered_Bbox_Circular_Kernel_pred(Compare_xy_2, compare_xy_2_object)
  CGAL_Filtered_Bbox_Circular_Kernel_pred(Has_on_2, has_on_2_object)
  CGAL_Filtered_Bbox_Circular_Kernel_pred(Compare_y_at_x_2, compare_y_at_x_2_object)
  CGAL_Filtered_Bbox_Circular_Kernel_pred(Do_overlap_2, do_overlap_2_object)
  CGAL_Filtered_Bbox_Circular_Kernel_pred(Equal_2, equal_2_object)
  CGAL_Filtered_Bbox_Circular_Kernel_pred(In_x_range_2, in_x_range_2_object)

  CGAL_Filtered_Bbox_Circular_Kernel_cons(Intersect_2,
  intersect_2_object)

#undef CGAL_Filtered_Bbox_Circular_Kernel_pred
#undef CGAL_Filtered_Bbox_Circular_Kernel_cons
