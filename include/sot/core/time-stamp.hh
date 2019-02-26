/*
 * Copyright 2010, CNRS/AIST
 * Copyright 2011-2019 LAAS, CNRS
 *
 * Florent Lamiraux
 * Guilhem Saurel
 * Rohan Budhiraja
 *
 * See LICENSE file
 *
 */

#ifndef __SOT_TIME_STAMP__HH
#define __SOT_TIME_STAMP__HH

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */


/* Matrix */
#include <dynamic-graph/linear-algebra.h>
namespace dg = dynamicgraph;

/* Classes standards. */
#ifndef WIN32
#include <sys/time.h>
#else /*WIN32*/
#include <sot/core/utils-windows.hh>
#endif /*WIN32*/

/* SOT */
#include <dynamic-graph/entity.h>
#include <dynamic-graph/all-signals.h>
#include <sot/core/debug.hh>

/* --------------------------------------------------------------------- */
/* --- API ------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

#if defined (WIN32)
#  if defined (time_stamp_EXPORTS)
#    define TimeStamp_EXPORT __declspec(dllexport)
#  else
#    define TimeStamp_EXPORT __declspec(dllimport)
#  endif
#else
#  define TimeStamp_EXPORT
#endif

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

namespace dynamicgraph { namespace sot {
namespace dg = dynamicgraph;

class TimeStamp_EXPORT TimeStamp
:public dg::Entity
{
 public:
  static const std::string CLASS_NAME;
  virtual const std::string& getClassName( void ) const { return CLASS_NAME; }

 protected:

  struct timeval val;
  unsigned int offsetValue;
  bool offsetSet;

 public:

  /* --- CONSTRUCTION --- */
  TimeStamp( const std::string& name );

 public: /* --- DISPLAY --- */
  virtual void display( std::ostream& os ) const;

 public: /* --- SIGNALS --- */

  /* These signals can be called several time per period, given
   * each time a different results. Useful for chronos. */
  dg::Signal<dg::Vector,int> timeSOUT;
  dg::Signal<double,int> timeDoubleSOUT;

  /* These signals can be called several time per period, but give
   * always the same results different results. Useful for synchro. */
  dg::SignalTimeDependent<dg::Vector,int> timeOnceSOUT;
  dg::SignalTimeDependent<double,int> timeOnceDoubleSOUT;


 protected: /* --- SIGNAL FUNCTIONS --- */
  dg::Vector& getTimeStamp( dg::Vector& res,const int& time );
  double& getTimeStampDouble( const dg::Vector& vect,double& res );

};


} /* namespace sot */} /* namespace dynamicgraph */


#endif /* #ifndef __SOT_SOT_HH */
