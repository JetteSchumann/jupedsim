// Copyright (c) 2010, 2014 INRIA Sophia-Antipolis (France).
// Copyright (c) 2017 GeometryFactory (France).
// All rights reserved.
//
// This file is part of CGAL (www.cgal.org).
//
// $URL: https://github.com/CGAL/cgal/blob/v6.0.1/Periodic_3_mesh_3/include/CGAL/Periodic_3_mesh_triangulation_3.h $
// $Id: include/CGAL/Periodic_3_mesh_triangulation_3.h 50cfbde3b84 $
// SPDX-License-Identifier: GPL-3.0-or-later OR LicenseRef-Commercial
//
// Author(s)     : Mikhail Bogdanov
//                 Aymeric Pellé
//                 Mael Rouxel-Labbé

#ifndef CGAL_PERIODIC_3_MESH_3_MESH_TRIANGULATION_3_H
#define CGAL_PERIODIC_3_MESH_3_MESH_TRIANGULATION_3_H

#include <CGAL/license/Periodic_3_mesh_3.h>

#include <CGAL/Periodic_3_mesh_3/config.h>

// traits class
#include <CGAL/Kernel_traits.h>
#include <CGAL/Robust_weighted_circumcenter_filtered_traits_3.h>
#include <CGAL/Periodic_3_triangulation_3/internal/Robust_periodic_weighted_circumcenter_traits_3.h>
#include <CGAL/Periodic_3_triangulation_3/internal/canonicalize_helper.h>

// periodic triangulations
#include <CGAL/Periodic_3_regular_triangulation_traits_3.h>
#include <CGAL/Periodic_3_regular_triangulation_3.h>

// vertex and cell bases
#include <CGAL/Triangulation_vertex_base_with_info_3.h> // to mark dummy vertices
#include <CGAL/Mesh_vertex_base_3.h>
#include <CGAL/Mesh_cell_base_3.h>

#include <CGAL/array.h>
#include <CGAL/assertions.h>
#include <CGAL/Cartesian_converter.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Kernel_traits.h>
#include <CGAL/SMDS_3/io_signature.h>
#include <CGAL/tags.h>

#include <iostream>
#include <iterator>
#include <limits>
#include <unordered_set>
#include <utility>
#include <vector>

namespace CGAL {

/// This class currently provides an interface between the class
/// `CGAL::Periodic_3_regular_triangulation_3` and the mesher `Mesh_3`.
/// If periodic triangulations are parallelized, a lot of these functions will
/// become obsolete.
template<class Gt_, class Tds_>
class Periodic_3_regular_triangulation_3_wrapper
  : public Periodic_3_regular_triangulation_3<Gt_, Tds_>
{
public:
  typedef Sequential_tag                                      Concurrency_tag;
  typedef void                                                Lock_data_structure;

  void *get_lock_data_structure() const { return 0; }
  void set_lock_data_structure(void *) const { }

  typedef Periodic_3_regular_triangulation_3<Gt_, Tds_>       Base;

  typedef typename Base::Geom_traits                          Geom_traits;
  typedef typename Base::Triangulation_data_structure         Triangulation_data_structure;

  typedef typename Base::Cell_iterator                        Finite_cells_iterator;
  typedef typename Base::Facet_iterator                       Finite_facets_iterator;
  typedef typename Base::Edge_iterator                        Finite_edges_iterator;
  typedef typename Base::Vertex_iterator                      Finite_vertices_iterator;

  typedef typename Base::Vertex_handle                        Vertex_handle;
  typedef typename Base::Edge                                 Edge;
  typedef typename Base::Facet                                Facet;
  typedef typename Base::Cell_handle                          Cell_handle;

  typedef typename Base::FT                                   FT;

  typedef typename Base::Bare_point                           Bare_point;
  typedef typename Base::Weighted_point                       Weighted_point;
  typedef typename Base::Periodic_bare_point                  Periodic_bare_point;
  typedef typename Base::Periodic_weighted_point              Periodic_weighted_point;

  typedef typename Base::Locate_type                          Locate_type;

  typedef typename Base::Segment                              Segment;
  typedef typename Base::Periodic_segment                     Periodic_segment;

  typedef typename Base::Triangle                             Triangle;
  typedef typename Base::Periodic_triangle                    Periodic_triangle;
  typedef typename Base::Tetrahedron                          Tetrahedron;
  typedef typename Base::Periodic_tetrahedron                 Periodic_tetrahedron;

  typedef typename Base::Offset                               Offset;
  typedef typename Base::Iso_cuboid                           Iso_cuboid;
  typedef typename Base::Conflict_tester                      Conflict_tester;
  typedef typename Base::Covering_sheets                      Covering_sheets;

  typedef typename Geom_traits::Vector_3                      Vector_3;
  typedef typename Geom_traits::Ray_3                         Ray;

  using Base::construct_point;
  using Base::construct_weighted_point;
  using Base::construct_segment;
  using Base::construct_triangle;
  using Base::construct_tetrahedron;
  using Base::domain;
  using Base::dual;
  using Base::get_offset;
  using Base::geom_traits;
  using Base::adjacent_vertices;
  using Base::incident_cells;
  using Base::incident_edges;
  using Base::incident_facets;
  using Base::is_vertex;
  using Base::number_of_vertices;
  using Base::periodic_triangle;
  using Base::periodic_tetrahedron;
  using Base::point;
  using Base::set_point;
  using Base::tds;
  using Base::set_offsets;
#ifndef CGAL_NO_STRUCTURAL_FILTERING
  using Base::inexact_locate;
#endif

  static std::string io_signature() { return Get_io_signature<Base>()(); }

  /// Constructor
  Periodic_3_regular_triangulation_3_wrapper(const Iso_cuboid& domain = Iso_cuboid(0,0,0,1,1,1),
                                             const Geom_traits& gt = Geom_traits())
    : Base(domain, gt)
  {
  }

  /// Concurrency related
  template <typename Cell_handle>
  bool try_lock_cell(const Cell_handle &, int = 0) const
  {
    std::cerr << "ERROR: P3M3 does not yet support parallel execution" << std::endl;
    CGAL_assertion(false);
    return true;
  }

  bool try_lock_and_get_incident_cells(Vertex_handle /* v */,
                                       std::vector<Cell_handle>& /* cells */) const
  {
    std::cerr << "ERROR: P3M3 does not yet support parallel execution" << std::endl;
    CGAL_assertion(false);
    return true;
  }

  /// Basic setters-getters
  // there are no infinite elements in a periodic triangulation
  bool is_infinite(const Vertex_handle) const { return false; }
  bool is_infinite(const Edge&) const { return false; }
  bool is_infinite(const Facet&) const { return false; }
  bool is_infinite(const Cell_handle) const { return false; }
  bool is_infinite(const Cell_handle, int) const { return false; }
  bool is_infinite(const Cell_handle, int, int) const { return false; }

  Cell_handle infinite_cell() const
  {
    // there are no infinite cells in a periodic triangulation
    CGAL_assertion(false);
    return Cell_handle();
  }

  Vertex_handle infinite_vertex() const
  {
    // there is no infinite vertex in a periodic triangulation
    CGAL_assertion(false);
    return Vertex_handle();
  }

  int dimension() const
  {
    // there can be no degenerate dimensions in a periodic triangulation
    return (number_of_vertices() == 0) ? -2 : 3;
  }

  void set_domain(const Iso_cuboid& domain)
  {
    Base::set_domain(domain);
  }

  Bare_point canonicalize_point(const Bare_point& p) const
  {
    return P3T3::internal::robust_canonicalize_point(p, geom_traits());
  }

  // @fixme it might be dangerous to call robust_canonicalize() without also changing
  // <p, offset> = construct_periodic_point(p) (lack of consistency in the result)
  Weighted_point canonicalize_point(const Weighted_point& p) const
  {
    return P3T3::internal::robust_canonicalize_point(p, geom_traits());
  }

  // 1-cover, so we can take a const&
  const Weighted_point& point(const Vertex_handle v) const
  {
    return v->point();
  }

  Triangle triangle(const Facet& f) const
  {
    Periodic_triangle ptri = periodic_triangle(f);
    return construct_triangle(ptri);
  }

  Tetrahedron tetrahedron(const Cell_handle c) const
  {
    Periodic_tetrahedron ptet = periodic_tetrahedron(c);
    return construct_tetrahedron(ptet);
  }

  /*!
  Copies all finite `Edge`s incident to `v` to the
  output iterator `edges`. Returns the resulting output iterator.

  Since there are no infinite edges in a periodic triangulation, this
  function is simply a wrapper around `incident_edges`

  \pre `t.dimension() > 0`, `v != Vertex_handle()`, `t.is_vertex(v)`.
  */
  template<class OutputIterator>
  OutputIterator
  finite_incident_edges(Vertex_handle v, OutputIterator edges) const
  {
    return incident_edges(v, edges);
  }

  /*!
  Copies the `Cell_handle`s of all finite cells incident to `v` to the output
  iterator `cells`.
  Returns the resulting output iterator.

  Since there are no infinite cells in a periodic triangulation, this
  function is simply a wrapper around `incident_cells`

  \pre `t.dimension() == 3`, `v != Vertex_handle()`, `t.is_vertex(v)`.
  */
  template<class OutputIterator>
  OutputIterator
  finite_incident_cells(Vertex_handle v, OutputIterator cells) const
  {
    return incident_cells(v, cells);
  }

  /*!
  Copies all finite `Facet`s incident to `v` to the output iterator
  `facets`.
  Returns the resulting output iterator.

  Since there are no infinite facets in a periodic triangulation, this
  function is simply a wrapper around `incident_facets`

  \pre `t.dimension() > 1`, `v != Vertex_handle()`, `t.is_vertex(v)`.
  */
  template<class OutputIterator>
  OutputIterator
  finite_incident_facets(Vertex_handle v, OutputIterator facets) const
  {
    return incident_facets(v, facets);
  }

  // Periodic triangulations cannot be used in parallel (yet), but the functions below
  // are required for compilation. Note that these functions could already
  // be implemented and moved to P3T3.h but to make it clear that P3M3 is
  // not available in parallel, they are put here and left empty.

  template <class OutputIterator>
  OutputIterator
  incident_edges_threadsafe(Vertex_handle /* v */, OutputIterator edges) const
  {
    CGAL_assertion(false); // not yet supported
    return edges;
  }

  template <class OutputIterator>
  OutputIterator
  incident_facets_threadsafe(Vertex_handle /* v */, OutputIterator facets) const
  {
    CGAL_assertion(false); // not yet supported
    return facets;
  }

  template <typename OutputIterator>
  void
  incident_cells_threadsafe(Vertex_handle /* v */, OutputIterator /* cells */) const
  {
    CGAL_assertion(false); // not yet supported
  }

  void clear_v_offsets() const
  {
    for (typename std::vector<Vertex_handle>::iterator voit = this->v_offsets.begin();
         voit != this->v_offsets.end() ; ++voit) {
      (*voit)->clear_offset();
    }
    this->v_offsets.clear();
  }

  FT compute_power_distance_to_power_sphere(const Cell_handle c, const int i) const
  {
    typename Geom_traits::Compute_power_distance_to_power_sphere_3 cr =
      geom_traits().compute_power_distance_to_power_sphere_3_object();

    Offset o_nb = this->neighbor_offset(c, i, c->neighbor(i));
    Offset o_vt = get_offset(c->neighbor(i), c->neighbor(i)->index(c));

    const Weighted_point& wp0 = point(c->vertex(0)); // need the canonical point
    const Weighted_point& wp1 = point(c->vertex(1));
    const Weighted_point& wp2 = point(c->vertex(2));
    const Weighted_point& wp3 = point(c->vertex(3));
    const Weighted_point& wq = point(c->neighbor(i)->vertex(c->neighbor(i)->index(c)));
    const Offset& op0 = get_offset(c, 0);
    const Offset& op1 = get_offset(c, 1);
    const Offset& op2 = get_offset(c, 2);
    const Offset& op3 = get_offset(c, 3);
    const Offset oq = o_vt - o_nb;

    return cr(wp0, wp1, wp2, wp3, wq, op0, op1, op2, op3, oq);
  }

  // The functions below are used in Mesh_3 and need a specific implementation
  // for the periodic case because we need to try with different offsets to get the result
  FT compute_power_distance_to_power_sphere(const Cell_handle c,
                                            const Vertex_handle v) const
  {
    // @fixme need to introduce Compare_power_distances_to_power_sphere_3(4 points, query)
    typename Geom_traits::Compute_power_distance_to_power_sphere_3 cr =
      geom_traits().compute_power_distance_to_power_sphere_3_object();

    FT min_power_dist = std::numeric_limits<FT>::infinity();

    const Weighted_point& wp0 = point(c->vertex(0)); // need the canonical point
    const Weighted_point& wp1 = point(c->vertex(1));
    const Weighted_point& wp2 = point(c->vertex(2));
    const Weighted_point& wp3 = point(c->vertex(3));
    const Weighted_point& wq = point(v);
    const Offset& op0 = get_offset(c, 0);
    const Offset& op1 = get_offset(c, 1);
    const Offset& op2 = get_offset(c, 2);
    const Offset& op3 = get_offset(c, 3);

    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        for(int k = 0; k < 3; ++k) {
          const Offset oq(i-1, j-1, k-1);

          FT power_dist = cr(wp0, wp1, wp2, wp3, wq, op0, op1, op2, op3, oq);

          if(power_dist < min_power_dist)
            min_power_dist = power_dist;
        }
      }
    }

