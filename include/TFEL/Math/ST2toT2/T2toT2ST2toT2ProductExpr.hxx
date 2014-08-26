/*! 
 * ile  T2toT2ST2toT2ProductExpr.hxx
 * rief
 * uthor Helfer Thomas
 * rief 16 juin 2014
 */

#ifndef _LIB_TFEL_MATH_T2TOT2ST2TOT2PRODUCTEXPR_H_
#define _LIB_TFEL_MATH_T2TOT2ST2TOT2PRODUCTEXPR_H_ 

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#include"TFEL/Math/T2toT2/T2toT2Concept.hxx"
#include"TFEL/Math/ST2toT2/ST2toT2Concept.hxx"
#include"TFEL/Math/ST2toT2/ST2toT2Expr.hxx"

namespace tfel
{

  namespace math
  {

    //! Empty structure used for partial specialisation of the
    //! ST2toT2Expr class
    template<unsigned short N>
    struct TFEL_VISIBILITY_LOCAL T2toT2ST2toT2ProductExpr
    {}; // end of struct T2toT2ST2toT2ProductExpr

/*!
 * Partial specialisation
 */
template<typename ST2toT2ResultType>
struct TFEL_VISIBILITY_LOCAL ST2toT2Expr<ST2toT2ResultType,T2toT2ST2toT2ProductExpr<1u> >
  : public ST2toT2Concept<ST2toT2Expr<ST2toT2ResultType,T2toT2ST2toT2ProductExpr<1u> > >,
	public fsarray<TensorDimeToSize<ST2toT2Traits<ST2toT2ResultType>::dime>::value*
		       StensorDimeToSize<ST2toT2Traits<ST2toT2ResultType>::dime>::value,
		       typename ST2toT2Traits<ST2toT2ResultType>::NumType>
{
  //! a simple alias
  typedef EmptyRunTimeProperties RunTimeProperties;
  //! a simple alias
  typedef typename ST2toT2Traits<ST2toT2ResultType>::NumType value_type;
  //! a simple check
  TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2ResultType>::dime==1u));
  /*!
   * \param[in] a : first term of the product
   * \param[in] b : second term of the product
   */
  template<typename T2toT2Type,
	       typename ST2toT2Type2>
  TFEL_MATH_INLINE
  ST2toT2Expr(const T2toT2Type& a,
		 const ST2toT2Type2& b)
  {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<ST2toT2Type2,ST2toT2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2Type>::dime==1u));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toT2Traits <ST2toT2Type2>::dime==1u));
this->v[0]=a(0,0)*b(0,0)+a(0,1)*b(1,0)+a(0,2)*b(2,0);
this->v[1]=a(0,0)*b(0,1)+a(0,1)*b(1,1)+a(0,2)*b(2,1);
this->v[2]=a(0,0)*b(0,2)+a(0,1)*b(1,2)+a(0,2)*b(2,2);
this->v[3]=a(1,0)*b(0,0)+a(1,1)*b(1,0)+a(1,2)*b(2,0);
this->v[4]=a(1,0)*b(0,1)+a(1,1)*b(1,1)+a(1,2)*b(2,1);
this->v[5]=a(1,0)*b(0,2)+a(1,1)*b(1,2)+a(1,2)*b(2,2);
this->v[6]=a(2,0)*b(0,0)+a(2,1)*b(1,0)+a(2,2)*b(2,0);
this->v[7]=a(2,0)*b(0,1)+a(2,1)*b(1,1)+a(2,2)*b(2,1);
this->v[8]=a(2,0)*b(0,2)+a(2,1)*b(1,2)+a(2,2)*b(2,2);
  } // end of ST2toT2Expr
  /*!
   * \brief access operator
   * \param[in] i : line index
   * \param[in] j : column index
   */
  TFEL_MATH_INLINE const value_type&
  operator()(const unsigned short i,
		 const unsigned short j) const
  {
	return this->v[i*3+j];
  } // end of operator()
  /*!
   * \return the runtime properties
   */
  TFEL_MATH_INLINE RunTimeProperties
  getRunTimeProperties() const
  {
	return RunTimeProperties();
  }
}; // end of struct ST2toT2Expr<ST2toT2ResultType,T2toT2ST2toT2ProductExpr>

/*!
 * Partial specialisation
 */
