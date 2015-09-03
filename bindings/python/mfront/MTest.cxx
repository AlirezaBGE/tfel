/*! 
 * \file  bindings/python/mfront/MTest.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 20 sept. 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<boost/python.hpp>

#include"MFront/MTest.hxx"
#include"MFront/MTestEvolution.hxx"
#include"MFront/MTestFunctionEvolution.hxx"
#include"MFront/MTestCastemEvolution.hxx"
#include"MFront/MTestConstraint.hxx"
#include"MFront/MTestImposedThermodynamicForce.hxx"
#include"MFront/MTestImposedDrivingVariable.hxx"

static void
MTest_addEvolution(mfront::MTest& t,
		   const std::string&  n,
		   const mfront::real& v,
		   const bool b1,
		   const bool b2)
{
  using namespace tfel::utilities;
  using namespace mfront;
  shared_ptr<MTestEvolution> pev(new MTestConstantEvolution(v));
  t.addEvolution(n,pev,b1,b2);
}

static void
MTest_addEvolution2(mfront::MTest& t,
		    const std::string&  n,
		    const std::map<mfront::real,
				   mfront::real>& v,
		    const bool b1,
		    const bool b2)
{
  using namespace std;
  using namespace mfront;
  using tfel::utilities::shared_ptr;
  using mfront::real;
  vector<real> tv(v.size());
  vector<real> ev(v.size());
  vector<real>::size_type i;
  map<real,real>::const_iterator pv;
  for(pv=v.begin(),i=0;pv!=v.end();++pv,++i){
    tv[i] = pv->first;
    ev[i] = pv->second;
  }
  shared_ptr<MTestEvolution> pev(new MTestLPIEvolution(tv,ev));
  t.addEvolution(n,pev,b1,b2);
}

static void
MTest_addEvolution3(mfront::MTest& t,
		    const std::string&  n,
		    const std::string& f,
		    const bool b1,
		    const bool b2)
{
  using namespace std;
  using namespace mfront;
  using tfel::utilities::shared_ptr;
  shared_ptr<MTestEvolution> pev(new MTestFunctionEvolution(f,t.getEvolutions()));
  t.addEvolution(n,pev,b1,b2);
}

static void
MTest_setMaterialProperty(mfront::MTest& t,
			  const std::string&  n,
			  const mfront::real& v,
			  const bool b)
{
  using namespace tfel::utilities;
  using namespace mfront;
  shared_ptr<MTestEvolution> pev(new MTestConstantEvolution(v));
  t.setMaterialProperty(n,pev,b);
}

static void
MTest_setMaterialProperty2(mfront::MTest& t,
			   const std::string& n,
			   const std::string& f,
			   const bool b)
{
  using namespace std;
  using namespace mfront;
  using tfel::utilities::shared_ptr;
  using mfront::real;
  shared_ptr<MTestEvolution> mpev;
  mpev = shared_ptr<MTestEvolution>(new MTestFunctionEvolution(f,t.getEvolutions()));
  t.setMaterialProperty(n,mpev,b);
}

static void
MTest_setMaterialProperty3(mfront::MTest& t,
		    const std::string&  n,
		    const std::string& f,
		    const bool b)
{
  using namespace std;
  using namespace mfront;
  using tfel::utilities::shared_ptr;
  shared_ptr<MTestEvolution> pev(new MTestFunctionEvolution(f,t.getEvolutions()));
  t.setMaterialProperty(n,pev,b);
}

static void
MTest_setCastemMaterialProperty(mfront::MTest& t,
				const std::string& n,
				const std::string& l,
				const std::string& f,
				const bool b)
{
  using namespace std;
  using namespace mfront;
  using tfel::utilities::shared_ptr;
  using mfront::real;
  shared_ptr<MTestEvolution> mpev;
  mpev = shared_ptr<MTestEvolution>(new MTestCastemEvolution(l,f,t.getEvolutions()));
  t.setMaterialProperty(n,mpev,b);
}

static void
MTest_setExternalStateVariable(mfront::MTest& t,
			       const std::string&  n,
			       const mfront::real& v,
			       const bool b)
{
  using namespace tfel::utilities;
  using namespace mfront;
  shared_ptr<MTestEvolution> pev(new MTestConstantEvolution(v));
  t.setExternalStateVariable(n,pev,b);
}

static void
MTest_setExternalStateVariable2(mfront::MTest& t,
				const std::string&  n,
				const std::map<mfront::real,
				mfront::real>& v,
				const bool b)
{
  using namespace std;
  using namespace mfront;
  using tfel::utilities::shared_ptr;
  using mfront::real;
  vector<real> tv(v.size());
  vector<real> ev(v.size());
  vector<real>::size_type i;
  map<real,real>::const_iterator pv;
  for(pv=v.begin(),i=0;pv!=v.end();++pv,++i){
    tv[i] = pv->first;
    ev[i] = pv->second;
  }
  shared_ptr<MTestEvolution> pev(new MTestLPIEvolution(tv,ev));
  t.setExternalStateVariable(n,pev,b);
}

static void
MTest_setExternalStateVariable3(mfront::MTest& t,
				const std::string&  n,
				const std::string& f,
				const bool b)
{
  using namespace std;
  using namespace mfront;
  using tfel::utilities::shared_ptr;
  shared_ptr<MTestEvolution> pev(new MTestFunctionEvolution(f,t.getEvolutions()));
  t.setExternalStateVariable(n,pev,b);
}

static void
MTest_setImposedThermodynamicForce(mfront::MTest& t,
				   const std::string&  n,
				   const mfront::real& v)
{
  using namespace std;
  using namespace mfront;
  using tfel::utilities::shared_ptr; 
  shared_ptr<MTestConstraint> sc;
  shared_ptr<MTestEvolution> sev(new MTestConstantEvolution(v));
  sc = shared_ptr<MTestConstraint>(new MTestImposedThermodynamicForce(*(t.getBehaviour()),
								      t.getModellingHypothesis(),
								      n,sev));
  t.addEvolution(n,sev,false,true);
  t.addConstraint(sc);
} // end of MTest_setImposedThermodynamicForce

static void
MTest_setImposedThermodynamicForce2(mfront::MTest& t,
				   const std::string&  n,
				   const std::map<mfront::real,
						  mfront::real>& v)
{
  using namespace std;
  using namespace mfront;
  using tfel::utilities::shared_ptr;
  using mfront::real;
  shared_ptr<MTestConstraint> sc;
  vector<real> tv(v.size());
  vector<real> ev(v.size());
  vector<real>::size_type i;
  map<real,real>::const_iterator pv;
  for(pv=v.begin(),i=0;pv!=v.end();++pv,++i){
    tv[i] = pv->first;
    ev[i] = pv->second;
  }
  shared_ptr<MTestEvolution> sev(new MTestLPIEvolution(tv,ev));
  sc = shared_ptr<MTestConstraint>(new MTestImposedThermodynamicForce(*(t.getBehaviour()),
								      t.getModellingHypothesis(),
								      n,sev));
  t.addEvolution(n,sev,false,true);
  t.addConstraint(sc);
} // end of MTest_setImposedThermodynamicForce

static void
MTest_setImposedStress(mfront::MTest& t,
		       const std::string&  n,
		       const mfront::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if((t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)&&
     (t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
    string msg("MTest::handleImposedStress : "
	       "the setImposedStress method is only valid "
	       "for small and finite strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedThermodynamicForce(t,n,v);
}

static void
MTest_setImposedStress2(mfront::MTest& t,
			const std::string&  n,
			const std::map<mfront::real,
			mfront::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if((t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR)&&
     (t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR)){
    string msg("MTestParser::handleImposedStress : "
	       "the setImposedStress method is only valid "
	       "for small and finite strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedThermodynamicForce2(t,n,v);
}

static void
MTest_setImposedCohesiveForce(mfront::MTest& t,
			      const std::string&  n,
			      const mfront::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
    string msg("MTestParser::handleImposedCohesiveForce : "
	       "the setImposedCohesiveForce method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedThermodynamicForce(t,n,v);
}

static void
MTest_setImposedCohesiveForce2(mfront::MTest& t,
			       const std::string&  n,
			       const std::map<mfront::real,
			       mfront::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
    string msg("MTestParser::handleImposedCohesiveForce : "
	       "the setImposedCohesiveForce method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedThermodynamicForce2(t,n,v);
}

static void
MTest_setImposedDrivingVariable(mfront::MTest& t,
				const std::string&  n,
				const mfront::real& v)
{
  using namespace std;
  using namespace mfront;
  using tfel::utilities::shared_ptr;
  shared_ptr<MTestConstraint> sc;
  shared_ptr<MTestEvolution> sev(new MTestConstantEvolution(v));
  sc = shared_ptr<MTestConstraint>(new MTestImposedDrivingVariable(*(t.getBehaviour()),
								   t.getModellingHypothesis(),
								   n,sev));
  t.addEvolution(n,sev,false,true);
  t.addConstraint(sc);
} // end of MTest_setImposedDrivingVariable

static void
MTest_setImposedDrivingVariable2(mfront::MTest& t,
				   const std::string&  n,
				   const std::map<mfront::real,
						  mfront::real>& v)
{
  using namespace std;
  using namespace mfront;
  using tfel::utilities::shared_ptr;
  using mfront::real;
  shared_ptr<MTestConstraint> sc;
  vector<real> tv(v.size());
  vector<real> ev(v.size());
  vector<real>::size_type i;
  map<real,real>::const_iterator pv;
  for(pv=v.begin(),i=0;pv!=v.end();++pv,++i){
    tv[i] = pv->first;
    ev[i] = pv->second;
  }
  shared_ptr<MTestEvolution> sev(new MTestLPIEvolution(tv,ev));
  sc = shared_ptr<MTestConstraint>(new MTestImposedDrivingVariable(*(t.getBehaviour()),
								   t.getModellingHypothesis(),
								   n,sev));
  t.addEvolution(n,sev,false,true);
  t.addConstraint(sc);
} // end of MTest_setImposedDrivingVariable

static void
MTest_setImposedStrain(mfront::MTest& t,
		       const std::string&  n,
		       const mfront::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
    string msg("MTestParser::handleImposedStrain : "
	       "the setImposedStrain method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable(t,n,v);
}

static void
MTest_setImposedStrain2(mfront::MTest& t,
			const std::string&  n,
			const std::map<mfront::real,
			mfront::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::SMALLSTRAINSTANDARDBEHAVIOUR){
    string msg("MTestParser::handleImposedStrain : "
	       "the setImposedStrain method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable2(t,n,v);
}

static void
MTest_setImposedDeformationGradient(mfront::MTest& t,
				    const std::string&  n,
				    const mfront::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR){
    string msg("MTestParser::handleImposedDeformationGradient : "
	       "the setImposedDeformationGradient method is only valid "
	       "for finite strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable(t,n,v);
}

static void
MTest_setImposedDeformationGradient2(mfront::MTest& t,
				     const std::string&  n,
				     const std::map<mfront::real,
				                    mfront::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::FINITESTRAINSTANDARDBEHAVIOUR){
    string msg("MTestParser::handleImposedDeformationGradient : "
	       "the setImposedDeformationGradient method is only valid "
	       "for finite strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable2(t,n,v);
}

static void
MTest_setImposedOpeningDisplacement(mfront::MTest& t,
				    const std::string&  n,
				    const mfront::real& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
    string msg("MTestParser::handleImposedOpeningDisplacement : "
	       "the setImposedOpeningDisplacement method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable(t,n,v);
}

static void
MTest_setImposedOpeningDisplacement2(mfront::MTest& t,
				     const std::string&  n,
				     const std::map<mfront::real,
				     mfront::real>& v)
{
  using namespace std;
  using namespace tfel::material;
  if(t.getBehaviourType()!=MechanicalBehaviourBase::COHESIVEZONEMODEL){
    string msg("MTestParser::handleImposedOpeningDisplacement : "
	       "the setImposedOpeningDisplacement method is only valid "
	       "for small strain behaviours");
    throw(runtime_error(msg));
  }
  MTest_setImposedDrivingVariable2(t,n,v);
}

static void
MTest_setRotationMatrix(mfront::MTest& t,
			const std::vector<std::vector<mfront::real> >& m){
  using namespace std;
  typedef tfel::math::tmatrix<3u,3u,mfront::real> matrix;
  matrix r;
  for(int i = 0; i!=3;++i){
    for(int j = 0; j!=3;++j){
      r(i,j) = m[i][j];
    }
  }
  t.setRotationMatrix(r);
} // end of MTest_setRotationMatrix

static mfront::MTest::MTestCurrentState
MTestCurrentState_copy(const mfront::MTest::MTestCurrentState& src)
{
  return src;
}

#define TFEL_PYTHON_MTESTCURRENTSTATEGETTER( X )		        \
  static tfel::math::vector<mfront::real>				\
  MTestCurrentState_get##X(const mfront::MTest::MTestCurrentState& t)	\
  {                                                                     \
    return t. X ;                                                       \
  }

#define TFEL_PYTHON_MTESTCURRENTSTATEGETTER2( X )		        \
  static mfront::real			                         	\
  MTestCurrentState_get##X(const mfront::MTest::MTestCurrentState& t)	\
  {                                                                     \
    return t. X ;                                                       \
  }

TFEL_PYTHON_MTESTCURRENTSTATEGETTER(u_1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(u0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(u1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(s_1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(s0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(s1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(e0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(e1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(e_th0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(e_th1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(mprops0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(mprops1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(iv_1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(iv0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(iv1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(esv0)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER(desv)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER2(dt_1)
TFEL_PYTHON_MTESTCURRENTSTATEGETTER2(Tref)

void declareMTest(void)
{
  using namespace boost;
  using namespace boost::python;
  using namespace mfront;
  using boost::python::arg;
  using tfel::tests::TestResult;

  enum_<MTest::StiffnessUpdatingPolicy>("StiffnessUpdatingPolicy")
    .value("CONSTANTSTIFFNESS",MTest::CONSTANTSTIFFNESS)
    .value("CONSTANTSTIFFNESSBYPERIOD",MTest::CONSTANTSTIFFNESSBYPERIOD)
    .value("UPDATEDSTIFFNESSMATRIX",MTest::UPDATEDSTIFFNESSMATRIX)
    ;

  enum_<MTest::PredictionPolicy>("PredictionPolicy")
    .value("NOPREDICTION",MTest::NOPREDICTION)
    .value("LINEARPREDICTION",MTest::LINEARPREDICTION)
    .value("ELASTICPREDICTION",MTest::ELASTICPREDICTION)
    .value("SECANTOPERATORPREDICTION",MTest::SECANTOPERATORPREDICTION)
    .value("TANGENTOPERATORPREDICTION",MTest::TANGENTOPERATORPREDICTION)
    ;
  
  enum_<MTestStiffnessMatrixType::mtype>("StiffnessMatrixType")
    .value("NOSTIFFNESS",MTestStiffnessMatrixType::NOSTIFFNESS)
    .value("ELASTIC",MTestStiffnessMatrixType::ELASTIC)
    .value("SECANTOPERATOR",MTestStiffnessMatrixType::SECANTOPERATOR)
    .value("TANGENTOPERATOR",MTestStiffnessMatrixType::TANGENTOPERATOR)
    .value("CONSISTENTTANGENTOPERATOR",
	   MTestStiffnessMatrixType::CONSISTENTTANGENTOPERATOR)
    ;
  
  TestResult (MTest:: *pm)(void) = &MTest::execute;
  void (MTest:: *pm2)(MTest::MTestCurrentState&,
		      MTest::MTestWorkSpace&,
		      const real,
		      const real) = &MTest::execute;

  class_<MTest::MTestCurrentState>("MTestCurrentState")
    .def("copy",&MTestCurrentState_copy)
    .add_property("u_1",MTestCurrentState_getu_1)
    .add_property("u0",MTestCurrentState_getu0)
    .add_property("u1",MTestCurrentState_getu1)
    .add_property("s_1",MTestCurrentState_gets_1)
    .add_property("s0",MTestCurrentState_gets0)
    .add_property("s1",MTestCurrentState_gets1)
    .add_property("e0",MTestCurrentState_gete0)
    .add_property("e1",MTestCurrentState_gete1)
    .add_property("e_th0",MTestCurrentState_gete_th0)
    .add_property("e_th1",MTestCurrentState_gete_th1)
    .add_property("mprops0",MTestCurrentState_getmprops0)
    .add_property("mprops1",MTestCurrentState_getmprops1)
    .add_property("iv_1",MTestCurrentState_getiv_1)
    .add_property("iv0",MTestCurrentState_getiv0)
    .add_property("iv1",MTestCurrentState_getiv1)
    .add_property("evs0",MTestCurrentState_getesv0)
    .add_property("desv",MTestCurrentState_getdesv)
    .add_property("dt_1",MTestCurrentState_getdt_1)
    .add_property("Tref",MTestCurrentState_getTref)
    ;

  class_<MTest::MTestWorkSpace,noncopyable>("MTestWorkSpace")
    ;

  class_<MTest,noncopyable>("MTest")
    .def("execute",pm)
    .def("execute",pm2)
    .def("completeInitialisation",
	 &MTest::completeInitialisation)
    .def("initializeCurrentState",
	 &MTest::initializeCurrentState)
    .def("initializeWorkSpace",
	 &MTest::initializeWorkSpace)
    .def("printOutput",
	 &MTest::printOutput)
    .def("setBehaviour",&MTest::setBehaviour)
    .def("setEvolutionValue",
	 &MTest::setEvolutionValue)
    .def("setOutputFileName",
	 &MTest::setOutputFileName)
    .def("setOutputFilePrecision",
	 &MTest::setOutputFilePrecision)
    .def("setResidualFileName",
	 &MTest::setResidualFileName)
    .def("setResidualFilePrecision",
	 &MTest::setResidualFilePrecision)
    .def("setDescription",&MTest::setDescription)
    .def("setAuthor",&MTest::setAuthor)
    .def("setDate",&MTest::setDate)
    .def("setPredictionPolicy",&MTest::setPredictionPolicy)
    .def("setStiffnessMatrixType",
	 &MTest::setStiffnessMatrixType)
    .def("setStiffnessUpdatingPolicy",
	 &MTest::setStiffnessUpdatingPolicy)
    .def("setUseCastemAccelerationAlgorithm",
	 &MTest::setUseCastemAccelerationAlgorithm)
    .def("setCastemAccelerationTrigger",
	 &MTest::setCastemAccelerationTrigger)
    .def("setCastemAccelerationPeriod",
	 &MTest::setCastemAccelerationPeriod)
    .def("setAccelerationAlgorithm",
	 &MTest::setAccelerationAlgorithm)
    .def("setAccelerationParameter",
	 &MTest::setAccelerationAlgorithmParameter)
    .def("setMaximumNumberOfIterations",
	 &MTest::setMaximumNumberOfIterations)
    .def("setMaximumNumberOfSubSteps",
	 &MTest::setMaximumNumberOfSubSteps)
    .def("setDrivingVariableEpsilon",
	 &MTest::setDrivingVariableEpsilon)
    .def("setThermodynamicForceEpsilon",
	 &MTest::setThermodynamicForceEpsilon)
    .def("setParameter",&MTest::setParameter)
    .def("setIntegerParameter",
	 &MTest::setIntegerParameter)
    .def("setUnsignedIntegerParameter",
	 &MTest::setUnsignedIntegerParameter)
    .def("setModellingHypothesis",
	 &MTest::setModellingHypothesis)
    .def("setTimes",&MTest::setTimes)
    .def("addReal",MTest_addEvolution,
	 (arg("name"),"value",arg("b1")=true,arg("b2")=true))
    .def("addEvolution",MTest_addEvolution,
	 (arg("name"),"value",arg("b1")=true,arg("b2")=true))
    .def("addEvolution",MTest_addEvolution2,
	 (arg("name"),"value",arg("b1")=true,arg("b2")=true))
    .def("addEvolution",MTest_addEvolution3,
	 (arg("name"),"value","function",arg("b1")=true,arg("b2")=true))
    .def("setMaterialProperty",MTest_setMaterialProperty,
	 (arg("name"),"value",arg("check")=true))
    .def("setMaterialProperty",MTest_setMaterialProperty2,
	 (arg("name"),"value",arg("check")=true))
    .def("setMaterialProperty",MTest_setMaterialProperty3,
	 (arg("name"),"value","function",arg("check")=true))
    .def("setCastemMaterialProperty",MTest_setCastemMaterialProperty,
	 (arg("name"),arg("library"),arg("function"),arg("check")=true))
    .def("setExternalStateVariable",MTest_setExternalStateVariable,
	 (arg("name"),"value",arg("check")=true))
    .def("setExternalStateVariable",MTest_setExternalStateVariable2,
	 (arg("name"),"value",arg("check")=true))
    .def("setExternalStateVariable",MTest_setExternalStateVariable3,
	 (arg("name"),"value","function",arg("check")=true))
    .def("setImposedStress",MTest_setImposedStress,
	 (arg("name"),"values"))
    .def("setImposedStress",MTest_setImposedStress2,
	 (arg("name"),"values"))
    .def("setImposedCohesiveForce",MTest_setImposedCohesiveForce,
	 (arg("name"),"values"))
    .def("setImposedCohesiveForce",MTest_setImposedCohesiveForce2,
	 (arg("name"),"values"))
    .def("setImposedThermodynamicForce",MTest_setImposedThermodynamicForce,
	 (arg("name"),"values"))
    .def("setImposedThermodynamicForce",MTest_setImposedThermodynamicForce2,
	 (arg("name"),"values"))
    .def("setImposedStrain",MTest_setImposedStrain,
	 (arg("name"),"values"))
    .def("setImposedStrain",MTest_setImposedStrain2,
	 (arg("name"),"values"))
    .def("setImposedDeformationGradient",MTest_setImposedDeformationGradient,
	 (arg("name"),"values"))
    .def("setImposedDeformationGradient",MTest_setImposedDeformationGradient2,
	 (arg("name"),"values"))
    .def("setImposedOpeningDisplacement",MTest_setImposedOpeningDisplacement,
	 (arg("name"),"values"))
    .def("setImposedOpeningDisplacement",MTest_setImposedOpeningDisplacement2,
	 (arg("name"),"values"))
    .def("setImposedDrivingVariable",MTest_setImposedDrivingVariable,
	 (arg("name"),"values"))
    .def("setImposedDrivingVariable",MTest_setImposedDrivingVariable2,
	 (arg("name"),"values"))
    .def("setRotationMatrix",
	 &MTest_setRotationMatrix)
    .def("setOutOfBoundsPolicy",
	 &MTest::setOutOfBoundsPolicy)
    .def("setScalarInternalStateVariableInitialValue",
	 &MTest::setScalarInternalStateVariableInitialValue)
    .def("setScalarInternalStateVariableInitialValue",
	 &MTest::setScalarInternalStateVariableInitialValue)
    .def("setStensorInternalStateVariableInitialValues",
	 &MTest::setStensorInternalStateVariableInitialValues)
    .def("setTensorInternalStateVariableInitialValues",
	 &MTest::setTensorInternalStateVariableInitialValues)
  ;

} // end of declareExternalLibraryManager
