/*!
 * \file   TinyMatrixSolve.ixx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   02 Aug 2006
 */

#ifndef _LIB_TFEL_TINYMATRIXSOLVE_I_
#define _LIB_TFEL_TINYMATRIXSOLVE_I_ 

#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/LU/LUDecomp.hxx"
#include"TFEL/Math/LU/TinyPermutation.hxx"

namespace tfel{

  namespace math{

    template<unsigned short N,typename T>
    void
    TinyMatrixSolveBase<N,T>::back_substitute(const tmatrix<N,N,T>& m,
					      const TinyPermutation<N>& p,
					      tvector<N,T>& b,
					      const T eps)
    {
      typedef typename MatrixTraits<tmatrix<N,N,T> >::IndexType IndexType;
      tvector<N,T> x;
      IndexType i,j;
      IndexType pi;
      IndexType pi2;
      x = b;
      for(i=0;i!=N;++i){
	pi = p(i);
	for(j=0;j!=i;++j){
	  x(pi) -= m(pi,j)*x(p(j));
	}
	if(std::abs(m(pi,i))<eps){
	  throw(LUException("TinyMatrixSolve<N,T>::exe : null pivot"));
	}
	x(pi) /= m(pi,i);
      }
      b(N-1) = x(p(N-1));
      for(i=N-1;i!=0;--i){
	pi2 = i;
	--pi2;
	pi  = p(pi2);
	b(pi2) = x(pi);
	for(j=i;j!=N;++j){
	  b(pi2) -= m(pi,j)*b(j);
	}
      }
    } // end of TinyMatrixSolve<N,T>::exe


    template<unsigned short N,typename T>
    void
    TinyMatrixSolve<N,T>::exe(tmatrix<N,N,T>& m, tvector<N,T>& b,
			      const T eps)
    {
      TinyPermutation<N> p;
      TinyMatrixSolve<N,T>::decomp(m,p,eps);
      TinyMatrixSolve<N,T>::back_substitute(m,p,b,eps);
    } // end of TinyMatrixSolve<N,T>::exe

    template<unsigned short N,typename T>
    void
    TinyMatrixSolveBase<N,T>::decomp(tmatrix<N,N,T>& m,
				     TinyPermutation<N>& p,
				     const T eps)
    {
      LUDecomp::exe(m,p,eps);
    } // end of TinyMatrixSolve<N,T>::exe

    // Partial specialisation for 2*2 matrix
    template<typename T>
    void
    TinyMatrixSolve<2u,T>::exe(const tmatrix<2u,2u,T>& m,
			       tvector<2u,T>& b,
			       const T eps)
    {
      using namespace std;
      const T det = m(0,0)*m(1,1) - m(0,1)*m(1,0);
      if(abs(det)<eps){
	throw(LUException("TinyMatrixSolve<2u,T>::exe : null determinant"));
      }
      const T b0 = b(0);
      const T b1 = b(1);
      b(0) = ( m(1,1)*b0 - m(0,1)*b1)/det;
      b(1) = (-m(1,0)*b0 + m(0,0)*b1)/det;
    } // end of TinyMatrixSolve<2u,T>::exe

    // Partial specialisation for 3*3 matrix
    template<typename T>
    void
    TinyMatrixSolve<3u,T>::exe(const tmatrix<3u,3u,T>& m,
			       tvector<3u,T>& b,
			       const T eps)
    {
      using namespace std;
      const T det = m(0,0)*(m(1,1)*m(2,2) - m(1,2)*m(2,1)) - m(0,1)*(m(1,0)*m(2,2) - m(1,2)*m(2,0))
	+ m(0,2)*(m(1,0)*m(2,1) - m(1,1)*m(2,0));
      if(abs(det)<eps){
	throw(LUException("TinyMatrixSolve<3u,T>::exe : null determinant"));
      }
      const T b0 = b(0);
      const T b1 = b(1);
      const T b2 = b(2);
      b(0) = ((m(1,1)*m(2,2)-m(1,2)*m(2,1))*b0-(m(0,1)*m(2,2)-m(0,2)*m(2,1))*b1+(m(0,1)*m(1,2)-m(0,2)*m(1,1))*b2)/det;
      b(1) = ((m(1,2)*m(2,0)-m(1,0)*m(2,2))*b0+(m(0,0)*m(2,2)-m(0,2)*m(2,0))*b1-(m(0,0)*m(1,2)-m(0,2)*m(1,0))*b2)/det;
      b(2) = ((m(1,0)*m(2,1)-m(1,1)*m(2,0))*b0-(m(0,0)*m(2,1)-m(0,1)*m(2,0))*b1+(m(0,0)*m(1,1)-m(0,1)*m(1,0))*b2)/det;
    } // end of TinyMatrixSolve<2u,T>::exe

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_TINYMATRIXSOLVE_I_ */

