/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      sotFeatureAbstract.h
 * Project:   SOT
 * Author:    Nicolas Mansard
 *
 * Version control
 * ===============
 *
 *  $Id$
 *
 * Description
 * ============
 *
 *
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

#include <sot-core/sotFeatureAbstract.h>
#include <sot-core/sotPool.h>

const std::string 
sotFeatureAbstract::CLASS_NAME = "FeatureAbstract";


sotFeatureAbstract::
sotFeatureAbstract( const std::string& name ) 
  :Entity(name)
   ,desiredValueSIN(NULL,"sotFeatureAbstract("+name+")::input(feature)::sdes")
   ,selectionSIN(NULL,"sotFeatureAbstract("+name+")::input(flag)::selec")
   ,errorSOUT( boost::bind(&sotFeatureAbstract::computeError,this,_1,_2),
	       selectionSIN<<desiredValueSIN,
	       "sotFeatureAbstract("+name+")::output(vector)::error" )
   ,jacobianSOUT( boost::bind(&sotFeatureAbstract::computeJacobian,this,_1,_2),
		  selectionSIN,
		  "sotFeatureAbstract("+name+")::output(matrix)::jacobian" )
   ,activationSOUT( boost::bind(&sotFeatureAbstract::computeActivation,this,_1,_2),
		    selectionSIN<<desiredValueSIN,
		    "sotFeatureAbstract("+name+")::output(vector)::activation" )
   ,dimensionSOUT( boost::bind(&sotFeatureAbstract::getDimension,this,_1,_2),
		   selectionSIN,
		   "sotFeatureAbstract("+name+")::output(uint)::dim" )
{
  selectionSIN = true;
  signalRegistration( desiredValueSIN<<selectionSIN
		      <<errorSOUT<<jacobianSOUT<<activationSOUT<<dimensionSOUT );
  featureRegistration();

}


void sotFeatureAbstract::
featureRegistration( void )
{
  sotPool.registerFeature(name,this);
}


std::ostream& sotFeatureAbstract::
writeGraph( std::ostream& os ) const
{
  Entity::writeGraph(os);

  if( desiredValueSIN )
    {
      //      const SignalAbstract<int> & sdesAbs = desiredValueSIN;
      const SignalPtr<sotFeatureAbstract *,int>  & sdesSig = desiredValueSIN;
      
      if (sdesSig!=0)
	{
	  sotFeatureAbstract *asotFA = sdesSig.accessCopy();
	  if (asotFA!=0)
	    {
	      os << "\t\"" << asotFA->getName() << "\" -> \"" << getName() << "\""
		 << "[ color=darkseagreen4 ]" << std::endl;
	    }
	  else std::cout << "asotFAT : 0" << std::endl;
	}
      else std::cout << "sdesSig : 0" << std::endl;
    }

  return os;
}