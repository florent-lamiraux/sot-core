/*
 * Copyright 2010,
 * François Bleibel,
 * Olivier Stasse,
 *
 * CNRS/AIST
 *
 * This file is part of sot-core.
 * sot-core is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 * sot-core is distributed in the hope that it will be
 * useful, but WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.  You should
 * have received a copy of the GNU Lesser General Public License along
 * with sot-core.  If not, see <http://www.gnu.org/licenses/>.
 */

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/* --- SOT --- */
#include <sot/core/joint-limitator.hh>
#include <sot/core/exception-feature.hh>
#include <sot/core/debug.hh>
#include <sot/core/factory.hh>


using namespace std;
using namespace dynamicgraph::sot;
using namespace dynamicgraph;

DYNAMICGRAPH_FACTORY_ENTITY_PLUGIN(JointLimitator,"JointLimitator");

/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */

const double JointLimitator::THRESHOLD_DEFAULT = .99;


JointLimitator::
JointLimitator( const string& fName )
  : Entity( fName )
    ,threshold(THRESHOLD_DEFAULT)
//     ,freeFloatingIndex( FREE_FLOATING_INDEX )
//     ,freeFloatingSize( FREE_FLOATING_SIZE )

    ,jointSIN( NULL,"JointLimitator("+name+")::input(vector)::joint" )
    ,upperJlSIN( NULL,"JointLimitator("+name+")::input(vector)::upperJl" )
    ,lowerJlSIN( NULL,"JointLimitator("+name+")::input(vector)::lowerJl" )
    ,controlSIN( NULL,"JointLimitator("+name+")::input(vector)::controlIN" )
    ,controlSOUT( boost::bind(&JointLimitator::computeControl,this,_1,_2),
		  jointSIN<<upperJlSIN<<lowerJlSIN<<controlSIN,
		  "JointLimitator("+name+")::output(vector)::control" )
    ,widthJlSINTERN( boost::bind(&JointLimitator::computeWidthJl,this,_1,_2),
		     upperJlSIN<<lowerJlSIN,
		     "JointLimitator("+name+")::input(vector)::widthJl" )
  
{
  signalRegistration( jointSIN<<upperJlSIN<<lowerJlSIN
		      <<controlSIN<<controlSOUT<<widthJlSINTERN );
}


/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */
/* --------------------------------------------------------------------- */

ml::Vector&
JointLimitator::computeWidthJl( ml::Vector& res,const int& time )
{
  sotDEBUGIN(15);

  const ml::Vector UJL = upperJlSIN.access(time);
  const ml::Vector LJL = lowerJlSIN.access(time);
  const unsigned int SIZE=UJL.size();
  res.resize(SIZE);

  for( unsigned int i=0;i<SIZE;++i )
    {      res(i)=UJL(i)-LJL(i);    }

  sotDEBUGOUT(15);
  return res;
}


ml::Vector&
JointLimitator::computeControl( ml::Vector& qdot,int time )
{
  sotDEBUGIN(15);

  const ml::Vector& q = jointSIN.access(time);
  const ml::Vector& UJL = upperJlSIN.access(time);
  const ml::Vector& LJL = lowerJlSIN.access(time);
  //const ml::Vector WJL = widthJlSINTERN.access(time);
  const ml::Vector& qdotIN = controlSIN.access(time);

  unsigned int SIZE = qdotIN.size();
  qdot.resize(SIZE); qdot.fill(0.);

  /* TODO: change this */
  unsigned int STOPSIZE = q.size() - 6;
  if(STOPSIZE > SIZE){ STOPSIZE = SIZE; }

  for( unsigned int i=0;i<STOPSIZE;++i )
    {
      double qnext = q(i+6)+qdotIN(i)*0.005;
      if( (qnext<UJL(i+6))&&(qnext>LJL(i+6)) ) qdot(i)=qdotIN(i);
      sotDEBUG(25) <<i<<": " <<qnext<<" in? [" << LJL(i)<<","<<UJL(i)<<"]"<<endl;
    }

  sotDEBUGOUT(15);
  return qdot;
}


void JointLimitator::
display( std::ostream& os ) const
{
  

  os <<"JointLimitator <"<<name<<"> ... TODO";

}




void JointLimitator::
commandLine( const std::string& cmdLine,
	     std::istringstream& cmdArgs,
	     std::ostream& os )
{
  if( cmdLine == "help" )
    {
      os << "JointLimitator:"<<endl
	 <<"  - setThreshold  <int> " <<endl
	 <<"  - getThreshold  " <<endl;
    }
  else if( cmdLine == "setThreshold" )
    {
      double th;
      cmdArgs >> th; if( th<0. ) th=0.;if( th>1. ) th=1.;
      threshold = th;
    }
  else if( cmdLine == "getThreshold" )
    {
      os << "threshold = " << threshold <<endl; 
    }
  else { Entity::commandLine( cmdLine,cmdArgs,os ); }
}



/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */