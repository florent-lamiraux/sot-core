/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Copyright Projet JRL-Japan, 2007
 *+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *
 * File:      sotFlags.cpp
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



/* --------------------------------------------------------------------- */
/* --- INCLUDE --------------------------------------------------------- */
/* --------------------------------------------------------------------- */

/*! System framework */
#include <stdlib.h>

/*! Local Framework */
#include <sot-core/sotFlags.h>
#include <sot-core/sotDebug.h>

using namespace std;
/* --------------------------------------------------------------------- */
/* --- CLASS ----------------------------------------------------------- */
/* --------------------------------------------------------------------- */


static void displaybool( ostream& os, const char c, const bool reverse=false )
{
  for( unsigned int j=sizeof(char)*8;j-->0; )
    {
      //os<<i<<","<<j<<": "<<c+0<<std::endl; 
      if(reverse) os<< (!((c>>j)&0x1)); else os<< (((c>>j)&0x1));//?"1":"0");
    }
}
static string displaybool(const char c, const bool reverse=false )
{
  stringstream oss;
  for( unsigned int j=sizeof(char)*8;j-->0; )
    {
      //os<<i<<","<<j<<": "<<c+0<<std::endl; 
      if(reverse) oss<< (!((c>>j)&0x1)); else oss<< (((c>>j)&0x1));//?"1":"0");
    }
  return oss.str();
}
/* --------------------------------------------------------------------- */

  
sotFlags::
sotFlags( const bool& b ) : flags(),reverse(b) { }

sotFlags::
sotFlags( const char& c ) : flags(),reverse(false) { add(c); }

sotFlags::
sotFlags( const int& c4 ) : flags(),reverse(false) { add(c4);}

sotFlags::
operator bool ( void ) const
{
  if(reverse) return true;
  for( unsigned int i=0;i<flags.size();++i ) if( flags[i] ) return true;
  return false;
}

/* --------------------------------------------------------------------- */
char sotFlags::
operator[] (const unsigned int& i) const
{ 
  char res;
  if( i<flags.size() )
    res = flags[i];
  else res=0; 
  //cout<<"["<<i<<"] "<<res+0<<"||"<<(!res)+0<<std::endl;
  if( reverse ) return ~res;//(!res);
  return res;
}

char operator>> (const sotFlags& f,const int& i)
{
  const div_t q = div(i,8); 

  char res = f[q.quot] >> q.rem;
  res |= f[q.quot+1] << (8-q.rem);
  
  return res;
}

bool sotFlags::
operator() (const int& i) const
{
  return ((*this)>>i)&0x01;
}


/* --------------------------------------------------------------------- */
void sotFlags::
add( const char& c ) 
{ flags.push_back( c );    }

void sotFlags::
add( const int& c4 ) 
{
  const char* c4p = (const char*)&c4; 
  for(unsigned int i=0;i<sizeof(int);++i) add(c4p[i]);
}
 

/* --------------------------------------------------------------------- */
sotFlags sotFlags::
operator! (void) const
{
  sotFlags res = *this;
  res.reverse=!reverse; 
  return res;
}

sotFlags operator& ( const sotFlags& f1,const sotFlags& f2 ) 
{
  sotFlags res = f1; res &= f2; return res;
}

sotFlags operator| ( const sotFlags& f1,const sotFlags& f2 ) 
{
  sotFlags res = f1; res |= f2; return res;
}

sotFlags& sotFlags::
operator&= ( const sotFlags& f2 ) 
{ 
  sotFlags &f1=*this;
  const unsigned int max=std::max(flags.size(),f2.flags.size());
  if( flags.size()<max ){ flags.resize(max); }
  bool revres = reverse&&f2.reverse;
  char c; int pos = 0;
  for( unsigned int i=0;i<max;++i )
    {
      c=f1[i]&f2[i]; if(revres)  c=0xff-c; 
      flags[i]=c; if(c) pos=i+1;
    }
  flags.resize(pos);reverse=revres;
  return *this;
}

sotFlags& sotFlags::
operator|= ( const sotFlags& f2 ) 
{ 
  sotFlags &f1=*this;
  const unsigned int max=std::max(flags.size(),f2.flags.size());
  if( flags.size()<max ){ flags.resize(max); }
  bool revres = reverse||f2.reverse;
  char c; int pos = 0;
  for( unsigned int i=0;i<max;++i )
    {
      //cout<<"DGi ";displaybool(cout,f1[i],false); cout<<" "; displaybool(cout,f2[i],false); cout<<endl; 
      c=f1[i]|f2[i]; 
      //cout<<"DGr ";displaybool(cout,c,false); cout<<" "; displaybool(cout,c,revres); cout<<endl; 
      if(revres)  c=0xff-c; 
      flags[i]=c; if(c) pos=i+1;
    }
  flags.resize(pos); reverse=revres;
  return *this;
}

