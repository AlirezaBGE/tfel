/*! 
 * \file  T2toT2TensorProductExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 juin 2014
 */

#ifndef _LIB_TFEL_MATH_T2TOT2TENSORPRODUCTEXPR_H_
#define _LIB_TFEL_MATH_T2TOT2TENSORPRODUCTEXPR_H_ 

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Math/Tensor/TensorConcept.hxx"
#include"TFEL/Math/Tensor/TensorExpr.hxx"

namespace tfel
{

  namespace math
  {

    //! Empty structure used for partial specialisation of the
    //! TensorExpr class
    template<unsigned short N>
    struct T2toT2TensorProductExpr
    {}; // end of struct T2toT2TensorProductExpr

    /*!
     * Partial specialisation
     */
    template<typename TensorResultType>
    struct TensorExpr<TensorResultType,T2toT2TensorProductExpr<1u> >
      : public TensorConcept<TensorExpr<TensorResultType,T2toT2TensorProductExpr<1u> > >,
	public fsarray<TensorDimeToSize<TensorTraits<TensorResultType>::dime>::value,
		       typename TensorTraits<TensorResultType>::NumType>
    {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename TensorTraits<TensorResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((TensorTraits<TensorResultType>::dime==1u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template<typename T2toT2Type,
	       typename TensorType>
      TFEL_MATH_INLINE
      TensorExpr(const T2toT2Type& a,
		 const TensorType& b)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2Type>::dime==1u));
	//! a simple check
	TFEL_STATIC_ASSERT((TensorTraits <TensorType>::dime==1u));
	this->v[0]=a(0,0)*b(0)+a(0,1)*b(1)+a(0,2)*b(2);
	this->v[1]=a(1,0)*b(0)+a(1,1)*b(1)+a(1,2)*b(2);
	this->v[2]=a(2,0)*b(0)+a(2,1)*b(1)+a(2,2)*b(2);
      } // end of TensorExpr
      /*!
       * \brief access operator
       * \param[in] i : index
       */
      TFEL_MATH_INLINE const value_type&
      operator()(const unsigned short i) const
      {
	return this->v[i];
      } // end of operator()
      /*!
       * \return the runtime properties
       */
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }
    }; // end of struct TensorExpr<TensorResultType,T2ST2toT2TensorProductExp>

    /*!
     * Partial specialisation
     */
    template<typename TensorResultType>
    struct TensorExpr<TensorResultType,T2toT2TensorProductExpr<2u> >
      : public TensorConcept<TensorExpr<TensorResultType,T2toT2TensorProductExpr<2u> > >,
	public fsarray<TensorDimeToSize<TensorTraits<TensorResultType>::dime>::value,
		       typename TensorTraits<TensorResultType>::NumType>
    {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename TensorTraits<TensorResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((TensorTraits<TensorResultType>::dime==2u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template<typename T2toT2Type,
	       typename TensorType>
      TFEL_MATH_INLINE
      TensorExpr(const T2toT2Type& a,
		 const TensorType& b)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2Type>::dime==2u));
	//! a simple check
	TFEL_STATIC_ASSERT((TensorTraits <TensorType>::dime==2u));
	this->v[0]=a(0,4)*b[4]+a(0,3)*b[3]+a(0,2)*b[2]+a(0,1)*b[1]+a(0,0)*b[0];
	this->v[1]=a(1,4)*b[4]+a(1,3)*b[3]+a(1,2)*b[2]+a(1,1)*b[1]+a(1,0)*b[0];
	this->v[2]=a(2,4)*b[4]+a(2,3)*b[3]+a(2,2)*b[2]+a(2,1)*b[1]+a(2,0)*b[0];
	this->v[3]=a(3,4)*b[4]+a(3,3)*b[3]+a(3,2)*b[2]+a(3,1)*b[1]+a(3,0)*b[0];
	this->v[4]=a(4,4)*b[4]+a(4,3)*b[3]+a(4,2)*b[2]+a(4,1)*b[1]+a(4,0)*b[0];
      } // end of TensorExpr
      /*!
       * \brief access operator
       * \param[in] i : index
       */
      TFEL_MATH_INLINE const value_type&
      operator()(const unsigned short i) const
      {
	return this->v[i];
      } // end of operator()
      /*!
       * \return the runtime properties
       */
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }
    }; // end of struct TensorExpr<TensorResultType,T2ST2toT2TensorProductExp>

    /*!
     * Partial specialisation
     */
    template<typename TensorResultType>
    struct TensorExpr<TensorResultType,T2toT2TensorProductExpr<3u> >
      : public TensorConcept<TensorExpr<TensorResultType,T2toT2TensorProductExpr<3u> > >,
	public fsarray<TensorDimeToSize<TensorTraits<TensorResultType>::dime>::value,
		       typename TensorTraits<TensorResultType>::NumType>
    {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename TensorTraits<TensorResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((TensorTraits<TensorResultType>::dime==3u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template<typename T2toT2Type,
	       typename TensorType>
      TFEL_MATH_INLINE
      TensorExpr(const T2toT2Type& a,
		 const TensorType& b)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<TensorType,TensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2Type>::dime==3u));
	//! a simple check
	TFEL_STATIC_ASSERT((TensorTraits <TensorType>::dime==3u));
	this->v[0]=a(0,8)*b[8]+a(0,7)*b[7]+a(0,6)*b[6]+a(0,5)*b[5]+a(0,4)*b[4]+a(0,3)*b[3]+a(0,2)*b[2]+a(0,1)*b[1]+a(0,0)*b[0];
	this->v[1]=a(1,8)*b[8]+a(1,7)*b[7]+a(1,6)*b[6]+a(1,5)*b[5]+a(1,4)*b[4]+a(1,3)*b[3]+a(1,2)*b[2]+a(1,1)*b[1]+a(1,0)*b[0];
	this->v[2]=a(2,8)*b[8]+a(2,7)*b[7]+a(2,6)*b[6]+a(2,5)*b[5]+a(2,4)*b[4]+a(2,3)*b[3]+a(2,2)*b[2]+a(2,1)*b[1]+a(2,0)*b[0];
	this->v[3]=a(3,8)*b[8]+a(3,7)*b[7]+a(3,6)*b[6]+a(3,5)*b[5]+a(3,4)*b[4]+a(3,3)*b[3]+a(3,2)*b[2]+a(3,1)*b[1]+a(3,0)*b[0];
	this->v[4]=a(4,8)*b[8]+a(4,7)*b[7]+a(4,6)*b[6]+a(4,5)*b[5]+a(4,4)*b[4]+a(4,3)*b[3]+a(4,2)*b[2]+a(4,1)*b[1]+a(4,0)*b[0];
	this->v[5]=a(5,8)*b[8]+a(5,7)*b[7]+a(5,6)*b[6]+a(5,5)*b[5]+a(5,4)*b[4]+a(5,3)*b[3]+a(5,2)*b[2]+a(5,1)*b[1]+a(5,0)*b[0];
	this->v[6]=a(6,8)*b[8]+a(6,7)*b[7]+a(6,6)*b[6]+a(6,5)*b[5]+a(6,4)*b[4]+a(6,3)*b[3]+a(6,2)*b[2]+a(6,1)*b[1]+a(6,0)*b[0];
	this->v[7]=a(7,8)*b[8]+a(7,7)*b[7]+a(7,6)*b[6]+a(7,5)*b[5]+a(7,4)*b[4]+a(7,3)*b[3]+a(7,2)*b[2]+a(7,1)*b[1]+a(7,0)*b[0];
	this->v[8]=a(8,8)*b[8]+a(8,7)*b[7]+a(8,6)*b[6]+a(8,5)*b[5]+a(8,4)*b[4]+a(8,3)*b[3]+a(8,2)*b[2]+a(8,1)*b[1]+a(8,0)*b[0];
      } // end of TensorExpr
      /*!
       * \brief access operator
       * \param[in] i : index
       */
      TFEL_MATH_INLINE const value_type&
      operator()(const unsigned short i) const
      {
	return this->v[i];
      } // end of operator()
      /*!
       * \return the runtime properties
       */
      TFEL_MATH_INLINE RunTimeProperties
      getRunTimeProperties() const
      {
	return RunTimeProperties();
      }
    }; // end of struct TensorExpr<TensorResultType,T2ST2toT2TensorProductExp>

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_T2TOT2TENSORPRODUCTEXPR_H */

