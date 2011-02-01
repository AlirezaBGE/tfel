/*! 
 * \file  FactorizedKriging.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 12 avr 2009
 */

#ifndef _LIB_TFEL_MATH_FACTORIZEDKRIGING_H_
#define _LIB_TFEL_MATH_FACTORIZEDKRIGING_H_ 

#include"TFEL/Math/Kriging.hxx"

namespace tfel
{

  namespace math
  {

    template<typename Model>
    struct KrigingModelAdaptator
      : public Model
    {
      typedef typename Model::Drifts Drifts;
      static const unsigned short nb = Model::nb-1u; /* number of drifts */
      static const Drifts* drifts;
    };

    template<typename Model>
    const typename KrigingModelAdaptator<Model>::Drifts*
    KrigingModelAdaptator<Model>::drifts = Model::drifts+1u;
    
    template<unsigned short N,
	     unsigned short M,
	     typename T = double,
	     typename Model1 = KrigingDefaultModel<N,T>,
	     typename Model2 = KrigingModelAdaptator<KrigingDefaultModel<M,T> > >
    struct FactorizedKriging
    {

      FactorizedKriging();
      
      FactorizedKriging(const Model1&,
			const Model2&);

      void
      addValue(const typename KrigingVariable<N,T>::type&,
	       const typename KrigingVariable<M,T>::type&,
	       const T&);

      void
      buildInterpolation(void);
  
      T
      operator()(const typename KrigingVariable<N,T>::type&,
		 const typename KrigingVariable<M,T>::type&) const;

    private:

      Model1 m1;
      Model2 m2;

      tfel::math::vector<typename KrigingVariable<N,T>::type> x1;
      tfel::math::vector<typename KrigingVariable<M,T>::type> x2;
      tfel::math::vector<T> f;
      tfel::math::vector<T> a;

    }; // end of struct FactorizedKriging

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Kriging/FactorizedKriging.ixx"

#endif /* _LIB_TFEL_MATH_FACTORIZEDKRIGING_H */

