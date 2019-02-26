/*
 * Copyright 2010, CNRS/AIST
 * Copyright 2011-2019 LAAS, CNRS
 *
 * Julian Viereck
 *
 * See LICENSE file
 *
 */

#ifndef __SOT_EXPMOVINGAVG_H__
#define __SOT_EXPMOVINGAVG_H__

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

class SOT_CORE_DLLAPI ExpMovingAvg
: public Entity
{
  DYNAMIC_GRAPH_ENTITY_DECL();
 public:

  SignalPtr< dg::Vector,int > updateSIN;
  SignalTimeDependent<int,int> refresherSINTERN;
  SignalTimeDependent<dg::Vector,int> averageSOUT;

 public:
  ExpMovingAvg( const std::string& n );
  virtual ~ExpMovingAvg( void );

  void setAlpha(const double& alpha_);

 protected:

  dg::Vector& update(dg::Vector& res, const int& inTime);

  dg::Vector average;

  double alpha;
  bool init;

};

  } /* namespace sot */
} /* namespace dynamicgraph */

#endif /* #ifndef __SOT_TRACER_H__ */


