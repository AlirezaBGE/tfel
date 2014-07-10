/*!
 * \file   TinyMatrixSolve.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   01 Aug 2006
 */

#ifndef _LIB_TFEL_TINYMATRIXSOLVE_H_
#define _LIB_TFEL_TINYMATRIXSOLVE_H_ 

#include<cmath>
#include<limits>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/TypeTraits/IsReal.hxx"

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/LU/TinyPermutation.hxx"
#include"TFEL/Math/tmatrix.hxx"

#include"TFEL/Math/LU/LUException.hxx"

namespace tfel{

  namespace math{

    /*!
     * solve a linear problem using LU decomposition
     * \param N : dimension of the system
     * \param T : numerical type
     */
    template<unsigned short N,typename T>
    struct TinyMatrixSolveBase
    {
      /*!
       * \brief LU decompose the given matrix
       * \param m   : matrix to be decomposed
       * \param p   : permutation vector
       * \param eps : numerical parameter to detect null pivot
       */
      static TFEL_MATH_INLINE2 void
      decomp(tmatrix<N,N,T>&,
	     TinyPermutation<N>&,
	     const T = 100*std::numeric_limits<T>::min());
      
      /*!
       * \brief solve the linear system m.x = b once the matrix has been decomposed
       * \param m   : LU decomposition of a matrix
       * \param p   : permutation vector
       * \param b   : right member
       * \param eps : numerical parameter to detect null pivot
       */
      static TFEL_MATH_INLINE2 void
      back_substitute(const tmatrix<N,N,T>&,
		      const TinyPermutation<N>&,
		      tvector<N,T>&,
		      const T = 100*std::numeric_limits<T>::min());
    };

    /*!
     * solve a linear problem using LU decomposition
     * \param N : dimension of the system
     * \param T : numerical type
     */
    template<unsigned short N,typename T>
    struct TinyMatrixSolve
      : public TinyMatrixSolveBase<N,T>
    {

      /*!
       * solve the linear system m.x = b
       * \param m   : matrix to be inverted
       * \param b   : right member
       * \param eps : numerical paramater to detect null pivot
       * \note the matrix m is overwritten during computations
       * \note the right member is overwritten by the solution
       */
      static TFEL_MATH_INLINE2 void 
      exe(tmatrix<N,N,T>& m, tvector<N,T>&,
	  const T = 100*std::numeric_limits<T>::min());
    }; 

    /*!
     * solve a linear problem by direct invertion
     * \param T : numerical type
     */
    template<typename T>
    struct TinyMatrixSolve<2u,T>
      : public TinyMatrixSolveBase<2u,T>
    {

      /*!
       * solve the linear system m.x = b
       * \param m   : matrix to be inverted
       * \param b   : right member
       * \param eps : numerical paramater to detect null pivot
       * \note the right member is overwritten by the solution
       */
      static TFEL_MATH_INLINE2 void 
      exe(const tmatrix<2u,2u,T>& m, tvector<2u,T>&,
	  const T = 100*std::numeric_limits<T>::min());

    }; 

    /*!
     * solve a linear problem by direct invertion
     * \param T : numerical type
     */
    template<typename T>
    struct TinyMatrixSolve<3u,T>
      : public TinyMatrixSolveBase<3u,T>
    {

      /*!
       * \brief solve the linear system m.x = b
       * \param m   : matrix to be inverted
       * \param b   : right member
       * \param eps : numerical parameter to detect null pivot
       * \note the right member is overwritten by the solution
       * \note the matrix is destroyed
       */
      static TFEL_MATH_INLINE2 void 
      exe(const tmatrix<3u,3u,T>& m, tvector<3u,T>&,
	  const T = 100*std::numeric_limits<T>::min());

    }; 

  } // end of namespace math

} // end of namespace tfel

#include "LU/TinyMatrixSolve.ixx"

#endif /* _LIB_TFEL_TINYMATRIXSOLVE_H */

