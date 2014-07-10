#ifndef _TENSOR_CONCEPT_IMPL_
#define _TENSOR_CONCEPT_IMPL_ 1

#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Tensor/TensorSizeToDime.hxx"

namespace tfel{

  namespace math{

    namespace internals{

      template<unsigned short N>
      struct TensorAbs;

      template<>
      struct TensorAbs<1u>
      {
	template<typename TensorType>
	static typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
	exe(const TensorType& s)
	{
	  return tfel::math::abs(s(0))+tfel::math::abs(s(1))+tfel::math::abs(s(2));
	}
      };

      template<>
      struct TensorAbs<2u>
      {
	template<typename TensorType>
	static typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
	exe(const TensorType& s)
	{
	  return tfel::math::abs(s(0))+tfel::math::abs(s(1))+tfel::math::abs(s(2))
	    +tfel::math::abs(s(3))+tfel::math::abs(s(4));
	}
      };

      template<>
      struct TensorAbs<3u>
      {
	template<typename TensorType>
	static typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
	exe(const TensorType& s)
	{
	  return tfel::math::abs(s(0))+tfel::math::abs(s(1))+tfel::math::abs(s(2))
	    +tfel::math::abs(s(3))+tfel::math::abs(s(4))+tfel::math::abs(s(5))
	    +tfel::math::abs(s(6))+tfel::math::abs(s(7))+tfel::math::abs(s(8));
	}
      };
      
    } // end of namespace internals
    
    template<class T>
    typename TensorConcept<T>::ValueType
    TensorConcept<T>::operator()(const unsigned short i) const {
      return static_cast<const T&>(*this).operator()(i);
    }

    template<class T>
    typename TensorConcept<T>::ValueType
    TensorConcept<T>::operator[](const unsigned short i) const {
      return static_cast<const T&>(*this).operator()(i);
    }

    template<class T>
    typename TensorConcept<T>::NumType
    TensorConcept<T>::operator()(const unsigned short i,
				 const unsigned short j) const
    {
      if((i==j)&&(i<3)){
	return static_cast<const T&>(*this).operator()(i);
      } else if((i==0)&&(j==1)){
	return (TensorTraits<T>::dime<2) ? NumType(0) : static_cast<const T&>(*this).operator()(3);
      } else if((i==1)&&(j==0)){
	return (TensorTraits<T>::dime<2) ? NumType(0) : static_cast<const T&>(*this).operator()(4);
      } else if((i==0)&&(j==2)){
	return (TensorTraits<T>::dime<3) ? NumType(0) : static_cast<const T&>(*this).operator()(5);
      } else if((i==2)&&(j==0)){
	return (TensorTraits<T>::dime<3) ? NumType(0) : static_cast<const T&>(*this).operator()(6);
      } else if((i==1)&&(j==2)){
	return (TensorTraits<T>::dime<3) ? NumType(0) : static_cast<const T&>(*this).operator()(7);
      } else if((i==2)&&(j==1)){
	return (TensorTraits<T>::dime<3) ? NumType(0) : static_cast<const T&>(*this).operator()(8);
      }
      throw(TensorInvalidIndexesException(i,j));
      return NumType(0);
    }
    
    template<class T>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<T,TensorConcept>::cond,
      typename TensorTraits<T>::NumType
      >::type
    trace(const T& s)
    {
      return s(0)+s(1)+s(2);
    }     
    
    template<typename TensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond,
      typename tfel::typetraits::AbsType<typename TensorTraits<TensorType>::NumType>::type
      >::type
    abs(const TensorType& s){
      return tfel::math::internals::TensorAbs<TensorTraits<TensorType>::dime>::exe(s);
    }
    
