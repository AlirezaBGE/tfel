/*!
 * \file   StensorComputeEigenVectorsDerivatives.hxx
 * \brief    
 * \author Thomas Helfer
 * \date   04 janv. 2017
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_TFEL_MATH_INTERNALS_STENSORCOMPUTEEIGENVECTORSDERIVATIVES_HXX
#define LIB_TFEL_MATH_INTERNALS_STENSORCOMPUTEEIGENVECTORSDERIVATIVES_HXX

namespace tfel{

  namespace math {

    namespace internals{

      template<unsigned short N>
      struct StensorComputeEigenTensorsDerivatives;

      struct StensorComputeEigenTensorsDerivativesBase
      {
	template<typename NumType>
	TFEL_MATH_INLINE static
	typename ComputeBinaryResult<tfel::typetraits::base_type<NumType>,
				     NumType,OpDiv>::Result
	regularized_inverse(const NumType x,
			    const NumType eps)
	{
	  if(std::abs(x)<100*std::numeric_limits<NumType>::min()){
	    return NumType(0);
	  }
	  return regularization_function(x/eps)/x;
	} // end of regularization
      protected:
	template<typename NumType>
	TFEL_MATH_INLINE static
	NumType
	regularization_function(const NumType x)
	{
	  if(std::abs(x)>1){
	    return NumType(1);
	  }
	  return x*x*(4-x*x)/3;
	}
      };
      
      template<>
      struct StensorComputeEigenTensorsDerivatives<1u>
      {
	template<typename ST2toST2Type,
		 typename NumType>
	static TFEL_MATH_INLINE2
	typename std::enable_if<
	  (tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond)&&
	  (ST2toST2Traits<ST2toST2Type>::dime==1u)&&
	  (tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<tfel::typetraits::base_type<NumType>,
									 NumType,OpDiv>::Result,
					    ST2toST2NumType<ST2toST2Type>>::cond),
	  void>::type
	exe(ST2toST2Type& dn0_ds,ST2toST2Type& dn1_ds,ST2toST2Type& dn2_ds,
	    const tfel::math::tvector<3u,NumType>&,
	    const tfel::math::tmatrix<3u,3u,tfel::typetraits::base_type<NumType>>&,
	    const NumType)
	{
	  using namespace tfel::math;
	  using namespace tfel::typetraits;
	  typedef typename ComputeBinaryResult<base_type<NumType>,
					       NumType,OpDiv>::Result InvNumType;
	  dn0_ds = st2tost2<1u,InvNumType>(InvNumType(0));
	  dn1_ds = dn0_ds;
	  dn2_ds = dn0_ds;
	} // end of exe
      };

      template<>
      struct StensorComputeEigenTensorsDerivatives<2u>
	: public StensorComputeEigenTensorsDerivativesBase
      {
	template<typename ST2toST2Type,typename NumType>
	static TFEL_MATH_INLINE2
	typename std::enable_if<
	  (tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond)&&
	  (ST2toST2Traits<ST2toST2Type>::dime==2u)&&
	  (tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<tfel::typetraits::base_type<NumType>,
									 NumType,OpDiv>::Result,
					    ST2toST2NumType<ST2toST2Type>>::cond),
	  void>::type
	exe(ST2toST2Type& dn0_ds,ST2toST2Type& dn1_ds,ST2toST2Type& dn2_ds,
	    const tfel::math::tvector<3u,NumType>& vp,
	    const tfel::math::tmatrix<3u,3u,tfel::typetraits::base_type<NumType>>& m,
	    const NumType eps)
	{
	  typedef tfel::typetraits::base_type<NumType> base;
	  typedef typename ComputeBinaryResult<base,NumType,OpDiv>::Result InvNumType;
	  constexpr const auto icste = Cste<base>::isqrt2;
	  const tvector<3u,base> v0 = m.template column_view<0u>();
	  const tvector<3u,base> v1 = m.template column_view<1u>();
	  const stensor<2u,base> n01 = stensor<2u,base>::buildFromVectorsSymmetricDiadicProduct(v0,v1)*icste;
	  dn0_ds = regularized_inverse(vp(0)-vp(1),eps)*(n01^n01);
	  dn1_ds = -dn0_ds;
	  dn2_ds = st2tost2<2u,InvNumType>(InvNumType(0));
	} // end of exe
      };

      template<>
      struct StensorComputeEigenTensorsDerivatives<3u>
	: public StensorComputeEigenTensorsDerivativesBase
      {
	template<typename ST2toST2Type,
		 typename NumType>
	static TFEL_MATH_INLINE2
	typename std::enable_if<
	  (tfel::meta::Implements<ST2toST2Type,ST2toST2Concept>::cond)&&
	  (ST2toST2Traits<ST2toST2Type>::dime==3u)&&
	  (tfel::typetraits::IsAssignableTo<typename ComputeBinaryResult<tfel::typetraits::base_type<NumType>,
									 NumType,OpDiv>::Result,
					    ST2toST2NumType<ST2toST2Type>>::cond),
	  void>::type
	exe(ST2toST2Type& dn0_ds,ST2toST2Type& dn1_ds,ST2toST2Type& dn2_ds,
	    const tfel::math::tvector<3u,NumType>& vp,
	    const tfel::math::tmatrix<3u,3u,tfel::typetraits::base_type<NumType>>& m,
	    const NumType eps)
	{
	  using namespace tfel::math;
	  typedef tfel::typetraits::base_type<NumType> base;
	  constexpr const auto cste = Cste<base>::isqrt2;
	  const tvector<3u,base> v0 = m.template column_view<0u>();
	  const tvector<3u,base> v1 = m.template column_view<1u>();
	  const tvector<3u,base> v2 = m.template column_view<2u>();
	  const stensor<3u,base> n01 = stensor<3u,base>::buildFromVectorsSymmetricDiadicProduct(v0,v1)*cste;
	  const stensor<3u,base> n02 = stensor<3u,base>::buildFromVectorsSymmetricDiadicProduct(v0,v2)*cste;
	  const stensor<3u,base> n12 = stensor<3u,base>::buildFromVectorsSymmetricDiadicProduct(v1,v2)*cste;
	  dn0_ds = regularized_inverse(vp(0)-vp(1),eps)*(n01^n01)+regularized_inverse(vp(0)-vp(2),eps)*(n02^n02);
	  dn1_ds = regularized_inverse(vp(1)-vp(0),eps)*(n01^n01)+regularized_inverse(vp(1)-vp(2),eps)*(n12^n12);
	  dn2_ds = regularized_inverse(vp(2)-vp(0),eps)*(n02^n02)+regularized_inverse(vp(2)-vp(1),eps)*(n12^n12);
	} // end of exe
      };
      
    } // end of namespace internals

  } //end of namespace math

} // end of namespace tfel

#endif /* LIB_TFEL_MATH_INTERNALS_STENSORCOMPUTEEIGENVECTORSDERIVATIVES_HXX */
