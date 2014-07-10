/*!
 * \file   stensor.ixx
 * \brief  This file implements the methods of the class stensor.
 * \author Helfer Thomas
 * \date   02 jun 2006
 */

#ifndef _LIB_TFEL_STENSOR_IXX_
#define _LIB_TFEL_STENSOR_IXX_ 

#include <cmath>
#include <iterator>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include"TFEL/Math/General/StorageTraits.hxx"
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"

#include"TFEL/Math/Stensor/Internals/StensorComputeEigenValues.hxx"
#include"TFEL/Math/Stensor/Internals/StensorComputeEigenVectors.hxx"
#include"TFEL/Math/Stensor/Internals/StensorImport.hxx"
#include"TFEL/Math/Stensor/Internals/StensorExport.hxx"
#include"TFEL/Math/Stensor/Internals/StensorChangeBasis.hxx"

namespace tfel{

  namespace math {

#ifndef DOXYGENSPECIFIC

    namespace internals{
      template<unsigned short N>
      class StensorComputeEigenVectors_;

      template<unsigned short N>
      struct StensorDeterminant;

      template<>
      struct StensorDeterminant<1u>
      {
	template<typename T>
	static typename ComputeUnaryResult<T,Power<3> >::Result
	exe(const tfel::math::stensor<1u,T>& s)
	{
	  return s(0)*s(1)*s(2);
	}
      }; // end of struct StensorDeterminant

      template<>
      struct StensorDeterminant<2u>
      {
	template<typename T>
	static TFEL_MATH_INLINE typename ComputeUnaryResult<T,Power<3> >::Result
	exe(const tfel::math::stensor<2u,T>& s)
	{
	  typedef typename tfel::typetraits::BaseType<T>::type base;
	  return s(2)*(s(0)*s(1)-s(3)*s(3)/base(2));
	}
      }; // end of struct StensorDeterminant

      template<>
      struct StensorDeterminant<3u>
      {
	template<typename T>
	static TFEL_MATH_INLINE typename ComputeUnaryResult<T,Power<3> >::Result
	exe(const tfel::math::stensor<3u,T>& s)
	{
	  typedef typename tfel::typetraits::BaseType<T>::type base;
	  static const base cste = std::sqrt(base(2));
	  return (T(2)*s(0)*s(1)*s(2)+cste*s(3)*s(4)*s(5)-s(2)*s(3)*s(3)-s(1)*s(4)*s(4)-s(0)*s(5)*s(5))/base(2);
	}
      }; // end of struct StensorDeterminant

      template<unsigned short N>
      struct StensorInvert;
      
      template<>
      struct StensorInvert<1u>
      {
	template<typename T>
	static TFEL_MATH_INLINE
	stensor<1u,typename ComputeBinaryResult<typename tfel::typetraits::BaseType<T>::type,T,OpDiv>::Result>
	exe(const stensor<1u,T>& s)
	{
	  typedef typename tfel::typetraits::BaseType<T>::type base;
	  typedef typename ComputeBinaryResult<base,T,OpDiv>::Result T2;
	  stensor<1u,T2> inv;
	  inv(0)=base(1)/s(0);
	  inv(1)=base(1)/s(1);
	  inv(2)=base(1)/s(2);
	  return inv;
	}
      };

      template<>
      struct StensorInvert<2u>
      {
	template<typename T>
	static TFEL_MATH_INLINE
	stensor<2u,typename ComputeBinaryResult<typename tfel::typetraits::BaseType<T>::type,T,OpDiv>::Result>
	exe(const stensor<2u,T>& s)
	{
	  typedef typename tfel::typetraits::BaseType<T>::type base;
	  typedef typename ComputeBinaryResult<base,T,OpDiv>::Result T2;
	  typedef typename ComputeUnaryResult<T,Power<3> >::Result T3;
	  typedef typename ComputeBinaryResult<base,T3,OpDiv>::Result T4;
	  stensor<2u,T2> inv;
	  const T3 inv_det = base(1)/det(s);
	  inv(0)=s(1)*s(2)*inv_det;
	  inv(1)=s(0)*s(2)*inv_det;
	  inv(2)=base(1)/s(2);
	  inv(3)=-s(2)*s(3)*inv_det;
	  return inv;
	}
      };

