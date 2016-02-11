/*~--------------------------------------------------------------------------~*
 *  @@@@@@@@  @@           @@@@@@   @@@@@@@@ @@
 * /@@/////  /@@          @@////@@ @@////// /@@
 * /@@       /@@  @@@@@  @@    // /@@       /@@
 * /@@@@@@@  /@@ @@///@@/@@       /@@@@@@@@@/@@
 * /@@////   /@@/@@@@@@@/@@       ////////@@/@@
 * /@@       /@@/@@//// //@@    @@       /@@/@@
 * /@@       @@@//@@@@@@ //@@@@@@  @@@@@@@@ /@@
 * //       ///  //////   //////  ////////  // 
 * 
 * Copyright (c) 2016 Los Alamos National Laboratory, LLC
 * All rights reserved
 *~--------------------------------------------------------------------------~*/

#pragma once

//! system includes
#include <string>

//! user includes
#include "flecsi/state/state.h"
#include "flecsi/execution/task.h"

#include "ale/mesh/burton/burton_types.h"

/*!
 * \file burton_mesh.h
 * \authors bergen
 * \date Initial file creation: Sep 02, 2015
 */

namespace ale {
namespace mesh {

/*----------------------------------------------------------------------------*
 * class burton_mesh_t
 *----------------------------------------------------------------------------*/

/*!
  \class burton_mesh_t burton_mesh.h
  \brief A specialization of the flecsi low-level mesh topology, state and
    execution models.
 */
class burton_mesh_t
{
public:

  //! Type for storing instance of template specialized low level mesh.
  using mesh_t = mesh_topology_t<burton_mesh_types_t>;


  //! a compile string type
  using const_string_t = flecsi::const_string_t;
  
  //! a bitfield type
  using bitfield_t = flecsi::bitfield_t;


  //! Type defining the execution policy.
#ifndef MESH_EXECUTION_POLICY
  using mesh_execution_t = flecsi::execution_t<>;
#else
  using mesh_execution_t = flecsi::execution_t<MESH_EXECUTION_POLICY>;
#endif

  //! Type defining the data attachment sites on the burton mesh.
  using attachment_site_t = burton_mesh_traits_t::attachment_site_t;

  /*!
    \brief Accessor type.

    \tparam T The type of the underlying data to access.
   */
  template<typename T>
  using accessor_t = burton_mesh_traits_t::mesh_state_t::accessor_t<T>;

  /*!
    \brief Register state for the named variable at the given attachment
      site with attributes.

    \tparam T The type of the underlying data to access.

    \param[in] key A name for the state variable, e.g., "density".
    \param[in] site The data attachement site where the state variable should
      be defined.  Valid sites are defined in flecsi::burton_mesh_traits_t.
    \param[in] attributes A bitfield specifying various attributes of the state.

    \return An accessor to the newly registered state.
   */
  template<typename T>
  decltype(auto) register_state_(const const_string_t && key,
    attachment_site_t site, bitfield_t::field_type_t attributes = 0x0) {

    switch(site) {
      case attachment_site_t::vertices:
        return state_.register_state<T>(key, num_vertices(),
          attachment_site_t::vertices, attributes);
        break;
      case attachment_site_t::edges:
        return state_.register_state<T>(key, num_edges(),
          attachment_site_t::edges, attributes);
        break;
      case attachment_site_t::cells:
        return state_.register_state<T>(key, num_cells(),
          attachment_site_t::cells, attributes);
        break;
      case attachment_site_t::corners:
        return state_.register_state<T>(key, num_corners(),
          attachment_site_t::corners, attributes);
        break;
      default:
        assert(false && "Error: invalid state registration site.");
    } // switch

  } // register_state_

  /*!
    \brief Access state associated with \e key.

    \tparam T Data type of underlying state.
    \tparam NS The namespace in which the state variable is registered.
      See \ref state_t::register_state for additional information.

    \param[in] key The \e key for the state to access.

    \return Accessor to the state with \e key.
   */
  template<typename T, size_t NS = flecsi::flecsi_user_space>
  decltype(auto) access_state_(const const_string_t && key) {
    return state_.accessor<T,NS>(key);
  } // access_state_

  /*!
    \brief Access state registered with type \e T.

    \tparam T All state variables of this type will be returned.
    \tparam NS Namespace to use.

    \return A vector of accessors to state registered with type \e T.
   */
  template<typename T, size_t NS = flecsi::flecsi_user_space>
  decltype(auto) access_type_() {
    return state_.accessors<T,NS>();
  } // access_type_

