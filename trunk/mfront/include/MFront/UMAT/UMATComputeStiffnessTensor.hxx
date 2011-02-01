/*!
 * \file   UMATComputeStiffnessTensor.hxx
 * \brief  This file declares the UMATComputeStiffnessTensor class
 * \author Helfer Thomas
 * \date   18 mar 2008
 */

#ifndef _LIB_MFRONT_UMATCOMPUTESTIFFNESSTENSOR_H_
#define _LIB_MFRONT_UMATCOMPUTESTIFFNESSTENSOR_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Config/TFELTypes.hxx"
#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATTraits.hxx"

namespace umat
{

  template<unsigned short N,
	   UMATBehaviourType>
  struct UMATComputeStiffnessTensor;

  template<>
  struct TFEL_VISIBILITY_EXPORT UMATComputeStiffnessTensor<1u,ISOTROPIC>
  {
    static void
    exe(const UMATReal* const,
	tfel::config::Types<1u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct TFEL_VISIBILITY_EXPORT UMATComputeStiffnessTensor<2u,ISOTROPIC>
  {
    static void
    exe(const UMATReal* const,
	tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct TFEL_VISIBILITY_EXPORT UMATComputeStiffnessTensor<3u,ISOTROPIC>
  {
    static void
    exe(const UMATReal* const,
	tfel::config::Types<3u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct TFEL_VISIBILITY_EXPORT UMATComputeStiffnessTensor<1u,ORTHOTROPIC>
  {
    static void
    exe(const UMATReal* const,
	tfel::config::Types<1u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct TFEL_VISIBILITY_EXPORT UMATComputeStiffnessTensor<2u,ORTHOTROPIC>
  {
    static void
    exe(const UMATReal* const,
	tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

  template<>
  struct TFEL_VISIBILITY_EXPORT UMATComputeStiffnessTensor<3u,ORTHOTROPIC>
  {
    static void
    exe(const UMATReal* const,
	tfel::config::Types<3u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessTensor

} // end of namespace umat

#endif /* _LIB_MFRONT_UMATCOMPUTESTIFFNESSTENSOR_H */