      template<>
      struct StensorInvert<3u>
      {
	template<typename T>
	static TFEL_MATH_INLINE
	stensor<3u,typename ComputeBinaryResult<typename tfel::typetraits::BaseType<T>::type,T,OpDiv>::Result>
	exe(const stensor<3u,T>& s)
	{
	  typedef typename tfel::typetraits::BaseType<T>::type base;
	  typedef typename ComputeBinaryResult<base,T,OpDiv>::Result T2;
	  typedef typename ComputeUnaryResult<T,Power<3> >::Result T3;
	  typedef typename ComputeBinaryResult<base,T3,OpDiv>::Result T4;
	  static const base two          = base(2);
	  static const base inv_sqrt_two = base(1)/std::sqrt(two);
	  static const base one_half     = base(1)/two;
	  stensor<3u,T2> inv;
	  const T3 inv_det = base(1)/det(s);
	  inv(0)=(s(1)*s(2)-s(5)*s(5)*one_half)*inv_det;
	  inv(1)=(s(0)*s(2)-s(4)*s(4)*one_half)*inv_det;
	  inv(2)=(s(0)*s(1)-s(3)*s(3)*one_half)*inv_det;
	  inv(3)=(inv_sqrt_two*s(4)*s(5)-s(2)*s(3))*inv_det;
	  inv(4)=(inv_sqrt_two*s(3)*s(5)-s(1)*s(4))*inv_det;
	  inv(5)=(inv_sqrt_two*s(3)*s(4)-s(0)*s(5))*inv_det;
	  return inv;
	}
      };
      
    }

    template<typename Child>
    template<typename StensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<StensorType,StensorConcept>::cond &&
      StensorTraits<Child>::dime==StensorTraits<StensorType>::dime &&
      tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
				       typename StensorTraits<Child>::NumType>::cond,
      Child&>::type
    stensor_base<Child>::operator=(const StensorType& src){
      Child& child = static_cast<Child&>(*this);
      vectorToTab<StensorDimeToSize<StensorTraits<Child>::dime>::value>::exe(src,child);
      return child;
    }

    template<typename Child>
    template<typename StensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<StensorType,StensorConcept>::cond &&
      StensorTraits<Child>::dime==StensorTraits<StensorType>::dime &&
      tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
				       typename StensorTraits<Child>::NumType>::cond,
      Child&>::type
    stensor_base<Child>::operator+=(const StensorType& src){
      Child& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::PlusEqual(child,src);
      return child;
    }

    template<typename Child>
    template<typename StensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<StensorType,StensorConcept>::cond &&
      StensorTraits<Child>::dime==StensorTraits<StensorType>::dime &&
      tfel::typetraits::IsAssignableTo<typename StensorTraits<StensorType>::NumType,
				       typename StensorTraits<Child>::NumType>::cond,
      Child&>::type
    stensor_base<Child>::operator-=(const StensorType& src){
      Child& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::MinusEqual(child,src);
      return child;
    }

    // *= operator
    template<typename Child>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<typename StensorTraits<Child>::NumType,
						 T2,OpMult>::type,
			     typename StensorTraits<Child>::NumType>::cond,
      Child&>::type
    stensor_base<Child>::operator*=(const T2 s)
    {
      Child& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::scale(child,s);
      return child;
    }