  /*!
    \brief Access state registered with type \e T that matches predicate
      function of type \e P.

    \tparam T All state variables of this type will be returned that match
      predicate \e P will be returned.
    \tparam P Predicate function type.

    \param[in] predicate Predicate function.

    \return Accessors to the state variables of type \e T matching the
      predicate function.
   */
  template<typename T, typename P>
  decltype(auto) access_type_if_(P && predicate) {
    return state_.accessors<T,P>(std::forward<P>(predicate));
  } // access_type_if

  /*!
    \brief Return the attributes for the state with \e key.

    \param[in] key The \e key for the state to return attributes for.

    \return The attributes for the state with \e key.
   */
  decltype(auto) state_attributes_(const const_string_t && key) {
    return state_.meta_data<>((key)).attributes;
  } // state_attribtutes_

  /*--------------------------------------------------------------------------*
   * FIXME: Other crap
   *--------------------------------------------------------------------------*/

  // Geometry
  //! Floating point data type.
  using real_t = burton_mesh_traits_t::real_t;

  //! Point data type.
  using point_t = burton_mesh_traits_t::point_t;

  //! Physics vector type.
  using vector_t = burton_mesh_traits_t::vector_t;

  // Entity types
  //! Vertex type.
  using vertex_t = burton_mesh_types_t::vertex_t;

  //! Edge type.
  using edge_t = burton_mesh_types_t::edge_t;

  //! Cell type.
  using cell_t = burton_mesh_types_t::cell_t;

  //! 2D quadrilateral cell type.
  using quadrilateral_cell_t = burton_mesh_types_t::quadrilateral_cell_t;

  //! Corner type.
  using corner_t = burton_mesh_types_t::corner_t;

  //! Default constructor
  burton_mesh_t() {}

  //! Copy constructor (disabled)
  burton_mesh_t(const burton_mesh_t &) = delete;

  //! Assignment operator (disabled)
  burton_mesh_t &operator=(const burton_mesh_t &) = delete;

  //! Destructor
  ~burton_mesh_t() {}

  /*!
    \brief Return the topological dimension of the burton mesh.

    \return A non-negative number describing the highest dimension
      of the entities in the burton mesh, e.g., 3 for a three-dimensional
      burton mesh.
   */
  static constexpr auto dimension() {
    return burton_mesh_traits_t::dimension;
  } // dimension

  /*--------------------------------------------------------------------------*
   * Vertex Interface
   *--------------------------------------------------------------------------*/

  /*!
    \brief Return number of vertices in the burton mesh.

    \return The number of vertices in the burton mesh.
   */
  size_t num_vertices() const {
    return mesh_.num_entities<0,0>();
  } // num_vertices

  /*!
    \brief Return all vertices in the burton mesh.

    \return Return all vertices in the burton mesh as a sequence for use, e.g.,
      in range based for loops.
   */
  auto vertices() {
    return mesh_.entities<0,0>();
  } // vertices

  /*!
    \brief Return vertices associated with entity instance of type \e E.

    \tparam E entity type of instance to return vertices for.

    \param[in] e instance of entity to return vertices for.

    \return Return vertices associated with entity instance \e e as a
      sequence.
   */
  template <class E>
  auto vertices(E *e) {
    return mesh_.entities<0,0>(e);
  } // vertices

  /*!
    \brief Return vertices for entity \e e in domain \e M.

    \tparam M Domain.
    \tparam E Entity type to get vertices for.

    \param[in] e Entity to get vertices for.

    \return Vertices for entity \e e in domain \e M.
   */
  template<size_t M, class E>
  auto vertices(domain_entity<M, E> & e) {
    return mesh_.entities<0,M,0>(e.entity());
  }

  /*!
    \brief Return ids for all vertices in the burton mesh.

    \return Ids for all vertices in the burton mesh.
   */
  auto vertex_ids() {
    return mesh_.entity_ids<0,0>();
  } // vertex_ids

  /*!
    \brief Return vertex ids associated with entity instance of type \e E.

    \tparam E entity type of instance to return vertex ids for.

    \param[in] e instance of entity to return vertex ids for.

    \return Return vertex ids associated with entity instance \e e as a
      sequence.
   */
  template <class E>
  auto vertex_ids(E *e) {
    return mesh_.entity_ids<0,0>(e);
  } // vertex_ids

  /*--------------------------------------------------------------------------*
   * Edge Interface
   *--------------------------------------------------------------------------*/

  /*!
    \brief Return the number of burton mesh edges.

    \return The number of burton mesh edges.
   */
  size_t num_edges() const {
    return mesh_.num_entities<1,0>();
  } // num_edges

