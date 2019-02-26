/*
 * Copyright 2010, CNRS/AIST
 * Copyright 2011-2019 LAAS, CNRS
 *
 * Julian Viereck
 *
 * See LICENSE file
 *
 */

#ifndef __SOT_GRADIENTASCENT_H__
#define __SOT_GRADIENTASCENT_H__

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#include <sot/core/config.hh>
#include <dynamic-graph/entity.h>
#include <dynamic-graph/signal-ptr.h>
#include <dynamic-graph/signal-time-dependent.h>

namespace dg = ::dynamicgraph;

namespace dynamicgraph {
  namespace sot {

/* --------------------------------------------------------------------- */
/* --- TRACER ---------------------------------------------------------- */
/* --------------------------------------------------------------------- */

using dynamicgraph::Entity;
using dynamicgraph::SignalPtr;
using dynamicgraph::SignalTimeDependent;

class SOT_CORE_DLLAPI GradientAscent
: public Entity
{
  DYNAMIC_GRAPH_ENTITY_DECL();
 public:

  SignalPtr< dg::Vector,int > gradientSIN;
  SignalPtr< double,int > learningRateSIN;
  SignalTimeDependent<int,int> refresherSINTERN;
  SignalTimeDependent<dg::Vector,int> valueSOUT;

 public:
  GradientAscent( const std::string& n );
  virtual ~GradientAscent( void );

 protected:

  dg::Vector& update(dg::Vector& res, const int& inTime);

  dg::Vector value;

  double alpha;
  bool init;

};

  } /* namespace sot */
} /* namespace dynamicgraph */

#endif /* #ifndef __SOT_TRACER_H__ */


