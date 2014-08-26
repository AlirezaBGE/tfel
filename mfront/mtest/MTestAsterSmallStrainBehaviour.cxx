/*! 
 * \file  MTestAsterSmallBehaviour.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#include<cmath>
#include<algorithm>

#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"
#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterComputeStiffnessTensor.hxx"

#include"MFront/MTestUmatNormaliseTangentOperator.hxx"
#include"MFront/MTestAsterSmallStrainBehaviour.hxx"

namespace mfront
{

  MTestAsterSmallStrainBehaviour::MTestAsterSmallStrainBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
								 const std::string& l,
								 const std::string& b)
    : MTestAsterStandardBehaviour(h,l,b)
  {}

  void
  MTestAsterSmallStrainBehaviour::getDrivingVariablesDefaultInitialValues(tfel::math::vector<real>& v) const
  {
    using namespace std;
    fill(v.begin(),v.end(),real(0));
  }

  bool
  MTestAsterSmallStrainBehaviour::call_behaviour(tfel::math::matrix<real>& Kt,
						 tfel::math::vector<real>& s1,
						 tfel::math::vector<real>& iv1,
						 const tfel::math::tmatrix<3u,3u,real>& r,
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
    using namespace aster;
    typedef tfel::material::ModellingHypothesis MH;
    using tfel::math::vector;
    static const real sqrt2 = sqrt(real(2));
    unsigned short dimension;
    AsterInt ntens;
    AsterInt nprops = static_cast<AsterInt>(mp.size());
    AsterInt nstatv;
    if(h==MH::AXISYMMETRICALGENERALISEDPLANESTRAIN){
      ntens = 3;
      dimension = 1u;
    } else if (h==MH::AXISYMMETRICAL){
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::PLANESTRESS){
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::PLANESTRAIN){
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::GENERALISEDPLANESTRAIN){
      ntens = 4;
      dimension = 2u;
    } else if (h==MH::TRIDIMENSIONAL){
      ntens = 6;
      dimension = 3u;
    } else {
      string msg("MTestAsterSmallStrainBehaviour::call_beahviour : ");
      msg += "unsupported hypothesis";
      throw(runtime_error(msg));
    }
    fill(this->D.begin(),this->D.end(),0.);
    // choosing the type of stiffness matrix
    MTestUmatBehaviourBase::initializeTangentOperator(ktype,b);
    // using a local copy of internal state variables to handle the
    // case where iv0 is empty
    copy(iv0.begin(),iv0.end(),this->iv.begin());
    if(iv0.empty()){
      iv[0] = real(0.);
    }
    nstatv = static_cast<AsterInt>(iv.size());
    // rotation matrix
    tmatrix<3u,3u,real> drot;
    tmatrix<3u,3u,real>::size_type i,j;
    for(i=0;i!=3u;++i){
      for(j=0;j!=3u;++j){
	drot(i,j) = r(j,i);
      }
    }
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
    AsterReal ndt(1.);
    (this->fct)(&s1(0),&iv(0),&D(0,0),
		0,0,0,0,0,0,0,
		&ue0(0),&ude(0),0,&dt,
		&ev0(0),&dev(0),
		&ev0(0)+1,&dev(0)+1,
		0,0,0,&ntens,&nstatv,&mp(0),
		&nprops,0,&drot(0,0),&ndt,
		0,0,0,0,0,0,0,0,0);
    if(ndt<0.){
      return false;
    }
    if(ktype!=MTestStiffnessMatrixType::NOSTIFFNESS){
      MTestUmatNormaliseTangentOperator::exe(Kt,D,dimension);
    }
    if(b){
      if(!iv0.empty()){
	copy(iv.begin(),iv.end(),iv1.begin());
      }
      // turning things in standard conventions
      for(i=3;i!=static_cast<unsigned short>(ntens);++i){
	s1(i) *= sqrt2;
      }
    }
    return true;
  }

  MTestAsterSmallStrainBehaviour::~MTestAsterSmallStrainBehaviour()
  {}
  
} // end of namespace mfront