    return min_power_dist;
  }

  // Return the tetrahedron made of 'f' + 'wp'
  // \pre there exists an offset such that 'f.first' is in conflict with 'wp'
  Tetrahedron tetrahedron(const Facet& f, const Weighted_point& wp) const
  {
    Weighted_point canonic_wp = canonicalize_point(wp);
    Conflict_tester tester(canonic_wp, this);

    const Cell_handle c = f.first;
    const int index = f.second;

    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        for(int k = 0; k < 3; ++k) {
          const Offset off(i-1, j-1, k-1);
          if(tester(c, off))
          {
            return construct_tetrahedron(canonic_wp,
                                         point(c->vertex((index+1)&3)),
                                         point(c->vertex((index+2)&3)),
                                         point(c->vertex((index+3)&3)),
                                         off,
                                         get_offset(c, (index+1)&3),
                                         get_offset(c, (index+2)&3),
                                         get_offset(c, (index+3)&3));
          }
        }
      }
    }

    CGAL_assertion(false);
    return Tetrahedron();
  }

  Bounded_side side_of_power_sphere(const Cell_handle c, const Weighted_point& p,
                                    bool perturb = false) const
  {
    Weighted_point canonical_p = canonicalize_point(p);

    Bounded_side bs = ON_UNBOUNDED_SIDE;
    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        for(int k = 0; k < 3; ++k) {
          bs = Base::_side_of_power_sphere(c, canonical_p,
                                           Offset(i-1, j-1, k-1), perturb);

          if(bs != ON_UNBOUNDED_SIDE)
            return bs;
        }
      }
    }

    return bs;
  }

  // Warning: This is a periodic version that computes the smallest possible distance
  // between 'p' and 'q', for all possible combinations of offsets
  FT min_squared_distance(const Bare_point& p, const Bare_point& q) const
  {
    typename Geom_traits::Compare_squared_distance_3 compare_sd =
      geom_traits().compare_squared_distance_3_object();
    typename Geom_traits::Compute_squared_distance_3 compute_sd =
      geom_traits().compute_squared_distance_3_object();

    bool used_exact = false;
    std::pair<Bare_point, Offset> pp_p = P3T3::internal::construct_periodic_point(p, used_exact, geom_traits());
    std::pair<Bare_point, Offset> pp_q = P3T3::internal::construct_periodic_point(q, used_exact, geom_traits());

    Offset min_off;

    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        for(int k = 0; k < 3; ++k)
        {
          const Offset o(i-1, j-1, k-1);

          if((i == 0 && j == 0 && k == 0) ||
              compare_sd(q, p, q, p,
                         pp_q.second, pp_p.second + o,
                         pp_q.second, pp_p.second + min_off) == SMALLER)
          {
            min_off = o;
          }
        }
      }
    }

    return compute_sd(q, p, pp_q.second, pp_p.second + min_off);
  }

  // Warning: This function finds which offset 'Oq' should be applied to 'q' such
  // that the distance between 'p' and '(q, Oq)' is minimal.
  //
  // \pre 'p' lives in the canonical instance.
  Bare_point get_closest_point(const Bare_point& p, const Bare_point& q) const
  {
    CGAL_precondition(p.x() < domain().xmax());
    CGAL_precondition(p.y() < domain().ymax());
    CGAL_precondition(p.z() < domain().zmax());
    CGAL_precondition(p.x() >= domain().xmin());
    CGAL_precondition(p.y() >= domain().ymin());
    CGAL_precondition(p.z() >= domain().zmin());

    typename Geom_traits::Compare_squared_distance_3 compare_sd =
      geom_traits().compare_squared_distance_3_object();
    typename Geom_traits::Construct_point_3 cp =
      geom_traits().construct_point_3_object();

    bool used_exact = false;
    std::pair<Bare_point, Offset> pp_q = P3T3::internal::construct_periodic_point(q, used_exact, geom_traits());

    Offset min_off;
    Offset null_offset(0,0,0);

    for(int i = 0; i < 3; ++i) {
      for(int j = 0; j < 3; ++j) {
        for(int k = 0; k < 3; ++k)
        {
          const Offset o(i-1, j-1, k-1);

          if((i == 0 && j == 0 && k == 0) ||
             compare_sd(p, q, p, q,
                        null_offset, pp_q.second + o,
                        null_offset, pp_q.second + min_off) == SMALLER)
          {
            min_off = o;
          }
        }
      }
    }

    return cp(q, pp_q.second + min_off);
  }

  Weighted_point get_closest_point(const Weighted_point& wp, const Weighted_point& wq) const
  {
    typename Geom_traits::Compute_weight_3 cw = geom_traits().compute_weight_3_object();
    typename Geom_traits::Construct_point_3 cp = geom_traits().construct_point_3_object();
    typename Geom_traits::Construct_weighted_point_3 cwp = geom_traits().construct_weighted_point_3_object();

    return cwp(get_closest_point(cp(wp), cp(wq)), cw(wq));
  }

  // returns the triangle corresponding to f, with a geometric shift
  // so that it is incident to ref_v's canonical position
  Triangle get_incident_triangle(const Facet& f, const Vertex_handle ref_v) const
  {
    typename Geom_traits::Construct_point_3 cp = geom_traits().construct_point_3_object();
    typename Geom_traits::Construct_translated_point_3 tr = geom_traits().construct_translated_point_3_object();
    typename Geom_traits::Construct_vector_3 cv = geom_traits().construct_vector_3_object();
    typename Geom_traits::Construct_triangle_3 ct = geom_traits().construct_triangle_3_object();

    CGAL_precondition(f.first != Cell_handle() && f.first->has_vertex(ref_v));
    const int ref_v_pos = f.first->index(ref_v);
    const Bare_point& ref_p = cp(point(ref_v));
    const Bare_point ref_p_in_f = cp(point(f.first, ref_v_pos));
    Vector_3 move_to_canonical = cv(ref_p_in_f, ref_p);

    const int s = f.second;
    const Bare_point mp0 = tr(cp(point(f.first, (s+1)%4)), move_to_canonical);
    const Bare_point mp1 = tr(cp(point(f.first, (s+2)%4)), move_to_canonical);
    const Bare_point mp2 = tr(cp(point(f.first, (s+3)%4)), move_to_canonical);
    const Triangle t = ct(mp0, mp1, mp2);

    return t;
  }

  // Warning: This is a periodic version that computes the smallest possible
  // distances between p and q, and between p and r FOR ALL POSSIBLE OFFSETS
  // before comparing these distances.
  //
  // It is used in facet encroachement checks in Periodic_3_mesh_3.
  bool greater_or_equal_power_distance(const Bare_point& p,
                                       const Weighted_point& q,
                                       const Weighted_point& r) const
  {
    CGAL_precondition(this->is_1_cover());

    typename Geom_traits::Construct_point_3 cp =
      geom_traits().construct_point_3_object();
    typename Geom_traits::Compare_power_distance_3 compare_power_distance =
      geom_traits().compare_power_distance_3_object();

    // Compute the offsets that would bring p, q, and r into the canonical domain
    bool used_exact = false;
    std::pair<Bare_point, Offset> pp_p = P3T3::internal::construct_periodic_point(p, used_exact, geom_traits());
    std::pair<Bare_point, Offset> pp_q = P3T3::internal::construct_periodic_point(cp(q), used_exact, geom_traits());
    std::pair<Bare_point, Offset> pp_r = P3T3::internal::construct_periodic_point(cp(r), used_exact, geom_traits());

    // To compare pp(p, q) to pp(p, r), we first need to know the best offsets that minimize these distances
    auto get_offset_minimizing_power_product = [&](const Weighted_point& wp,
                                                   const Offset& base_wp_offset) -> Offset
    {
      Offset min_wp_offset;
      for(int i = 0; i < 3; ++i) {
        for(int j = 0; j < 3; ++j) {
          for(int k = 0; k < 3; ++k)
          {
            const Offset off(i-1, j-1, k-1);
            if((i == 0 && j == 0 && k == 0) ||
               compare_power_distance(p, wp, wp,
                                      pp_p.second,
                                      base_wp_offset + off,
                                      base_wp_offset + min_wp_offset) == SMALLER)
            {
              min_wp_offset = off;
            }
          }
        }
      }

      return min_wp_offset;
    };

    Offset min_q_off = get_offset_minimizing_power_product(q, pp_q.second);
    Offset min_r_off = get_offset_minimizing_power_product(r, pp_r.second);

    return !(compare_power_distance(p, q, r,
                                    pp_p.second,
                                    pp_q.second + min_q_off,
                                    pp_r.second + min_r_off) == SMALLER);
  }

  /// \name Locate functions
  ///
  /// Locate points within a periodic triangulation.
  ///
  /// These functions are temporarily here to interface between Mesh_3 and
  /// the periodic triangulations, until the latter are made parallel.
  ///
  /// \sa `CGAL::Regular_triangulation_3::locate`
  /// @{
  bool is_vertex(const Weighted_point& p, Vertex_handle& v) const
  {
    return Base::is_vertex(canonicalize_point(p), v);
  }

  Vertex_handle nearest_power_vertex(const Bare_point& p, Cell_handle start) const
  {
    return Base::nearest_power_vertex(canonicalize_point(p), start);
  }

  /// Return the squared distance (note: _NOT_ the power distance)
  /// between 'p' and the closest vertex for the power distance.
  std::pair<Vertex_handle, FT>
  nearest_power_vertex_with_sq_distance(const Bare_point& p, Cell_handle start) const
  {
    // The function below is almost a copy from 'nearest_power_vertex' in P3RT3.
    // Any change should be mirrored.
    CGAL_precondition(number_of_vertices() > 0);

    Bare_point canonical_p = canonicalize_point(p);

    Locate_type lt;
    int li, lj;

    typename Geom_traits::Compute_squared_distance_3 csd =
      geom_traits().compute_squared_distance_3_object();
    typename Geom_traits::Construct_point_3 cp =
      geom_traits().construct_point_3_object();
    typename Geom_traits::Construct_weighted_point_3 cwp =
      geom_traits().construct_weighted_point_3_object();

    Offset query_offset;
    Cell_handle c = Base::locate(cwp(canonical_p), query_offset, lt, li, lj, start);

    // - start with the closest vertex from the located cell.
    // - repeatedly take the nearest of its incident vertices if any
    // - if not, we're done.

    // Take the opposite because periodic_locate() returns the offset such that
    // cell + offset contains 'p' but here we need to move 'p'
    query_offset = this->combine_offsets(Offset(), -query_offset);

    Vertex_handle nearest = Base::nearest_vertex_in_cell(c, canonical_p, query_offset);
    const Weighted_point& nearest_wp = point(nearest);
    Offset offset_of_nearest = Base::get_min_dist_offset(canonical_p, query_offset, nearest);
    FT min_sq_dist = csd(canonical_p, cp(nearest_wp), query_offset, offset_of_nearest);

    std::vector<Vertex_handle> vs;
    vs.reserve(32);

    for(;;)
    {
      Vertex_handle tmp = nearest;

      adjacent_vertices(nearest, std::back_inserter(vs));
      for(typename std::vector<Vertex_handle>::const_iterator vsit = vs.begin();
                                                              vsit != vs.end(); ++vsit)
      {
        // Can happen in periodic triangulations composed of few points
        if(point(*vsit) == point(nearest))
          continue;

        const Offset min_dist_offset = this->get_min_dist_offset(canonical_p, query_offset, *vsit);
        if(this->compare_distance(canonical_p, point(*vsit), point(tmp),
                                  query_offset, min_dist_offset, offset_of_nearest) == SMALLER)
        {
          tmp = *vsit;
          offset_of_nearest = min_dist_offset;
          const Weighted_point& vswp = point(tmp);
          min_sq_dist = csd(canonical_p, cp(vswp), query_offset, min_dist_offset);
        }
      }

      if(tmp == nearest)
        break;

      vs.clear();
      nearest = tmp;
    }

    return std::make_pair(nearest, min_sq_dist);
  }

  std::pair<Vertex_handle, FT>
  nearest_power_vertex_with_sq_distance(const Vertex_handle v) const
  {
    typename Geom_traits::Construct_point_3 cp = geom_traits().construct_point_3_object();
    typename Geom_traits::Compute_squared_distance_3 csd = geom_traits().compute_squared_distance_3_object();

    Vertex_handle min_v {};
    FT min_sq_dist = -1;

    std::vector<Cell_handle> inc_cells;
    std::unordered_set<Vertex_handle> visited_vertices;
    incident_cells(v, std::back_inserter(inc_cells));
    for(Cell_handle ch : inc_cells)
    {
      CGAL_assertion(ch->has_vertex(v));
      int v_pos = ch->index(v);
      for(int i=1; i<4; ++i)
      {
        int vi_pos = (v_pos + i) % 4;
        Vertex_handle vi = ch->vertex(vi_pos);
        if(!visited_vertices.insert(vi).second) // already visited
          continue;

        FT sq_dist_i = csd(cp(point(ch, v_pos)), cp(point(ch, vi_pos)));
        if(min_v == Vertex_handle() || sq_dist_i < min_sq_dist)
        {
          min_v = vi;
          min_sq_dist = sq_dist_i;
        }
      }
    }

    return { min_v, min_sq_dist };
  }

  Cell_handle locate(const Weighted_point& p,
                     Cell_handle start = Cell_handle(),
                     bool* CGAL_assertion_code(could_lock_zone) = nullptr) const
  {
    CGAL_assertion(could_lock_zone == nullptr);
    return Base::locate(canonicalize_point(p), start);
  }

  Cell_handle locate(const Weighted_point& p,
                     Vertex_handle hint,
                     bool* CGAL_assertion_code(could_lock_zone) = nullptr) const
  {
    CGAL_assertion(could_lock_zone == nullptr);
    // Compared to the non-periodic version in T3, the infinite cell cannot
    // be used as default hint, so `Cell_handle()` is used instead.
    return Base::locate(canonicalize_point(p),
                        hint == Vertex_handle() ? Cell_handle() : hint->cell());
  }

  Cell_handle locate(const Weighted_point& p,
                     Locate_type& l, int& i, int& j,
                     Cell_handle start = Cell_handle(),
                     bool* CGAL_assertion_code(could_lock_zone) = nullptr) const
  {
    CGAL_assertion(could_lock_zone == nullptr);
    return Base::locate(canonicalize_point(p), l, i, j, start);
  }

  Cell_handle locate(const Weighted_point& p,
                     Locate_type& l, int& i, int& j,
                     Vertex_handle hint,
                     bool* CGAL_assertion_code(could_lock_zone) = nullptr) const
  {
    CGAL_assertion(could_lock_zone == nullptr);
    return Base::locate(canonicalize_point(p), l, i, j,
                        hint == Vertex_handle() ? Cell_handle() : hint->cell());
  }
  /// @}

  /// \name Conflict functions
  /// Returns the vertices on the interior of the conflict hole.
  ///
  /// These functions are temporarily here to interface between Mesh_3 and
  /// the periodic triangulations, until the latter are made parallel.
  ///
  /// @{
  template <class OutputIterator>
  OutputIterator
  vertices_inside_conflict_zone(const Weighted_point& /* p */,
                                Cell_handle /* c */,
                                OutputIterator res) const
  {
    return res;
    CGAL_assertion(false); // not yet supported
  }

  template <class OutputIteratorBoundaryFacets, class OutputIteratorCells,
            class OutputIteratorInternalFacets>
  Triple<OutputIteratorBoundaryFacets, OutputIteratorCells, OutputIteratorInternalFacets>
  find_conflicts(const Weighted_point& p,
                 Cell_handle c,
                 OutputIteratorBoundaryFacets bfit,
                 OutputIteratorCells cit,
                 OutputIteratorInternalFacets ifit,
                 bool* CGAL_assertion_code(could_lock_zone) = nullptr,
                 const Facet* /* this_facet_must_be_in_the_cz */ = nullptr,
                 bool* /* the_facet_is_in_its_cz */ = nullptr) const
  {
    CGAL_precondition(could_lock_zone == nullptr);
    CGAL_precondition(number_of_vertices() != 0);

    clear_v_offsets();

    Weighted_point canonic_p = canonicalize_point(p);

    Locate_type lt;
    int li, lj;
    c = locate(canonic_p, lt, li, lj, c);

    std::vector<Facet> facets;
    facets.reserve(64);
    std::vector<Cell_handle> cells;
    cells.reserve(32);

    Conflict_tester tester(canonic_p, this);
    Triple<typename std::back_insert_iterator<std::vector<Facet> >,
           typename std::back_insert_iterator<std::vector<Cell_handle> >,
           OutputIteratorInternalFacets> tit =
             Base::find_conflicts(c, tester,
                                  make_triple(std::back_inserter(facets),
                                              std::back_inserter(cells), ifit));
    ifit = tit.third;

    // Reset the conflict flag on the boundary.
    for(typename std::vector<Facet>::iterator fit=facets.begin();
        fit != facets.end(); ++fit) {
      fit->first->neighbor(fit->second)->tds_data().clear();
      *bfit++ = *fit;
    }

    // Reset the conflict flag in the conflict cells.
    for(typename std::vector<Cell_handle>::iterator ccit=cells.begin();
        ccit != cells.end(); ++ccit) {
      (*ccit)->tds_data().clear();
      *cit++ = *ccit;
    }

    return make_triple(bfit, cit, ifit);
  }

  template <class OutputIteratorBoundaryFacets, class OutputIteratorCells>
  std::pair<OutputIteratorBoundaryFacets, OutputIteratorCells>
  find_conflicts(const Weighted_point &p, Cell_handle c,
                 OutputIteratorBoundaryFacets bfit,
                 OutputIteratorCells cit,
                 bool* could_lock_zone = nullptr) const
  {
    CGAL_assertion(could_lock_zone == nullptr);

    Triple<OutputIteratorBoundaryFacets,
           OutputIteratorCells,
           Emptyset_iterator> t = find_conflicts(p, c, bfit, cit,
                                                 Emptyset_iterator(),
                                                 could_lock_zone);
    return std::make_pair(t.first, t.second);
  }
  /// @}

  void set_point(const Vertex_handle v,
                 const Vector_3& move,
                 const Weighted_point& new_position)
  {
    // calling robust canonical here means we don't necessarily have
    // canonical(v + move) = new_position... @fixme
    return Base::set_point(v, move, canonicalize_point(new_position));
  }

  /// \name Insert functions
  ///
  /// Insert points in the triangulation.
  ///
  /// These functions are temporarily here to interface between Mesh_3 and
  /// the periodic triangulations, until the latter are made parallel.
  ///
  /// @{
  template <class CellIt>
  Vertex_handle insert_in_hole(const Weighted_point& p,
                               CellIt /*cell_begin*/, CellIt /*cell_end*/,
                               Cell_handle /*begin*/, int /*i*/)
  {
    // Could be optimized to use the conflict zone by extracting relevant
    // code from periodic_insert() in P3T3.
    return this->insert(p);
  }

  Vertex_handle insert(const Weighted_point& p,
                       Cell_handle start = Cell_handle(),
                       bool* CGAL_assertion_code(could_lock_zone) = nullptr)
  {
    CGAL_assertion(could_lock_zone == nullptr);
    return Base::insert(canonicalize_point(p), start);
  }

  Vertex_handle insert(const Weighted_point& p,
                       Vertex_handle hint,
                       bool* CGAL_assertion_code(could_lock_zone) = nullptr)
  {
    CGAL_assertion(could_lock_zone == nullptr);
    // compared to the non-periodic version in T3, the infinite cell cannot
    // be used; `Cell_handle()` is used instead
    return Base::insert(canonicalize_point(p),
                        hint == Vertex_handle() ? Cell_handle() : hint->cell());
  }

  Vertex_handle insert(const Weighted_point& p,
                       Locate_type lt, Cell_handle loc, int li, int lj,
                       bool* CGAL_assertion_code(could_lock_zone) = nullptr)
  {
    CGAL_assertion(could_lock_zone == nullptr);
    return Base::insert(canonicalize_point(p), lt, loc, li, lj);
  }
  /// @}

  /// Remove functions
  bool remove(Vertex_handle v,
              bool* CGAL_assertion_code(could_lock_zone) = nullptr)
  {
    CGAL_assertion(could_lock_zone == nullptr);
    bool b = Base::remove_if_no_cover_change(v);
    CGAL_postcondition(this->is_1_cover()); // do not ever allow cover change
    return b;
  }

  /// Dual computations
  Object dual(const Facet & f) const
  {
    Segment s = construct_segment(Base::dual(f));
    return make_object(s);
  }

  void dual_segment(const Facet& facet, Bare_point& p, Bare_point& q) const
  {
    typename Base::Periodic_segment_3 ps = Base::dual(facet);
    p = construct_point(ps[0]);
    q = construct_point(ps[1]);
  }

  void dual_segment_exact(const Facet& facet, Bare_point& p, Bare_point& q) const
  {
    typedef typename Kernel_traits<Bare_point>::Kernel                Kernel;
    typedef Exact_predicates_exact_constructions_kernel               EKernel;

    typedef Cartesian_converter<Kernel, EKernel>                      To_exact;
    typedef Cartesian_converter<EKernel, Kernel>                      Back_from_exact;

    typedef CGAL::Periodic_3_regular_triangulation_traits_3<EKernel>  Exact_Rt;
    typedef typename Exact_Rt::Point_3                                EPoint_3;

    To_exact to_exact;
    Back_from_exact back_from_exact;

    Exact_Rt etraits(to_exact(domain()));
    const typename EKernel::Iso_cuboid_3& dom = etraits.get_domain();

    Exact_Rt::Construct_weighted_circumcenter_3 exact_weighted_circumcenter =
      etraits.construct_weighted_circumcenter_3_object();
    Exact_Rt::Construct_point_3 exact_construct_point =
      etraits.construct_point_3_object();

    Cell_handle c = facet.first;
    const int i = facet.second;
    Cell_handle n = c->neighbor(i);

    EPoint_3 exact_wc1 = exact_weighted_circumcenter(
                           to_exact(point(c->vertex(0))), to_exact(point(c->vertex(1))),
                           to_exact(point(c->vertex(2))), to_exact(point(c->vertex(3))),
                           get_offset(c, 0), get_offset(c, 1),
                           get_offset(c, 2), get_offset(c, 3));
    EPoint_3 exact_wc2 = exact_weighted_circumcenter(
                           to_exact(point(n->vertex(0))), to_exact(point(n->vertex(1))),
                           to_exact(point(n->vertex(2))), to_exact(point(n->vertex(3))),
                           get_offset(n, 0), get_offset(n, 1),
                           get_offset(n, 2), get_offset(n, 3));

    typename EKernel::Point_3 dp;

    // get the offset of the first weighted circumcenter
    Offset transl_wc1;
    for(;;) /* while not in */
    {
      // can safely perform a construction here because the kernel has exact constructions
      dp = etraits.construct_point_3_object()(exact_wc1, transl_wc1);

      if(dp.x() < dom.xmin())
        transl_wc1.x() += 1;
      else if(dp.y() < dom.ymin())
        transl_wc1.y() += 1;
      else if(dp.z() < dom.zmin())
        transl_wc1.z() += 1;
      else if(!(dp.x() < dom.xmax()))
        transl_wc1.x() -= 1;
      else if(!(dp.y() < dom.ymax()))
        transl_wc1.y() -= 1;
      else if(!(dp.z() < dom.zmax()))
        transl_wc1.z() -= 1;
      else
        break;
    }

    // get the offset of the second weighted circumcenter
    Offset transl_wc2;
    for(;;) /* while not in */
    {
      dp = etraits.construct_point_3_object()(exact_wc2, transl_wc2);

      if(dp.x() < dom.xmin())
        transl_wc2.x() += 1;
      else if(dp.y() < dom.ymin())
        transl_wc2.y() += 1;
      else if(dp.z() < dom.zmin())
        transl_wc2.z() += 1;
      else if(!(dp.x() < dom.xmax()))
        transl_wc2.x() -= 1;
      else if(!(dp.y() < dom.ymax()))
        transl_wc2.y() -= 1;
      else if(!(dp.z() < dom.zmax()))
        transl_wc2.z() -= 1;
      else
        break;
    }

    Offset off = this->neighbor_offset(c, i, n);
    Offset o1 = -transl_wc1;
    Offset o2 = this->combine_offsets(-transl_wc2, -off);
    Offset cumm_off((std::min)(o1.x(), o2.x()),
                    (std::min)(o1.y(), o2.y()),
                    (std::min)(o1.z(), o2.z()));

    EPoint_3 ewc1 = exact_construct_point(exact_wc1, transl_wc1);
    EPoint_3 ewc2 = exact_construct_point(exact_wc2, transl_wc2);
    p = back_from_exact(exact_construct_point(ewc1, o1 - cumm_off));
    q = back_from_exact(exact_construct_point(ewc2, o2 - cumm_off));
  }

  // dual rays are impossible in a periodic triangulation since there are no
  // infinite cells, but these functions are still required to compile Mesh_3
  void dual_ray(const Facet& /*f*/, Ray& /*ray*/) const { CGAL_assertion(false); }
  void dual_ray_exact(const Facet& /*facet*/, Ray& /*ray*/) const { CGAL_assertion(false); }
};