sotFlags operator& ( const sotFlags& f1,const bool& b ){ if(b)return f1; else return sotFlags();}
sotFlags operator| ( const sotFlags& f1,const bool& b ){ if(b)return sotFlags(true); else return f1;}
sotFlags& sotFlags::
operator&= ( const bool& b ){ if(!b) { flags.clear(); reverse=false; } return *this; }
sotFlags& sotFlags::
operator|= ( const bool& b ){ if(b) { flags.clear(); reverse=true; } return *this;}

/* --------------------------------------------------------------------- */
void sotFlags::
set( const unsigned int & idx )
{
  unsigned int d= (idx/8), m=(idx%8);
  
  char brik = (reverse)?(255-(1<<m)):(1<<m);

  if( flags.size()>d )
    {
      sotDEBUG(45) << "List long enough. Modify." << std::endl;
      char & el = flags[d];
      if( reverse ) el &= brik; else el |= brik;
    }
  else
    {
      sotDEBUG(45) << "List not long enough. Add " 
		   << flags.size() <<" "<<d << std::endl;
      if(! reverse )
	{
	  for( unsigned i=flags.size();i<d;++i ) add((char)0);
	  add(brik);
	}
    }
  sotDEBUG(45) << "New flag: "<< *this << endl;
}

void sotFlags::
unset( const unsigned int & idx )
{
  unsigned int d= (idx/8), m=(idx%8);
  
  char brik = (reverse)?(1<<m):(255-(1<<m));
  if( flags.size()>d )
    {
      sotDEBUG(45) << "List long enough. Modify." << std::endl;
      char & el = flags[d];
      if( reverse ) el |= brik;  else el &= brik;
    }
  else
    {
      sotDEBUG(45) << "List not long enough. Add." << std::endl;
     if( reverse )
       {
	 for( unsigned i=flags.size();i<d;++i ) add((char)255);
	 add(brik);
       }
    }
  sotDEBUG(45) << "New flag: "<< *this << endl;
}



/* --------------------------------------------------------------------- */
std::ostream& operator<< (std::ostream& os, const sotFlags& fl )
{
  if( fl.reverse ) os << "...11111 ";
  unsigned int s = fl.flags.size();
  for( unsigned int i=0;i<fl.flags.size();++i )
    {
      char c=fl.flags[s-i-1];
      displaybool(os,c,fl.reverse);
      os<<" " ;
    }
  return os;
}

#include <list>

static char MASK [] = { 0,1,3,7,15,31,63,127,255 };

std::istream& operator>> (std::istream& is, sotFlags& fl )
{
  sotDEBUGIN(15);
  std::list<char> listing;
  unsigned char count = 0,total = 0;
  char c,cur=0;
  bool reverse=false,contin=true;
  do
    {
      is.get(c);
      sotDEBUG(25) << "Read " << total+0 <<endl;

      total++;
      if(! is.good() ) break;
      switch(c)
	{
	case '.':
	  {
	    if(total<=3)
	      {
		if(total==3) { reverse=true; sotDEBUG(20) << " Reverse" << endl; }
	      }
	    else total=10;
	    break;
	  }
	case '0': cur &= (~(0x01 << (7-count++))) ; break;
	case '1': cur |= (0x01 << (7-count++)) ; break;
	case '#': 
	  {
	    char cnot; is.get(cnot);
	    if( cnot=='!' ) 
	      fl = (! sotFlags::readIndexMatlab( is ));
	    else 
	      { is.unget(); fl = sotFlags::readIndexMatlab( is ); }

	    return is;
	  }
	case '&': 
	  {
	    char cnot; is.get(cnot);
	    if( cnot=='!' ) 
	      fl &= (! sotFlags::readIndexMatlab( is ));
	    else 
	      { is.unget(); fl &= sotFlags::readIndexMatlab( is ); }
	    return is;
	  }
	case '|': 
	  {
	    char cnot; is.get(cnot);
	    if( cnot=='!' ) 
	      fl |= (! sotFlags::readIndexMatlab( is ));
	    else 
	      { is.unget(); fl |= (sotFlags::readIndexMatlab( is )); }
	    return is;
	  }
	default: 
	  is.unget();
	  contin=false;
	}
      sotDEBUG(25) << "Get cur= " << displaybool(cur) <<endl;
     
      if( count==8 )
	{
	  sotDEBUG(20) << "Store " << displaybool(cur) <<endl;
	  count=0; 
	  listing.push_front(cur); 
	  cur=0;
	}
    }while( contin );

  sotDEBUG(20) << "finish with " << displaybool(cur) <<  " ("<<0+count<<")"<<endl;
  char insert = 0;	
  fl.flags.resize(listing.size()+((count>0)?1:0));
  total=0;
  for( std::list<char>::iterator iter = listing.begin();iter!=listing.end();++iter )
    {
      insert = ((*iter)<<count);
      insert &= (~MASK[count]);
      cur = (MASK[count])&(cur>>(8-count));

      insert |= cur;
      cur = *iter; 
      
      if( reverse ) fl.flags[total++] = ~insert;
      else fl.flags[total++] = insert;
      sotDEBUG(25) << "Insert " << displaybool(insert) <<endl;
    }

  if( count>0 )
    {
      sotDEBUG(25) << "Cur fin " << displaybool(cur) <<endl;
      cur = (MASK[count])&(cur>>(8-count));
      
      sotDEBUG(25) << "Cur fin >> " <<8-count<<":" << displaybool(cur) <<endl;
      sotDEBUG(25) << "Mask fin "<<0+count<<": " << displaybool(MASK[count]) <<endl;
      
      cur &= MASK[count];
      if( reverse ) 
	{
	  cur |= ~MASK[count];
	  fl.flags[total++] = ~cur;
	}
      else
	{
	  cur &= MASK[count];
	  fl.flags[total++] = cur;
	}
      sotDEBUG(25) << "Insert fin " << displaybool(cur) <<endl;
    }

  fl.reverse=reverse;

  sotDEBUGOUT(15);
  return is;
}


/* --------------------------------------------------------------------- */
const sotFlags FLAG_LINE_1( (char)0x1 );
const sotFlags FLAG_LINE_2( (char)0x2 );
const sotFlags FLAG_LINE_3( (char)0x4 );
const sotFlags FLAG_LINE_4( (char)0x8 );
const sotFlags FLAG_LINE_5( (char)0x10 );
const sotFlags FLAG_LINE_6( (char)0x20 );
const sotFlags FLAG_LINE_7( (char)0x40 );
const sotFlags FLAG_LINE_8( (char)0x80 );

/* --------------------------------------------------------------------- */

void sotFlags::
readIndexMatlab( std::istream& cmdArgs,
		 unsigned int & idx_beg,
		 unsigned int &idx_end,
		 bool& no_end )
{
  char col;

  cmdArgs >> ws; 
  if(! cmdArgs.good()) { idx_end=idx_beg=0; no_end=false; return; } 
  cmdArgs.get(col); if( col==':' )
    { idx_beg=0; cmdArgs>>ws;} 
  else 
    { 
      cmdArgs.putback(col); cmdArgs>>idx_beg>>ws;
      cmdArgs.get(col); 
      if( col!=':' ) { idx_end=idx_beg; no_end=false; return; }
    }
  cmdArgs>>ws;
  if( cmdArgs.good() ) 
    {
      sotDEBUG(15) << "Read end" << endl;
      cmdArgs >> idx_end; no_end=false;
    }
  else no_end = true;
  
  sotDEBUG(25) <<"Selec: " << idx_beg << " : "<< idx_end 
	       << "(" << no_end <<")"<<endl;
}

sotFlags sotFlags::
readIndexMatlab( std::istream& cmdArgs )
{
  sotDEBUGIN(15) ;
  unsigned int idxEnd,idxStart;
  bool idxUnspec;
  
  readIndexMatlab( cmdArgs,idxStart,idxEnd,idxUnspec );
  
  sotFlags newFlag( idxUnspec );
  if( idxUnspec )
    {    for( unsigned int i=0;i<idxStart;++i )       newFlag.unset(i);  }
  else { for( unsigned int i=idxStart;i<=idxEnd;++i ) newFlag.set(i);    }
  
  sotDEBUG(25) << newFlag <<std::endl;
  sotDEBUGOUT(15) ;
  return newFlag;
}