/*! 
 * \file  mfront/include/MFront/Aster/AsterBehaviourHandler.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_ASTER_ASTERBEHAVIOURHANDLER_H_
#define _LIB_MFRONT_ASTER_ASTERBEHAVIOURHANDLER_H_ 

#ifndef _LIB_MFRONT_ASTER_CALL_H_
#error "This header shall not be called directly"
#endif

#include"TFEL/Material/MechanicalBehaviour.hxx"

#include"MFront/Aster/AsterTangentOperator.hxx"
#include"MFront/Aster/AsterComputeStiffnessTensor.hxx"
#include"MFront/Aster/AsterComputeThermalExpansionCoefficientTensor.hxx"

namespace aster
{

  template<AsterBehaviourType btype>
  struct AsterTangentOperatorFlag;

  template<>
  struct AsterTangentOperatorFlag<aster::SMALLSTRAINSTANDARDBEHAVIOUR>
  {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase; 
    typedef tfel::material::TangentOperatorTraits<MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR>
    TangentOperatorTraits;
    static const TangentOperatorTraits::SMFlag value = TangentOperatorTraits::STANDARDTANGENTOPERATOR;
  };

  template<>
  struct AsterTangentOperatorFlag<aster::FINITESTRAINSTANDARDBEHAVIOUR>
  {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase; 
    typedef tfel::material::TangentOperatorTraits<MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR>
    TangentOperatorTraits;
    static const TangentOperatorTraits::SMFlag value = TangentOperatorTraits::DTAU_DDF;
  };

  template<>
  struct AsterTangentOperatorFlag<aster::COHESIVEZONEMODEL>
  {
    typedef tfel::material::MechanicalBehaviourBase MechanicalBehaviourBase; 
    typedef tfel::material::TangentOperatorTraits<MechanicalBehaviourBase::COHESIVEZONEMODEL>
    TangentOperatorTraits;
    static const TangentOperatorTraits::SMFlag value = TangentOperatorTraits::STANDARDTANGENTOPERATOR;
  };

  template<AsterBehaviourType btype,
	   unsigned short N>
  struct AsterTangentOperatorType;

  template<unsigned short N>
  struct AsterTangentOperatorType<aster::SMALLSTRAINSTANDARDBEHAVIOUR,N>
  {
    typedef tfel::math::st2tost2<N,AsterReal> type;
  };

  template<unsigned short N>
  struct AsterTangentOperatorType<aster::FINITESTRAINSTANDARDBEHAVIOUR,N>
  {
    typedef tfel::math::t2tost2<N,AsterReal> type;
  };

  template<unsigned short N>
  struct AsterTangentOperatorType<aster::COHESIVEZONEMODEL,N>
  {
    typedef tfel::math::tmatrix<N,N,AsterReal> type;
  };

  template<tfel::material::ModellingHypothesis::Hypothesis H,
	   template<tfel::material::ModellingHypothesis::Hypothesis,typename,bool> class Behaviour>
  struct TFEL_VISIBILITY_LOCAL AsterBehaviourHandler
    : public AsterInterfaceBase
  {

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithStressFreeExpansion
      : public AsterInterfaceBase
      {
      //! a simple alias
      typedef Behaviour<H,AsterReal,false> BV;
      /*!
       * \param[out] b      : behaviour
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                      time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_ASTER_INLINE static 
      void exe(BV& b,
	       const AsterReal *const STRAN,
	       const AsterReal *const DSTRAN,
	       const StressFreeExpansionHandler& sfeh)
      {
	using tfel::material::ModellingHypothesisToSpaceDimension;
	using tfel::utilities::Name;
	using tfel::fsalgo::copy;
	using std::pair;
	typedef typename BV::StressFreeExpansionType StressFreeExpansionType;
	const AsterInt N = ModellingHypothesisToSpaceDimension<H>::value;
	AsterReal dv0[AsterTraits<BV>::DrivingVariableSize];
	AsterReal dv1[AsterTraits<BV>::DrivingVariableSize];
	copy<AsterTraits<BV>::DrivingVariableSize>::exe(STRAN,dv0);
	copy<AsterTraits<BV>::DrivingVariableSize>::exe(DSTRAN,dv1);
	// check that the function pointer are not null
	if(sfeh==0){
	  throwUnsupportedStressFreeExpansionException(Name<BV>::getName());
	}
	pair<StressFreeExpansionType,StressFreeExpansionType> s;
	b.computeStressFreeExpansion(s);
	const StressFreeExpansionType& s0 = s.first;
	const StressFreeExpansionType& s1 = s.second;
	sfeh(dv0,dv1,&s0[0],&s1[0],N);
	b.setASTERBehaviourDataDrivingVariables(dv0);
	b.setASTERIntegrationDataDrivingVariables(dv1);
      } // end of exe

    }; // end of struct DrivingVariableInitialiserWithStressFreeExpansion

    /*!
     * An helper structure used to initialise the driving variables
     */
    struct TFEL_VISIBILITY_LOCAL DrivingVariableInitialiserWithoutStressFreeExpansion
    {
      //! a simple alias
      typedef Behaviour<H,AsterReal,false> BV;
      /*!
       * \param[out] b      : b
       * \param[in]  STRAN  : driving variable at the beginning of the
       *                     time step
       * \param[in]  DSTRAN : driving variable at the end of the
       *                      time step or driving variable increment
       * \param[in]  sfeh   : function handling the stress-free expansion
       *                      at the beginning of the time step
       */
      TFEL_ASTER_INLINE static 
      void exe(BV& b,
	       const AsterReal *const STRAN,
	       const AsterReal *const DSTRAN,
	       const StressFreeExpansionHandler&)
      {
	b.setASTERBehaviourDataDrivingVariables(STRAN);
	b.setASTERIntegrationDataDrivingVariables(DSTRAN);
      } // end of exe
    }; // end of struct DrivingVariableInitialiserWithoutStressFreeExpansion

    struct TFEL_VISIBILITY_LOCAL StiffnessOperatorInitializer
    {
      typedef Behaviour<H,AsterReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_ASTER_INLINE static void
      exe(BData& data,const AsterReal * const props){
	typedef AsterTraits<BV> Traits;
	const bool buas = Traits::requiresUnAlteredStiffnessTensor;
	AsterComputeStiffnessTensor<AsterTraits<BV>::btype,H,
				    AsterTraits<BV>::etype,buas>::exe(data.getStiffnessTensor(),
								      props);
      } // end of exe
    }; // end of struct StiffnessOperatorInitializer

    struct TFEL_VISIBILITY_LOCAL ThermalExpansionCoefficientTensorInitializer
    {
      typedef Behaviour<H,AsterReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_ASTER_INLINE static void
	exe(BData& data,const AsterReal * const props){
	const unsigned short o =
	  AsterTraits<BV>::elasticPropertiesOffset;
	AsterComputeThermalExpansionCoefficientTensor<AsterTraits<BV>::btype,H,
						      AsterTraits<BV>::stype>::exe(props+o,
										   data.getThermalExpansionCoefficientTensor());
      } // end of exe
    }; // end of struct ThermalExpansionCoefficientTensorInitializer

    struct TFEL_VISIBILITY_LOCAL DoNothingInitializer
    {
      typedef Behaviour<H,AsterReal,false> BV;
      typedef typename BV::BehaviourData  BData;
      TFEL_ASTER_INLINE static void
	exe(BData&,const AsterReal * const)
      {}
    }; // end of struct DoNothingInitializer

    struct TFEL_VISIBILITY_LOCAL Error
    {
	
      TFEL_ASTER_INLINE
      Error(const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const AsterReal *const,
	    const StressFreeExpansionHandler&)
      {} // end of Error

      TFEL_ASTER_INLINE void exe(AsterReal *const,
				 AsterReal *const,
				 AsterReal *const)
	throw(AsterException)
      {
	using namespace std;
	using namespace tfel::utilities;
	typedef Behaviour<H,AsterReal,false> BV;
	throw(AsterInvalidModellingHypothesis(Name<BV>::getName(),H));
	return;
      } // end of Error::exe
	
    }; // end of struct Error

    template<const bool bs,     // requires StiffnessOperator
	     const bool ba>     // requires ThermalExpansionCoefficientTensor
      struct TFEL_VISIBILITY_LOCAL Integrator
    {
      typedef typename tfel::meta::IF<bs,
				      StiffnessOperatorInitializer,
				      DoNothingInitializer>::type SInitializer;

      typedef typename tfel::meta::IF<ba,
				      ThermalExpansionCoefficientTensorInitializer,
				      DoNothingInitializer>::type AInitializer;

      TFEL_ASTER_INLINE Integrator(const AsterReal *const DTIME ,
				   const AsterReal *const STRAN ,
				   const AsterReal *const DSTRAN,
				   const AsterReal *const TEMP,
				   const AsterReal *const DTEMP,
				   const AsterReal *const PROPS ,
				   const AsterReal *const PREDEF,
				   const AsterReal *const DPRED,
				   const AsterReal *const STATEV,
				   const AsterReal *const STRESS,
				   const tfel::material::OutOfBoundsPolicy op,
				   const StressFreeExpansionHandler& sfeh)
	: behaviour(DTIME,TEMP,DTEMP,
		    PROPS+AsterTraits<BV>::elasticPropertiesOffset+
		    AsterTraits<BV>::thermalExpansionPropertiesOffset,
		    STATEV,PREDEF,DPRED),
	dt(*DTIME)
	  {
	    using namespace tfel::material;
	    typedef MechanicalBehaviourTraits<BV> Traits;
	    typedef typename tfel::meta::IF<
	      Traits::hasStressFreeExpansion,
	      DrivingVariableInitialiserWithStressFreeExpansion,
	      DrivingVariableInitialiserWithoutStressFreeExpansion>::type DVInitializer;
	    SInitializer::exe(this->behaviour,PROPS);
	    AInitializer::exe(this->behaviour,PROPS);
	    DVInitializer::exe(this->behaviour,STRAN,DSTRAN,sfeh);
	    this->behaviour.setASTERBehaviourDataThermodynamicForces(STRESS);
	    this->behaviour.setOutOfBoundsPolicy(op);
	    this->behaviour.initialize();
	  } // end of Integrator::Integrator
	
      TFEL_ASTER_INLINE2
	void exe(AsterReal *const DDSOE,
		 AsterReal *const STRESS,
		 AsterReal *const STATEV)
      {
	using namespace tfel::utilities;
	using namespace tfel::material;
	typedef MechanicalBehaviourTraits<BV> Traits;
	typedef typename tfel::meta::IF<
	  Traits::hasConsistentTangentOperator,
	  typename tfel::meta::IF<
	    Traits::isConsistentTangentOperatorSymmetric,
	    SymmetricConsistentTangentOperatorComputer,
	    GeneralConsistentTangentOperatorComputer>::type,
	  ConsistentTangentOperatorIsNotAvalaible
	  >::type ConsistentTangentOperatorHandler;
	typedef typename tfel::meta::IF<
	  Traits::hasPredictionOperator,
	  StandardPredictionOperatorComputer,
	  PredictionOperatorIsNotAvalaible
	  >::type PredictionOperatorComputer;
	if(this->dt<0.){
	  throwNegativeTimeStepException(Name<BV>::getName());
	}
	this->behaviour.checkBounds();
	typename BV::IntegrationResult r = BV::SUCCESS;
	const typename BV::SMFlag smflag = AsterTangentOperatorFlag<AsterTraits<BV>::btype>::value;
	if((-3.25<*DDSOE)&&(*DDSOE<-2.75)){
	  r = PredictionOperatorComputer::exe(this->behaviour,smflag,
					      BV::TANGENTOPERATOR);
	} else if((-2.25<*DDSOE)&&(*DDSOE<-1.75)){
	  r = PredictionOperatorComputer::exe(this->behaviour,smflag,
					      BV::SECANTOPERATOR);
	} else if((-1.25<*DDSOE)&&(*DDSOE<-0.75)){
	  r = PredictionOperatorComputer::exe(this->behaviour,smflag,
					      BV::ELASTIC);
	} else if((-0.25<*DDSOE)&&(*DDSOE<0.25)){
	  r = this->behaviour.integrate(smflag,BV::NOSTIFFNESSREQUESTED);
	} else if((0.75<*DDSOE)&&(*DDSOE<1.25)){
	  r = this->behaviour.integrate(smflag,BV::ELASTIC);
	} else if((1.75<*DDSOE)&&(*DDSOE<2.25)){
	  r = this->behaviour.integrate(smflag,BV::SECANTOPERATOR);
	} else if((2.75<*DDSOE)&&(*DDSOE<3.25)){
	  r = this->behaviour.integrate(smflag,BV::TANGENTOPERATOR);
	} else if((3.75<*DDSOE)&&(*DDSOE<4.25)){
	  r = this->behaviour.integrate(smflag,BV::CONSISTENTTANGENTOPERATOR);
	} else {
	  throwInvalidDDSOEException(Name<BV>::getName(),*DDSOE);
	}
	if(r==BV::FAILURE){
	  // Il manque un vraie gestion locale de résultats imprécis
	  if(*DDSOE<-0.5){
	    throwPredictionComputationFailedException(Name<BV>::getName());
	  } else {
	    throwBehaviourIntegrationFailedException(Name<BV>::getName());
	  }
	}
	this->behaviour.checkBounds();
	this->behaviour.ASTERexportStateData(STRESS,STATEV);
	if((*DDSOE>0.5)||(*DDSOE<-0.5)){
	  ConsistentTangentOperatorHandler::exe(this->behaviour,DDSOE);
	}
      } // end of Integrator::exe
	
    private:

      struct StandardPredictionOperatorComputer
      {
	typedef Behaviour<H,AsterReal,false> BV;
	static typename BV::IntegrationResult
	exe(BV& b,const typename BV::SMFlag smf,const typename BV::SMType smt)
	{
	  return b.computePredictionOperator(smf,smt);
	} // end of exe	  
      };

      struct PredictionOperatorIsNotAvalaible
      {
	typedef Behaviour<H,AsterReal,false> BV;
	static typename BV::IntegrationResult
	exe(BV&,const typename BV::SMFlag,
	    const typename BV::SMType)
	{
	  using namespace tfel::utilities;
	  throwPredictionOperatorIsNotAvalaible(Name<BV>::getName());
	  return BV::FAILURE;
	} // end of exe	  
      };
      
      struct ConsistentTangentOperatorIsNotAvalaible
      {
	typedef Behaviour<H,AsterReal,false> BV;
	static void exe(BV&,AsterReal *const)
	{
	  using namespace tfel::utilities;
	  throwConsistentTangentOperatorIsNotAvalaible(Name<BV>::getName());
	} // end of exe	  
      };

      struct ConsistentTangentOperatorComputer
      {
	typedef Behaviour<H,AsterReal,false> BV;
	static void exe(const BV& bv,AsterReal *const DDSOE)
	{
	  using tfel::material::ModellingHypothesisToSpaceDimension;
	  const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
	  typedef typename AsterTangentOperatorType<AsterTraits<BV>::btype,N>::type TangentOperatorType;
	  TangentOperatorType& Dt = *(reinterpret_cast<TangentOperatorType*>(DDSOE));
	  Dt = bv.getTangentOperator();
	  // l'opérateur tangent contient des sqrt(2) en petites déformations...
	  AsterTangentOperator::normalize(Dt);
	} // end of exe	  
      };

      struct SymmetricConsistentTangentOperatorComputer
      {
	typedef Behaviour<H,AsterReal,false> BV;
	static void exe(const BV& bv,AsterReal *const DDSOE)
	{
	  ConsistentTangentOperatorComputer::exe(bv,DDSOE);
	} // end of exe	  
      };

      struct GeneralConsistentTangentOperatorComputer
      {
	typedef Behaviour<H,AsterReal,false> BV;
	static void exe(const BV& bv,AsterReal *const DDSOE)
	{
	  using tfel::material::ModellingHypothesisToSpaceDimension;
	  const unsigned short N = ModellingHypothesisToSpaceDimension<H>::value;
	  typedef typename AsterTangentOperatorType<AsterTraits<BV>::btype,N>::type TangentOperatorType;
	  ConsistentTangentOperatorComputer::exe(bv,DDSOE);
	  // les conventions fortran.... (petites déformations et modèles de zones cohésives)
	  TangentOperatorType&  Dt = *(reinterpret_cast<TangentOperatorType*>(DDSOE));
	  AsterTangentOperator::transpose(Dt);
	} // end of exe	  
      };

      typedef Behaviour<H,AsterReal,false> BV;
      BV behaviour;
      AsterReal dt;
    }; // end of struct Integrator

    TFEL_ASTER_INLINE2 static void
      checkNPROPS(const AsterInt NPROPS)
    {
      using namespace std;
      using namespace tfel::utilities;
      using namespace tfel::material;
      typedef Behaviour<H,AsterReal,false> BV;
      typedef MechanicalBehaviourTraits<BV> Traits;
      const unsigned short offset  = (AsterTraits<BV>::elasticPropertiesOffset+
				      AsterTraits<BV>::thermalExpansionPropertiesOffset);
      const unsigned short nprops  = AsterTraits<BV>::material_properties_nb;
      const unsigned short NPROPS_ = offset+nprops == 0 ? 1u : offset+nprops; 
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of properties matches Behaviour requirements
      if((NPROPS!=NPROPS_)&&is_defined_){
	throwUnMatchedNumberOfMaterialProperties(Name<BV>::getName(),
						 NPROPS_,NPROPS);
      }
    } // end of checkNPROPS
      
    TFEL_ASTER_INLINE2 static void
      checkNSTATV(const AsterInt NSTATV)
    {
      using namespace tfel::utilities;
      typedef Behaviour<H,AsterReal,false> BV;
      typedef tfel::material::MechanicalBehaviourTraits<BV> Traits;
      const unsigned short nstatv  = Traits::internal_variables_nb;
      const unsigned short NSTATV_ = nstatv == 0 ? 1u : nstatv;
      const bool is_defined_       = Traits::is_defined;
      //Test if the nb of state variables matches Behaviour requirements
      if((NSTATV_!=NSTATV)&&is_defined_){
	throwUnMatchedNumberOfStateVariables(Name<BV>::getName(),
					     NSTATV_,NSTATV);
      }
    } // end of checkNSTATV
      
  }; // end of struct AsterBehaviourHandler
  
} // end of namespace aster

#include"MFront/Aster/AsterIsotropicBehaviourHandler.hxx"
#include"MFront/Aster/AsterOrthotropicBehaviourHandler.hxx"

#endif /* _LIB_MFRONT_ASTER_ASTERBEHAVIOURHANDLER_H */