namespace details {

template<typename K>
struct Periodic_3_mesh_geom_traits_generator
{
private:
  // - 'Robust_periodic_weighted_circumcenter_traits_3': provides robust periodic
  //   constructions for construct_weighted_circumcenter()
  // - 'Periodic_3_regular_triangulation_traits_3': makes the base (non-periodic)
  //   traits usable in a periodic setting
  // - 'Robust_weighted_circumcenter_filtered_traits_3': provides robust versions
  //   of some non-periodic constructions.

  typedef Robust_periodic_weighted_circumcenter_traits_3<
            Periodic_3_regular_triangulation_traits_3<
              Robust_weighted_circumcenter_filtered_traits_3<K> > > Geom_traits;

public:
  typedef Geom_traits type;
  typedef type Type;
};  // end struct Periodic_3_mesh_geom_traits_generator

}  // end namespace details

template<class MD,
         class K_ = Default,
         class Vertex_base_ = Default,
         class Cell_base_ = Default>
class Periodic_3_mesh_triangulation_3
{
  // Triangulation_vertex_base_with_info_3 only does default initialization
  // and not value initialization, but we cannot initialize info() during Mesh_3's refinement
  struct Boolean_with_def_value
  {
    bool is_dummy_vertex = false;
  };

  // default K
  typedef typename Default::Get<K_, typename Kernel_traits<MD>::Kernel>::type K;

