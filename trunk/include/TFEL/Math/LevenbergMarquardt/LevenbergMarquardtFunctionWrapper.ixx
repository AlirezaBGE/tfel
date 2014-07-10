/*! 
 * \file  LevenbergMarquardtFunctionWrapper.ixx
 * \brief
 * \author Helfer Thomas
 * \brief 24 f�v 2010
 */

#ifndef _LIB_LEVENBERGMARQUARDTFUNCTIONWRAPPER_I_
#define _LIB_LEVENBERGMARQUARDTFUNCTIONWRAPPER_I_ 

namespace tfel
{

  namespace math
  {

    template<typename T>
    LevenbergMarquardtFunctionWrapper<T>::LevenbergMarquardtFunctionWrapper(const LevenbergMarquardtFunctionWrapper<T>::size_type nv_,
									    const LevenbergMarquardtFunctionWrapper<T>::size_type np_,
									    const LevenbergMarquardtFunctionWrapper<T>::PtrFun f_)
      : nv(nv_),
	np(np_),
	f(f_)
    {} // end of LevenbergMarquardtFunctionWrapper::LevenbergMarquardtFunctionWrapper<T>

    template<typename T>
    LevenbergMarquardtFunctionWrapper<T>::LevenbergMarquardtFunctionWrapper(const LevenbergMarquardtFunctionWrapper<T>& src)
      : nv(src.nv),
	np(src.np),
	f(src.f)	
    {} // end of LevenbergMarquardtFunctionWrapper<T>::LevenbergMarquardtFunctionWrapper

    template<typename T>
    LevenbergMarquardtFunctionWrapper<T>&
    LevenbergMarquardtFunctionWrapper<T>::operator=(const LevenbergMarquardtFunctionWrapper<T>& src)
    {
      this->f = src.f;
      this->np = src.np;
      this->nv = src.nv;
      return *this;
    } // end of LevenbergMarquardtFunctionWrapper<T>::operator=

    template<typename T>
    typename LevenbergMarquardtFunctionWrapper<T>::size_type
    LevenbergMarquardtFunctionWrapper<T>::getNumberOfVariables(void) const
    {
      return this->nv;
    } // end of LevenbergMarquardtFunctionWrapper<T>::getNumberOfVariables()

    template<typename T>
    typename LevenbergMarquardtFunctionWrapper<T>::size_type
    LevenbergMarquardtFunctionWrapper<T>::getNumberOfParameters(void) const
    {
      return this->np;
    } // end of LevenbergMarquardtFunctionWrapper<T>::getNumberOfParameters()

    template<typename T>
    void
    LevenbergMarquardtFunctionWrapper<T>::operator()(T& r,tfel::math::vector<T>& g,
						     const tfel::math::vector<T>& v,
						     const tfel::math::vector<T>& p) const
    {
      (*(this->f))(r,g,v,p);
    } // end of LevenbergMarquardtFunctionWrapper<T>::operator()

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_LEVENBERGMARQUARDTFUNCTIONWRAPPER_H */

