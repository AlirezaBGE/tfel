/*!
 * \file   tensor.ixx
 * \brief  This file implements the methods of the class tensor.
 * \author Helfer Thomas
 * \date   02 jun 2006
 */

#ifndef _LIB_TFEL_TENSOR_IXX_
#define _LIB_TFEL_TENSOR_IXX_ 

#include <cmath>
#include <iterator>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

#include"TFEL/TypeTraits/IsSafelyReinterpretCastableTo.hxx"
#include"TFEL/Math/General/Abs.hxx"
#include"TFEL/Math/Vector/VectorUtilities.hxx"
#include"TFEL/Math/Tensor/TensorChangeBasis.hxx"

namespace tfel{

  namespace math {

#ifndef DOXYGENSPECIFIC

    template<typename Child>
    template<typename TensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<Child>::dime==TensorTraits<TensorType>::dime &&
      tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,
				       typename TensorTraits<Child>::NumType>::cond,
      Child&>::type
    tensor_base<Child>::operator=(const TensorType& src){
      Child& child = static_cast<Child&>(*this);
      vectorToTab<TensorDimeToSize<TensorTraits<Child>::dime>::value>::exe(src,child);
      return child;
    }

    template<typename Child>
    template<typename TensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<Child>::dime==TensorTraits<TensorType>::dime &&
      tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,
				       typename TensorTraits<Child>::NumType>::cond,
      Child&>::type
    tensor_base<Child>::operator+=(const TensorType& src){
      Child& child = static_cast<Child&>(*this);
      VectorUtilities<TensorDimeToSize<TensorTraits<Child>::dime>::value>::PlusEqual(child,src);
      return child;
    }

    template<typename Child>
    template<typename TensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<Child>::dime==TensorTraits<TensorType>::dime &&
      tfel::typetraits::IsAssignableTo<typename TensorTraits<TensorType>::NumType,
				       typename TensorTraits<Child>::NumType>::cond,
      Child&>::type
    tensor_base<Child>::operator-=(const TensorType& src){
      Child& child = static_cast<Child&>(*this);
      VectorUtilities<TensorDimeToSize<TensorTraits<Child>::dime>::value>::MinusEqual(child,src);
      return child;
    }

    // *= operator
    template<typename Child>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<typename TensorTraits<Child>::NumType,
						 T2,OpMult>::type,
			     typename TensorTraits<Child>::NumType>::cond,
      Child&>::type
    tensor_base<Child>::operator*=(const T2 s)
    {
      Child& child = static_cast<Child&>(*this);
      VectorUtilities<TensorDimeToSize<TensorTraits<Child>::dime>::value>::scale(child,s);
      return child;
    }

    // /= operator
    template<typename Child>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T2>::cond&&
      tfel::meta::IsSameType<typename ResultType<typename TensorTraits<Child>::NumType,
						 T2,OpDiv>::type,
			     typename TensorTraits<Child>::NumType>::cond,
      Child&>::type
    tensor_base<Child>::operator/=(const T2 s)
    {
      Child& child = static_cast<Child&>(*this);
      VectorUtilities<TensorDimeToSize<TensorTraits<Child>::dime>::value>::scale(child,(static_cast<typename tfel::typetraits::BaseType<T2>::type>(1u))/s);
      return child;
    }

    template<unsigned short N, typename T>
    tensor<N,T>::tensor(const T init)
    {
      tfel::fsalgo::fill<TensorDimeToSize<N>::value>::exe(this->v,init);
    }

