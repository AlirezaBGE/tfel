/*!
 * \file   Permutation.ixx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   01 Aug 2006
 */

#ifndef _LIB_TFEL_PERMUTATION_I_
#define _LIB_TFEL_PERMUTATION_I_ 

#include"TFEL/Math/MathException.hxx"

namespace tfel{

  namespace math{

    template<typename T>
    Permutation<T>::Permutation()
    {}

    template<typename T>
    Permutation<T>::Permutation(const typename vector<T>::size_type n)
      : vector<T>(n)
    {
      this->reset();
    }

    template<typename T>
    void
    Permutation<T>::reset()
    {
      typename vector<T>::size_type i;
      for(i=0;i!=this->size();++i){
	this->operator[](i) = i;
      }
    }

    template<typename T>
    void
    Permutation<T>::resize(const typename vector<T>::size_type s)
    {
      vector<T>::resize(s);
      this->reset();
    }

  } // end of namespace math

} // end of namespace tfel


#endif /* _LIB_TFEL_PERMUTATION_I_ */

