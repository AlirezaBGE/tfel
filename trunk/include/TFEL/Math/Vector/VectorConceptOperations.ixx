/*!
 * \file   VectorConceptOperations.ixx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 f�v 2008
 */

#ifndef _LIB_TFEL_VECTORCONCEPTOPERATIONS_IXX_
#define _LIB_TFEL_VECTORCONCEPTOPERATIONS_IXX_ 

namespace tfel{

  namespace math{

    template<typename T1,typename T2>
    TFEL_MATH_INLINE
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpPlus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpPlus>::Handle
    >::type
    operator + (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpPlus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMinus>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMinus>::Handle
    >::type
    operator - (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMinus>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiv>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDiv>::Handle Handle;
      return  Handle(a,b);
    }
    
    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T1>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1 a, const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Handle
    >::type
    operator ^ (const T1& a,const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDiadicProduct>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpMult>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpMult>::Handle
    >::type
    operator * (const T1& a,const T2 b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpMult>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1,typename T2>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::typetraits::IsScalar<T2>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDiv>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDiv>::Handle
    >::type
    operator / (const T1& a,const T2 b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDiv>::Handle Handle;
      return  Handle(a,b);
    }

    template<typename T1>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond,
      typename ComputeUnaryResult<T1,OpNeg>::Handle
    >::type
    operator - (const T1& a)
    {
      typedef typename ComputeUnaryResult<T1,OpNeg>::Handle Handle;
      return Handle(a);
    }

    template<typename T1,typename T2>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      tfel::meta::Implements<T2,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result>::cond,
      typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result
    >::type
    operator | (const T1& a, const T2& b)
    {
      typedef typename ComputeBinaryResult<T1,T2,OpDotProduct>::Result Result;
      typedef typename ComputeBinaryResult<T1,T2,OpDotProduct>::Handle Handle;
      return Handle::template exe<Result,T1,T2>(a,b);
    }

    template<typename T1>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T1,VectorConcept>::cond&&
      !tfel::typetraits::IsInvalid<typename tfel::typetraits::RealPartType<typename ComputeBinaryResult<T1,T1,OpDotProduct>::Result>::type>::cond,
      typename tfel::typetraits::RealPartType<typename ComputeBinaryResult<T1,T1,OpDotProduct>::Result>::type
    >::type
    norm(const T1& v)
    {
      using namespace std;
      return sqrt(real(v|v));
    }

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_VECTORCONCEPTOPERATIONS_IXX */