  // traits
  typedef typename details::Periodic_3_mesh_geom_traits_generator<K>::type Geom_traits;

  // Periodic vertex and cell bases
  typedef Periodic_3_triangulation_ds_vertex_base_3<> VbDS;
  typedef Regular_triangulation_vertex_base_3<Geom_traits, VbDS> PVb;
  typedef Triangulation_vertex_base_with_info_3<Boolean_with_def_value, Geom_traits, PVb> Vb;

  typedef Periodic_3_triangulation_ds_cell_base_3<> CbDS;
  typedef Regular_triangulation_cell_base_3<Geom_traits, CbDS> RCb;
  typedef Regular_triangulation_cell_base_with_weighted_circumcenter_3<Geom_traits, RCb> PCb;

  typedef Mesh_vertex_base_3<Geom_traits, MD, Vb> Default_Vb;
  typedef Mesh_cell_base_3<Geom_traits, MD, PCb> Default_Cb;

  // default Vb/Cb
  typedef typename Default::Get<Vertex_base_, Default_Vb>::type Vertex_base;
  typedef typename Default::Get<Cell_base_, Default_Cb>::type Cell_base;

  // Triangulation and tds
  typedef Triangulation_data_structure_3<Vertex_base, Cell_base> Tds;
  typedef Periodic_3_regular_triangulation_3_wrapper<Geom_traits, Tds> Triangulation;

public:
  typedef Triangulation type;
  typedef type Type;
};

} // namespace CGAL

#endif // CGAL_PERIODIC_3_MESH_3_MESH_TRIANGULATION_3_H
