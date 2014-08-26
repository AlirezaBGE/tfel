/*! 
 * \file  ST2toST2StensorProductExpr.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 juin 2014
 */

#ifndef _LIB_TFEL_MATH_ST2TOST2STENSORPRODUCTEXPR_H_
#define _LIB_TFEL_MATH_ST2TOST2STENSORPRODUCTEXPR_H_ 

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"
#include"TFEL/Math/Stensor/StensorExpr.hxx"

namespace tfel
{

  namespace math
  {

    //! Empty structure used for partial specialisation of the
    //! StensorExpr class
    template<unsigned short N>
    struct ST2toST2StensorProductExpr
    {}; // end of struct ST2toST2StensorProductExpr

    /*!
     * Partial specialisation
     */
    template<typename StensorResultType>
    struct StensorExpr<StensorResultType,ST2toST2StensorProductExpr<1u> >
      : public StensorConcept<StensorExpr<StensorResultType,ST2toST2StensorProductExpr<1u> > >,
	public fsarray<StensorDimeToSize<StensorTraits<StensorResultType>::dime>::value,
		       typename StensorTraits<StensorResultType>::NumType>
    {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename StensorTraits<StensorResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((StensorTraits<StensorResultType>::dime==1u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template<typename ST2toST2Type,
	       typename StensorType>
      TFEL_MATH_INLINE
      StensorExpr(const ST2toST2Type& a,
		  const StensorType&  b)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2Type>::dime==1u));
	//! a simple check
	TFEL_STATIC_ASSERT((StensorTraits <StensorType>::dime==1u));
	this->v[0]=a(0,0)*b(0)+a(0,1)*b(1)+a(0,2)*b(2);
	this->v[1]=a(1,0)*b(0)+a(1,1)*b(1)+a(1,2)*b(2);
	this->v[2]=a(2,0)*b(0)+a(2,1)*b(1)+a(2,2)*b(2);
      } // end of StensorExpr
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
    }; // end of struct StensorExpr<StensorResultType,T2SST2toST2StensorProductExp>

    /*!
     * Partial specialisation
     */
    template<typename StensorResultType>
    struct StensorExpr<StensorResultType,ST2toST2StensorProductExpr<2u> >
      : public StensorConcept<StensorExpr<StensorResultType,ST2toST2StensorProductExpr<2u> > >,
	public fsarray<StensorDimeToSize<StensorTraits<StensorResultType>::dime>::value,
		       typename StensorTraits<StensorResultType>::NumType>
    {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename StensorTraits<StensorResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((StensorTraits<StensorResultType>::dime==2u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template<typename ST2toST2Type,
	       typename StensorType>
      TFEL_MATH_INLINE
      StensorExpr(const ST2toST2Type& a,
		  const StensorType&  b)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2Type>::dime==2u));
	//! a simple check
	TFEL_STATIC_ASSERT((StensorTraits <StensorType>::dime==2u));
	this->v[0]=a(0,0)*b(0)+a(0,1)*b(1)+a(0,2)*b(2)+a(0,3)*b(3);
	this->v[1]=a(1,0)*b(0)+a(1,1)*b(1)+a(1,2)*b(2)+a(1,3)*b(3);
	this->v[2]=a(2,0)*b(0)+a(2,1)*b(1)+a(2,2)*b(2)+a(2,3)*b(3);
	this->v[3]=a(3,0)*b(0)+a(3,1)*b(1)+a(3,2)*b(2)+a(3,3)*b(3);
      } // end of StensorExpr
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
    }; // end of struct StensorExpr<StensorResultType,T2SST2toST2StensorProductExp>

    /*!
     * Partial specialisation
     */
    template<typename StensorResultType>
    struct StensorExpr<StensorResultType,ST2toST2StensorProductExpr<3u> >
      : public StensorConcept<StensorExpr<StensorResultType,ST2toST2StensorProductExpr<3u> > >,
	public fsarray<StensorDimeToSize<StensorTraits<StensorResultType>::dime>::value,
		       typename StensorTraits<StensorResultType>::NumType>
    {
      //! a simple alias
      typedef EmptyRunTimeProperties RunTimeProperties;
      //! a simple alias
      typedef typename StensorTraits<StensorResultType>::NumType value_type;
      //! a simple check
      TFEL_STATIC_ASSERT((StensorTraits<StensorResultType>::dime==3u));
      /*!
       * \param[in] a : first term of the product
       * \param[in] b : second term of the product
       */
      template<typename ST2toST2Type,
	       typename StensorType>
      TFEL_MATH_INLINE
      StensorExpr(const ST2toST2Type& a,
		  const StensorType&  b)
      {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<StensorType,StensorConcept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toST2Traits<ST2toST2Type>::dime==3u));
	//! a simple check
	TFEL_STATIC_ASSERT((StensorTraits <StensorType>::dime==3u));
	this->v[0]=a(0,0)*b(0)+a(0,1)*b(1)+a(0,2)*b(2)+a(0,3)*b(3)+a(0,4)*b(4)+a(0,5)*b(5);
	this->v[1]=a(1,0)*b(0)+a(1,1)*b(1)+a(1,2)*b(2)+a(1,3)*b(3)+a(1,4)*b(4)+a(1,5)*b(5);
	this->v[2]=a(2,0)*b(0)+a(2,1)*b(1)+a(2,2)*b(2)+a(2,3)*b(3)+a(2,4)*b(4)+a(2,5)*b(5);
	this->v[3]=a(3,0)*b(0)+a(3,1)*b(1)+a(3,2)*b(2)+a(3,3)*b(3)+a(3,4)*b(4)+a(3,5)*b(5);
	this->v[4]=a(4,0)*b(0)+a(4,1)*b(1)+a(4,2)*b(2)+a(4,3)*b(3)+a(4,4)*b(4)+a(4,5)*b(5);
	this->v[5]=a(5,0)*b(0)+a(5,1)*b(1)+a(5,2)*b(2)+a(5,3)*b(3)+a(5,4)*b(4)+a(5,5)*b(5);
      } // end of StensorExpr
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
    }; // end of struct StensorExpr<StensorResultType,T2SST2toST2StensorProductExp>

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_ST2TOST2STENSORPRODUCTEXPR_H */