    template<unsigned short N, typename T>
    T& 
    tensor<N,T>::operator()(const unsigned short i){
      assert(i<TensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T>
    const T& 
    tensor<N,T>::operator()(const unsigned short i) const{
      assert(i<TensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T>
    T& 
    tensor<N,T>::operator[](const unsigned short i){
      assert(i<TensorDimeToSize<N>::value);
      return this->v[i];
    }

    template<unsigned short N, typename T>
    typename tensor<N,T>::RunTimeProperties
    tensor<N,T>::getRunTimeProperties(void) const{
      return RunTimeProperties();
    }

    template<unsigned short N, typename T>
    const T& 
    tensor<N,T>::operator[](const unsigned short i) const{
      assert(i<TensorDimeToSize<N>::value);
      return this->v[i];
    }

    // Import from values
    template<unsigned short N,typename T>
    template<typename T2>
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsSafelyReinterpretCastableTo<T2,typename tfel::typetraits::BaseType<T>::type>::cond,
      void>::type
    tensor<N,T>::import(const T2 * const src)
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<TensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(src,reinterpret_cast<base*>(this->v));
    }

    // Write to Tab
    template<unsigned short N, typename T>
    void tensor<N,T>::write(typename tfel::typetraits::BaseType<T>::type* const t) const
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<TensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(reinterpret_cast<const base*>(this->v),t);
    }

    // ChangeBasis
    template<unsigned short N,typename T>
    void 
    tensor<N,T>::changeBasis(const tmatrix<3u,3u,typename tfel::typetraits::BaseType<T>::type>& m)
    {
      TensorChangeBasis<N,T>::exe(this->v,m);
    }

    // Return Id
    template<unsigned short N,typename T>
    const tensor<N,T>&
    tensor<N,T>::Id(void)
    {
      typedef typename tfel::typetraits::BaseType<T>::type base;
      static const base zero = static_cast<base>(0);
      static const base one  = static_cast<base>(1);
      static const base IdCoef[]  = {one,one,one,
				     zero,zero,zero,
				     zero,zero,zero};
      static const tensor<N,T> id(IdCoef);
      return id;
    } // end of tensor<N,T>::Id

    template<unsigned short N, typename T>
    template<typename InputIterator>
    void 
    tensor<N,T>::copy(const InputIterator src)
    {
      tfel::fsalgo::copy<TensorDimeToSize<N>::value>::exe(src,*this);
    }

    template<unsigned short N, typename T,
	     typename OutputIterator>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond,
      void>::type
    exportToBaseTypeArray(const tensor<N,T>& t,
			  OutputIterator p)
    {    
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef tfel::fsalgo::copy<StensorDimeToSize<N>::value> Copy;
      TFEL_STATIC_ASSERT((tfel::typetraits::IsSafelyReinterpretCastableTo<T,base>::cond));
      Copy::exe(reinterpret_cast<const base*>(&t[0]),p);
    }

    template<typename TensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 1u,
      tensor<1u,typename ComputeBinaryResult<typename tfel::typetraits::BaseType<typename TensorTraits<TensorType>::NumType>::type,
					     typename TensorTraits<TensorType>::NumType,OpDiv>::Result>
      >::type
    invert(const TensorType& t){
      typedef typename TensorTraits<TensorType>::NumType T;
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef typename ComputeBinaryResult<base,T,OpDiv>::Result T2;
      tensor<1u,T2> t2;
      t2(0) = base(1)/t(0);
      t2(1) = base(1)/t(1);
      t2(2) = base(1)/t(2);
      return t2;
    }
    
    template<typename TensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 2u,
      tensor<2u,typename ComputeBinaryResult<typename tfel::typetraits::BaseType<typename TensorTraits<TensorType>::NumType>::type,
					     typename TensorTraits<TensorType>::NumType,OpDiv>::Result>
      >::type
    invert(const TensorType& t){
      typedef typename TensorTraits<TensorType>::NumType T;
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
					  Power<2> >::Result T2;
      typedef typename ComputeBinaryResult<base,T,OpDiv>::Result T3;
      tensor<2u,T3> t2;
      T2 det = t(0)*t(1)-t(3)*t(4);
      t2(0) = t(1)/det;
      t2(1) = t(0)/det;
      t2(3) = -t(3)/det;
      t2(4) = -t(4)/det;
      t2(2) = base(1)/t(2);
      return t2;
    }

    template<typename TensorType>
    typename tfel::meta::EnableIf<
      tfel::meta::Implements<TensorType,TensorConcept>::cond &&
      TensorTraits<TensorType>::dime == 3u,
      tensor<3u,typename ComputeBinaryResult<typename tfel::typetraits::BaseType<typename TensorTraits<TensorType>::NumType>::type,
    					     typename TensorTraits<TensorType>::NumType,OpDiv>::Result>
      >::type
    invert(const TensorType& t){
      typedef typename TensorTraits<TensorType>::NumType T;
      typedef typename tfel::typetraits::BaseType<T>::type base;
      typedef typename ComputeUnaryResult<typename TensorTraits<TensorType>::NumType,
					  Power<3> >::Result T2;
      typedef typename ComputeBinaryResult<base,T2,OpDiv>::Result T3;
      typedef typename ComputeBinaryResult<base,T,OpDiv>::Result  T4;
      tensor<3u,T4> t2;
      const T3 id = base(1)/det(t);
      t2(0) = (t(1)*t(2)-t(7)*t(8))*id;
      t2(1) = (t(0)*t(2)-t(5)*t(6))*id;
      t2(2) = (t(0)*t(1)-t(3)*t(4))*id;
      t2(3) = (t(5)*t(8)-t(2)*t(3))*id;
      t2(4) = (t(6)*t(7)-t(2)*t(4))*id;
      t2(5) = (t(3)*t(7)-t(1)*t(5))*id;
      t2(6) = (t(4)*t(8)-t(1)*t(6))*id;
      t2(7) = (t(4)*t(5)-t(0)*t(7))*id;
      t2(8) = (t(3)*t(6)-t(0)*t(8))*id;
      return t2;
    }
    
#endif

  } //end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_TENSOR_IXX */

