/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      sotExceptionTask.h
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


#ifndef __SOT_EXCEPTION_TASK_H
#define __SOT_EXCEPTION_TASK_H

/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */


#include <sot-core/sotExceptionAbstract.h>
#include <sot-core/sot-core-api.h>
/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */


/* \class sotExceptionTask
 */
class SOT_CORE_EXPORT sotExceptionTask 
:public sotExceptionAbstract

{
public:

  enum ErrorCodeEnum
    {
      GENERIC = sotExceptionAbstract::TASK
      ,EMPTY_LIST
      ,NON_ADEQUATE_FEATURES
      ,MATRIX_SIZE
      ,BOUND_TYPE
      ,PARSER_MULTI_BOUND
    };

  static const std::string EXCEPTION_NAME;
  virtual const std::string& getExceptionName( void ) const { return EXCEPTION_NAME; }

  sotExceptionTask ( const sotExceptionTask::ErrorCodeEnum& errcode,
		     const std::string & msg = "" );
  sotExceptionTask( const sotExceptionTask::ErrorCodeEnum& errcode,
		    const std::string & msg,const char* format, ... );
  virtual ~sotExceptionTask( void ){}

};





#endif /* #ifndef __SOT_EXCEPTION_TASK_H */

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */