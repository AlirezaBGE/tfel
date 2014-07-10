/*!
 * \file   tvector-header.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 f�v 2008
 */

#ifndef _LIB_TFEL_TVECTOR_HEADER_HXX_
#define _LIB_TFEL_TVECTOR_HEADER_HXX_ 

#include<cstdlib>
#include<cassert>
#include<cmath>
#include<limits>
#include"TFEL/Math/functions.hxx"
#include"TFEL/Math/tvector.hxx"

template<typename T>
bool test1(void);

template<typename T>
bool test1(void)
{
  using namespace std;
  using namespace tfel::math;
  USING_TFEL_FUNCTIONS;
  const T eps = 10*numeric_limits<T>::epsilon();
  tvector<3,T> v;
  tvector<3,T> v2;
  tvector<3,T> v3;
  v(0) = T(2.);
  v(1) = T(7.);
  v(2) = T(9.);
  v2(0) = T(2.34);
  v2(1) = T(54.2);
  v2(2) = T(412.232);
  v3 = exp(sin(T(2.)*v+T(3.5)*cos(v2))); 
  if(!(abs(v3(0)-exp(sin(T(2)*v(0)+T(3.5)*cos(v2(0)))))<eps)){
    return false;
  }
  if(!(abs(v3(1)-exp(sin(T(2)*v(1)+T(3.5)*cos(v2(1)))))<eps)){
    return false;
  }
  if(!(abs(v3(2)-exp(sin(T(2)*v(2)+T(3.5)*cos(v2(2)))))<eps)){
    return false;
  }
  return true;
}

bool test2(void);

bool test3(void);

#endif /* _LIB_TFEL_TVECTOR_HEADER_HXX */