    // /= operator
    template<typename Child>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<typename StensorTraits<Child>::NumType,
						 T2,OpDiv>::type,
			     typename StensorTraits<Child>::NumType>::cond,
      Child&>::type
    stensor_base<Child>::operator/=(const T2 s)
    {
      Child& child = static_cast<Child&>(*this);
      VectorUtilities<StensorDimeToSize<StensorTraits<Child>::dime>::value>::scale(child,(static_cast<typename tfel::typetraits::BaseType<T2>::type>(1u))/s);
      return child;
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    stensor<N,T,Storage>::stensor(const T init)
    {
      tfel::fsalgo::fill<StensorDimeToSize<N>::value>::exe(this->v,init);
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    T& 
    stensor<N,T,Storage>::operator()(const unsigned short i){
      assert(i<StensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    const T& 
    stensor<N,T,Storage>::operator()(const unsigned short i) const{
      assert(i<StensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    T& 
    stensor<N,T,Storage>::operator[](const unsigned short i){
      assert(i<StensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    typename stensor<N,T,Storage>::RunTimeProperties
    stensor<N,T,Storage>::getRunTimeProperties(void) const{
      return RunTimeProperties();
    }

    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    const T& 
    stensor<N,T,Storage>::operator[](const unsigned short i) const{
      assert(i<StensorDimeToSize<N>::value);
      return this->v[i];
    }

    // Import from Voigt
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    stensor<N,T,Storage>::importVoigt(const T2 * const src)
    {
      tfel::math::internals::ImportFromVoigt<N>::exe(reinterpret_cast<T2*>(this->v),src);
    }
    
    // Import from Tab
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    stensor<N,T,Storage>::importTab(const T2* const src)
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::math::internals::ImportFromTab<N> Import;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Import::exe(reinterpret_cast<T2*>(this->v),src);
    }

    // Import from values
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    stensor<N,T,Storage>::import(const T2 * const src)
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(src,reinterpret_cast<base*>(this->v));
    }

    // Export to Tab
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    stensor<N,T,Storage>::exportTab(T2* const src) const
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::math::internals::ExportToTab<N> Export;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Export::exe(reinterpret_cast<const T2*>(this->v),src);
    }

    // Write to Tab
    template<unsigned short N, typename T, template<unsigned short,typename> class Storage>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void
      >::type
    stensor<N,T,Storage>::write(T2* const t) const
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(reinterpret_cast<const base*>(this->v),t);
    }

    // computeEigenValues
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    void stensor<N,T,Storage>::computeEigenValues(T& vp1,T& vp2,T& vp3,
						  const bool b) const 
    {
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,typename tfel::typetraits::BaseType<T>::type>::cond));
      tfel::math::internals::StensorComputeEigenValues_<N>::exe(reinterpret_cast<const typename tfel::typetraits::BaseType<T>::type*>(this->v),reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp1),reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp2),reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp3),b);
    }

