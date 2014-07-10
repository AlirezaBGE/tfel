/*!
 * \file   UMATComputeStiffnessOperator.hxx
 * \brief  This file declares the UMATComputeStiffnessOperator class
 * \author Helfer Thomas
 * \date   18 mar 2008
 */

#ifndef _LIB_MFRONT_UMATCOMPUTESTIFFNESSOPERATOR_H_
#define _LIB_MFRONT_UMATCOMPUTESTIFFNESSOPERATOR_H_ 

#include"TFEL/Config/TFELTypes.hxx"
#include"TFEL/Material/ModellingHypothesis.hxx"

#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATConfig.hxx"
#include"MFront/UMAT/UMATTraits.hxx"

namespace umat
{

  template<UMATBehaviourType,
	   tfel::material::ModellingHypothesis::Hypothesis,
	   UMATSymmetryType>
  struct UMATComputeStiffnessOperator;

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<1u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessOperator

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::AXISYMMETRICAL,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessOperator

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::PLANESTRESS,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessOperator


  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::PLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessOperator

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessOperator

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::TRIDIMENSIONAL,ISOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<3u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessOperator

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<1u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessOperator

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::AXISYMMETRICAL,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessOperator

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::PLANESTRESS,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessOperator


  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::PLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessOperator

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::GENERALISEDPLANESTRAIN,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<2u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessOperator

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATComputeStiffnessOperator<umat::SMALLSTRAINSTANDARDBEHAVIOUR,
			       tfel::material::ModellingHypothesis::TRIDIMENSIONAL,ORTHOTROPIC>
  {
    static void
      exe(const UMATReal* const,
	  tfel::config::Types<3u,UMATReal,false>::StiffnessTensor&);
  }; // end of struct UMATComputeStiffnessOperator

} // end of namespace umat

#endif /* _LIB_MFRONT_UMATCOMPUTESTIFFNESSOPERATOR_H */