    template<class T>
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==1u)),
      stensor<1u,typename TensorTraits<T>::NumType>
      >::type
    syme(const T& t)
    {
      return stensor<1u,typename TensorTraits<T>::NumType>(t.begin());
    }

    template<class T>
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==2u)),
      stensor<2u,typename TensorTraits<T>::NumType>
      >::type
    syme(const T& t){
      using namespace std;
      typedef typename TensorTraits<T>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type real;
      static const real cste = sqrt(real(2))/real(2);
      stensor<2u,NumType> s;
      s[0] = t[0];
      s[1] = t[1];
      s[2] = t[2];
      s[3] = cste*(t[3]+t[4]);
      return s;
    }

    template<class T>
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==3u)),
      stensor<3u,typename TensorTraits<T>::NumType>
      >::type
    syme(const T& t)
    {
      using namespace std;
      typedef typename TensorTraits<T>::NumType NumType;
      typedef typename tfel::typetraits::BaseType<NumType>::type real;
      static const real cste = sqrt(real(2))/real(2);
      stensor<3u,NumType> s;
      s[0] = t[0];
      s[1] = t[1];
      s[2] = t[2];
      s[3] = cste*(t[3]+t[4]);
      s[4] = cste*(t[5]+t[6]);
      s[5] = cste*(t[7]+t[8]);
      return s;
    }

    template<class T>
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==1u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<1u,typename TensorTraits<T>::NumType> >::type
    computeCauchyGreenTensor(const T& t)
    {
      using namespace std;
      typedef typename TensorTraits<T>::NumType real;
      stensor<1u,real> r;
      r[0] = t[0]*t[0];
      r[1] = t[1]*t[1];
      r[2] = t[2]*t[2];
      return r;
    }
  
    template<class T>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==2u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<2u,typename TensorTraits<T>::NumType>
      >::type
    computeCauchyGreenTensor(const T& t){
      using namespace std;
      typedef typename TensorTraits<T>::NumType real;
      static const real cste = sqrt(real(2));
      stensor<2u,real> r;
      r[0] = t[0]*t[0]+t[4]*t[4];
      r[1] = t[1]*t[1]+t[3]*t[3];
      r[2] = t[2]*t[2];
      r[3] = cste*(t[1]*t[4]+t[0]*t[3]);
      return r;
    }
    
    template<class T>
    TFEL_MATH_INLINE 
    typename tfel::meta::EnableIf<
      ((tfel::meta::Implements<T,TensorConcept>::cond) &&
       (TensorTraits<T>::dime==3u)&&
       (tfel::typetraits::IsFundamentalNumericType<typename TensorTraits<T>::NumType>::cond)),
      stensor<3u,typename TensorTraits<T>::NumType>
      >::type
    computeCauchyGreenTensor(const T& t)
    {
      using namespace std;
      typedef typename TensorTraits<T>::NumType real;
      static const real cste = sqrt(real(2));
      stensor<3u,real> r;
      r[0] = t[0]*t[0]+t[4]*t[4]+t[6]*t[6];
      r[1] = t[1]*t[1]+t[3]*t[3]+t[8]*t[8];
      r[2] = t[2]*t[2]+t[5]*t[5]+t[7]*t[7];
      r[3] = cste*(t[6]*t[8]+t[1]*t[4]+t[0]*t[3]);
      r[4] = cste*(t[4]*t[7]+t[2]*t[6]+t[0]*t[5]);
      r[5] = cste*(t[2]*t[8]+t[1]*t[7]+t[3]*t[5]);
      return r;
    }

    template<typename TensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 1u,
      typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
				  Power<3> >::Result
    >::type
    det(const TensorType&t)
    {
      return t(0)*t(1)*t(2);
    } // end of 

    template<typename TensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 2u,
      typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
				  Power<3> >::Result
    >::type
    det(const TensorType& t)
    {
      return (t(0)*t(1)-t(3)*t(4))*t(2);
    }

    template<typename TensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 3u,
      typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
				  Power<3> >::Result
    >::type
    det(const TensorType& t)
    {
      typedef typename TensorTraits<TensorType>::NumType T;
      const T a = t(0);
      const T b = t(3);
      const T c = t(5);
      const T d = t(4);
      const T e = t(1);
      const T f = t(7);
      const T g = t(6);
      const T h = t(8);
      const T i = t(2);
      return a*(e*i-f*h)+b*(f*g-d*i)+c*(d*h-e*g);
    }
  
    template<typename T>
    std::ostream&
    operator << (std::ostream & os,const TensorConcept<T>& s)
    {
      unsigned short i;
      os << "[ ";
      for(i=0;i!=TensorDimeToSize<TensorTraits<T>::dime>::value;++i){
	os << s(i) << " ";
      }
      os << "]";
      return os;
    } // end of operator << 
    
  } // end of namespace math
  
} // end of namespace tfel

#endif /* _TENSOR_CONCEPT_IMPL_ */
