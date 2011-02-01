/*!
 * \file   SFTMCV.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   16 oct 2008
 */

#ifndef _LIB_TFEL_MATH_SFTMCV_HXX_
#define _LIB_TFEL_MATH_SFTMCV_HXX_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"

#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"
#include"TFEL/Math/Stensor/StensorExpr.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/tmatrix.hxx"

namespace tfel
{
  
  namespace math
  {

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T>
    struct SFTMCVExpr
    {}; // end of struct SFTMCVExpr

    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T>
    struct StensorExpr<stensor<N,T>,SFTMCVExpr<N,Mn,Mm,In,Im,T> >
      : public StensorConcept<StensorExpr<stensor<N,T>,SFTMCVExpr<N,Mn,Mm,In,Im,T> > >
    {

      StensorExpr(tmatrix<Mn,Mm,T>& m_)
	: m(m_)
      {} // end of StensorExpr

      const T&
      operator()(const unsigned short i) const
      {
	return this->m(In+i,Im);
      } // end of operator() const

      T&
      operator()(const unsigned short i)
      {
	return this->m(In+i,Im);
      } // end of operator()

      /*!
       * Assignement operator
       */
      template<typename T2,template<unsigned short,typename> class Storage2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	StensorExpr&>::type
      operator=(const stensor<N,T2,Storage2>& s){
	VectorUtilities<StensorDimeToSize<N>::value>::copy(s,*this);
	return *this;
      }
      
      /*!
       * Assignement operator
       */
      template<typename T2,template<unsigned short,typename> class Storage2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	StensorExpr&
      >::type
      operator=(const StensorExpr<stensor<N,T2,Storage2>,Expr>& s)
      {
	VectorUtilities<StensorDimeToSize<N>::value>::copy(s,*this);
	return *this;
      }

      // Assignement operator
      template<typename T2,template<unsigned short,typename> class Storage2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	StensorExpr&
      >::type
      operator+=(const stensor<N,T2,Storage2>& s){
	VectorUtilities<StensorDimeToSize<N>::value>::PlusEqual(s,*this);
	return *this;
      }
    
      // Assignement operator
      template<typename T2,template<unsigned short,typename> class Storage2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	StensorExpr&
      >::type
      operator+=(const StensorExpr<stensor<N,T2,Storage2>,Expr>& s){
	VectorUtilities<StensorDimeToSize<N>::value>::PlusEqual(s,*this);
	return *this;
      }

      // Assignement operator
      template<typename T2,template<unsigned short,typename> class Storage2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	StensorExpr&
      >::type
      operator-=(const stensor<N,T2,Storage2>& s){
	VectorUtilities<StensorDimeToSize<N>::value>::MinusEqual(s,*this);
	return *this;
      }
    
      // Assignement operator
      template<typename T2,template<unsigned short,typename> class Storage2,typename Expr>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	StensorExpr&
      >::type
      operator-=(const StensorExpr<stensor<N,T2,Storage2>,Expr>& s){
	VectorUtilities<StensorDimeToSize<N>::value>::MinusEqual(s,*this);
	return *this;
      }

      /*!
       * operator*=
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	StensorExpr&
      >::type
      operator*=(const T2 a){
	VectorUtilities<N>::scale(*this,a);
	return *this;
      }

      /*!
       * operator/=
       */
      template<typename T2>
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsScalar<T2>::cond&&
        tfel::meta::IsSameType<typename ResultType<T,T2,OpMult>::type,T>::cond,
	StensorExpr&
      >::type
      operator/=(const T2 a){
	VectorUtilities<N>::scale(*this,1/a);
	return *this;
      }

    protected:

      tmatrix<Mn,Mm,T>& m;

    private:

      /*!
       * Simple checks
       */
      TFEL_STATIC_ASSERT((N==1u)||(N==2u)||(N==3u));
      TFEL_STATIC_ASSERT((Im<Mm));
      TFEL_STATIC_ASSERT((StensorDimeToSize<N>::value<=Mn-In));

    }; // end of struct StensorExpr


    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T = double>
    struct SFTMCV
    {
      typedef StensorExpr<stensor<N,T>,SFTMCVExpr<N,Mn,Mm,In,Im,T> > type;
    }; // end of struct SFTMCV

    // Serialisation operator
    template<unsigned short N,
	     unsigned short Mn,
	     unsigned short Mm,
	     unsigned short In,
	     unsigned short Im,
	     typename T>
    std::ostream&
    operator << (std::ostream & os,const StensorExpr<stensor<N,T>,SFTMCVExpr<N,Mn,Mm,In,Im,T> >& s)
    {
      os << "[ ";
      for(unsigned short i=0;i<StensorDimeToSize<N>::value;++i){
	os << s(i) << " ";
      }
      os << "]";
      return os;
    } // end of operator << 
    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_SFTMCV_HXX */

