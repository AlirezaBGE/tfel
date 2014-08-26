/*!
 * \file   MTestCyranoBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#include<cmath>
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Cyrano/Cyrano.hxx"
#include"MFront/MTestCyranoBehaviour.hxx"
#include"MFront/MTestUmatNormaliseTangentOperator.hxx"
#include"MFront/Cyrano/CyranoComputeStiffnessOperator.hxx"


namespace mfront
{

  MTestCyranoBehaviour::MTestCyranoBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
							       const std::string& l,
							       const std::string& b)
    : MTestUmatBehaviourBase(l,b)
  {
    using namespace std;
    using namespace tfel::system;
    using namespace tfel::material;
    typedef ExternalLibraryManager ELM;
    ELM& elm = ELM::getExternalLibraryManager();
    this->fct = elm.getUMATFunction(l,b);
    this->mpnames = elm.getUMATMaterialPropertiesNames(l,b);
    if(this->stype==0){
      this->mpnames.insert(this->mpnames.begin(),"ThermalExpansion");
      this->mpnames.insert(this->mpnames.begin(),"MassDensity");
      this->mpnames.insert(this->mpnames.begin(),"PoissonRatio");
      this->mpnames.insert(this->mpnames.begin(),"YoungModulus");
    } else {
      vector<string> tmp;
      if(h==ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	tmp.push_back("YoungModulus1");
	tmp.push_back("YoungModulus2");
	tmp.push_back("YoungModulus3");
	tmp.push_back("PoissonRatio12");
	tmp.push_back("PoissonRatio23");
	tmp.push_back("PoissonRatio13");
	tmp.push_back("MassDensity");
	tmp.push_back("ThermalExpansion1");
	tmp.push_back("ThermalExpansion2");
	tmp.push_back("ThermalExpansion3");
      } else {
	string msg("MTestUmatStandardBehaviour::MTestUmatStandardBehaviour : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }

      this->mpnames.insert(this->mpnames.begin(),tmp.begin(),tmp.end());
    }
    // if(h==ModellingHypothesis::PLANESTRESS){
    //   ELM& elm = ELM::getExternalLibraryManager();
    //   if(elm.checkIfCyranoBehaviourUsesGenericPlaneStressAlgorithm(l,b)){
    // 	this->ivnames.push_back("AxialStrain");
    // 	this->ivtypes.push_back(0);
    //   }
    // }
  }

  void
  MTestCyranoBehaviour::allocate(const tfel::material::ModellingHypothesis::Hypothesis h)
  {
    const unsigned short ndv     = this->getDrivingVariablesSize(h);
    const unsigned short nth     = this->getThermodynamicForcesSize(h);
    const unsigned short nstatev = this->getInternalStateVariablesSize(h);
    this->D.resize(nth,ndv);
    this->iv.resize(nstatev);
    if(iv.size()==0){
      iv.push_back(0.);
    }
  }

  MTestStiffnessMatrixType::mtype
  MTestCyranoBehaviour::getDefaultStiffnessMatrixType(void) const
  {
    return MTestStiffnessMatrixType::CONSISTANTTANGENTOPERATOR;
  } // end of MTestCyranoBehaviour::getDefaultStiffnessMatrixType

  void
  MTestCyranoBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    using namespace std;
    fill(v.begin(),v.end(),real(0));
  } // end of MTestCyranoBehaviour::setDrivingVariablesDefaultInitialValue  

  bool
  MTestCyranoBehaviour::computePredictionOperator(tfel::math::matrix<real>& Kt,
						  const tfel::math::tmatrix<3u,3u,real>&,
						  const tfel::math::vector<real>& e0,
						  const tfel::math::vector<real>& s0,
						  const tfel::math::vector<real>& mp,
						  const tfel::math::vector<real>& iv0,
						  const tfel::math::vector<real>& esv0,
						  const tfel::material::ModellingHypothesis::Hypothesis h,
						  const MTestStiffnessMatrixType::mtype ktype) const
  {
    using namespace tfel::math;
    if(ktype!=MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
      vector<real> s1(s0);
      vector<real> de(real(0));
      vector<real> iv1(iv0);
      vector<real> desv(esv0.size(),real(0));
      return this->call_behaviour(Kt,s1,iv1,e0,de,s0,
				  mp,iv0,esv0,desv,
				  h,real(1),ktype,false);
    }
    this->computeElasticStiffness(Kt,mp,h);
    return true;
  } // end of MTestCyranoBehaviour::computePredictionOperator

  bool
  MTestCyranoBehaviour::integrate(tfel::math::matrix<real>& Kt,
					    tfel::math::vector<real>& s1,
					    tfel::math::vector<real>& iv1,
					    const tfel::math::tmatrix<3u,3u,real>&,
					    const tfel::math::vector<real>& e0,
					    const tfel::math::vector<real>& e1,
					    const tfel::math::vector<real>& s0,
					    const tfel::math::vector<real>& mp,
					    const tfel::math::vector<real>& iv0,
					    const tfel::math::vector<real>& ev0,
					    const tfel::math::vector<real>& dev,
					    const tfel::material::ModellingHypothesis::Hypothesis h,
					    const real dt,
					    const MTestStiffnessMatrixType::mtype ktype) const
  {
    return this->call_behaviour(Kt,s1,iv1,e0,e1,s0,
				mp,iv0,ev0,dev,h,dt,
				ktype,true);
  } // end of MTestCyranoBehaviour::integrate

  bool
  MTestCyranoBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
						tfel::math::vector<real>& s1,
						tfel::math::vector<real>& iv1,
						const tfel::math::vector<real>& e0,
						const tfel::math::vector<real>& e1,
						const tfel::math::vector<real>& s0,
						const tfel::math::vector<real>& mp,
						const tfel::math::vector<real>& iv0,
						const tfel::math::vector<real>& ev0,
						const tfel::math::vector<real>& dev,
						const tfel::material::ModellingHypothesis::Hypothesis h,
						const real dt,
						const MTestStiffnessMatrixType::mtype ktype,
						const bool b) const
  {
    using namespace std;
    using namespace tfel::math;
    using namespace cyrano;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    using cyrano::CyranoComputeStiffnessOperator;
    static const real sqrt2 = sqrt(real(2));
    CyranoInt ntens;
    CyranoInt ndi;
    CyranoInt nprops = static_cast<CyranoInt>(mp.size());
    CyranoInt nstatv;
    unsigned short dimension;
    if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      ndi   = 14;
      ntens = 3;
      dimension = 1u;
    } else if (h==MH::AXISYMMETRICAL){
      ndi = 0;
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::PLANESTRESS){
      ndi = -2;
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::PLANESTRAIN){
      ndi = -1;
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::GENERALISEDPLANESTRAIN){
      ndi = -3;
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::TRIDIMENSIONAL){
      ndi = 2;
      ntens = 6;
      dimension = 3u;
    } else {
      string msg("MTestCyranoBehaviour::integrate : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    if((this->D.getNbRows()!=Kt.getNbRows())||
       (this->D.getNbCols()!=Kt.getNbCols())){
      string msg("MTestCyranoBehaviour::integrate : ");
      msg += "the memory has not been allocated correctly";
      throw(runtime_error(msg));
    }
    if(((iv0.size()==0)&&(this->iv.size()!=1u))||
       ((iv0.size()!=0)&&(iv0.size()!=this->iv.size()))){
      string msg("MTestCyranoBehaviour::integrate : ");
      msg += "the memory has not been allocated correctly";
      throw(runtime_error(msg));
    }
    fill(this->D.begin(),this->D.end(),0.);
    // choosing the type of stiffness matrix
    if(b){
      if((ktype==MTestStiffnessMatrixType::NOSTIFFNESS)||
	 (ktype==MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES)){
	// do nothing
      } else if(ktype==MTestStiffnessMatrixType::ELASTIC){
	D(0,0) = real(1);
      } else if(ktype==MTestStiffnessMatrixType::SECANTOPERATOR){
	D(0,0) = real(2);
      } else if(ktype==MTestStiffnessMatrixType::TANGENTOPERATOR){
	D(0,0) = real(3);
      } else if(ktype==MTestStiffnessMatrixType::CONSISTANTTANGENTOPERATOR){
	D(0,0) = real(4);
      } else {
	string msg("MTestAsterSmallStrainBehaviour::call_behaviour : "
		   "invalid or unspecified stiffness matrix type");
	throw(runtime_error(msg));
      }
    } else {
      if(ktype==MTestStiffnessMatrixType::ELASTIC){
	D(0,0) = real(-1);
      } else if(ktype==MTestStiffnessMatrixType::SECANTOPERATOR){
	D(0,0) = real(-2);
      } else if(ktype==MTestStiffnessMatrixType::TANGENTOPERATOR){
	D(0,0) = real(-3);
      } else {
	string msg("MTestAsterSmallStrainBehaviour::call_behaviour : "
		   "invalid or unspecified stiffness matrix type");
	throw(runtime_error(msg));
      }
    }
    if(iv0.size()!=0){
      copy(iv0.begin(),iv0.end(),
	   this->iv.begin());
    }
    nstatv = static_cast<CyranoInt>(iv.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot(0.);
    tmatrix<3u,3u,real>::size_type i,j;
    for(i=0;i!=3u;++i){
      for(j=0;j!=3u;++j){
	if(i==j){
	  drot(i,j) = 1;
	} else {
	  drot(i,j) = 0;
	}
      }
    }
    CyranoInt kinc(1);
    stensor<3u,real> ue0(real(0));
    stensor<3u,real> ude(real(0));
    copy(e0.begin(),e0.end(),ue0.begin());
    for(i=0;i!=e1.size();++i){
      ude(i) = e1(i)-e0(i);
    }
    copy(s0.begin(),s0.end(),s1.begin());
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i)  /= sqrt2;
      ue0(i) *= sqrt2;
      ude(i) *= sqrt2;
    }
    (this->fct)(&ntens,&dt,&drot(0,0),
		&D(0,0),&ue0(0),&ude(0),
		&ev0(0),&dev(0),
		&mp(0),&nprops,
		&ev0(0)+1,&dev(0)+1,
		&iv(0),&nstatv,&s1(0),
		&ndi,&kinc);
    if(kinc!=1){
      return false;
    }
    if(!iv1.empty()){
      copy(iv.begin(),iv.end(),iv1.begin());
    }
    // tangent operator (...)
    if(ktype!=MTestStiffnessMatrixType::NOSTIFFNESS){ 
      if(ktype==MTestStiffnessMatrixType::ELASTICSTIFNESSFROMMATERIALPROPERTIES){
	this->computeElasticStiffness(Kt,mp,h);
      } else {
	MTestUmatNormaliseTangentOperator::exe(Kt,D,dimension);
      }
    }
    // turning things in standard conventions
    for(i=3;i!=static_cast<unsigned short>(ntens);++i){
      s1(i) *= sqrt2;
    }
    return true;
  } // end of MTestCyranoBehaviour::integrate

  void
  MTestCyranoBehaviour::computeElasticStiffness(tfel::math::matrix<real>& Kt,
						const tfel::math::vector<real>& mp,
						const tfel::material::ModellingHypothesis::Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::math;
    using cyrano::CyranoComputeStiffnessOperator;
    typedef tfel::material::ModellingHypothesis MH;
    tmatrix<3u,3u,real>::size_type i,j;
    if(this->stype==0u){
      if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	st2tost2<1u,real> De;
	CyranoComputeStiffnessOperator<MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				       cyrano::ISOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else {
	string msg("MTestCyranoBehaviour::integrate : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }
    } else if(this->stype==1u){
      if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
	st2tost2<1u,real> De;
	CyranoComputeStiffnessOperator<MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
				       cyrano::ORTHOTROPIC>::exe(&mp(0),De);
	for(i=0;i!=3u;++i){
	  for(j=0;j!=3u;++j){
	    Kt(i,j) = De(i,j);
	  }
	}
      } else {
	string msg("MTestCyranoBehaviour::integrate : ");
	msg += "unsupported hypothesis";
	throw(runtime_error(msg));
      }
    } else {
      string msg("MTestCyranoBehaviour::integrate : ");
      msg += "invalid behaviour type (neither "
	"isotropic or orthotropic)";
      throw(runtime_error(msg));
    }

  }
      
  MTestCyranoBehaviour::~MTestCyranoBehaviour()
  {}
  
} // end of namespace mfront

