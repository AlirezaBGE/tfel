/*!
 * \file   LUSolve.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   06 Jul 2006
 */

#ifndef _LIB_TFEL_MATH_LUSOLVE_HXX_
#define _LIB_TFEL_MATH_LUSOLVE_HXX_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Math/Matrix/MatrixConcept.hxx"
#include"TFEL/Math/LU/LUException.hxx"
#include"TFEL/Math/LU/Permutation.hxx"
#include"TFEL/Math/LU/LUDecomp.hxx"

namespace tfel{

  namespace math{

    /*!
     * This structure contains two static methods for solving linear
     * systems by LU decomposition.
     */
    struct LUSolve
    {

      template<typename MatrixType,typename VectorType>
      static TFEL_MATH_INLINE2
      void exe(MatrixType& m,
	       VectorType& b)
      {
	typedef typename MatrixTraits<MatrixType>::IndexType IndexType;
	Permutation<IndexType> p(m.getNbRows());
	VectorType x(b.size());
	LUSolve::exe(m,b,x,p);
      } // end of exe

      template<typename MatrixType,typename VectorType>
      static TFEL_MATH_INLINE2
      void exe(MatrixType& m,
	       VectorType& b,
	       VectorType& x,
	       Permutation<typename MatrixTraits<MatrixType>::IndexType>& p)
      {
	using namespace std;
	if(m.getNbRows()!=m.getNbCols()){
	  throw(LUException("matrix is not square"));
	}
	if((m.getNbRows()!=b.size())||
	   (m.getNbRows()!=p.size())||
	   (m.getNbRows()!=p.size())){
	  throw(LUException("matrix size and vector size does not match"));
	}
	if(m.getNbRows()==0){
	  throw(LUException("invalid matrix size"));
	}
	p.reset();
	LUDecomp::exe(m,p);
	LUSolve::back_substitute(m,b,x,p);
      } // end of LUSolve::exe

      template<typename MatrixType,typename VectorType>
      static TFEL_MATH_INLINE2
      void back_substitute(MatrixType& m,
			   VectorType& b,
			   VectorType& x,
			   Permutation<typename MatrixTraits<MatrixType>::IndexType>& p)
      {	
	using namespace std;
	typedef typename MatrixTraits<MatrixType>::IndexType IndexType;
	if(m.getNbRows()!=m.getNbCols()){
	  throw(LUException("matrix is not square"));
	}
	if((m.getNbRows()!=b.size())||
	   (m.getNbRows()!=p.size())||
	   (m.getNbRows()!=p.size())){
	  throw(LUException("matrix size and vector size does not match"));
	}
	if(m.getNbRows()==0){
	  throw(LUException("invalid matrix size"));
	}
	IndexType i,j;
	IndexType pi;
	IndexType pi2;
	IndexType n = m.getNbRows();
	copy(b.begin(),b.end(),x.begin());
	for(i=0;i!=n;++i){
	  pi = p(i);
	  for(j=0;j!=i;++j){
	    x(pi) -= m(pi,j)*x(p(j));
	  }
	  x(pi) /= m(pi,i);
	}
	b(n-1) = x(p(n-1));
	for(i=n-1;i!=0;--i){
	  pi2 = i-1;
	  pi  = p(pi2);
	  b(pi2) = x(pi);
	  for(j=i;j!=n;++j){
	    b(pi2) -= m(pi,j)*b(j);
	  }
	}
      } // end of LUSolve::back_substitute

    }; // end of struct LUSolve 

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_LUSOLVE_HXX */

