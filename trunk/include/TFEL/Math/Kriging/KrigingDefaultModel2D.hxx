/*! 
 * \file  KrigingDefaultModel2D.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 */

#ifndef _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL2D_H_
#define _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL2D_H_ 

#include<cmath>
#include<limits>

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/Kriging/KrigingVariable.hxx"
#include"TFEL/Math/Kriging/KrigingDefaultNuggetModel.hxx"

namespace tfel
{

  namespace math
  {

    template<typename T>
    struct KrigingDefaultModel<2u,T>
      : public KrigingDefaultNuggetModel<2u,T>
    {
      static TFEL_MATH_INLINE T
      one(const typename KrigingVariable<2u,T>::type&)
      {
	return T(1);
      }

      static TFEL_MATH_INLINE T
      x(const typename KrigingVariable<2u,T>::type& v)
      {
	return v(0);
      }

      static TFEL_MATH_INLINE  T
      y(const typename KrigingVariable<2u,T>::type& v)
      {
	return v(1);
      }

      static TFEL_MATH_INLINE T
      covariance(const typename KrigingVariable<2u,T>::type& v)
      {
	using namespace std;
	T h2 = v(0)*v(0)+v(1)*v(1);
	if(h2<10*numeric_limits<T>::epsilon()){
	  return T(0);
	}
	return 0.5*h2*log(h2);
      } // end of covariance

      typedef T (*Drifts)(const typename KrigingVariable<2u,T>::type&);

      static const unsigned short nb = 3u; /* number of drifts */
      static const Drifts drifts[3u];
    };

    template<typename T>
    const typename KrigingDefaultModel<2u,T>::Drifts
    KrigingDefaultModel<2u,T>::drifts[3u] = {KrigingDefaultModel<2u,T>::one,
					     KrigingDefaultModel<2u,T>::x,
					     KrigingDefaultModel<2u,T>::y};

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_KRIGINGDEFAULTMODEL2D_H */