    // computeEigenValues
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    void stensor<N,T,Storage>::computeEigenValues(tvector<3u,T>& vp,
						  const bool b) const 
    {
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,typename tfel::typetraits::BaseType<T>::type>::cond));
      tfel::math::internals::StensorComputeEigenValues_<N>::exe(reinterpret_cast<const typename tfel::typetraits::BaseType<T>::type* const>(this->v),reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp(0)),reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp(1)),reinterpret_cast<typename tfel::typetraits::BaseType<T>::type&>(vp(2)),b);
    }


    // computeEigenVectors
    template<unsigned short N,typename T, 
	     template<unsigned short,typename> class Storage>
    bool stensor<N,T,Storage>::computeEigenVectors(tvector<3u,T>& vp,
						   tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>& vec,
						   const bool b) const 
    {
      TFEL_STATIC_ASSERT(tfel::typetraits::IsFundamentalNumericType<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<typename tfel::typetraits::BaseType<T>::type>::cond);
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,typename tfel::typetraits::BaseType<T>::type>::cond));
      return tfel::math::internals::StensorComputeEigenVectors_<N>::exe(reinterpret_cast<const typename tfel::typetraits::BaseType<T>::type*>(this->v),reinterpret_cast<tvector<3u,typename tfel::typetraits::BaseType<T>::type>&>(vp),vec,b);
    }

    // ChangeBasis
    template<unsigned short N,typename T, template<unsigned short,typename> class Storage>
    void 
    stensor<N,T,Storage>::changeBasis(const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>& m)
    {
      return tfel::math::internals::StensorChangeBasis<N>::exe(this->v,m);
    }

    // Return Id
    template<unsigned short N,typename T,
	     template<unsigned short,typename> class Storage>
    TFEL_MATH_INLINE
    const stensor<N,T,Storage>&
    stensor<N,T,Storage>::Id(void)
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      static const base zero = static_cast<base>(0);
      static const base one  = static_cast<base>(1);
      static const base IdCoef[]  = {one,one,one,zero,zero,zero};
      static const stensor<N,T,Storage> id(IdCoef);
      return id;
    } // end of stensor<N,T,Storage>::Id

    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage>
    template<typename InputIterator>
    void 
    stensor<N,T,Storage>::copy(const InputIterator src)
    {
      tfel::fsalgo::copy<StensorDimeToSize<N>::value>::exe(src,*this);
    }

    template<unsigned short N, typename T,
	     template<unsigned short,typename> class Storage,
	     typename OutputIterator>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond,
      void>::type
    exportToBaseTypeArray(const stensor<N,T,Storage>& s,
			  OutputIterator p)
    {    
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(reinterpret_cast<const base*>(&s[0]),p);
    }

    template<typename T,
	     template<unsigned short,typename> class Storage>
    T tresca(const stensor<1u,T,Storage>& s,
	     const bool)
    {
      using namespace std;
      const T sd1 = abs(s[0]-s[1]);
      const T sd2 = abs(s[0]-s[2]);
      const T sd3 = abs(s[2]-s[1]);
      const T tmp  = sd1 > sd2 ? sd1 : sd2;
      const T tmp2 = sd3 > tmp ? sd3 : tmp;
      return tmp2;
    } // end of tresca
    
    template<unsigned short N,
	     typename T,
	     template<unsigned short,typename> class Storage>
    T tresca(const stensor<N,T,Storage>& s,
	     const bool b)
    {
      using namespace std;
      T s1;
      T s2;
      T s3;
      s.computeEigenValues(s1,s2,s3,b);
      const T sd1 = abs(s1-s2);
      const T sd2 = abs(s1-s3);
      const T sd3 = abs(s3-s2);
      const T tmp  = sd1 > sd2  ? sd1 : sd2;
      const T tmp2 = sd3 > tmp  ? sd3 : tmp;
      return tmp2;
    } // end of tresca

    template<typename StensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<StensorType,StensorConcept>::cond,
      stensor<StensorTraits<StensorType>::dime,
	      typename StensorTraits<StensorType>::NumType>
    >::type
    square_root(const StensorType& s)
    {
      using tfel::fsalgo::copy;
      typedef typename StensorTraits<StensorType>::NumType T;
      typedef typename tfel::typetraits::BaseType<T>::type real;
      stensor<StensorTraits<StensorType>::dime,T> tmp(s);
      stensor<StensorTraits<StensorType>::dime,T> r(T(0));
      tmatrix<3u,3u,real> vec;
      tvector<3u,T> vp;
      tmp.computeEigenVectors(vp,vec);
      r[0] = std::sqrt(vp[0]);
      r[1] = std::sqrt(vp[1]);
      r[2] = std::sqrt(vp[2]);
      r.changeBasis(transpose(vec));
      return r;
    } // end of square_root

    template<typename StensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<StensorType,StensorConcept>::cond,
      typename ComputeUnaryResult<typename StensorTraits<StensorType>::NumType,Power<3> >::Result
      >::type
    det(const StensorType& s)
    {
      const stensor<StensorTraits<StensorType>::dime,
		    typename StensorTraits<StensorType>::NumType> tmp(s);
      return tfel::math::internals::StensorDeterminant<StensorTraits<StensorType>::dime>::exe(tmp);
    }

    template<typename StensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<StensorType,StensorConcept>::cond,
      stensor<StensorTraits<StensorType>::dime,
	      typename ComputeBinaryResult<typename tfel::typetraits::BaseType<typename StensorTraits<StensorType>::NumType>::type,
					   typename StensorTraits<StensorType>::NumType,OpDiv>::Result>
      >::type
    invert(const StensorType& s)
    {
      const stensor<StensorTraits<StensorType>::dime,
    		    typename StensorTraits<StensorType>::NumType> tmp(s);
      return tfel::math::internals::StensorInvert<StensorTraits<StensorType>::dime>::exe(tmp);
    }
    
#endif

  } //end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_STENSOR_IXX */