template<typename ST2toT2ResultType>
struct TFEL_VISIBILITY_LOCAL ST2toT2Expr<ST2toT2ResultType,T2toT2ST2toT2ProductExpr<2u> >
  : public ST2toT2Concept<ST2toT2Expr<ST2toT2ResultType,T2toT2ST2toT2ProductExpr<2u> > >,
	public fsarray<TensorDimeToSize<ST2toT2Traits<ST2toT2ResultType>::dime>::value*
		       StensorDimeToSize<ST2toT2Traits<ST2toT2ResultType>::dime>::value,
		       typename ST2toT2Traits<ST2toT2ResultType>::NumType>
{
  //! a simple alias
  typedef EmptyRunTimeProperties RunTimeProperties;
  //! a simple alias
  typedef typename ST2toT2Traits<ST2toT2ResultType>::NumType value_type;
  //! a simple check
  TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2ResultType>::dime==2u));
  /*!
   * \param[in] a : first term of the product
   * \param[in] b : second term of the product
   */
  template<typename T2toT2Type,
	       typename ST2toT2Type2>
  TFEL_MATH_INLINE
  ST2toT2Expr(const T2toT2Type& a,
		 const ST2toT2Type2& b)
  {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<ST2toT2Type2,ST2toT2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2Type>::dime==2u));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toT2Traits <ST2toT2Type2>::dime==2u));
this->v[0]=a(0,0)*b(0,0)+a(0,1)*b(1,0)+a(0,2)*b(2,0)+a(0,3)*b(3,0)+a(0,4)*b(4,0);
this->v[1]=a(0,0)*b(0,1)+a(0,1)*b(1,1)+a(0,2)*b(2,1)+a(0,3)*b(3,1)+a(0,4)*b(4,1);
this->v[2]=a(0,0)*b(0,2)+a(0,1)*b(1,2)+a(0,2)*b(2,2)+a(0,3)*b(3,2)+a(0,4)*b(4,2);
this->v[3]=a(0,0)*b(0,3)+a(0,1)*b(1,3)+a(0,2)*b(2,3)+a(0,3)*b(3,3)+a(0,4)*b(4,3);
this->v[4]=a(1,0)*b(0,0)+a(1,1)*b(1,0)+a(1,2)*b(2,0)+a(1,3)*b(3,0)+a(1,4)*b(4,0);
this->v[5]=a(1,0)*b(0,1)+a(1,1)*b(1,1)+a(1,2)*b(2,1)+a(1,3)*b(3,1)+a(1,4)*b(4,1);
this->v[6]=a(1,0)*b(0,2)+a(1,1)*b(1,2)+a(1,2)*b(2,2)+a(1,3)*b(3,2)+a(1,4)*b(4,2);
this->v[7]=a(1,0)*b(0,3)+a(1,1)*b(1,3)+a(1,2)*b(2,3)+a(1,3)*b(3,3)+a(1,4)*b(4,3);
this->v[8]=a(2,0)*b(0,0)+a(2,1)*b(1,0)+a(2,2)*b(2,0)+a(2,3)*b(3,0)+a(2,4)*b(4,0);
this->v[9]=a(2,0)*b(0,1)+a(2,1)*b(1,1)+a(2,2)*b(2,1)+a(2,3)*b(3,1)+a(2,4)*b(4,1);
this->v[10]=a(2,0)*b(0,2)+a(2,1)*b(1,2)+a(2,2)*b(2,2)+a(2,3)*b(3,2)+a(2,4)*b(4,2);
this->v[11]=a(2,0)*b(0,3)+a(2,1)*b(1,3)+a(2,2)*b(2,3)+a(2,3)*b(3,3)+a(2,4)*b(4,3);
this->v[12]=a(3,0)*b(0,0)+a(3,1)*b(1,0)+a(3,2)*b(2,0)+a(3,3)*b(3,0)+a(3,4)*b(4,0);
this->v[13]=a(3,0)*b(0,1)+a(3,1)*b(1,1)+a(3,2)*b(2,1)+a(3,3)*b(3,1)+a(3,4)*b(4,1);
this->v[14]=a(3,0)*b(0,2)+a(3,1)*b(1,2)+a(3,2)*b(2,2)+a(3,3)*b(3,2)+a(3,4)*b(4,2);
this->v[15]=a(3,0)*b(0,3)+a(3,1)*b(1,3)+a(3,2)*b(2,3)+a(3,3)*b(3,3)+a(3,4)*b(4,3);
this->v[16]=a(4,0)*b(0,0)+a(4,1)*b(1,0)+a(4,2)*b(2,0)+a(4,3)*b(3,0)+a(4,4)*b(4,0);
this->v[17]=a(4,0)*b(0,1)+a(4,1)*b(1,1)+a(4,2)*b(2,1)+a(4,3)*b(3,1)+a(4,4)*b(4,1);
this->v[18]=a(4,0)*b(0,2)+a(4,1)*b(1,2)+a(4,2)*b(2,2)+a(4,3)*b(3,2)+a(4,4)*b(4,2);
this->v[19]=a(4,0)*b(0,3)+a(4,1)*b(1,3)+a(4,2)*b(2,3)+a(4,3)*b(3,3)+a(4,4)*b(4,3);
  } // end of ST2toT2Expr
  /*!
   * \brief access operator
   * \param[in] i : line index
   * \param[in] j : column index
   */
  TFEL_MATH_INLINE const value_type&
  operator()(const unsigned short i,
		 const unsigned short j) const
  {
	return this->v[i*4+j];
  } // end of operator()
  /*!
   * \return the runtime properties
   */
  TFEL_MATH_INLINE RunTimeProperties
  getRunTimeProperties() const
  {
	return RunTimeProperties();
  }
}; // end of struct ST2toT2Expr<ST2toT2ResultType,T2toT2ST2toT2ProductExpr>