  /*!
    \brief Return all edges in the burton mesh.
    \return Return all edges in the burton mesh as a sequence for use, e.g., in
      range based for loops.
   */
  auto edges() {
    return mesh_.entities<1,0>();
  } // edges

  /*!
    \brief Return edges for entity \e e in domain \e M.

    \tparam M Domain.
    \tparam E Entity type to get edges for.

    \param[in] e Entity to get edges for.

    \return Edges for entity \e e in domain \e M.
   */
  template<size_t M, class E>
  auto edges(domain_entity<M, E>& e) {
    return mesh_.entities<1,M,0>(e.entity());
  } // edges

  /*!
    \brief Return edges associated with entity instance of type \e E.

    \tparam E entity type of instance to return edges for.

    \param[in] e instance of entity to return edges for.

    \return Return edges associated with entity instance \e e as a sequence.
   */
#if 0
  template <size_t FM, class E>
  auto edges(domain_entity<FM,E> & e) {
    return mesh_.entities<1,FM,0>(e.entity());
  } // edges
#endif

  /*!
    \brief Return ids for all edges in the burton mesh.

    \return Ids for all edges in the burton mesh.
   */
  auto edge_ids() {
    return mesh_.entity_ids<1,0>();
  } // edge_ids

  /*!
    \brief Return edge ids associated with entity instance of type \e E.

    \tparam E entity type of instance to return edge ids for.

    \param[in] e instance of entity to return edge ids for.

    \return Return edge ids associated with entity instance \e e as a sequence.
   */
  template <class E>
  auto edge_ids(E *e) {
    return mesh_.entity_ids<1,0>(e);
  } // edge_ids

  /*--------------------------------------------------------------------------*
   * Face Interface
   *--------------------------------------------------------------------------*/

  /*--------------------------------------------------------------------------*
   * Cell Interface
   *--------------------------------------------------------------------------*/

  /*!
    \brief Return the number of cells in the burton mesh.

    \return The number of cells in the burton mesh.
   */
  size_t num_cells() const {
    return mesh_.num_entities<dimension(),0>();
  } // num_cells

  /*!
    \brief Return all cells in the burton mesh.

    \return Return all cells in the burton mesh as a sequence for use, e.g.,
      in range based for loops.
   */
  auto cells() const {
    return mesh_.entities<dimension(),0>();
  } // cells

  /*!
    \brief Return all cells in the burton mesh.

    \return Return all cells in the burton mesh as a sequence for use, e.g.,
      in range based for loops.
   */
  auto cells() {
    return mesh_.entities<dimension(),0>();
  } // cells

  /*!
    \brief Return cells associated with entity instance of type \e E.

    \tparam E entity type of instance to return cells for.

    \param[in] e instance of entity to return cells for.

    \return Return cells associated with entity instance \e e as a sequence.
   */
  template <class E>
  auto cells(E *e) {
    return mesh_.entities<dimension(),0>(e);
  } // cells

  /*!
    \brief Return cells for entity \e e in domain \e M.

    \tparam M Domain.
    \tparam E Entity type to get cells for.

    \param[in] e Entity to get cells for.

    \return Cells for entity \e e in domain \e M.
   */
  template<size_t M, class E>
  auto cells(domain_entity<M, E>& e) {
    return mesh_.entities<dimension(),M,0>(e);
  } // cells

  /*!
    \brief Return ids for all cells in the burton mesh.

    \return Ids for all cells in the burton mesh.
   */
  auto cell_ids() {
    return mesh_.entity_ids<dimension(),0>();
  } // cell_ids

  /*!
    \brief Return cell ids associated with entity instance of type \e E.

    \tparam E entity type of instance to return cell ids for.

    \param[in] e instance of entity to return cell ids for.

    \return Return cell ids associated with entity instance \e e as a sequence.
   */
  template <class E>
  auto cell_ids(E *e) {
    return mesh_.entity_ids<dimension(),0>(e);
  } // cell_ids

  /*--------------------------------------------------------------------------*
   * Corner Interface
   *--------------------------------------------------------------------------*/

  /*!
    \brief Return number of corners in the burton mesh.

    \return The number of corners in the burton mesh.
   */
  size_t num_corners() const {
    return mesh_.num_entities<1,1>();
  } // num_corners

  /*!
    \brief Return all corners in the burton mesh.

    \return Return all corners in the burton mesh as a sequence for use, e.g.,
      in range based for loops.
   */
  auto corners() {
    return mesh_.entities<1,1>();
  } // corners