/*!
 * Partial specialisation
 */
template<typename ST2toT2ResultType>
struct TFEL_VISIBILITY_LOCAL ST2toT2Expr<ST2toT2ResultType,T2toT2ST2toT2ProductExpr<3u> >
  : public ST2toT2Concept<ST2toT2Expr<ST2toT2ResultType,T2toT2ST2toT2ProductExpr<3u> > >,
	public fsarray<TensorDimeToSize<ST2toT2Traits<ST2toT2ResultType>::dime>::value*
		       StensorDimeToSize<ST2toT2Traits<ST2toT2ResultType>::dime>::value,
		       typename ST2toT2Traits<ST2toT2ResultType>::NumType>
{
  //! a simple alias
  typedef EmptyRunTimeProperties RunTimeProperties;
  //! a simple alias
  typedef typename ST2toT2Traits<ST2toT2ResultType>::NumType value_type;
  //! a simple check
  TFEL_STATIC_ASSERT((ST2toT2Traits<ST2toT2ResultType>::dime==3u));
  /*!
   * \param[in] a : first term of the product
   * \param[in] b : second term of the product
   */
  template<typename T2toT2Type,
	       typename ST2toT2Type2>
  TFEL_MATH_INLINE
  ST2toT2Expr(const T2toT2Type& a,
		 const ST2toT2Type2& b)
  {
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<T2toT2Type,T2toT2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((tfel::meta::Implements<ST2toT2Type2,ST2toT2Concept>::cond));
	//! a simple check
	TFEL_STATIC_ASSERT((T2toT2Traits<T2toT2Type>::dime==3u));
	//! a simple check
	TFEL_STATIC_ASSERT((ST2toT2Traits <ST2toT2Type2>::dime==3u));
this->v[0]=a(0,0)*b(0,0)+a(0,1)*b(1,0)+a(0,2)*b(2,0)+a(0,3)*b(3,0)+a(0,4)*b(4,0)+a(0,5)*b(5,0)+a(0,6)*b(6,0)+a(0,7)*b(7,0)+a(0,8)*b(8,0);
this->v[1]=a(0,0)*b(0,1)+a(0,1)*b(1,1)+a(0,2)*b(2,1)+a(0,3)*b(3,1)+a(0,4)*b(4,1)+a(0,5)*b(5,1)+a(0,6)*b(6,1)+a(0,7)*b(7,1)+a(0,8)*b(8,1);
this->v[2]=a(0,0)*b(0,2)+a(0,1)*b(1,2)+a(0,2)*b(2,2)+a(0,3)*b(3,2)+a(0,4)*b(4,2)+a(0,5)*b(5,2)+a(0,6)*b(6,2)+a(0,7)*b(7,2)+a(0,8)*b(8,2);
this->v[3]=a(0,0)*b(0,3)+a(0,1)*b(1,3)+a(0,2)*b(2,3)+a(0,3)*b(3,3)+a(0,4)*b(4,3)+a(0,5)*b(5,3)+a(0,6)*b(6,3)+a(0,7)*b(7,3)+a(0,8)*b(8,3);
this->v[4]=a(0,0)*b(0,4)+a(0,1)*b(1,4)+a(0,2)*b(2,4)+a(0,3)*b(3,4)+a(0,4)*b(4,4)+a(0,5)*b(5,4)+a(0,6)*b(6,4)+a(0,7)*b(7,4)+a(0,8)*b(8,4);
this->v[5]=a(0,0)*b(0,5)+a(0,1)*b(1,5)+a(0,2)*b(2,5)+a(0,3)*b(3,5)+a(0,4)*b(4,5)+a(0,5)*b(5,5)+a(0,6)*b(6,5)+a(0,7)*b(7,5)+a(0,8)*b(8,5);
this->v[6]=a(1,0)*b(0,0)+a(1,1)*b(1,0)+a(1,2)*b(2,0)+a(1,3)*b(3,0)+a(1,4)*b(4,0)+a(1,5)*b(5,0)+a(1,6)*b(6,0)+a(1,7)*b(7,0)+a(1,8)*b(8,0);
this->v[7]=a(1,0)*b(0,1)+a(1,1)*b(1,1)+a(1,2)*b(2,1)+a(1,3)*b(3,1)+a(1,4)*b(4,1)+a(1,5)*b(5,1)+a(1,6)*b(6,1)+a(1,7)*b(7,1)+a(1,8)*b(8,1);
this->v[8]=a(1,0)*b(0,2)+a(1,1)*b(1,2)+a(1,2)*b(2,2)+a(1,3)*b(3,2)+a(1,4)*b(4,2)+a(1,5)*b(5,2)+a(1,6)*b(6,2)+a(1,7)*b(7,2)+a(1,8)*b(8,2);
this->v[9]=a(1,0)*b(0,3)+a(1,1)*b(1,3)+a(1,2)*b(2,3)+a(1,3)*b(3,3)+a(1,4)*b(4,3)+a(1,5)*b(5,3)+a(1,6)*b(6,3)+a(1,7)*b(7,3)+a(1,8)*b(8,3);
this->v[10]=a(1,0)*b(0,4)+a(1,1)*b(1,4)+a(1,2)*b(2,4)+a(1,3)*b(3,4)+a(1,4)*b(4,4)+a(1,5)*b(5,4)+a(1,6)*b(6,4)+a(1,7)*b(7,4)+a(1,8)*b(8,4);
this->v[11]=a(1,0)*b(0,5)+a(1,1)*b(1,5)+a(1,2)*b(2,5)+a(1,3)*b(3,5)+a(1,4)*b(4,5)+a(1,5)*b(5,5)+a(1,6)*b(6,5)+a(1,7)*b(7,5)+a(1,8)*b(8,5);
this->v[12]=a(2,0)*b(0,0)+a(2,1)*b(1,0)+a(2,2)*b(2,0)+a(2,3)*b(3,0)+a(2,4)*b(4,0)+a(2,5)*b(5,0)+a(2,6)*b(6,0)+a(2,7)*b(7,0)+a(2,8)*b(8,0);
this->v[13]=a(2,0)*b(0,1)+a(2,1)*b(1,1)+a(2,2)*b(2,1)+a(2,3)*b(3,1)+a(2,4)*b(4,1)+a(2,5)*b(5,1)+a(2,6)*b(6,1)+a(2,7)*b(7,1)+a(2,8)*b(8,1);
this->v[14]=a(2,0)*b(0,2)+a(2,1)*b(1,2)+a(2,2)*b(2,2)+a(2,3)*b(3,2)+a(2,4)*b(4,2)+a(2,5)*b(5,2)+a(2,6)*b(6,2)+a(2,7)*b(7,2)+a(2,8)*b(8,2);
this->v[15]=a(2,0)*b(0,3)+a(2,1)*b(1,3)+a(2,2)*b(2,3)+a(2,3)*b(3,3)+a(2,4)*b(4,3)+a(2,5)*b(5,3)+a(2,6)*b(6,3)+a(2,7)*b(7,3)+a(2,8)*b(8,3);
this->v[16]=a(2,0)*b(0,4)+a(2,1)*b(1,4)+a(2,2)*b(2,4)+a(2,3)*b(3,4)+a(2,4)*b(4,4)+a(2,5)*b(5,4)+a(2,6)*b(6,4)+a(2,7)*b(7,4)+a(2,8)*b(8,4);
this->v[17]=a(2,0)*b(0,5)+a(2,1)*b(1,5)+a(2,2)*b(2,5)+a(2,3)*b(3,5)+a(2,4)*b(4,5)+a(2,5)*b(5,5)+a(2,6)*b(6,5)+a(2,7)*b(7,5)+a(2,8)*b(8,5);
this->v[18]=a(3,0)*b(0,0)+a(3,1)*b(1,0)+a(3,2)*b(2,0)+a(3,3)*b(3,0)+a(3,4)*b(4,0)+a(3,5)*b(5,0)+a(3,6)*b(6,0)+a(3,7)*b(7,0)+a(3,8)*b(8,0);
this->v[19]=a(3,0)*b(0,1)+a(3,1)*b(1,1)+a(3,2)*b(2,1)+a(3,3)*b(3,1)+a(3,4)*b(4,1)+a(3,5)*b(5,1)+a(3,6)*b(6,1)+a(3,7)*b(7,1)+a(3,8)*b(8,1);
this->v[20]=a(3,0)*b(0,2)+a(3,1)*b(1,2)+a(3,2)*b(2,2)+a(3,3)*b(3,2)+a(3,4)*b(4,2)+a(3,5)*b(5,2)+a(3,6)*b(6,2)+a(3,7)*b(7,2)+a(3,8)*b(8,2);
this->v[21]=a(3,0)*b(0,3)+a(3,1)*b(1,3)+a(3,2)*b(2,3)+a(3,3)*b(3,3)+a(3,4)*b(4,3)+a(3,5)*b(5,3)+a(3,6)*b(6,3)+a(3,7)*b(7,3)+a(3,8)*b(8,3);
this->v[22]=a(3,0)*b(0,4)+a(3,1)*b(1,4)+a(3,2)*b(2,4)+a(3,3)*b(3,4)+a(3,4)*b(4,4)+a(3,5)*b(5,4)+a(3,6)*b(6,4)+a(3,7)*b(7,4)+a(3,8)*b(8,4);
this->v[23]=a(3,0)*b(0,5)+a(3,1)*b(1,5)+a(3,2)*b(2,5)+a(3,3)*b(3,5)+a(3,4)*b(4,5)+a(3,5)*b(5,5)+a(3,6)*b(6,5)+a(3,7)*b(7,5)+a(3,8)*b(8,5);
this->v[24]=a(4,0)*b(0,0)+a(4,1)*b(1,0)+a(4,2)*b(2,0)+a(4,3)*b(3,0)+a(4,4)*b(4,0)+a(4,5)*b(5,0)+a(4,6)*b(6,0)+a(4,7)*b(7,0)+a(4,8)*b(8,0);
this->v[25]=a(4,0)*b(0,1)+a(4,1)*b(1,1)+a(4,2)*b(2,1)+a(4,3)*b(3,1)+a(4,4)*b(4,1)+a(4,5)*b(5,1)+a(4,6)*b(6,1)+a(4,7)*b(7,1)+a(4,8)*b(8,1);
this->v[26]=a(4,0)*b(0,2)+a(4,1)*b(1,2)+a(4,2)*b(2,2)+a(4,3)*b(3,2)+a(4,4)*b(4,2)+a(4,5)*b(5,2)+a(4,6)*b(6,2)+a(4,7)*b(7,2)+a(4,8)*b(8,2);
this->v[27]=a(4,0)*b(0,3)+a(4,1)*b(1,3)+a(4,2)*b(2,3)+a(4,3)*b(3,3)+a(4,4)*b(4,3)+a(4,5)*b(5,3)+a(4,6)*b(6,3)+a(4,7)*b(7,3)+a(4,8)*b(8,3);
this->v[28]=a(4,0)*b(0,4)+a(4,1)*b(1,4)+a(4,2)*b(2,4)+a(4,3)*b(3,4)+a(4,4)*b(4,4)+a(4,5)*b(5,4)+a(4,6)*b(6,4)+a(4,7)*b(7,4)+a(4,8)*b(8,4);
this->v[29]=a(4,0)*b(0,5)+a(4,1)*b(1,5)+a(4,2)*b(2,5)+a(4,3)*b(3,5)+a(4,4)*b(4,5)+a(4,5)*b(5,5)+a(4,6)*b(6,5)+a(4,7)*b(7,5)+a(4,8)*b(8,5);
this->v[30]=a(5,0)*b(0,0)+a(5,1)*b(1,0)+a(5,2)*b(2,0)+a(5,3)*b(3,0)+a(5,4)*b(4,0)+a(5,5)*b(5,0)+a(5,6)*b(6,0)+a(5,7)*b(7,0)+a(5,8)*b(8,0);
this->v[31]=a(5,0)*b(0,1)+a(5,1)*b(1,1)+a(5,2)*b(2,1)+a(5,3)*b(3,1)+a(5,4)*b(4,1)+a(5,5)*b(5,1)+a(5,6)*b(6,1)+a(5,7)*b(7,1)+a(5,8)*b(8,1);
this->v[32]=a(5,0)*b(0,2)+a(5,1)*b(1,2)+a(5,2)*b(2,2)+a(5,3)*b(3,2)+a(5,4)*b(4,2)+a(5,5)*b(5,2)+a(5,6)*b(6,2)+a(5,7)*b(7,2)+a(5,8)*b(8,2);
this->v[33]=a(5,0)*b(0,3)+a(5,1)*b(1,3)+a(5,2)*b(2,3)+a(5,3)*b(3,3)+a(5,4)*b(4,3)+a(5,5)*b(5,3)+a(5,6)*b(6,3)+a(5,7)*b(7,3)+a(5,8)*b(8,3);
this->v[34]=a(5,0)*b(0,4)+a(5,1)*b(1,4)+a(5,2)*b(2,4)+a(5,3)*b(3,4)+a(5,4)*b(4,4)+a(5,5)*b(5,4)+a(5,6)*b(6,4)+a(5,7)*b(7,4)+a(5,8)*b(8,4);
this->v[35]=a(5,0)*b(0,5)+a(5,1)*b(1,5)+a(5,2)*b(2,5)+a(5,3)*b(3,5)+a(5,4)*b(4,5)+a(5,5)*b(5,5)+a(5,6)*b(6,5)+a(5,7)*b(7,5)+a(5,8)*b(8,5);
this->v[36]=a(6,0)*b(0,0)+a(6,1)*b(1,0)+a(6,2)*b(2,0)+a(6,3)*b(3,0)+a(6,4)*b(4,0)+a(6,5)*b(5,0)+a(6,6)*b(6,0)+a(6,7)*b(7,0)+a(6,8)*b(8,0);
this->v[37]=a(6,0)*b(0,1)+a(6,1)*b(1,1)+a(6,2)*b(2,1)+a(6,3)*b(3,1)+a(6,4)*b(4,1)+a(6,5)*b(5,1)+a(6,6)*b(6,1)+a(6,7)*b(7,1)+a(6,8)*b(8,1);
this->v[38]=a(6,0)*b(0,2)+a(6,1)*b(1,2)+a(6,2)*b(2,2)+a(6,3)*b(3,2)+a(6,4)*b(4,2)+a(6,5)*b(5,2)+a(6,6)*b(6,2)+a(6,7)*b(7,2)+a(6,8)*b(8,2);
this->v[39]=a(6,0)*b(0,3)+a(6,1)*b(1,3)+a(6,2)*b(2,3)+a(6,3)*b(3,3)+a(6,4)*b(4,3)+a(6,5)*b(5,3)+a(6,6)*b(6,3)+a(6,7)*b(7,3)+a(6,8)*b(8,3);
this->v[40]=a(6,0)*b(0,4)+a(6,1)*b(1,4)+a(6,2)*b(2,4)+a(6,3)*b(3,4)+a(6,4)*b(4,4)+a(6,5)*b(5,4)+a(6,6)*b(6,4)+a(6,7)*b(7,4)+a(6,8)*b(8,4);
this->v[41]=a(6,0)*b(0,5)+a(6,1)*b(1,5)+a(6,2)*b(2,5)+a(6,3)*b(3,5)+a(6,4)*b(4,5)+a(6,5)*b(5,5)+a(6,6)*b(6,5)+a(6,7)*b(7,5)+a(6,8)*b(8,5);
this->v[42]=a(7,0)*b(0,0)+a(7,1)*b(1,0)+a(7,2)*b(2,0)+a(7,3)*b(3,0)+a(7,4)*b(4,0)+a(7,5)*b(5,0)+a(7,6)*b(6,0)+a(7,7)*b(7,0)+a(7,8)*b(8,0);
this->v[43]=a(7,0)*b(0,1)+a(7,1)*b(1,1)+a(7,2)*b(2,1)+a(7,3)*b(3,1)+a(7,4)*b(4,1)+a(7,5)*b(5,1)+a(7,6)*b(6,1)+a(7,7)*b(7,1)+a(7,8)*b(8,1);
this->v[44]=a(7,0)*b(0,2)+a(7,1)*b(1,2)+a(7,2)*b(2,2)+a(7,3)*b(3,2)+a(7,4)*b(4,2)+a(7,5)*b(5,2)+a(7,6)*b(6,2)+a(7,7)*b(7,2)+a(7,8)*b(8,2);
this->v[45]=a(7,0)*b(0,3)+a(7,1)*b(1,3)+a(7,2)*b(2,3)+a(7,3)*b(3,3)+a(7,4)*b(4,3)+a(7,5)*b(5,3)+a(7,6)*b(6,3)+a(7,7)*b(7,3)+a(7,8)*b(8,3);
this->v[46]=a(7,0)*b(0,4)+a(7,1)*b(1,4)+a(7,2)*b(2,4)+a(7,3)*b(3,4)+a(7,4)*b(4,4)+a(7,5)*b(5,4)+a(7,6)*b(6,4)+a(7,7)*b(7,4)+a(7,8)*b(8,4);
this->v[47]=a(7,0)*b(0,5)+a(7,1)*b(1,5)+a(7,2)*b(2,5)+a(7,3)*b(3,5)+a(7,4)*b(4,5)+a(7,5)*b(5,5)+a(7,6)*b(6,5)+a(7,7)*b(7,5)+a(7,8)*b(8,5);
this->v[48]=a(8,0)*b(0,0)+a(8,1)*b(1,0)+a(8,2)*b(2,0)+a(8,3)*b(3,0)+a(8,4)*b(4,0)+a(8,5)*b(5,0)+a(8,6)*b(6,0)+a(8,7)*b(7,0)+a(8,8)*b(8,0);
this->v[49]=a(8,0)*b(0,1)+a(8,1)*b(1,1)+a(8,2)*b(2,1)+a(8,3)*b(3,1)+a(8,4)*b(4,1)+a(8,5)*b(5,1)+a(8,6)*b(6,1)+a(8,7)*b(7,1)+a(8,8)*b(8,1);
this->v[50]=a(8,0)*b(0,2)+a(8,1)*b(1,2)+a(8,2)*b(2,2)+a(8,3)*b(3,2)+a(8,4)*b(4,2)+a(8,5)*b(5,2)+a(8,6)*b(6,2)+a(8,7)*b(7,2)+a(8,8)*b(8,2);
this->v[51]=a(8,0)*b(0,3)+a(8,1)*b(1,3)+a(8,2)*b(2,3)+a(8,3)*b(3,3)+a(8,4)*b(4,3)+a(8,5)*b(5,3)+a(8,6)*b(6,3)+a(8,7)*b(7,3)+a(8,8)*b(8,3);
this->v[52]=a(8,0)*b(0,4)+a(8,1)*b(1,4)+a(8,2)*b(2,4)+a(8,3)*b(3,4)+a(8,4)*b(4,4)+a(8,5)*b(5,4)+a(8,6)*b(6,4)+a(8,7)*b(7,4)+a(8,8)*b(8,4);
this->v[53]=a(8,0)*b(0,5)+a(8,1)*b(1,5)+a(8,2)*b(2,5)+a(8,3)*b(3,5)+a(8,4)*b(4,5)+a(8,5)*b(5,5)+a(8,6)*b(6,5)+a(8,7)*b(7,5)+a(8,8)*b(8,5);
  } // end of ST2toT2Expr
  /*!
   * \brief access operator
   * \param[in] i : line index
   * \param[in] j : column index
   */
  TFEL_MATH_INLINE const value_type&
  operator()(const unsigned short i,
		 const unsigned short j) const
  {
	return this->v[i*6+j];
  } // end of operator()
  /*!
   * \return the runtime properties
   */
  TFEL_MATH_INLINE RunTimeProperties
  getRunTimeProperties() const
  {
	return RunTimeProperties();
  }
}; // end of struct ST2toT2Expr<ST2toT2ResultType,T2toT2ST2toT2ProductExpr>

} // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_MATH_T2TOT2ST2TOT2PRODUCTEXPR_H */