  /*!
    \brief Return corners associated with entity instance of type \e E.

    \tparam E entity type of instance to return corners for.

    \param[in] e instance of entity to return corners for.

    \return Return corners associated with entity instance \e e as a sequence.
   */
  template <class E>
  auto corners(E *e) {
    return mesh_.entities<1,1>(e);
  } // corners

#if 0
  /*!
    \brief Return corners for entity \e e in domain \e M.

    \tparam M Domain.
    \tparam E Entity type to get corners for.

    \param[in] e Entity to get corners for.

    \return Corners for entity \e e in domain \e M.
   */
  template<size_t M, class E>
  auto corners(domain_entity<M, E> & e) {
    return mesh_.entities<1, M>(e);
  } 
#endif

  /*!
    \brief Return ids for all corners in the burton mesh.

    \return Ids for all corners in the burton mesh.
   */
  auto corner_ids() {
    return mesh_.entity_ids<1,1>();
  } // corner_ids

  /*!
    \brief Return corner ids associated with entity instance of type \e E.

    \tparam E entity type of instance to return corner ids for.

    \param[in] e instance of entity to return corner ids for.

    \return Return corner ids associated with entity instance \e e as a
      sequence.
   */
  template <class E>
  auto corner_ids(E *e) {
    return mesh_.entity_ids<1,1>(e);
  } // corner_ids

//
//
//
//
//

  /*!
    \brief Create a vertex in the burton mesh.

    \param[in] pos The position (coordinates) for the vertex.

    \return Pointer to a vertex created at \e pos.
   */
  // FIXME: Complete changes to state storage
  vertex_t * create_vertex(const point_t &pos) {
    auto p = access_state_<point_t,flecsi::flecsi_internal>("coordinates");
    p[num_vertices()] = pos;

    auto v = mesh_.make<vertex_t>(state_);
    mesh_.add_entity<0, 0>(v);

    return v;
  }

  /*!
    \brief Create a cell in the burton mesh.

    \param[in] verts The vertices defining the cell.

    \return Pointer to cell created with \e verts.
   */
  cell_t * create_cell(std::initializer_list<vertex_t *> verts) {
    // FIXME: Add element types
    cell_t * c = mesh_.make<quadrilateral_cell_t>(mesh_);
    mesh_.add_entity<dimension(), 0>(c);

    // FIXME: Need to make mesh interface more general
    mesh_.init_cell<0>(c, verts);
    return c;
  } // create_cell

  /*!
    \brief Dump the burton mesh to standard out.
   */
  void dump(){
    mesh_.dump();
  }

  /*!
    \brief Initialize burton mesh state for the number of \e vertices.

    \param[in] vertices The number of \e vertices to initialize the burton mesh
      with.
   */
  void init_parameters(size_t vertices) {

    // register coordinate state
    state_.register_state<point_t,flecsi::flecsi_internal>("coordinates", vertices,
      attachment_site_t::vertices, flecsi::persistent);

  } // init_parameters

  /*!
    \brief Initialize the burton mesh.

    \verbatim

    After cells and vertices for the mesh have been defined, call init() to
    create the dual mesh entities. The following picture shows the vertex
    definitions used to create the dual mesh. For each vertex definition there
    is a corresponding point_t variable containing the geometry of the vertex,
    which is computed from the primal mesh vertex coordinates.

    v3-----e2-----v2
    |              |
    |              |
    e3     cv     e1
    |              |
    |              |
    v0-----e0-----v1

    The wedge indexing is shown below. A wedge is defined by three vertices in
    the order w = {cv, v*, e*}.

    v3------e2-------v2
    | \      |      / |
    |   \  w6|w5  /   |
    |  w7 \  |  / w4  |
    |       \|/       |
    e3------cv-------e1
    |       /|\       |
    |  w0 /  |  \ w3  |
    |   /  w1|w2  \   |
    | /      |      \ |
    v0------e0-------v1

    Corners are defined by the two wedges sharing the center vertex cv and a
    primal vertex v*.
    c0 = {w0, w1}
    c1 = {w2, w3}
    c2 = {w4, w5}
    c3 = {w6, w7}

    \endverbatim
   */
  void init() {
    mesh_.init<0>();
    mesh_.init_bindings<1>();

    // Create wedges
    for(auto c: mesh_.entities<1,1>()) {
      auto vertex = vertices(c).first();
      auto edge0 = edges(c).first();
      auto edge1 = edges(c).last();
      auto cell = cells(c).first();
    } // for
  } // init

private:

  mesh_t mesh_;

  burton_mesh_traits_t::mesh_state_t state_;

}; // class burton_mesh_t

} // namespace mesh
} // namespace ale


/*~-------------------------------------------------------------------------~-*
 * Formatting options
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
