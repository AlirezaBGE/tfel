/*!
 * \file   mfront/src/MFrontBehaviourParserCommon.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   05 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<algorithm>
#include<iterator>
#include<vector>
#include<sstream>
#include<utility>
#include<stdexcept>
#include<limits>
#include<cctype>
#include<cmath>

#include"TFEL/System/System.hxx"
#include"TFEL/Glossary/Glossary.hxx"
#include"TFEL/Glossary/GlossaryEntry.hxx"
#include"TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/MFrontPedanticMode.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/MFrontSearchFile.hxx"
#include"MFront/MFrontMaterialLawParser.hxx"
#include"MFront/MFrontBehaviourVirtualInterface.hxx"
#include"MFront/MFrontMFrontLawInterface.hxx"
#include"MFront/MFrontPerformanceProfiling.hxx"
#include"MFront/MFrontBehaviourInterfaceFactory.hxx"
#include"MFront/MFrontBehaviourAnalyserFactory.hxx"
#include"MFront/FiniteStrainBehaviourTangentOperatorConversionPath.hxx"
#include"MFront/MFrontBehaviourParserCommon.hxx"

namespace mfront{

  MFrontBehaviourParserCommon::CodeBlockOptions::CodeBlockOptions()
    : p(MechanicalBehaviourData::BODY),
      m(MechanicalBehaviourData::CREATE)
  {
    this->hypotheses.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
  } // end of MFrontBehaviourParserCommon::CodeBlockOptions::CodeBlockOptions

  std::string
  MFrontBehaviourParserCommon::getClassName(void) const
  {
    return this->mb.getClassName();
  } // end of MFrontBehaviourParserCommon::getClassName

  void
  MFrontBehaviourParserCommon::addMaterialLaw(const std::string& m)
  {
    this->mb.addMaterialLaw(m);
  } // end of MFrontBehaviourParserCommon::addMaterialLaw

  void
  MFrontBehaviourParserCommon::appendToIncludes(const std::string& c)
  {
    this->mb.appendToIncludes(c);
  } // end of MFrontBehaviourParserCommon::appendToIncludes

  void
  MFrontBehaviourParserCommon::appendToMembers(const std::string& c)
  {
    this->mb.appendToMembers(c);
  } // end of MFrontBehaviourParserCommon::appendToMembers

  void
  MFrontBehaviourParserCommon::appendToPrivateCode(const std::string& c)
  {
    this->mb.appendToPrivateCode(c);
  } // end of MFrontBehaviourParserCommon::appendToPrivateCode

  void
  MFrontBehaviourParserCommon::appendToSources(const std::string& c)
  {
    this->mb.appendToSources(c);
  } // end of MFrontBehaviourParserCommon::appendToSources

  void
  MFrontBehaviourParserCommon::appendToHypothesesList(std::set<Hypothesis>& h,
						      const std::string& v) const
  {
    using namespace std;
    if(v==".+"){
      const vector<Hypothesis>& ash = ModellingHypothesis::getModellingHypotheses();
      for(vector<Hypothesis>::const_iterator ph=ash.begin();ph!=ash.end();++ph){
	this->appendToHypothesesList(h,ModellingHypothesis::toString(*ph));
      }
    } else {
      const Hypothesis nh = ModellingHypothesis::fromString(v);
      if(!this->isModellingHypothesisSupported(nh)){
	this->throwRuntimeError("MFrontBehaviourParserCommon::appendToHypothesesList",
				"hypothesis '"+v+"' is not supported by this parser");
      }
      if(this->mb.areModellingHypothesesDefined()){
	const set<Hypothesis>& bh = this->mb.getModellingHypotheses();
	if(bh.find(nh)==bh.end()){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::appendToHypothesesList",
				  "hypothesis '"+v+"' is not supported by the "
				  "behaviour (This means that one of the "
				  "'@ModellingHypothesis' or '@ModellingHypotheses'"
				  "keyword was used earlier)");
	}
      }
      if(!h.insert(nh).second){
	this->throwRuntimeError("MFrontBehaviourParserCommon::appendToHypothesesList",
				"hypothesis '"+v+"' multiply defined");
      }
    }
  } // end of MFrontBehaviourParserCommon::appendToHypothesesList

  void
  MFrontBehaviourParserCommon::readCodeBlockOptions(CodeBlockOptions& o,
						    const bool s)
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::material;
    bool cposition = false;
    bool cmode     = false;
    o.hypotheses.clear();
    if(this->current==this->fileTokens.end()){
      o.hypotheses.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      return;
    }
    if(this->current->value!="<"){
      o.hypotheses.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      return;
    }
    vector<Token> tokens;
    this->readList(tokens,"MFrontBehaviourParserCommon::readCodeBlockOptions","<",">",true);
    for(vector<Token>::const_iterator p=tokens.begin();p!=tokens.end();++p){
      if(p->value=="Append"){
	if(cmode){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::readCodeBlockOptions",
				  "insertion mode already specificed");
	}
	cmode = true;
	o.m=MechanicalBehaviourData::CREATEORAPPEND;
      } else if(p->value=="Replace"){
	if(cmode){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::readCodeBlockOptions",
				  "insertion mode already specificed");
	}
	cmode = true;
	o.m=MechanicalBehaviourData::CREATEORREPLACE;
      } else if(p->value=="Create"){
	if(cmode){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::readCodeBlockOptions",
				  "insertion mode already specificed");
	}
	cmode = true;
	o.m=MechanicalBehaviourData::CREATE;
      } else if(p->value=="Body"){
	if(cposition){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::readCodeBlockOptions",
				  "insertion position already specificed");
	}
	cposition = true;
	o.p = MechanicalBehaviourData::BODY;
      } else if(p->value=="AtBeginning"){
	if(cposition){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::readCodeBlockOptions",
				  "insertion position already specificed");
	}
	cposition = true;
	o.p = MechanicalBehaviourData::AT_BEGINNING;
      } else if(p->value=="AtEnd"){
	if(cposition){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::readCodeBlockOptions",
				  "insertion position already specificed");
	}
	cposition = true;
	o.p = MechanicalBehaviourData::AT_END;
      } else if((p->flag==Token::String)&&(p->value.substr(1,p->value.size()-2)=="+")){
	this->appendToHypothesesList(o.hypotheses,p->value.substr(1,p->value.size()-2));
      } else if(ModellingHypothesis::isModellingHypothesis(p->value)){
	this->appendToHypothesesList(o.hypotheses,p->value);
      } else {
	o.untreated.push_back(*p);
      }
    }
    if(o.hypotheses.empty()){
      o.hypotheses.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
    // checks
    if(!s){
      if(o.hypotheses.size()!=1u){
	this->throwRuntimeError("MFrontBehaviourParserCommon::readCodeBlockOptions : ",
				"specialisation is not allowed");
      }
      if(*(o.hypotheses.begin())!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	this->throwRuntimeError("MFrontBehaviourParserCommon::readCodeBlockOptions : ",
				"specialisation is not allowed");
      }
    }
  } // end of MFrontBehaviourParserCommon::readCodeBlockOptions

  void
  MFrontBehaviourParserCommon::treatUnsupportedCodeBlockOptions(const CodeBlockOptions& o)
  {
    using namespace std;
    using namespace tfel::utilities;
    if(!o.untreated.empty()){
      ostringstream msg;
      msg << "MFrontBehaviourParserCommon::treatUnsupportedCodeBlockOptions : ";
      if(o.untreated.size()==1u){
	msg << "option '" << o.untreated[0].value << "' is invalid";
      } else {
	msg << "the";
	for(vector<Token>::const_iterator p=o.untreated.begin();p!=o.untreated.end();++p){
	  msg << " '" << p->value << "'";
	}
	msg << " options are invalid";
      }
      throw(runtime_error(msg.str()));
    }
  } // end of MFrontBehaviourParserCommon::treatUnsupportedCodeBlockOptions  

  void
  MFrontBehaviourParserCommon::addStaticVariableDescription(const StaticVariableDescription& v)
  {
    this->mb.addStaticVariable(ModellingHypothesis::UNDEFINEDHYPOTHESIS,v);
  } // end of MFrontBehaviourParserCommon::addStaticVariableDescription

  std::set<MFrontBehaviourParserCommon::Hypothesis>
  MFrontBehaviourParserCommon::getDefaultModellingHypotheses(void) const
  {
    using namespace std;
    typedef ModellingHypothesis MH;
    const Hypothesis h[6u] = {MH::AXISYMMETRICALGENERALISEDPLANESTRAIN,
			      MH::AXISYMMETRICAL,
			      MH::PLANESTRAIN,
			      MH::GENERALISEDPLANESTRAIN,
			      MH::TRIDIMENSIONAL};
    return set<Hypothesis>(h,h+6u);
  } // end of MFrontBehaviourParserCommon::getDefaultModellingHypotheses

  bool
  MFrontBehaviourParserCommon::isModellingHypothesisSupported(const Hypothesis) const
  {
    return true;
  } // end of MFrontBehaviourParserCommon::isModellingHypothesesSupported

  void
  MFrontBehaviourParserCommon::analyseMaterialProperty(const Hypothesis h,
						       const MaterialPropertyDescription& a)
  {
    using namespace std;
    for(VariableDescriptionContainer::const_iterator p = a.inputs.begin();p!=a.inputs.end();++p){
      const VariableDescription& v = *p;
      if((a.glossaryNames.find(v.name)==a.glossaryNames.end())&&
    	 (a.entryNames.find(v.name)==a.entryNames.end())){
    	string msg("MFrontBehaviourParserCommon::analyseMaterialProperty : "
    		   "no glossary name declared for variable '"+v.name+
    		   "' used by the material property '"+a.law+"'");
    	throw(runtime_error(msg));
      }
      const string& in = p->getExternalName(a.glossaryNames,a.entryNames);
      if(in!="Temperature"){
    	if(this->mb.isGlossaryName(h,in)||this->mb.isEntryName(h,in)){
    	  // a variable with the good glossary name has been found
    	  const string& n = this->mb.getVariableNameFromGlossaryNameOrEntryName(h,in);
	  if(!((this->mb.isMaterialPropertyName(h,n))||
	       (this->mb.isExternalStateVariableName(h,n))||
	       (this->mb.isParameterName(h,n))||
	       (this->mb.isStaticVariableName(h,n)))){
	    string msg("MFrontBehaviourParserCommon::analyseMaterialProperty : "
		       "variable '"+n+"' required for the material property '"+a.law+"' "
		       "has been declared but is neither a material property nor an "
		       "external state variable nor a parameter nor a static variable");
	    throw(runtime_error(msg));
	  } else {
    	    string msg("MFrontBehaviourParserCommon::analyseMaterialProperty : "
    		       "internal error (name '"+n+"' has not been declared but exists in the glossary");
    	    throw(runtime_error(msg));
    	  }
    	} else {
    	  // trying to declared a new variable
    	  const string nn = a.law+"_"+v.name;
    	  this->registerVariable(nn,false);
    	  this->mb.addExternalStateVariable(h,VariableDescription("real",nn,1u,0u));
    	  if(!this->mb.isGlossaryName(h,v.name)){
    	    this->mb.setGlossaryName(h,nn,in);
    	  } else {
    	    this->mb.setGlossaryName(h,nn,in);
    	  }
    	}
      }
    }
  } // end of MFrontBehaviourParserCommon::analyseMaterialProperty

  void
  MFrontBehaviourParserCommon::endsInputFileProcessing()
  {
    using namespace std;
    using tfel::utilities::shared_ptr;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "MFrontBehaviourParserCommon::endsInputFileProcessing : begin" << endl;
    }
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    vector<string>::const_iterator i;
    if(!this->mb.areModellingHypothesesDefined()){
      this->mb.setModellingHypotheses(this->getDefaultModellingHypotheses());
    }
    const set<Hypothesis>& h = this->mb.getModellingHypotheses();
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "behaviour '" << this->mb.getClassName()
	  << "' supports the following hypotheses : " << endl;
      for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
	log << " - " << ModellingHypothesis::toString(*ph);
	if(this->mb.hasSpecialisedMechanicalData(*ph)){
	  log << " (specialised)";
	}
	log << endl;	    
      }
    }
    // check of stiffness tensor requirement
    if((this->mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (this->mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR)){
      if((h.find(ModellingHypothesis::PLANESTRESS)!=h.end())||
	 (h.find(ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)!=h.end())){
	if(this->mb.hasAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor)){
	  if(!this->mb.hasAttribute(MechanicalBehaviourDescription::requiresUnAlteredStiffnessTensor)){
	    this->throwRuntimeError("MFrontBehaviourParserCommon::endsInputFileProcessing",
				    "No option was given to the '@RequireStiffnessTensor' keyword.\n"
				    "For plane stress hypotheses, it is required to precise whether "
				    "the expected stiffness tensor is 'Altered' (the plane stress "
				    "hypothesis is taken into account) or 'UnAltered' (the stiffness "
				    "tensor is the same as in plane strain)");
	  }
	}
      }
    }
    // some checks on thermal expansions
    if(this->mb.areThermalExpansionCoefficientsDefined()){
      const vector<shared_ptr<MaterialPropertyDescription> >& acs =
	this->mb.getThermalExpansionCoefficients();
      const Hypothesis uh = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if(this->mb.getSymmetryType()==mfront::ISOTROPIC){
	if(acs.size()!=1u){
	  string msg("MFrontBehaviourParserCommon::endsInputFileProcessing : "
		     "unexpected number of thermal expansion coefficients");
	  throw(runtime_error(msg));
	}
	this->analyseMaterialProperty(uh,*(acs[0]));
      } else if(this->mb.getSymmetryType()==mfront::ORTHOTROPIC){
	if(acs.size()!=3u){
	  string msg("MFrontBehaviourParserCommon::endsInputFileProcessing : "
		     "unexpected number of thermal expansion coefficients");
	  throw(runtime_error(msg));
	}
	this->analyseMaterialProperty(uh,*(acs[0]));
	this->analyseMaterialProperty(uh,*(acs[1]));
	this->analyseMaterialProperty(uh,*(acs[2]));
	} else {
	string msg("MFrontBehaviourParserCommon::endsInputFileProcessing : "
		   "unexpected symmetry type");
	throw(runtime_error(msg));
      }
      // check if the reference temperature is defined
      if(!this->mb.isParameterName(uh,"referenceTemperatureForThermalExpansion")){
	this->registerVariable("referenceTemperatureForThermalExpansion",false);
	this->mb.addParameter(uh,VariableDescription("real","referenceTemperatureForThermalExpansion",
						     1u,0u));
      	this->mb.setParameterDefaultValue(uh,"referenceTemperatureForThermalExpansion",293.15);
      }
    }
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->allowDynamicallyAllocatedArrays(this->areDynamicallyAllocatedVectorsAllowed());
    }
    if(getPedanticMode()){
      this->doPedanticChecks();
    }
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "MFrontBehaviourParserCommon::endsInputFileProcessing : end" << endl;
    }
  } // end of MFrontBehaviourParserCommon::endsInputFileProcessing

  /*!
   * \return the "true" integration variables (state variables are excluded)
   * \param[in] md : mechanical behaviour data
   */
  VarContainer
  getIntegrationVariables(const MechanicalBehaviourData& md)
  {
    const VarContainer& ivs = md.getIntegrationVariables();
    VarContainer v;
    for(VarContainer::const_iterator pv=ivs.begin();pv!=ivs.end();++pv){
      if(!md.isStateVariableName(pv->name)){
	v.push_back(*pv);
      }
    }
    return v;
  } // end of getIntegrationVariables

  /*!
   * \brief various checks
   * \param[in] v  : variables
   * \param[in] t  : variable type
   * \param[in] uv : list of all used variables
   * \param[in] b1 : check if the variable is used
   * \param[in] b2 : check if the variable increment (or rate) is used
   * \param[in] b3 : check if glossary name is declared
   * \param[in] b4 : check if variable is used in more than one code block (test for local variables)
   */
  static void
  performPedanticChecks(const MechanicalBehaviourData& md,
			const VarContainer& v,
			const std::string& t,
			const std::map<std::string,
				       unsigned short>& uv,
			const bool b1 = true,
			const bool b2 = true,
			const bool b3 = true,
			const bool b4 = false)
  {
    using namespace std;
    using namespace tfel::glossary;
    const Glossary& glossary = Glossary::getGlossary();
    ostream& log = getLogStream();
    for(VarContainer::const_iterator pv=v.begin();pv!=v.end();++pv){
      if(b1){
	const map<std::string,unsigned short>::const_iterator p = uv.find(pv->name);
	if(p==uv.end()){
	  log << "- " << t << " '" << pv->name << "' is unused." << endl;
	} else {
	  if(b4&&p->second==1){
	    log << "- " << t << " '" << pv->name << "' is used in one code block only." << endl;
	  }
	}
      }
      if(b2){
	if(uv.find("d"+pv->name)==uv.end()){
	  log << "- " << t << " increment 'd" << pv->name << "' is unused." << endl;
	}
      }
      if(b3){
	if((!md.hasGlossaryName(pv->name))&&
	   (!md.hasEntryName(pv->name))){
	  log << "- " << t << " '" << pv->name << "' has no glossary name." << endl;
	}
      }
      if(pv->description.empty()){
	bool hasDoc = false;
	if(md.hasGlossaryName(pv->name)){
	  const GlossaryEntry& e = glossary.getGlossaryEntry(md.getExternalName(pv->name));
	  hasDoc = (!e.getShortDescription().empty()) || (!e.getDescription().empty());
	}
	if(!hasDoc){
	  log << "- " << t << " '" << pv->name << "' has no description." << endl;
	}
      }
    }
  } 

  /*!
   * \brief various checks on static variables
   * \param[in] v  : variables
   * \param[in] uv : list of all static variables
   */
  static void
  performPedanticChecks(const StaticVarContainer& v,
			const std::map<std::string,
				       unsigned short>& uv)
  {
    using namespace std;
    ostream& log = getLogStream();
    for(StaticVarContainer::const_iterator pv=v.begin();pv!=v.end();++pv){
      if(uv.find(pv->name)==uv.end()){
	log << "- static variable '" << pv->name << "' is unused." << endl;
      }
    }      
  }
  
  void
  MFrontBehaviourParserCommon::doPedanticChecks(void) const
  {
    using namespace std;
    using namespace tfel::material;
    const set<Hypothesis>& hs = this->mb.getDistinctModellingHypotheses();
    ostream& log = getLogStream();
    log << endl << "* Pedantic checks" << endl;
    for(set<Hypothesis>::const_iterator ph=hs.begin();ph!=hs.end();++ph){
      const Hypothesis h = *ph;
      const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
      // checks if variables are used
      if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	log << endl << "** Beginning pedantic checks for default modelling hypothesis"
	    << endl << endl;
      } else {
	log << endl << "** Beginning pedantic checks for modelling hypothesis '"
	    << ModellingHypothesis::toString(h) << "'" << endl << endl;
      }
      // getting all used variables
      const vector<string>& cbs = md.getCodeBlockNames();
      map<string,unsigned short> vars;  // variable names and counts
      map<string,unsigned short> svars; // static variable nanes and counts
      for(vector<string>::const_iterator pcbs=cbs.begin();pcbs!=cbs.end();++pcbs){
	const CodeBlock& cb = md.getCodeBlock(*pcbs);
	if(cb.description.empty()){
	  log << "- code block '" << *pcbs << "' has no description" << endl;
	}
	for(set<string>::const_iterator pv=cb.variables.begin();
	    pv!=cb.variables.end();++pv){
	  if(vars.count(*pv)==0){
	    vars[*pv] = 1;
	  } else{
	    ++(vars[*pv]);
	  }
	}
	for(set<string>::const_iterator pv=cb.static_variables.begin();
	    pv!=cb.static_variables.end();++pv){
	  if(svars.count(*pv)==0){
	    svars[*pv] = 1;
	  } else{
	    ++(svars[*pv]);
	  }
	}
      }
      performPedanticChecks(md,md.getMaterialProperties(),"material property",vars,true,false,true);
      const VarContainer& ivs  = getIntegrationVariables(md);
      performPedanticChecks(md,ivs,"integration variable",vars,false,true,false);
      performPedanticChecks(md,md.getStateVariables(),"state variable",vars);
      performPedanticChecks(md,md.getAuxiliaryStateVariables(),"auxiliary state variable",vars,true,false);
      performPedanticChecks(md,md.getExternalStateVariables(),"external state variable",vars);
      performPedanticChecks(md,md.getLocalVariables(),"local variable",vars,true,false,false,true);
      performPedanticChecks(md,md.getParameters(),"parameter",vars,true,false);
      performPedanticChecks(md.getStaticVariables(),svars);
    }
    log << endl << "# End of pedantic checks" << endl;
  } // end of MFrontBehaviourParserCommon::pedanticChecks

  void 
  MFrontBehaviourParserCommon::writeOutputFiles(void)
  {
    using namespace std;
    using namespace tfel::system;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    typedef MFrontBehaviourAnalyserFactory  MBAF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    MBAF& mbaf = MBAF::getMFrontBehaviourAnalyserFactory();
    systemCall::mkdir("src");
    systemCall::mkdir("include");
    systemCall::mkdir("include/TFEL/");
    systemCall::mkdir("include/TFEL/Material");
    this->behaviourFileName  = this->mb.getClassName();
    this->behaviourFileName += ".hxx";
    this->behaviourFile.open(("include/TFEL/Material/"+this->behaviourFileName).c_str());
    if(!this->behaviourFile){
      string msg("MFrontBehaviourParserCommon::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->behaviourDataFileName  = this->mb.getClassName();
    this->behaviourDataFileName += "BehaviourData.hxx";
    this->behaviourDataFile.open(("include/TFEL/Material/"+this->behaviourDataFileName).c_str());
    if(!this->behaviourDataFile){
      string msg("MFrontBehaviourParserCommon::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->behaviourDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->integrationDataFileName  = this->mb.getClassName();
    this->integrationDataFileName += "IntegrationData.hxx";
    this->integrationDataFile.open(("include/TFEL/Material/"+this->integrationDataFileName).c_str());
    if(!this->integrationDataFile){
      string msg("MFrontBehaviourParserCommon::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->integrationDataFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));
    }
    this->srcFileName  = this->mb.getClassName();
    this->srcFileName += ".cxx";
    this->srcFile.open(("src/"+this->srcFileName).c_str());
    if(!this->srcFile){
      string msg("MFrontBehaviourParserCommon::writeOutputFiles : ");
      msg += "unable to open ";
      msg += this->srcFileName;
      msg += " for writing output file.";
      throw(runtime_error(msg));

    }
    // Adds some stuff
    this->endsInputFileProcessing();
    // generate outpout files
    this->writeBehaviourDataFileBegin();
    this->writeIntegrationDataFileBegin();
    this->writeBehaviourFileBegin();
    // modelling hypotheses handled by the interfaces (if at least one
    // interface is defined), or by the behaviour
    set<Hypothesis> hh;
    if(this->interfaces.empty()){
      const set<Hypothesis>& bh = this->mb.getModellingHypotheses();
      hh.insert(bh.begin(),bh.end());
    } else {
      // calling the interfaces
      vector<string>::const_iterator pi;
      for(pi  = this->interfaces.begin();
	  pi != this->interfaces.end();++pi){
	MFrontBehaviourVirtualInterface& i = *(mbif.getInterfacePtr(*pi));
	const set<Hypothesis>& ih = i.getModellingHypothesesToBeTreated(this->mb);
	hh.insert(ih.begin(),ih.end());
      }
    }
    if(!this->mb.areAllMechanicalDataSpecialised(hh)){
      if(getVerboseMode()>=VERBOSE_DEBUG){
	ostream& log = getLogStream();
	log << "MFrontBehaviourParserCommon::writeOutputFiles : "
	    << "treating default hypothesis" << endl;
      }
      const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      // Generating BehaviourData's outputClass
      if(getVerboseMode()>=VERBOSE_DEBUG){
	ostream& log = getLogStream();
	log << "MFrontBehaviourParserCommon::writeOutputFiles : writing behaviour data "
	    << "for default hypothesis"  << endl;
      }
      this->writeBehaviourDataClass(h);
      // Generating IntegrationData's outputClass
      if(getVerboseMode()>=VERBOSE_DEBUG){
	ostream& log = getLogStream();
	log << "MFrontBehaviourParserCommon::writeOutputFiles : writing integration data "
	    << "for default hypothesis"  << endl;
      }
      this->writeIntegrationDataClass(h);
      // Generating Behaviour's outputFile
      if(getVerboseMode()>=VERBOSE_DEBUG){
	ostream& log = getLogStream();
	log << "MFrontBehaviourParserCommon::writeOutputFiles : writing behaviour class "
	    << "for default hypothesis"  << endl;
      }
      this->writeBehaviourClass(h);
    }
    for(set<Hypothesis>::const_iterator ph = hh.begin();ph!=hh.end();++ph){
      if(mb.hasSpecialisedMechanicalData(*ph)){
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  ostream& log = getLogStream();
	  log << "MFrontBehaviourParserCommon::writeOutputFiles : "
	      << "treating hypothesis '" << ModellingHypothesis::toString(*ph)
	      << "'" << endl;
	}
	// Generating BehaviourData's outputClass
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  ostream& log = getLogStream();
	  log << "MFrontBehaviourParserCommon::writeOutputFiles : writing behaviour data "
	      << "for hypothesis '" << ModellingHypothesis::toString(*ph) << "'" << endl;
	}
	this->writeBehaviourDataClass(*ph);
	// Generating IntegrationData's outputClass
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  ostream& log = getLogStream();
	  log << "MFrontBehaviourParserCommon::writeOutputFiles : writing integration data "
	      << "for hypothesis '" << ModellingHypothesis::toString(*ph) << "'" << endl;
	}
	this->writeIntegrationDataClass(*ph);
	// Generating behaviour's outputClass
	if(getVerboseMode()>=VERBOSE_DEBUG){
	  ostream& log = getLogStream();
	  log << "MFrontBehaviourParserCommon::writeOutputFiles : writing behaviour class "
	      << "for hypothesis '" << ModellingHypothesis::toString(*ph) << "'" << endl;
	}
	this->writeBehaviourClass(*ph);
      }
    }
    this->writeBehaviourDataFileEnd();
    this->writeIntegrationDataFileEnd();
    this->writeBehaviourFileEnd();
    // Generating behaviour's source file
    if(getVerboseMode()>=VERBOSE_DEBUG){
      ostream& log = getLogStream();
      log << "MFrontBehaviourParserCommon::writeOutputFiles : writing source file" << endl;
    }
    this->writeSrcFile();
    // calling the interfaces
    vector<string>::const_iterator pi;
    for(pi  = this->interfaces.begin();
	pi != this->interfaces.end();++pi){
      MFrontBehaviourVirtualInterface& i = *(mbif.getInterfacePtr(*pi));
      if(getVerboseMode()>=VERBOSE_DEBUG){
	ostream& log = getLogStream();
	log << "MFrontBehaviourParserCommon::writeOutputFiles : "
	    << "calling interface '" << *pi << "'" << endl;
      }
      i.endTreatement(this->mb,*this);
    }
    // calling the analysers
    vector<string>::const_iterator pa;
    for(pa  = this->analysers.begin();
	pa != this->analysers.end();++pa){
      MFrontBehaviourAnalyser& a = *(mbaf.getAnalyserPtr(*pa));
      if(getVerboseMode()>=VERBOSE_DEBUG){
	ostream& log = getLogStream();
	log << "MFrontBehaviourParserCommon::writeOutputFiles : "
	    << "calling analyser '" << *pa << "'" << endl;
      }
      a.endTreatement(this->mb,*this);
    }
  }

  void
  MFrontBehaviourParserCommon::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(const Hypothesis h,
												      const std::string& n)
  {
    if(!this->explicitlyDeclaredUsableInPurelyImplicitResolution){
      this->mb.setUsableInPurelyImplicitResolution(h,false);
    }
    this->mb.declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(h,n);
  } // end of MFrontBehaviourParserCommon::declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution

  std::string
  MFrontBehaviourParserCommon::standardModifier(const Hypothesis h,
						const std::string& var,
						const bool addThisPtr)
  {
    if((this->mb.isExternalStateVariableIncrementName(h,var))||(var=="dT")){
      this->declareExternalStateVariableProbablyUnusableInPurelyImplicitResolution(h,var.substr(1));
    }
    if(addThisPtr){
      return "this->"+var;
    }
    return var;
  } // end of MFrontBehaviourParserCommon::standardModifier
    
  std::string
  MFrontBehaviourParserCommon::tangentOperatorVariableModifier(const Hypothesis h,
							       const std::string& var,
							       const bool addThisPtr)
  {
    return this->standardModifier(h,var,addThisPtr);
  } // end of MFrontBehaviourParserCommon::tangentOperatorVariableModifier
  
  void MFrontBehaviourParserCommon::treatTangentOperator(void)
  {
    using namespace std;
    using namespace tfel::material;
    using namespace tfel::utilities;
    CodeBlockOptions o;
    this->readCodeBlockOptions(o,true);
    if(this->mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      vector<FiniteStrainBehaviourTangentOperatorBase::Flag> to = getFiniteStrainBehaviourTangentOperatorFlags();
      vector<FiniteStrainBehaviourTangentOperatorBase::Flag>::const_iterator pto;
      bool found = false;
      if(o.untreated.size()!=1u){
	ostringstream msg;
	msg << "MFrontBehaviourParserCommon::treatTangentOperator : "
	    << "tangent operator type is undefined. Valid tanget operator type are :" << endl;
	for(pto=to.begin();pto!=to.end();++pto){
	  msg << "- " << convertFiniteStrainBehaviourTangentOperatorFlagToString(*pto) << " : "
	      << getFiniteStrainBehaviourTangentOperatorDescription(*pto) << endl;
	}
	throw(runtime_error(msg.str()));
      }
      if(o.untreated[0].flag!=Token::Standard){
	string msg("MFrontBehaviourParserCommon::treatTangentOperator : "
		   "invalid option '"+o.untreated[0].value+"'");
	throw(runtime_error(msg));
      }
      const string& ktype = o.untreated[0].value;
      for(pto=to.begin();(pto!=to.end())&&(!found);++pto){
	found = (ktype==convertFiniteStrainBehaviourTangentOperatorFlagToString(*pto));
      }
      if(!found){
	ostringstream msg;
	msg << "MFrontBehaviourParserCommon::treatTangentOperator : "
	    << "invalid tangent operator type '"+ktype+"'. Valid tanget operator type are :" << endl;
	for(pto=to.begin();pto!=to.end();++pto){
	  msg << "- " << convertFiniteStrainBehaviourTangentOperatorFlagToString(*pto) << " : "
	      << getFiniteStrainBehaviourTangentOperatorDescription(*pto) << endl;
	}
	throw(runtime_error(msg.str()));
      }
      this->readCodeBlock(*this,o,MechanicalBehaviourData::ComputeTangentOperator+"-"+ktype,
			  &MFrontBehaviourParserCommon::tangentOperatorVariableModifier,true);
      for(set<Hypothesis>::const_iterator p=o.hypotheses.begin();p!=o.hypotheses.end();++p){
	if(!this->mb.hasAttribute(*p,MechanicalBehaviourData::hasConsistentTangentOperator)){
	  this->mb.setAttribute(*p,MechanicalBehaviourData::hasConsistentTangentOperator,true);
	}
      }
    } else {
      this->treatUnsupportedCodeBlockOptions(o);
      this->readCodeBlock(*this,o,MechanicalBehaviourData::ComputeTangentOperator,
			  &MFrontBehaviourParserCommon::tangentOperatorVariableModifier,true);
      for(set<Hypothesis>::const_iterator p=o.hypotheses.begin();p!=o.hypotheses.end();++p){
	this->mb.setAttribute(*p,MechanicalBehaviourData::hasConsistentTangentOperator,true);
      }
    }
  } // end of MFrontBehaviourParserCommon::treatTangentOperator

  void MFrontBehaviourParserCommon::treatIsTangentOperatorSymmetric(void)
  {
    using namespace std;
    set<Hypothesis> h;
    this->readHypothesesList(h);
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatIsTangentOperatorSymmetric : ",
  			    "Expected 'true' or 'false'.");
    bool b = this->readBooleanValue("MFrontBehaviourParserCommon::treatIsTangentOperatorSymmetric");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatIsTangentOperatorSymmetric",";");
    for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
      this->mb.setAttribute(*ph,MechanicalBehaviourData::isConsistentTangentOperatorSymmetric,b);
    }
  } // end of MFrontBehaviourParserCommon::treatTangentOperator

  void
  MFrontBehaviourParserCommon::treatMaterial(void)
  {
    using namespace std;
    const string& m = this->readOnlyOneToken();
    if(!CxxTokenizer::isValidIdentifier(m,true)){
      string msg("MFrontBehaviourParserCommon::treatMaterial : ");
      msg += "invalid material name '"+m+"'";
      throw(runtime_error(msg));
    }
    this->mb.setMaterialName(m);
    if(!isValidIdentifier(this->mb.getClassName())){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatMaterial",
			      "resulting class name is not valid (read '"+
			      this->mb.getClassName()+"')");
    }
  } // end of MFrontBehaviourParserCommon::treatMaterial

  void
  MFrontBehaviourParserCommon::treatLibrary(void)
  {
    using namespace std;
    const string& m = this->readOnlyOneToken();
    if(!CxxTokenizer::isValidIdentifier(m,true)){
      string msg("MFrontBehaviourParserCommon::treatLibrary : ");
      msg += "invalid library name '"+m+"'";
      throw(runtime_error(msg));
    }
    this->mb.setLibrary(m);
  } // end of MFrontBehaviourParserCommon::treatLibrary

  void
  MFrontBehaviourParserCommon::treatComputeThermalExpansion(void)
  {
    using namespace std;
    using tfel::utilities::shared_ptr;
    const string m("MFrontBehaviourParserCommon::treatComputeThermalExpansion");
    const vector<string>& files = this->readStringOrArrayOfString(m);
    this->readSpecifiedToken(m,";");
    if((files.size()!=1u)&&(files.size()!=3u)){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatComputeThermalExpansion",
			      "invalid number of file names given");
    }
    if(files.size()==1u){
      // the material shall have been declared isotropic
      if(this->mb.getSymmetryType()!=mfront::ISOTROPIC){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatComputeThermalExpansion",
				"the mechanical behaviour must be isotropic to give more than "
				"one thermal expansion coefficient.");
      }
    }
    if(files.size()==3u){
      // the material shall have been declared orthotropic
      if(this->mb.getSymmetryType()!=mfront::ORTHOTROPIC){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatComputeThermalExpansion",
				"the mechanical behaviour must be orthotropic to give more than "
				"one thermal expansion coefficient.");
      }
    }
    vector<shared_ptr<MaterialPropertyDescription> > acs;
    for(vector<string>::const_iterator p=files.begin();p!=files.end();++p){
      const string& f = MFrontSearchFile::search(*p);
      shared_ptr<MaterialPropertyDescription> a;
      a = shared_ptr<MaterialPropertyDescription>(new MaterialPropertyDescription(this->handleMaterialLaw(f)));
      if(!a->staticVars.contains("ReferenceTemperature")){
	if(getVerboseMode()!=VERBOSE_QUIET){
	  ostream& os = getLogStream();
	  os  << "no reference temperature in material property '";
	  if(a->material.empty()){
	    os << a->material << '_';
	  }
	  os << a->law << "'" << endl;
	}
      }
      if(!((a->inputs.size())||(a->inputs.size()!=1u))){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatComputeThermalExpansion : ",
				"thermal expansion shall only depend on temperature");
      }
      if(a->inputs.size()==1u){
	const VariableDescription& v =  a->inputs.front();
	const string& vn = v.getExternalName(a->glossaryNames,a->entryNames);
	if(vn!="Temperature"){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::treatComputeThermalExpansion : ",
				  "thermal expansion shall only depend on temperature");
	}
      }
      acs.push_back(a);
    }
    if(acs.size()==1u){
      this->mb.setThermalExpansionCoefficient(acs.front());
    } else {
      this->mb.setThermalExpansionCoefficients(acs[0],acs[1],acs[2]);
    }
  } // end of MFrontBehaviourParserCommon::treatComputeThermalExpansion

  void
  MFrontBehaviourParserCommon::treatModellingHypothesis(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatModellingHypothesis");
    const Hypothesis h = ModellingHypothesis::fromString(this->current->value);
    ++(this->current);
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatModellingHypothesis");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatModellingHypothesis",";");
    if(!this->isModellingHypothesisSupported(h)){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatModellingHypothesis",
			      "unsupported modelling hypothesis '"+
			      ModellingHypothesis::toString(h)+"'");
    }
    set<Hypothesis> hypotheses;
    hypotheses.insert(h);
    this->mb.setModellingHypotheses(hypotheses);
  } // end of MFrontBehaviourParserCommon::treatModellingHypothesis

  void
  MFrontBehaviourParserCommon::treatModellingHypotheses(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    set<Hypothesis> hypotheses;
    vector<Token> tokens;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatModellingHypotheses");
    this->readList(tokens,"MFrontBehaviourParserCommon::treatModellingHypotheses","{","}",false);
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatModellingHypotheses");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatModellingHypotheses",";");
    for(vector<Token>::const_iterator p=tokens.begin();p!=tokens.end();++p){
      if(p->flag==Token::String){
	this->appendToHypothesesList(hypotheses,p->value.substr(1,p->value.size()-2));
      } else {
	this->appendToHypothesesList(hypotheses,p->value);
      }
    }
    if(hypotheses.empty()){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatModellingHypotheses",
			      "no hypothesis declared");
    }
    this->mb.setModellingHypotheses(hypotheses);
  } // end of MFrontBehaviourParserCommon::treatModellingHypotheses

  void
  MFrontBehaviourParserCommon::treatUsableInPurelyImplicitResolution(void)
  {
    const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatUsableInPurelyImplicitResolution",";");
    if(this->explicitlyDeclaredUsableInPurelyImplicitResolution){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatUsableInPurelyImplicitResolution",
			      "keyword '@UsableInPurelyImplicitResolution' already called");
    }
    this->explicitlyDeclaredUsableInPurelyImplicitResolution = true;
    this->mb.setUsableInPurelyImplicitResolution(h,true);
  } // end of MFrontBehaviourParserCommon::treatUsableInPurelyImplicitResolution

  void
  MFrontBehaviourParserCommon::treatParameterMethod(const Hypothesis h)
  {
    using namespace std;
    using namespace tfel::utilities;
    const string& n = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatParameterMethod");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatParameterMethod",".");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatParameterMethod");
    if(this->current->value=="setDefaultValue"){
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatParameterMethod");
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatParameterMethod","(");
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatParameterMethod");
      double value;
      istringstream converter(this->current->value);
      converter >> value;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatParameterMethod",
				"could not read default value for parameter '"+n+"'");
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatParameterMethod");
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatParameterMethod",")");
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatParameterMethod");
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatParameterMethod",";");
      this->mb.setParameterDefaultValue(h,n,value);
    } else {
      --(this->current);
      --(this->current);
      this->treatVariableMethod(h);
    }
  } // end of MFrontBehaviourParserCommon::treatParameterMethod

  bool
  MFrontBehaviourParserCommon::isCallableVariable(const Hypothesis h,
						  const std::string& n) const
  {
    return ((this->mb.isMaterialPropertyName(h,n))||
	    (this->mb.isStateVariableName(h,n))||
	    (this->mb.isAuxiliaryStateVariableName(h,n))||
	    (this->mb.isExternalStateVariableName(h,n))||
	    (this->mb.isParameterName(h,n)));
  } // end of MFrontBehaviourParserCommon::isCallableVariable

  void
  MFrontBehaviourParserCommon::treatVariableMethod(const Hypothesis h)
  {
    using namespace std;
    using namespace tfel::utilities;
    using namespace tfel::glossary;
    const Glossary& glossary = Glossary::getGlossary();
    map<string,string>::const_iterator p;
    const string& n = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatVariableMethod",".");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
    if(this->current->value=="setGlossaryName"){
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatVariableMethod","(");
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
      if(this->current->flag!=Token::String){
	string msg("MFrontBehaviourParserCommon::treatVariableMethod : ");
	msg += "expected to read a string";
	throw(runtime_error(msg));
      }
      const string& g = this->current->value.substr(1,this->current->value.size()-2);
      if(!glossary.contains(g)){
	string msg("MechanicalBehaviourData::setGlossaryName : "
		   "'"+g+"' is not a glossary name");
	throw(runtime_error(msg));
      }
      this->mb.setGlossaryName(h,n,g);
      ++(this->current);
    } else if(this->current->value=="setEntryName"){
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatVariableMethod","(");
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
      if(this->current->flag!=Token::String){
	string msg("MFrontBehaviourParserCommon::treatVariableMethod : ");
	msg += "expected to read a string";
	throw(runtime_error(msg));
      }
      const string& e = this->current->value.substr(1,this->current->value.size()-2);
      if(!this->isValidIdentifier(e)){
	string msg("MFrontBehaviourParserCommon::treatVariableMethod : ");
	msg += "invalid entry name '"+e+"'";
	throw(runtime_error(msg));
      }
      ++(this->current);
      this->mb.setEntryName(h,n,e);
    } else {
      this->treatUnknownVariableMethod(h,n);
    }
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatVariableMethod",")");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatVariableMethod");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatVariableMethod",";");
  } // end of MFrontBehaviourParserCommon::treatVariableMethod

  void
  MFrontBehaviourParserCommon::treatUnknownVariableMethod(const Hypothesis,
							  const std::string& n)
  {
    using namespace std;
    string msg("MFrontBehaviourParserCommon::treatUnknownVariableMethod : ");
    msg += "unknown method '"+this->current->value+"' for variable '"+n+"', ";
    msg += "valid methods are 'setGlossaryName' or 'setEntryName'";
    throw(runtime_error(msg));
  } // end of MFrontBehaviourParserCommon::treatUnknownVariableMethod

  void
  MFrontBehaviourParserCommon::treatUnknownKeyword(void)
  {
    using namespace std;
    using namespace tfel::utilities;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    typedef MFrontBehaviourAnalyserFactory  MBAF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    MBAF& mbaf = MBAF::getMFrontBehaviourAnalyserFactory();
    pair<bool,CxxTokenizer::TokensContainer::const_iterator> p;
    TokensContainer::const_iterator p2;
    vector<string>::const_iterator i;
    bool treated = false;
    string key;
    --(this->current);
    key = this->current->value;
    ++(this->current);
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUnknownKeyword");
    if(this->current->value=="["){
      set<string> s;
      set<string> s2;
      while(this->current->value!="]"){
	++(this->current);
	this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUnknownKeyword");
	string t;
	if(this->current->flag==Token::String){
	  t = this->current->value.substr(1,this->current->value.size()-2);
	} else {
	  t = this->current->value;
	}
	if(find(this->interfaces.begin(),this->interfaces.end(),t)!=this->interfaces.end()){
	  s.insert(t);
	}
	if(find(this->analysers.begin(),this->analysers.end(),t)!=this->analysers.end()){
	  s2.insert(t);
	}
	++(this->current);
	if((this->current->value!="]")&&(this->current->value!=",")){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::treatUnknownKeyword",
				  "unexpected token '"+this->current->value+"' (expected ']' or ',').");
	}
      }
      ++(this->current);
      if((s.empty())&&(s2.empty())){
	this->ignoreKeyWord(key);
      } else {
	for(set<string>::const_iterator pi  = s.begin();pi != s.end();++pi){
	  MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*pi);
	  p = interface->treatKeyword(key,this->current,
				      this->fileTokens.end());
	  if(!p.first){
	    string msg("MFrontBehaviourParserCommon::treatUnknownKeyword : the keyword '");
	    msg += key;
	    msg += " has not been treated by interface '"+*pi+"'";
	    throw(runtime_error(msg));
	  }
	  if(treated){
	    if(p2!=p.second){
	      string msg("MFrontBehaviourParserCommon::treatUnknownKeyword : the keyword '");
	      msg += key;
	      msg += "' has been treated by two interfaces/analysers but";
	      msg += " results were differents";
	      throw(runtime_error(msg));
	    }
	  }
	  p2 = p.second;
	  treated = true;
	}
	for(set<string>::const_iterator pa  = s2.begin();pa != s2.end();++pa){
	  MFrontBehaviourAnalyser *analyser = mbaf.getAnalyserPtr(*pa);
	  p = analyser->treatKeyword(key,this->current,
				      this->fileTokens.end());
	  if(!p.first){
	    string msg("MFrontBehaviourParserCommon::treatUnknownKeyword : the keyword '");
	    msg += key;
	    msg += " has not been treated by analyser '"+*pa+"'";
	    throw(runtime_error(msg));
	  }
	  if(treated){
	    if(p2!=p.second){
	      string msg("MFrontBehaviourParserCommon::treatUnknownKeyword : the keyword '");
	      msg += key;
	      msg += "' has been treated by two interfaces/analysers but";
	      msg += " results were differents";
	      throw(runtime_error(msg));
	    }
	  }
	  p2 = p.second;
	  treated = true;
	}
	this->current = p2;
      }
    } else {
      for(i  = this->interfaces.begin();
	  i != this->interfaces.end();++i){
	MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
	p = interface->treatKeyword(key,this->current,
				    this->fileTokens.end());
	if(p.first){
	  if(treated){
	    if(p2!=p.second){
	      string msg("MFrontBehaviourParserCommon::treatUnknownKeyword : the keyword '");
	      msg += key;
	      msg += "' has been treated by two interfaces/analysers but";
	      msg += " results were differents";
	      throw(runtime_error(msg));
	    }
	  }
	  p2 = p.second;
	  treated = true;
	}
      }
      for(i  = this->analysers.begin();
	  i != this->analysers.end();++i){
	MFrontBehaviourAnalyser *analyser = mbaf.getAnalyserPtr(*i);
	p = analyser->treatKeyword(key,this->current,
				   this->fileTokens.end());
	if(p.first){
	  if(treated){
	    if(p2!=p.second){
	      string msg("MFrontBehaviourParserCommon::treatUnknownKeyword : the keyword '");
	      msg += key;
	      msg += "' has been treated by two interfaces/analysers but";
	      msg += " results were differents";
	      throw(runtime_error(msg));
	    }
	  }
	  p2 = p.second;
	  treated = true;
	}
      }
      if(!treated){
	ParserBase::treatUnknownKeyword();
      }
      this->current = p2;
    }
  } // end of MFrontBehaviourParserCommon::treatUnknownKeyword

  void
  MFrontBehaviourParserCommon::treatUseQt(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatUseQt : ",
			    "Expected 'true' or 'false'.");
    if(this->current->value=="true"){
      this->mb.setUseQt(true);
    } else if(this->current->value=="false"){
      this->mb.setUseQt(false);
    } else {
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatUseQt",
			      "Expected to read 'true' or 'false' instead of '"+this->current->value+".");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatUseQt",";");
  } // end of MFrontBehaviourParserCommon::treatUseQt

  void
  MFrontBehaviourParserCommon::treatIsotropicBehaviour(void)
  {
    using namespace std;
    if(this->mb.getSymmetryType()!=mfront::ISOTROPIC){
      string msg("MFrontBehaviourParserCommon::treatIsotropicBehaviour  : ");
      msg += "this behaviour has been declared orthotropic";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatIsotropicBehaviour",";");
  } // end of MFrontBehaviourParserCommon::treatIsotropicBehaviour

  void
  MFrontBehaviourParserCommon::treatOrthotropicBehaviour(void)
  {
    using namespace std;
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatOrthotropicBehaviour",";");
    this->mb.setSymmetryType(mfront::ORTHOTROPIC);
    // by defaut the elastic behaviour is also orthotropic
    this->mb.setElasticSymmetryType(mfront::ORTHOTROPIC);
  } // end of MFrontBehaviourParserCommon::treatOrthotropicBehaviour

  void
  MFrontBehaviourParserCommon::treatIsotropicElasticBehaviour(void)
  {
    using namespace std;
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatIsotropicElasticBehaviour",";");
    if(this->mb.getSymmetryType()!=mfront::ORTHOTROPIC){
      string msg("MFrontBehaviourParserCommon::treatIsotropicElasticBehaviour  : ");
      msg += "this behaviour has not been declared orthotropic";
      throw(runtime_error(msg));
    }
    if(this->mb.getElasticSymmetryType()!=mfront::ORTHOTROPIC){
      string msg("MFrontBehaviourParserCommon::treatIsotropicElasticBehaviour  : ");
      msg += "the elastic behaviour has already been declared isotropic";
      throw(runtime_error(msg));
    }
    this->mb.setElasticSymmetryType(mfront::ISOTROPIC);
  } // end of MFrontBehaviourParserCommon::treatIsotropicElasticBehaviour

  void
  MFrontBehaviourParserCommon::treatRequireStiffnessOperator(void)
  {
    using namespace std;
    if(getVerboseMode()>=VERBOSE_LEVEL2){
      ostream& log = getLogStream();
      log << "MFrontBehaviourParserCommon::treatRequireStiffnessOperator : "
	  << "@RequireStiffnessOperator is deprecated" << endl
	  << "You shall use @RequireStiffnessTensor instead" << endl;
    }
    this->treatRequireStiffnessTensor();
  } // end of MFrontBehaviourParserCommon::treatRequireStiffnessOperator

  void
  MFrontBehaviourParserCommon::treatRequireStiffnessTensor(void)
  {
    using namespace std;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatRequireStiffnessTensor");
    if(this->current->value=="<"){
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatRequireStiffnessTensor","<");
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatRequireStiffnessTensor");
      if(this->current->value=="UnAltered"){
	this->mb.setAttribute(MechanicalBehaviourDescription::requiresUnAlteredStiffnessTensor,true,false);
      } else if(this->current->value=="Altered"){
	this->mb.setAttribute(MechanicalBehaviourDescription::requiresUnAlteredStiffnessTensor,false,false);
      } else {
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatRequireStiffnessTensor : ",
				"expected 'Altered' or 'UnAltered' option "
				"(read '"+this->current->value+"')");
      }
      ++(this->current);
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatRequireStiffnessTensor",">");
    }
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatRequireStiffnessTensor",";");
    this->mb.setAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,true,false);
  } // end of MFrontBehaviourParserCommon::treatRequireStiffnessTensor

  void
  MFrontBehaviourParserCommon::treatRequireThermalExpansionCoefficientTensor(void)
  {
    using namespace std;
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatRequireThermalExpansionCoefficientTensor",";");
    this->mb.setAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,true,false);
  } // end of MFrontBehaviourParserCommon::treatRequireThermalExpansionCoefficientTensor

  void MFrontBehaviourParserCommon::treatBehaviour(void)
  {
    using namespace std;
    const string& b = this->readOnlyOneToken();
    this->mb.setBehaviourName(b);
    if(!isValidIdentifier(this->mb.getClassName())){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatBehaviour",
			      "resulting class name is not valid (read '"+
			      this->mb.getClassName()+"')");
    }
  } // end of MFrontBehaviourParserCommon::treatMaterial

  void 
  MFrontBehaviourParserCommon::readStringList(std::vector<std::string>& cont)
  {
    using namespace std;
    string type;
    string s;
    bool endOfTreatement;
    vector<string>::iterator p;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::readStringList",
			    "Cannot read interface name.");
    endOfTreatement=false;
    while((this->current!=this->fileTokens.end())&&
	  (!endOfTreatement)){
      s = this->current->value;
      if(!isValidIdentifier(s)){
	--(this->current);
	this->throwRuntimeError("MFrontBehaviourParserCommon::readStringList",
				"interface name is not valid (read '"+s+"')");
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::readStringList");
      if(this->current->value==","){
	++(this->current);
      } else if (this->current->value==";"){
	endOfTreatement=true;
	++(this->current);
      } else {
	this->throwRuntimeError("MFrontBehaviourParserCommon::readStringList",
				"',' or ';' expected afer '"+s+"'");
      }
      if((p = std::find(cont.begin(),cont.end(),s))!=cont.end()){
	this->throwRuntimeError("MFrontBehaviourParserCommon::readStringList",
				"'"+s+"' has already been registred.\n");
      } 
      cont.push_back(s);
    }
    if(!endOfTreatement){
      --(this->current);
      this->throwRuntimeError("MFrontBehaviourParserCommon::readStringList",
			      "Expected ';' before end of file.");
    }
  }

  void
  MFrontBehaviourParserCommon::readHypothesesList(std::set<Hypothesis>& h)
  {
    using namespace std;
    using namespace tfel::utilities;
    h.clear();
    if(this->current==this->fileTokens.end()){
      h.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      return;
    }
    if(this->current->value!="<"){
      h.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
      return;
    }
    vector<Token> tokens;
    this->readList(tokens,"MFrontBehaviourParserCommon::readHypothesesList","<",">",true);
    for(vector<Token>::const_iterator p=tokens.begin();p!=tokens.end();++p){
      if(p->flag==Token::String){
	this->appendToHypothesesList(h,p->value.substr(1,p->value.size()-2));
      } else {
	this->appendToHypothesesList(h,p->value);
      }
    }
    if(h.empty()){
      h.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    }
  } // end of MFrontBehaviourParserCommon::readHypothesesList

  void
  MFrontBehaviourParserCommon::readVariableList(VariableDescriptionContainer& v,
						std::set<Hypothesis>& h,
						void (MechanicalBehaviourDescription::* m)(const Hypothesis,
											   const VariableDescriptionContainer&),
						const bool b1,
						const bool b2,
						const bool b3)
  {
    using namespace std;
    h.clear();
    v.clear();
    this->readHypothesesList(h);
    this->readVarList(v,b1,b2,true);
    this->addVariableList(h,v,m,b3);
  } // end of MFrontBehaviourParserCommon::readVariableList

  void
  MFrontBehaviourParserCommon::addVariableList(const std::set<Hypothesis>& h,
					       const VariableDescriptionContainer& v,
					       void (MechanicalBehaviourDescription::* m)(const Hypothesis,
											  const VariableDescriptionContainer&),
					       const bool b)
  {
    using namespace std;						
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      if(!b){
	if(!this->mb.getAttribute<bool>(*ph,MechanicalBehaviourData::allowsNewUserDefinedVariables,true)){
	  vector<string> cbn(this->mb.getCodeBlockNames(*ph));
	  if(cbn.empty()){
	    this->throwRuntimeError("MFrontBehaviourParserCommon::readVariableList : ",
				    "no more variable can be defined. This may mean that "
				    "the parser does not expect you to add variables");
	  } else {
	    string cbs;
	    for(vector<string>::const_iterator pn=cbn.begin();pn!=cbn.end();++pn){
	      cbs += "\n- "+*pn;
	    }
	    this->throwRuntimeError("MFrontBehaviourParserCommon::readVariableList : ",
				    "no more variable can be defined. This may mean that "
				    "you already declared a block of code (or that the parser does not "
				    "expect you to add variables for whatever reason). This is the list of "
				    "code blocks defined :"+cbs);
	  }
	}
      }
      (this->mb.*m)(*ph,v);
    }
  } // end of MFrontBehaviourParserCommon::addVariableList

  void MFrontBehaviourParserCommon::treatCoef(void)
  {
    using namespace std;
    VarContainer v;
    set<Hypothesis> h;
    this->readVariableList(v,h,&MechanicalBehaviourDescription::addMaterialProperties,true,false,false);
  } // end of MFrontBehaviourParserCommon::treatCoef

  void
  MFrontBehaviourParserCommon::treatLocalVar(void)
  {
    using namespace std;
    VarContainer v;
    set<Hypothesis> h;
    this->readVariableList(v,h,&MechanicalBehaviourDescription::addLocalVariables,true,false,true);
  } // end of MFrontBehaviourParserCommon::treatLocalVar

  void MFrontBehaviourParserCommon::treatInterface(void)
  {
    this->readStringList(this->interfaces);
  } // end of MFrontBehaviourParserCommon::treatInterface

  void
  MFrontBehaviourParserCommon::setInterfaces(const std::set<std::string>& i)
  {
    using namespace std;
    copy(i.begin(),i.end(),back_inserter(this->interfaces));
  } // end of MFrontBehaviourParserCommon::setInterfaces

  void
  MFrontBehaviourParserCommon::setAnalysers(const std::set<std::string>& i)
  {
    using namespace std;
    copy(i.begin(),i.end(),back_inserter(this->analysers));
  } // end of MFrontBehaviourParserCommon::setAnalysers

  void
  MFrontBehaviourParserCommon::treatIntegrator(void)
  {
    this->readCodeBlock(*this,MechanicalBehaviourData::Integrator,
			&MFrontBehaviourParserCommon::standardModifier,true,true);
  } // end of MFrontBehaviourParserCommon::treatIntegrator

  void MFrontBehaviourParserCommon::treatStateVariable(void)
  {
    using namespace std;
    VarContainer v;
    set<Hypothesis> h;
    this->readVariableList(v,h,&MechanicalBehaviourDescription::addStateVariables,true,false,false);
  }

  void MFrontBehaviourParserCommon::treatAuxiliaryStateVariable(void)
  {
    using namespace std;
    VarContainer v;
    set<Hypothesis> h;
    this->readVariableList(v,h,&MechanicalBehaviourDescription::addAuxiliaryStateVariables,true,false,false);
  }

  void MFrontBehaviourParserCommon::treatExternalStateVariable(void)
  {
    using namespace std;
    VarContainer v;
    set<Hypothesis> h;
    this->readVariableList(v,h,&MechanicalBehaviourDescription::addExternalStateVariables,true,true,false);
  }

  void
  MFrontBehaviourParserCommon::treatBounds(void)
  {
    using namespace std;
    set<Hypothesis> h;
    this->readHypothesesList(h);
    TokensContainer::const_iterator b = current;
    for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
      current = b;
      BoundsDescription d;
      d.category = BoundsDescription::Standard;
      this->treatBounds(d,*ph);
      this->mb.setBounds(*ph,d);
    }
  } // end of MFrontBehaviourParserCommon::treatBounds

  void
  MFrontBehaviourParserCommon::treatPhysicalBounds(void)
  {
    using namespace std;
    set<Hypothesis> h;
    this->readHypothesesList(h);
    TokensContainer::const_iterator b = current;
    for(set<Hypothesis>::const_iterator ph = h.begin();ph!=h.end();++ph){
      current = b;
      BoundsDescription d;
      d.category = BoundsDescription::Physical;
      this->treatBounds(d,*ph);
      this->mb.setBounds(*ph,d);
    }
  } // end of MFrontBehaviourParserCommon::treatPhysicalBounds

  void MFrontBehaviourParserCommon::treatBounds(BoundsDescription& d,
						const Hypothesis h)
  {
    using namespace std;
    VariableDescriptionContainer::const_iterator p;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    bool found;
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds");

    d.lineNumber = this->current->line;
    d.name       = this->current->value;
    d.varName    = this->current->value;
    d.component  = -1;

    found = false;
    d.arraySize = 1u;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      map<DrivingVariable,
	  ThermodynamicForce>::const_iterator p3;
      for(p3=this->mb.getMainVariables().begin();
	  p3!=this->mb.getMainVariables().end();++p3){
	// check if the driving variable match
	if(d.varName==p3->first.name){
	  found=true;
	  if(p3->first.increment_known){
	    d.varCategory = BoundsDescription::ExternalStateVariable;
	  } else {
	  d.varCategory = BoundsDescription::ExternalStateVariableII;
	  }
	  d.varType     = this->getTypeFlag(p3->first.type);
	}
	// check if the associated thermodynamic force match
	if(d.varName==p3->second.name){
	  found=true;
	  d.varCategory = BoundsDescription::StateVariable;
	  d.varType     = this->getTypeFlag(p3->second.type);
	}
      }
    }
    // temperature
    if(d.varName=="T"){
      found=true;
      d.varCategory = BoundsDescription::StateVariable;
      d.varType     = Scalar;
    }
    for(p   = md.getMaterialProperties().begin();
	(p != md.getMaterialProperties().end())&&(!found);++p){
      if(p->name==d.varName){
	found=true;
	d.varCategory = BoundsDescription::MaterialProperty;
	d.varType     = this->getTypeFlag(p->type);
	d.arraySize   = p->arraySize;
      }
    }
    for(p   = md.getLocalVariables().begin();
	(p != md.getLocalVariables().end())&&(!found);++p){
      if(p->name==d.varName){
	found=true;
	d.varCategory = BoundsDescription::LocalVariable;
	d.varType     = this->getTypeFlag(p->type);
	d.arraySize   = p->arraySize;
      }
    }
    for(p   = md.getStateVariables().begin();
	(p != md.getStateVariables().end())&&(!found);++p){
      if(p->name==d.varName){
	found=true;
	d.varCategory = BoundsDescription::StateVariable;
	d.varType     = this->getTypeFlag(p->type);
	d.arraySize   = p->arraySize;
      }
    }
    for(p   = md.getAuxiliaryStateVariables().begin();
	(p != md.getAuxiliaryStateVariables().end())&&(!found);++p){
      if(p->name==d.varName){
	found=true;
	d.varCategory = BoundsDescription::StateVariable;
	d.varType     = this->getTypeFlag(p->type);
	d.arraySize   = p->arraySize;
      }
    }
    for(p   = md.getExternalStateVariables().begin();
	(p != md.getExternalStateVariables().end())&&(!found);++p){
      if(p->name==d.varName){
	found=true;
	d.varCategory = BoundsDescription::ExternalStateVariable;
	d.varType     = this->getTypeFlag(p->type);
	d.arraySize   = p->arraySize;
      }
    }
    if(!found){
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
			      this->current->value+" is not a valid identifier.");
    }
    if ((d.varType==SupportedTypes::TVector)||
	(d.varType==SupportedTypes::Stensor)||
	(d.varType==SupportedTypes::Tensor)){
      ++(this->current);
      this->readSpecifiedToken("MFrontBehaviourParserCommon::treatBounds : ","(");
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds : ");
      if(this->current->value!="*"){
	unsigned short component;
	istringstream converter(this->current->value);
	converter >> component;
	if(!converter&&(!converter.eof())){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				  "could not read component number for variable '"+d.varName+"'");
	}
	d.component=component;
	d.varType=Scalar;
	d.varName+="(";
	d.varName+=toString(component);
	d.varName+=")";
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds");
      if(this->current->value!=")"){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"Expected ')' (read '"+this->current->value+"')");
      }
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatBounds","in");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds");
    if(this->current->value=="]"){
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds");
      if(this->current->value!="*"){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"Expected '*' (read '"+this->current->value+"')");
      }
      d.boundsType = BoundsDescription::Upper;
    } else if(this->current->value=="["){
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds",
			      "expected to read lower bound value for variable "+d.varName);
      istringstream converter(this->current->value);
      converter >> d.lowerBound;
      d.boundsType = BoundsDescription::LowerAndUpper;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"could not read lower bound value for variable '"+d.varName+"'");
      }
    } else {
      this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds : ",
			      "Expected ']' or '[' (read '"+this->current->value+"')");
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatBounds",":");
    this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds",
			    "expected to read upper bound value for variable "+d.varName);
    if(this->current->value=="*"){
      if(d.boundsType==BoundsDescription::Upper){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"upper and lower values bounds are both infinity. This is inconsistent.");
      }
      d.boundsType=BoundsDescription::Lower;
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds",
			      "Expected '['.");
      if(this->current->value!="["){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"Expected '[' (read '"+this->current->value+"')");
      }
    } else {
      istringstream converter(this->current->value);
      converter >> d.upperBound;
      if(!converter&&(!converter.eof())){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"could not read upper bound value for variable '"+d.varName+"'");
      }
      if(d.boundsType==BoundsDescription::LowerAndUpper){
	if(d.lowerBound>d.upperBound){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				  "lower bound value is greater than upper bound value for variable '"+
				  d.varName+"'");
	}
      }
      ++(this->current);
      this->checkNotEndOfFile("MFrontBehaviourParserCommon::treatBounds",
			      "Expected ']'.");
      if(this->current->value!="]"){
	this->throwRuntimeError("MFrontBehaviourParserCommon::treatBounds",
				"Expected ']' (read '"+this->current->value+"')");
      }
    }
    ++(this->current);
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatBounds",";");
  } // end of MFrontBehaviourParserCommon::treatBounds

  void
  MFrontBehaviourParserCommon::registerDefaultVarNames(void)
  {
    using namespace std;
    using namespace tfel::material;
    // all available tangent operators for finite strain behaviours
    const vector<FiniteStrainBehaviourTangentOperatorBase::Flag> tos(getFiniteStrainBehaviourTangentOperatorFlags());
    vector<FiniteStrainBehaviourTangentOperatorBase::Flag>::const_iterator pt;
    this->registerVariable("D",false);
    this->registerVariable("Dt",false);
    this->registerVariable("T",false);
    this->registerVariable("dT",false);
    this->registerVariable("dt",false);
    this->reserveName("N",false);
    this->reserveName("Type",false);
    this->reserveName("use_qt",false);
    this->reserveName("src1",false);
    this->reserveName("src2",false);
    this->reserveName("policy_value",false);
    this->reserveName("integrate",false);
    this->reserveName("computeStress",false);
    this->reserveName("computeFinalStress",false);
    this->reserveName("computeStressFreeExpansion",false);
    this->reserveName("computeFdF",false);
    this->reserveName("updateIntegrationVariables",false);
    this->reserveName("updateStateVariables",false);
    this->reserveName("updateAuxiliaryStateVariables",false);
    this->reserveName("getTangentOperator",false);
    this->reserveName("getTimeStepScalingFactor",false);
    this->reserveName("computeConsistentTangentOperator",false);
    for(pt=tos.begin();pt!=tos.end();++pt){
      const string ktype=convertFiniteStrainBehaviourTangentOperatorFlagToString(*pt);
      this->reserveName(ktype,false);
      this->reserveName("computeConsistentTangentOperator_"+ktype,false);
      this->reserveName("tangentOperator_"+ktype,false);
    }
    this->reserveName("tangentOperator_sk2",false);
    this->reserveName("computePredictionOperator",false);
    this->reserveName("computeTangentOperator_",false);
    this->reserveName("smt",false);
    this->reserveName("smflag",false);
    this->reserveName("dl_l0",false);
    this->reserveName("dl_l1",false);
    this->reserveName("dl_l01",false);
    this->reserveName("alpha_Ti",false);
    this->reserveName("alpha0_Ti",false);
    this->reserveName("alpha1_Ti",false);
    this->reserveName("alpha2_Ti",false);
    this->reserveName("alpha_T_t",false);
    this->reserveName("alpha_T_t_dt",false);
    this->reserveName("alpha0_T_t",false);
    this->reserveName("alpha0_T_t_dt",false);
    this->reserveName("alpha1_T_t",false);
    this->reserveName("alpha1_T_t_dt",false);
    this->reserveName("alpha2_T_t",false);
    this->reserveName("alpha2_T_t_dt",false);
    this->reserveName("time",false);
    this->reserveName("frequency",false);
    this->reserveName("stress",false);
    this->reserveName("length",false);
    this->reserveName("strain",false);
    this->reserveName("strainrate",false);
    this->reserveName("stressrate",false);
    this->reserveName("temperature",false);
    this->reserveName("thermalexpansion",false);
    this->reserveName("massdensity",false);
    this->reserveName("TVector",false);
    this->reserveName("Stensor",false);
    this->reserveName("Stensor4",false);
    this->reserveName("FrequencyStensor",false);
    this->reserveName("ForceTVector",false);
    this->reserveName("StressTensor",false);
    this->reserveName("StressStensor",false);
    this->reserveName("StressRateStensor",false);
    this->reserveName("DisplacementTVector",false);
    this->reserveName("StrainStensor",false);
    this->reserveName("StrainRateStensor",false);
    this->reserveName("StiffnessTensor",false);
    this->reserveName("Tensor",false);
    this->reserveName("ThermalExpansionCoefficientTensor",false);
    this->reserveName("DeformationGradientTensor",false);
    this->reserveName("TangentOperator",false);
    this->reserveName("StressFreeExpansionType",false);
    this->reserveName("behaviourData",false);
    this->reserveName("time_scaling_factor",false);
  } // end of MFrontBehaviourParserCommon::registerDefaultVarNames

  MFrontBehaviourParserCommon::MFrontBehaviourParserCommon()
    : MFrontVirtualParser(),
      ParserBase(),
      useStateVarTimeDerivative(false),
      explicitlyDeclaredUsableInPurelyImplicitResolution(false)
  {
    // Register var names
    this->registerDefaultVarNames();
    // By default disable use of quantities
    this->mb.setUseQt(false);
    // By default, a behaviour can be used in a purely implicit resolution
    const Hypothesis h=ModellingHypothesis::UNDEFINEDHYPOTHESIS;
    this->mb.setUsableInPurelyImplicitResolution(h,true);
  } // end of MFrontBehaviourParserCommon::MFrontParserCommon

  void MFrontBehaviourParserCommon::writeIncludes(std::ofstream& file) {
    using namespace std;
    if((!file)||
       (!file.good())){
      string msg("MFrontBehaviourParserCommon::writeIncludes : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    const string& h = this->mb.getIncludes();
    if(!h.empty()){
      file << h << endl;
    }
  }

  void MFrontBehaviourParserCommon::writeNamespaceBegin(std::ofstream& file) {
    using namespace std;
    if((!file)||
       (!file.good())){
      string msg("MFrontBehaviourParserCommon::writeNamespaceBegin : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    file << "namespace tfel{" << endl;
    file << endl;
    file << "namespace material{" << endl;
    file << endl;
  }

  void MFrontBehaviourParserCommon::writeNamespaceEnd(std::ofstream& file) {
    using namespace std;
    if((!file)||
       (!file.good())){
      string msg("MFrontBehaviourParserCommon::writeNamespaceEnd : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    file << "} // end of namespace material" << endl;
    file << endl;
    file << "} // end of namespace tfel" << endl;
    file << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegerConstants(std::ofstream& file)
  {
    using namespace std;
    map<string,int>::const_iterator p;
    if((!file)||(!file.good())){
      string msg("MFrontBehaviourParserCommon::writeIntegerConstants : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    if(!this->integerConstants.empty()){
      file << endl;
    }
    for(p=this->integerConstants.begin();p!=this->integerConstants.end();++p){
      file << "static const int " << p->first << " = " << p->second << ";" << endl;
    }
  } // end of MFrontBehaviourParserCommon::writeIntegerConstants

  void MFrontBehaviourParserCommon::writeStandardTFELTypedefs(std::ofstream& file) 
  {
    using namespace std;
    if((!file)||(!file.good())){
      string msg("MFrontBehaviourParserCommon::writeStandardTFELTypedefs : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
    file << "typedef unsigned short ushort;" << endl;
    if(this->mb.useQt()){        
      file << "typedef tfel::config::Types<N,Type,use_qt> Types;" << endl;
    } else {
      file << "typedef tfel::config::Types<N,Type,false> Types;" << endl;
    }
    file << "typedef typename Types::real                              real;" << endl;
    file << "typedef typename Types::time                              time;" << endl;
    file << "typedef typename Types::length                            length;" << endl;
    file << "typedef typename Types::frequency                         frequency;" << endl;
    file << "typedef typename Types::stress                            stress;" << endl;
    file << "typedef typename Types::strain                            strain;" << endl;
    file << "typedef typename Types::strainrate                        strainrate;" << endl;
    file << "typedef typename Types::stressrate                        stressrate;" << endl;
    file << "typedef typename Types::temperature                       temperature;" << endl;
    file << "typedef typename Types::thermalexpansion                  thermalexpansion;" << endl;
    file << "typedef typename Types::massdensity                       massdensity;" << endl;
    file << "typedef typename Types::TVector                           TVector;" << endl;
    file << "typedef typename Types::Stensor                           Stensor;" << endl;
    file << "typedef typename Types::Stensor4                          Stensor4;" << endl;
    file << "typedef typename Types::FrequencyStensor                  FrequencyStensor;" << endl;
    file << "typedef typename Types::ForceTVector                      ForceTVector;" << endl;
    file << "typedef typename Types::StressStensor                     StressStensor;" << endl;
    file << "typedef typename Types::StressRateStensor                 StressRateStensor;" << endl;
    file << "typedef typename Types::DisplacementTVector               DisplacementTVector;" << endl;
    file << "typedef typename Types::StrainStensor                     StrainStensor;" << endl;
    file << "typedef typename Types::StrainRateStensor                 StrainRateStensor;" << endl;
    file << "typedef typename Types::StiffnessTensor                   StiffnessTensor;" << endl;
    file << "typedef typename Types::Tensor                            Tensor;" << endl;
    file << "typedef typename Types::StressTensor                      StressTensor;" << endl;
    file << "typedef typename Types::ThermalExpansionCoefficientTensor ThermalExpansionCoefficientTensor;" << endl;
    file << "typedef typename Types::DeformationGradientTensor         DeformationGradientTensor;" << endl;
    // tangent operator
    file << "typedef " << this->mb.getTangentOperatorType() << " TangentOperator;" << endl;
  } // end of MFrontBehaviourParserCommon::writeStandardTFELTypedefs

  MFrontBehaviourParserCommon::~MFrontBehaviourParserCommon()
  {
    this->behaviourFile.close();
    this->behaviourDataFile.close();
    this->integrationDataFile.close();
    this->srcFile.close();    
  } // end of MFrontBehaviourParserCommon::~MFrontParserCommon

  void MFrontBehaviourParserCommon::checkBehaviourDataFile() const {
    using namespace std;
    if((!this->behaviourDataFile)||
       (!this->behaviourDataFile.good())){
      string msg("MFrontBehaviourParserCommon::checkBehaviourDataOutputFile : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataFileHeader(){
    using namespace std;

    this->checkBehaviourDataFile();

    this->behaviourDataFile << "/*!" << endl;
    this->behaviourDataFile << "* \\file   " << this->behaviourDataFileName << endl;
    this->behaviourDataFile << "* \\brief  " << "this file implements the " 
			    << this->mb.getClassName() << "BehaviourData" << " class." << endl;
    this->behaviourDataFile << "*         File generated by ";
    this->behaviourDataFile << MFrontHeader::getVersionName() << " ";
    this->behaviourDataFile << "version " << MFrontHeader::getVersionNumber();
    this->behaviourDataFile << endl;
    if(!this->authorName.empty()){
      this->behaviourDataFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->behaviourDataFile << "* \\date   " << this->date       << endl;
    }
    this->behaviourDataFile << " */" << endl;
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataFileHeaderBegin(){
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "#ifndef _LIB_TFELMATERIAL_";
    this->behaviourDataFile << makeUpperCase(this->mb.getClassName());
    this->behaviourDataFile << "_BEHAVIOUR_DATA_HXX_" << endl;
    this->behaviourDataFile << "#define _LIB_TFELMATERIAL_";
    this->behaviourDataFile << makeUpperCase(this->mb.getClassName());
    this->behaviourDataFile << "_BEHAVIOUR_DATA_HXX_" << endl;
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataFileHeaderEnd()
  {
    using namespace std;

    this->checkBehaviourDataFile();

    this->behaviourDataFile << "#endif /* _LIB_TFELMATERIAL_";
    this->behaviourDataFile << makeUpperCase(this->mb.getClassName());
    this->behaviourDataFile << "_BEHAVIOUR_DATA_HXX_ */" << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataStandardTFELIncludes(void)
  {
    using namespace std;
    bool b1 = false;
    bool b2 = false;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "#include<string>" << endl;
    this->behaviourDataFile << "#include<iostream>" << endl;
    this->behaviourDataFile << "#include<limits>" << endl;
    this->behaviourDataFile << "#include<stdexcept>" << endl;
    this->behaviourDataFile << "#include<algorithm>" << endl << endl;
    this->behaviourDataFile << "#include\"TFEL/Config/TFELConfig.hxx\"" << endl;
    this->behaviourDataFile << "#include\"TFEL/Config/TFELTypes.hxx\""  << endl;
    this->behaviourDataFile << "#include\"TFEL/Metaprogramming/StaticAssert.hxx\"" << endl;
    this->behaviourDataFile << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"" << endl;
    this->behaviourDataFile << "#include\"TFEL/TypeTraits/IsReal.hxx\"" << endl;
    this->behaviourDataFile << "#include\"TFEL/Utilities/Name.hxx\"" << endl;
    if(this->mb.useQt()){
      this->behaviourDataFile << "#include\"TFEL/Math/General/BaseCast.hxx\"" << endl;
    }
    this->mb.requiresTVectorOrVectorIncludes(b1,b2);
    if(b1){
      this->behaviourDataFile << "#include\"TFEL/Math/tvector.hxx\"" << endl;
    }
    if(b2){
      this->behaviourDataFile << "#include\"TFEL/Math/vector.hxx\"" << endl;
    }
    this->behaviourDataFile << "#include\"TFEL/Math/stensor.hxx\"" << endl;
    this->behaviourDataFile << "#include\"TFEL/Math/st2tost2.hxx\"" << endl;
    if(this->mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      this->behaviourDataFile << "#include\"TFEL/Math/tensor.hxx\"" << endl;
      this->behaviourDataFile << "#include\"TFEL/Math/t2tot2.hxx\"" << endl;
      this->behaviourDataFile << "#include\"TFEL/Math/t2tost2.hxx\"" << endl;
      this->behaviourDataFile << "#include\"TFEL/Math/st2tot2.hxx\"" << endl;
      this->behaviourDataFile << "#include\"TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx\"" << endl;
    }
    this->behaviourDataFile << "#include\"TFEL/Material/ModellingHypothesis.hxx\"" << endl;
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataDefaultMembers(void)
  {
    using namespace std;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p3;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "protected: " << endl << endl;
    if(this->mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
      this->behaviourDataFile << "StiffnessTensor D;" << endl;
    }
    if(this->mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      this->behaviourDataFile << "ThermalExpansionCoefficientTensor A;" << endl;
    }
    for(p3=this->mb.getMainVariables().begin();p3!=this->mb.getMainVariables().end();++p3){
      if(p3->first.increment_known){
	this->behaviourDataFile << p3->first.type  << " " << p3->first.name << ";" << endl << endl;
      } else {
	this->behaviourDataFile << p3->first.type  << " " << p3->first.name << "0;" << endl << endl;
      }
      this->behaviourDataFile << p3->second.type << " " << p3->second.name << ";" << endl << endl;
    }
    this->behaviourDataFile << "temperature T;" << endl;
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataStandardTFELTypedefs(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "static const unsigned short TVectorSize = N;" << endl;
    this->behaviourDataFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;" << endl;
    this->behaviourDataFile << "static const unsigned short StensorSize = ";
    this->behaviourDataFile << "StensorDimeToSize::value;" << endl;
    this->behaviourDataFile << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;" << endl;
    this->behaviourDataFile << "static const unsigned short TensorSize = ";
    this->behaviourDataFile << "TensorDimeToSize::value;" << endl;
    this->behaviourDataFile << endl;
    this->writeStandardTFELTypedefs(this->behaviourDataFile);
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataGetName(void){    
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "namespace tfel{" << endl;
    this->behaviourDataFile << "namespace utilities{" << endl;
    this->behaviourDataFile << "//! Partial specialisation of the Name class" << endl;
    if(this->mb.useQt()){        
      this->behaviourDataFile << "template<tfel::material::ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
      this->behaviourDataFile << "struct Name<tfel::material::" << this->mb.getClassName() << "BehaviourData<hypothesis,Type,use_qt> >" << endl;
    } else {
      this->behaviourDataFile << "template<tfel::material::ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
      this->behaviourDataFile << "struct Name<tfel::material::" << this->mb.getClassName() << "BehaviourData<hypothesis,Type,false> >" << endl;
    }
    this->behaviourDataFile << "{" << endl;
    this->behaviourDataFile << "/*!" << endl;
    this->behaviourDataFile << "* \\brief  Return the name of the class." << endl;
    this->behaviourDataFile << "* \\return the name of the class." << endl;
    this->behaviourDataFile << "* \\see    Name." << endl;
    this->behaviourDataFile << "*/" << endl;
    this->behaviourDataFile << "static std::string" << endl;
    this->behaviourDataFile << "getName(void){" << endl;
    this->behaviourDataFile << "return std::string(\"";
    this->behaviourDataFile << this->mb.getClassName() << "BehaviourData\");" << endl;
    this->behaviourDataFile << "}" << endl << endl;
    this->behaviourDataFile << "}; // end of struct Name" << endl;
    this->behaviourDataFile << "} // end of namespace utilities" << endl;
    this->behaviourDataFile << "} // end of namespace tfel" << endl << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataDisabledConstructors(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataConstructors(const Hypothesis h)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p3;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    VariableDescriptionContainer::const_iterator p;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "/*!" << endl;
    this->behaviourDataFile << "* \\brief Default constructor" << endl;
    this->behaviourDataFile << "*/" << endl;
    this->behaviourDataFile << this->mb.getClassName() << "BehaviourData()" << endl;
    this->behaviourDataFile << "{}" << endl << endl;
    this->behaviourDataFile << "/*!" << endl;
    this->behaviourDataFile << "* \\brief Copy constructor" << endl;
    this->behaviourDataFile << "*/" << endl;
    this->behaviourDataFile << this->mb.getClassName() << "BehaviourData(const ";
    this->behaviourDataFile << this->mb.getClassName() << "BehaviourData& src)" << endl;
    this->behaviourDataFile << ": ";
    if(this->mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
      this->behaviourDataFile << "D(src.D)," << endl;
    }
    if(this->mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      this->behaviourDataFile << "A(src.A)," << endl;
    }
    for(p3=this->mb.getMainVariables().begin();p3!=this->mb.getMainVariables().end();++p3){
      if(p3->first.increment_known){
	this->behaviourDataFile << p3->first.name  << "(src." << p3->first.name << ")," << endl;
      } else {
	this->behaviourDataFile << p3->first.name  << "0(src." << p3->first.name << "0)," << endl;
      }
      this->behaviourDataFile << p3->second.name << "(src." << p3->second.name << ")," << endl;
    }
    this->behaviourDataFile << "T(src.T)";
    if(!md.getMaterialProperties().empty()){
      for(p=md.getMaterialProperties().begin();p!=md.getMaterialProperties().end();++p){
	this->behaviourDataFile << "," << endl;
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    if(!md.getStateVariables().empty()){
      for(p=md.getStateVariables().begin();p!=md.getStateVariables().end();++p){
	this->behaviourDataFile << "," << endl;
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    if(!md.getAuxiliaryStateVariables().empty()){
      for(p=md.getAuxiliaryStateVariables().begin();p!=md.getAuxiliaryStateVariables().end();++p){
	this->behaviourDataFile << "," << endl;
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    if(!md.getExternalStateVariables().empty()){
      for(p =md.getExternalStateVariables().begin();
	  p!=md.getExternalStateVariables().end();++p){
	this->behaviourDataFile << "," << endl;
	this->behaviourDataFile << p->name << "(src." << p->name << ")";  
      }
    }
    this->behaviourDataFile << endl;
    this->behaviourDataFile << "{}" << endl << endl << endl;
    // Creating constructor for external interfaces
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin(); i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      if(interface->isModellingHypothesisHandled(h,this->mb)){
	interface->writeBehaviourDataConstructor(this->behaviourDataFile,h,this->mb);
      }
    }
  } // end of MFrontBehaviourParserCommon::WriteBehaviourDataConstructors

  void MFrontBehaviourParserCommon::writeBehaviourDataAssignementOperator(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p3;
    VariableDescriptionContainer::const_iterator p;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "/*" << endl;
    this->behaviourDataFile << "* \\brief Assignement operator" << endl;
    this->behaviourDataFile << "*/" << endl;
    this->behaviourDataFile << this->mb.getClassName() << "BehaviourData&" << endl;
    this->behaviourDataFile << "operator=(const " 
			    << this->mb.getClassName() << "BehaviourData& src){" << endl;
    for(p3=this->mb.getMainVariables().begin();p3!=this->mb.getMainVariables().end();++p3){
      if(p3->first.increment_known){
	this->behaviourDataFile << "this->" << p3->first.name  << " = src." << p3->first.name << ";" << endl;
      } else {
	this->behaviourDataFile << "this->" << p3->first.name  << "0 = src." << p3->first.name << "0;" << endl;
      }
      this->behaviourDataFile << "this->" << p3->second.name << " = src." << p3->second.name << ";" << endl;
    }
    if(!md.getMaterialProperties().empty()){
      for(p=md.getMaterialProperties().begin();p!=md.getMaterialProperties().end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";" << endl;  
      }
    }
    if(!md.getStateVariables().empty()){
      for(p=md.getStateVariables().begin();p!=md.getStateVariables().end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";" << endl;  
      }
    }
    if(!md.getAuxiliaryStateVariables().empty()){
      for(p=md.getAuxiliaryStateVariables().begin();p!=md.getAuxiliaryStateVariables().end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";" << endl;  
      }
    }
    if(!md.getExternalStateVariables().empty()){
      for(p =md.getExternalStateVariables().begin();
	  p!=md.getExternalStateVariables().end();++p){
	this->behaviourDataFile << "this->" << p->name << " = src." << p->name << ";" << endl;  
      }
    }
    this->behaviourDataFile << "return *this;" << endl;
    this->behaviourDataFile << "}" << endl << endl << endl;
  } // end of MFrontBehaviourParserCommon::writeBehaviourAssignementOperator

  void MFrontBehaviourParserCommon::writeBehaviourDataExport(const Hypothesis h)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    this->checkBehaviourDataFile();
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->exportMechanicalData(this->behaviourDataFile,h,
				      this->mb);
    }
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataPublicMembers(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
    if(this->mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
      this->behaviourDataFile << "StiffnessTensor&" << endl;
      this->behaviourDataFile << "getStiffnessTensor(void)" << endl;
      this->behaviourDataFile << "{\nreturn this->D;\n}" << endl << endl;
      this->behaviourDataFile << "const StiffnessTensor&" << endl;
      this->behaviourDataFile << "getStiffnessTensor(void) const" << endl;
      this->behaviourDataFile << "{\nreturn this->D;\n}" << endl << endl;
    }
    if(this->mb.getAttribute(MechanicalBehaviourDescription::requiresThermalExpansionCoefficientTensor,false)){
      this->behaviourDataFile << "ThermalExpansionCoefficientTensor&" << endl;
      this->behaviourDataFile << "getThermalExpansionCoefficientTensor(void)" << endl;
      this->behaviourDataFile << "{\nreturn this->A;\n}" << endl << endl;
      this->behaviourDataFile << "const ThermalExpansionCoefficientTensor&" << endl;
      this->behaviourDataFile << "getThermalExpansionCoefficientTensor(void) const" << endl;
      this->behaviourDataFile << "{\nreturn this->A;\n}" << endl << endl;
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourDataPublicMembers

  void MFrontBehaviourParserCommon::writeBehaviourDataClassHeader(void) 
  {
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "/*!" << endl;
    this->behaviourDataFile << "* \\class " << this->mb.getClassName() << "BehaviourData"  << endl;
    this->behaviourDataFile << "* \\brief This class implements the " 
			    << this->mb.getClassName() << "BehaviourData" << " ." << endl;
    this->behaviourDataFile << "* \\param H, modelling hypothesis." << endl;
    this->behaviourDataFile << "* \\param typename Type, numerical type." << endl;
    this->behaviourDataFile << "* \\param bool use_qt, conditional saying if quantities are use." << endl;
    if(!this->authorName.empty()){
      this->behaviourDataFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->behaviourDataFile << "* \\date   " << this->date << endl;
    }
    this->behaviourDataFile << "*/" << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataForwardDeclarations(void)
  {
    using namespace std;
    this->checkBehaviourDataFile();
    this->behaviourDataFile << "// Forward Declaration" << endl;
    this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
    this->behaviourDataFile << "class " << this->mb.getClassName() << "BehaviourData;" << endl << endl;
    this->behaviourDataFile << "// Forward Declaration" << endl;
    this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
    this->behaviourDataFile << "class " << this->mb.getClassName() << "IntegrationData;" << endl << endl;
    if(this->mb.useQt()){
      this->behaviourDataFile << "// Forward Declaration" << endl;
      this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
      this->behaviourDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourDataFile << "const " << this->mb.getClassName() 
			      << "BehaviourData<hypothesis,Type,use_qt>&);" << endl << endl;
    } else {
      this->behaviourDataFile << "// Forward Declaration" << endl;
      this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
      this->behaviourDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourDataFile << "const " << this->mb.getClassName() 
			      << "BehaviourData<hypothesis,Type,false>&);" << endl << endl;
    }
    // maintenant, il faut déclarer toutes les spécialisations partielles...
    const set<Hypothesis>& h = this->mb.getModellingHypotheses();
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      if(this->mb.hasSpecialisedMechanicalData(*ph)){
	if(this->mb.useQt()){
	  this->behaviourDataFile << "// Forward Declaration" << endl;
	  this->behaviourDataFile << "template<typename Type,bool use_qt>" << endl;
	  this->behaviourDataFile << "std::ostream&\n operator <<(std::ostream&,";
	  this->behaviourDataFile << "const " << this->mb.getClassName() 
				  << "BehaviourData<ModellingHypothesis::"
				  << ModellingHypothesis::toUpperCaseString(*ph) << ",Type,use_qt>&);" << endl << endl;
	} else {
	  this->behaviourDataFile << "// Forward Declaration" << endl;
	  this->behaviourDataFile << "template<typename Type>" << endl;
	  this->behaviourDataFile << "std::ostream&\n operator <<(std::ostream&,";
	  this->behaviourDataFile << "const " << this->mb.getClassName() 
				  << "BehaviourData<ModellingHypothesis::"
				  << ModellingHypothesis::toUpperCaseString(*ph) << ",Type,false>&);" << endl << endl;
	}
      }
    }
  }
    
  void MFrontBehaviourParserCommon::writeBehaviourDataClassBegin(const Hypothesis h) {
    using namespace std;
    this->checkBehaviourDataFile();
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){
	this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
	this->behaviourDataFile << "class " << this->mb.getClassName() << "BehaviourData" << endl;
      } else {
	this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
	this->behaviourDataFile << "class " << this->mb.getClassName() 
				<< "BehaviourData<hypothesis,Type,false>" << endl;
      }
    } else {
      if(this->mb.useQt()){
	this->behaviourDataFile << "template<typename Type,bool use_qt>" << endl;
	this->behaviourDataFile << "class " << this->mb.getClassName()
				<< "BehaviourData<ModellingHypothesis::"
				<< ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>" << endl;
      } else {
	this->behaviourDataFile << "template<typename Type>" << endl;
	this->behaviourDataFile << "class " << this->mb.getClassName() 
				<< "BehaviourData<ModellingHypothesis::"
				<< ModellingHypothesis::toUpperCaseString(h) << ",Type,false>" << endl;
      }
    }
    this->behaviourDataFile << "{" << endl;
    this->behaviourDataFile << endl;
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->behaviourDataFile << "static const ModellingHypothesis::Hypothesis hypothesis = " 
			      << "ModellingHypothesis::"
			      << ModellingHypothesis::toUpperCaseString(h) << ";" << endl;
    }
    this->behaviourDataFile << "static const unsigned short N = ModellingHypothesisToSpaceDimension<hypothesis>::value;" << endl;
    this->behaviourDataFile << "TFEL_STATIC_ASSERT(N==1||N==2||N==3);" << endl;
    this->behaviourDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::"
			    << "IsFundamentalNumericType<Type>::cond);" << endl;
    this->behaviourDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<Type>::cond);" << endl << endl;
    this->behaviourDataFile << "friend std::ostream& operator<< <>(std::ostream&,const ";
    this->behaviourDataFile << this->mb.getClassName() << "BehaviourData&);" << endl << endl;
    this->behaviourDataFile << "/* integration data is declared friend to access"
			    << "   driving variables at the beginning of the time step */" << endl;
    if(this->mb.useQt()){
      this->behaviourDataFile << "friend class " << this->mb.getClassName() << "IntegrationData<hypothesis,Type,use_qt>;" << endl << endl;
    } else {
      this->behaviourDataFile << "friend class " << this->mb.getClassName() << "IntegrationData<hypothesis,Type,false>;" << endl << endl;
    }
    this->writeIntegerConstants(this->behaviourDataFile);    
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataClassEnd() {    
    using namespace std;

    this->checkBehaviourDataFile();

    this->behaviourDataFile << "}; // end of " << this->mb.getClassName() << "BehaviourData" 
			    << "class" << endl;
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataMaterialProperties(const Hypothesis h) {
    using namespace std;
    this->checkBehaviourDataFile();
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     this->mb.getMechanicalBehaviourData(h).getMaterialProperties(),
				     "","",this->fileName,
				     false);

    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataStateVariables(const Hypothesis h) {
    using namespace std;
    this->checkBehaviourDataFile();
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     d.getStateVariables(),
				     "","",this->fileName,false);
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     d.getAuxiliaryStateVariables(),
				     "","",this->fileName,false);
    this->writeVariablesDeclarations(this->behaviourDataFile,
				     d.getExternalStateVariables(),
				     "","",this->fileName,false);
    this->behaviourDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataOutputOperator(const Hypothesis h)
  {    
    using namespace std;
    this->checkBehaviourFile();
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p2;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){
	this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
	this->behaviourDataFile << "std::ostream&" << endl;
	this->behaviourDataFile << "operator <<(std::ostream& os,";
	this->behaviourDataFile << "const " << this->mb.getClassName() 
				<< "BehaviourData<hypothesis,Type,use_qt>& b)" << endl;
      } else {
	this->behaviourDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
	this->behaviourDataFile << "std::ostream&" << endl;
	this->behaviourDataFile << "operator <<(std::ostream& os,";
	this->behaviourDataFile << "const " << this->mb.getClassName() 
				<< "BehaviourData<hypothesis,Type,false>& b)" << endl;
      }
    } else {
      if(this->mb.useQt()){
	this->behaviourDataFile << "template<typename Type,bool use_qt>" << endl;
	this->behaviourDataFile << "std::ostream&" << endl;
	this->behaviourDataFile << "operator <<(std::ostream& os,";
	this->behaviourDataFile << "const " << this->mb.getClassName() 
				<< "BehaviourData<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>& b)" << endl;
      } else {
	this->behaviourDataFile << "template<typename Type>" << endl;
	this->behaviourDataFile << "std::ostream&" << endl;
	this->behaviourDataFile << "operator <<(std::ostream& os,";
	this->behaviourDataFile << "const " << this->mb.getClassName() 
				<< "BehaviourData<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>& b)" << endl;
      }
    }
    this->behaviourDataFile << "{" << endl;
    this->behaviourDataFile << "using namespace std;" << endl;
    this->behaviourDataFile << "using namespace tfel::utilities;" << endl;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){        
	this->behaviourDataFile << "os << Name<" << this->mb.getClassName() 
				<< "BehaviourData<hypothesis,Type,use_qt> >::getName() << endl;" << endl;
      } else {
	this->behaviourDataFile << "os << Name<" << this->mb.getClassName() 
				<< "BehaviourData<hypothesis,Type,false> >::getName() << endl;" << endl;
      }
    } else {
      if(this->mb.useQt()){        
	this->behaviourDataFile << "os << Name<" << this->mb.getClassName() 
				<< "BehaviourData<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h)
				<< ",Type,use_qt> >::getName() << endl;" << endl;
      } else {
	this->behaviourDataFile << "os << Name<" << this->mb.getClassName() 
				<< "BehaviourData<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h)
				<< ",Type,false> >::getName() << endl;" << endl;
      }
    }
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->behaviourDataFile << "os << \"" << p2->first.name  << " : \" << b." << p2->first.name  << " << endl;" << endl;
      } else {
	this->behaviourDataFile << "os << \"" << p2->first.name  << "0 : \" << b." << p2->first.name  << "0 << endl;" << endl;
      }
      this->behaviourDataFile << "os << \"" << p2->second.name << " : \" << b." << p2->second.name << " << endl;" << endl;
    }
    this->behaviourDataFile << "os << \"T : \" << b.T << endl;" << endl;
    for(p=d.getMaterialProperties().begin();p!=d.getMaterialProperties().end();++p){
      this->behaviourDataFile <<  "os << \"" << p->name << " : \" << b." 
			      << p->name <<  " << endl;" << endl;  
    }
    for(p=d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
      this->behaviourDataFile << "os << \"" << p->name << " : \" << b." 
			      << p->name <<  " << endl;" << endl;  
    }    
    for(p=d.getAuxiliaryStateVariables().begin();
	p!=d.getAuxiliaryStateVariables().end();++p){
      this->behaviourDataFile << "os << \"" << p->name << " : \" << b." 
			      << p->name <<  " << endl;" << endl;  
    }    
    for(p=d.getExternalStateVariables().begin();
	p!=d.getExternalStateVariables().end();++p){
      this->behaviourDataFile << "os << \"" << p->name << " : \" << b." 
			      << p->name << " << endl;" << endl;
    }
    this->behaviourDataFile << "return os;" << endl;
    this->behaviourDataFile << "}" << endl << endl;
  } //  MFrontBehaviourParserCommon::writeBehaviourDataOutputOperator

  void MFrontBehaviourParserCommon::writeBehaviourDataFileBegin(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    this->checkBehaviourDataFile();
    this->writeBehaviourDataFileHeader();
    this->writeBehaviourDataFileHeaderBegin();
    this->writeBehaviourDataStandardTFELIncludes();
    this->writeIncludes(this->behaviourDataFile);
    // includes specific to interfaces
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->writeInterfaceSpecificIncludes(this->behaviourDataFile,this->mb);
    }
    this->writeNamespaceBegin(this->behaviourDataFile);
    this->writeBehaviourDataForwardDeclarations();
  } // end of MFrontBehaviourParserCommon::writeBehaviourDataFile

  void MFrontBehaviourParserCommon::writeBehaviourDataClass(const Hypothesis h){
    using namespace std;
    this->checkBehaviourDataFile();
    this->writeBehaviourDataClassBegin(h);
    this->writeBehaviourDataStandardTFELTypedefs();
    this->writeBehaviourDataDefaultMembers();
    this->writeBehaviourDataMaterialProperties(h);
    this->writeBehaviourDataStateVariables(h);
    this->behaviourDataFile << "public:" << endl << endl;
    this->writeBehaviourDataDisabledConstructors();
    this->writeBehaviourDataConstructors(h);
    this->writeBehaviourDataMainVariablesSetters();
    this->writeBehaviourDataPublicMembers();
    this->writeBehaviourDataAssignementOperator(h);
    this->writeBehaviourDataExport(h);
    this->writeBehaviourDataClassEnd();
    this->writeBehaviourDataOutputOperator(h);
  }

  void MFrontBehaviourParserCommon::writeBehaviourDataFileEnd(void)
  {
    this->writeNamespaceEnd(this->behaviourDataFile);
    this->writeBehaviourDataGetName();
    this->writeBehaviourDataFileHeaderEnd();
  } // end of MFrontBehaviourParserCommon::writeBehaviourDataFileEnd

  void MFrontBehaviourParserCommon::checkBehaviourFile() const {
    using namespace std;
    if((!this->behaviourFile)||
       (!this->behaviourFile.good())){
      string msg("MFrontBehaviourParserCommon::checkBehaviourDataOutputFile : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
  }

  void MFrontBehaviourParserCommon::writeBehaviourForwardDeclarations(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "// Forward Declaration" << endl;
    this->behaviourFile << "template<ModellingHypothesis::Hypothesis,typename Type,bool use_qt>" << endl;
    this->behaviourFile << "class " << this->mb.getClassName() << ";" << endl << endl;
    if(this->mb.useQt()){
      this->behaviourFile << "// Forward Declaration" << endl;
      this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
      this->behaviourFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourFile << "const " << this->mb.getClassName() << "<hypothesis,Type,use_qt>&);" << endl << endl;
    } else {
      this->behaviourFile << "// Forward Declaration" << endl;
      this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
      this->behaviourFile << "std::ostream&\n operator <<(std::ostream&,";
      this->behaviourFile << "const " << this->mb.getClassName() << "<hypothesis,Type,false>&);" << endl << endl;
    }
    // maintenant, il faut déclarer toutes les spécialisations partielles...
    const set<Hypothesis>& h = this->mb.getModellingHypotheses();
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      if(this->mb.hasSpecialisedMechanicalData(*ph)){
	if(this->mb.useQt()){
	  this->behaviourFile << "// Forward Declaration" << endl;
	  this->behaviourFile << "template<typename Type,bool use_qt>" << endl;
	  this->behaviourFile << "std::ostream&\n operator <<(std::ostream&,";
	  this->behaviourFile << "const " << this->mb.getClassName() 
			      << "<ModellingHypothesis::"
			      << ModellingHypothesis::toUpperCaseString(*ph) << ",Type,use_qt>&);" << endl << endl;
	} else {
	  this->behaviourFile << "// Forward Declaration" << endl;
	  this->behaviourFile << "template<typename Type>" << endl;
	  this->behaviourFile << "std::ostream&\n operator <<(std::ostream&,";
	  this->behaviourFile << "const " << this->mb.getClassName() 
			      << "<ModellingHypothesis::"
			      << ModellingHypothesis::toUpperCaseString(*ph) << ",Type,false>&);" << endl << endl;
	}
      }
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourClassForwardDeclarations

  void MFrontBehaviourParserCommon::writeBehaviourClassBegin(const Hypothesis h) {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\class " << this->mb.getClassName()    << endl;
    this->behaviourFile << "* \\brief This class implements the " 
			<< this->mb.getClassName() << " behaviour." << endl;
    this->behaviourFile << "* \\param hypothesis, modelling hypothesis." << endl;
    this->behaviourFile << "* \\param Type, numerical type." << endl;
    if(this->mb.useQt()){    
      this->behaviourFile << "* \\param use_qt, conditional "
			  << "saying if quantities are use." << endl;
    }
    if(!this->authorName.empty()){
      this->behaviourFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->behaviourFile << "* \\date   " << this->date << endl;
    }
    if(!this->description.empty()){
      this->behaviourFile << this->description << endl;
    }
    this->behaviourFile << "*/" << endl;
    const string btype = this->mb.getBehaviourTypeFlag();
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
	this->behaviourFile << "class " << this->mb.getClassName() << endl;
	this->behaviourFile << ": public MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>," << endl;
	if(this->mb.getAttribute(MechanicalBehaviourData::profiling,false)){
	  this->behaviourFile << "public "
			      << this->mb.getClassName() << "Profiler," << endl;
	}
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "BehaviourData<hypothesis,Type,use_qt>," << endl;
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "IntegrationData<hypothesis,Type,use_qt>";
	this->writeBehaviourParserSpecificInheritanceRelationship();
      } else {
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
	this->behaviourFile << "class " << this->mb.getClassName() 
				<< "<hypothesis,Type,false>" << endl;
	this->behaviourFile << ": public MechanicalBehaviour<" << btype << ",hypothesis,Type,false>," << endl;
	if(this->mb.getAttribute(MechanicalBehaviourData::profiling,false)){
	  this->behaviourFile << "public "
			      << this->mb.getClassName() << "Profiler," << endl;
	}
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "BehaviourData<hypothesis,Type,false>," << endl;
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "IntegrationData<hypothesis,Type,false>";
	this->writeBehaviourParserSpecificInheritanceRelationship();
      }
    } else {
      if(this->mb.useQt()){
	this->behaviourFile << "template<typename Type,bool use_qt>" << endl;
	this->behaviourFile << "class " << this->mb.getClassName()
				<< "<ModellingHypothesis::"
				<< ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>" << endl;
	this->behaviourFile << ": public MechanicalBehaviour<" << btype << ",ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>," << endl;
	if(this->mb.getAttribute(MechanicalBehaviourData::profiling,false)){
	  this->behaviourFile << "public "
			      << this->mb.getClassName() << "Profiler," << endl;
	}
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "BehaviourData<ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>," << endl;
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "IntegrationData<ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>";
	this->writeBehaviourParserSpecificInheritanceRelationship();
      } else {
	this->behaviourFile << "template<typename Type>" << endl;
	this->behaviourFile << "class " << this->mb.getClassName() 
			    << "<ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>" << endl;
	this->behaviourFile << ": public MechanicalBehaviour<" << btype << ",ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>," << endl;
	if(this->mb.getAttribute(MechanicalBehaviourData::profiling,false)){
	  this->behaviourFile << "public "
			      << this->mb.getClassName() << "Profiler," << endl;
	}
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "BehaviourData<ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>," << endl;
	this->behaviourFile << "public "
			    << this->mb.getClassName() << "IntegrationData<ModellingHypothesis::"
			    << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>";
      }
    }
    this->behaviourFile << "{" << endl;
    this->behaviourFile << endl;
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->behaviourFile << "static const ModellingHypothesis::Hypothesis hypothesis = " 
			  << "ModellingHypothesis::"
			  << ModellingHypothesis::toUpperCaseString(h) << ";" << endl;
    }
    this->behaviourFile << "static const unsigned short N = ModellingHypothesisToSpaceDimension<hypothesis>::value;" << endl;
    this->behaviourFile << endl;
    this->behaviourFile << "TFEL_STATIC_ASSERT(N==1||N==2||N==3);" << endl;
    this->behaviourFile << "TFEL_STATIC_ASSERT(tfel::typetraits::"
			<< "IsFundamentalNumericType<Type>::cond);" << endl;
    this->behaviourFile << "TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<Type>::cond);" << endl << endl;
    this->behaviourFile << "friend std::ostream& operator<< <>(std::ostream&,const ";
    this->behaviourFile << this->mb.getClassName() << "&);" << endl << endl;
    this->writeIntegerConstants(this->behaviourFile);
  }

  void MFrontBehaviourParserCommon::writeBehaviourFileHeader(){
    using namespace std;

    this->checkBehaviourFile();

    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\file   " << this->behaviourFileName  << endl;
    this->behaviourFile << "* \\brief  " << "this file implements the " 
			<< this->mb.getClassName() << " Behaviour." << endl;
    this->behaviourFile << "*         File generated by ";
    this->behaviourFile << MFrontHeader::getVersionName() << " ";
    this->behaviourFile << "version " << MFrontHeader::getVersionNumber();
    this->behaviourFile << endl;
    if(!this->authorName.empty()){
      this->behaviourFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->behaviourFile << "* \\date   " << this->date       << endl;
    }
    this->behaviourFile << " */" << endl;
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourFileHeaderBegin(){
    using namespace std;
    this->checkBehaviourFile();

    this->behaviourFile << "#ifndef _LIB_TFELMATERIAL_";
    this->behaviourFile << makeUpperCase(this->mb.getClassName());
    this->behaviourFile << "_HXX_" << endl;
    this->behaviourFile << "#define _LIB_TFELMATERIAL_";
    this->behaviourFile << makeUpperCase(this->mb.getClassName());
    this->behaviourFile << "_HXX_" << endl;
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourFileHeaderEnd() {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "#endif /* _LIB_TFELMATERIAL_";
    this->behaviourFile << makeUpperCase(this->mb.getClassName());
    this->behaviourFile << "_HXX_ */" << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourClassEnd() {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "}; // end of " << this->mb.getClassName() 
			<< " class" << endl;
    this->behaviourFile << endl;
  }
  
  void
  MFrontBehaviourParserCommon::treatUpdateAuxiliaryStateVariables(void)
  {
    this->readCodeBlock(*this,MechanicalBehaviourData::UpdateAuxiliaryStateVariables,
			&MFrontBehaviourParserCommon::standardModifier,true,true);
  } // end of MFrontBehaviourParserCommon::treatUpdateAuxiliaryStateVarBase

  void
  MFrontBehaviourParserCommon::writeBehaviourUpdateIntegrationVariables(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief Update internal variables at end of integration" << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << "void" << endl;
    this->behaviourFile << "updateIntegrationVariables(void)";
    if(!d.getIntegrationVariables().empty()){
      this->behaviourFile << "{" << endl;
      for(p=d.getIntegrationVariables().begin();p!=d.getIntegrationVariables().end();++p){
	if(!d.isStateVariableName(p->name)){
	  if(d.isVariableUsedInCodeBlocks(p->name)){
	    this->behaviourFile << "this->"  << p->name << " += ";
	    this->behaviourFile << "this->d" << p->name << ";" << endl;
	  }
	}
      }
      this->behaviourFile << "}" << endl << endl;
    } else {
      this->behaviourFile << "\n{}" << endl << endl;
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourUpdateIntegrationVariables

  void
  MFrontBehaviourParserCommon::writeBehaviourUpdateStateVariables(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief Update internal variables at end of integration" << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << "void" << endl;
    this->behaviourFile << "updateStateVariables(void)";
    if(!d.getStateVariables().empty()){
      this->behaviourFile << "{" << endl;
      for(p=d.getStateVariables().begin();p!=d.getStateVariables().end();++p){
	this->behaviourFile << "this->"  << p->name << " += ";
	this->behaviourFile << "this->d" << p->name << ";" << endl;
      }
      this->behaviourFile << "}" << endl << endl;
    } else {
      this->behaviourFile << "\n{}" << endl << endl;
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourUpdateStateVariables

  void
  MFrontBehaviourParserCommon::writeBehaviourUpdateAuxiliaryStateVariables(const Hypothesis h) 
  {
    using namespace std;
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief Update auxiliary state variables at end of integration" << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << "void" << endl;
    this->behaviourFile << "updateAuxiliaryStateVariables(void)";
    if(this->mb.hasCode(h,MechanicalBehaviourData::UpdateAuxiliaryStateVariables)){
      this->behaviourFile << "{" << endl;
      this->behaviourFile << "using namespace std;" << endl;
      this->behaviourFile << "using namespace tfel::math;" << endl;
      writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourUpdateAuxiliaryStateVariables",
			this->behaviourFile,this->mb.getMaterialLaws());		      
      this->behaviourFile << this->mb.getCode(h,MechanicalBehaviourData::UpdateAuxiliaryStateVariables) << endl;
      this->behaviourFile << "}" << endl << endl;
    } else {
      this->behaviourFile << "\n{}" << endl << endl;
    }
  } // end of  MFrontBehaviourParserCommon::writeBehaviourUpdateAuxiliaryStateVariables

  bool
  MFrontBehaviourParserCommon::hasUserDefinedTangentOperatorCode(const Hypothesis h) const
  {
    using namespace std;
    using namespace tfel::material;
    if(this->mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      // all available tangent operators for finite strain behaviours
      const vector<FiniteStrainBehaviourTangentOperatorBase::Flag> tos(getFiniteStrainBehaviourTangentOperatorFlags());
      vector<FiniteStrainBehaviourTangentOperatorBase::Flag>::const_iterator pt;
      // search tangent operators defined by the user
      vector<FiniteStrainBehaviourTangentOperatorBase::Flag> ktos;
      for(pt=tos.begin();pt!=tos.end();++pt){
	const string ktype=convertFiniteStrainBehaviourTangentOperatorFlagToString(*pt);
	if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeTangentOperator+'-'+ktype)){
	  return true;
	}
      }
    } else {
      if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeTangentOperator)){
	return true;
      }
    }
    return false;
  } // end of MFrontBehaviourParserCommon::hasUserDefinedTangentOperatorCode

  void MFrontBehaviourParserCommon::writeBehaviourIntegrator(const Hypothesis h) {
    using namespace std;
    const string btype = this->mb.getBehaviourTypeFlag();
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    vector<BoundsDescription>::const_iterator p;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief Integrate behaviour  over the time step" << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << "IntegrationResult" << endl;
    this->behaviourFile << "integrate(const SMFlag smflag, const SMType smt){" << endl;
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::math;" << endl;
    writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourIntegrator",
		      this->behaviourFile,this->mb.getMaterialLaws());		      
    if((this->mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (this->mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL)){
      if(this->mb.useQt()){
	this->behaviourFile << "if(smflag!=MechanicalBehaviour<" << btype 
			    << ",hypothesis,Type,use_qt>::STANDARDTANGENTOPERATOR){" << endl
			    << "throw(runtime_error(\"invalid tangent operator flag\"));" << endl
			    << "}" << endl;
      } else {
	this->behaviourFile << "if(smflag!=MechanicalBehaviour<" << btype 
			    << ",hypothesis,Type,false>::STANDARDTANGENTOPERATOR){" << endl
			    << "throw(runtime_error(\"invalid tangent operator flag\"));" << endl
			    << "}" << endl;
      }
    }
    this->behaviourFile << "bool computeTangentOperator_ = smt!=NOSTIFFNESSREQUESTED;" << endl;
    if(this->mb.hasCode(h,MechanicalBehaviourData::Integrator)){
      this->behaviourFile << this->mb.getCode(h,MechanicalBehaviourData::Integrator) << endl;
    }
    this->behaviourFile << "this->updateIntegrationVariables();" << endl;
    this->behaviourFile << "this->updateStateVariables();" << endl;
    this->behaviourFile << "this->updateAuxiliaryStateVariables();" << endl;
    for(p  = md.getBounds().begin();p !=md.getBounds().end();++p){
      if(p->varCategory==BoundsDescription::StateVariable){
    	p->writeBoundsChecks(this->behaviourFile);
      }
    }
    if(this->hasUserDefinedTangentOperatorCode(h)){
      this->behaviourFile << "if(computeTangentOperator_){" << endl;
      if(this->mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
	this->behaviourFile << "if(!this->computeConsistentTangentOperator(smflag,smt)){\n";
      } else {
	this->behaviourFile << "if(!this->computeConsistentTangentOperator(smt)){\n";
      }
      if(this->mb.useQt()){        
	this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::FAILURE;\n";
      } else {
	this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::FAILURE;\n";
      }
      this->behaviourFile << "}\n";
      this->behaviourFile << "}\n";
    }
    if(this->mb.useQt()){        
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,use_qt>::SUCCESS;" << endl;
    } else {
      this->behaviourFile << "return MechanicalBehaviour<" << btype << ",hypothesis,Type,false>::SUCCESS;" << endl;
    }
    this->behaviourFile << "}" << endl << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDisabledConstructors(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief Default constructor (disabled)" << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << this->mb.getClassName() << "();" << endl << endl;
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief Copy constructor (disabled)" << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << this->mb.getClassName() << "(const ";
    this->behaviourFile << this->mb.getClassName()<< "&);" << endl << endl;
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief Assignement operator (disabled)" << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << this->mb.getClassName() << "& operator = (const ";
    this->behaviourFile << this->mb.getClassName()<< "&);" << endl << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourSetOutOfBoundsPolicy(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief set the policy for \"out of bounds\" conditions" << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << "void\nsetOutOfBoundsPolicy(const OutOfBoundsPolicy policy_value){" << endl;
    this->behaviourFile << "this->policy = policy_value;" << endl;
    this->behaviourFile << "} // end of setOutOfBoundsPolicy" << endl << endl;
  } // end of MFrontBehaviourParserCommon::writeBehaviourOutOfBoundsEnumeration(void)

  void MFrontBehaviourParserCommon::writeBehaviourCheckBounds(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    this->checkBehaviourFile();
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief check bounds" << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << "void\ncheckBounds(void) const{" << endl;
    vector<BoundsDescription>::const_iterator b;
    for(b  = md.getBounds().begin();
    	b != md.getBounds().end();++b){
      b->writeBoundsChecks(this->behaviourFile);
    }      
    this->behaviourFile << "} // end of checkBounds" << endl << endl;
  } // end of MFrontBehaviourParserCommon::writeBehaviourCheckBounds(void)

  std::string
  MFrontBehaviourParserCommon::getBehaviourConstructorsInitializers(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    // variable initialisation
    string init;
    init = this->getIntegrationVariablesIncrementsInitializers(md.getIntegrationVariables(),
							       this->useStateVarTimeDerivative);    
    if(!this->localVariablesInitializers.empty()){
      if(!init.empty()){
	init += ",\n";
      }
      init += this->localVariablesInitializers;
    }
    return init;
  } // end of MFrontBehaviourParserCommon::getBehaviourConstructorsInitializers

  void MFrontBehaviourParserCommon::writeBehaviourConstructors(const Hypothesis h)
  {    
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    this->checkBehaviourFile();
    // initializers
    const string& init = this->getBehaviourConstructorsInitializers(h);;
    // writing constructors
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief Constructor" << endl;
    this->behaviourFile << "*/" << endl;
    if(this->mb.useQt()){        
      this->behaviourFile << this->mb.getClassName() << "("
			  << "const " << this->mb.getClassName() 
			  << "BehaviourData<hypothesis,Type,use_qt>& src1," << endl
			  << "const " << this->mb.getClassName() 
			  << "IntegrationData<hypothesis,Type,use_qt>& src2)"
			  << endl;
      this->behaviourFile << ": " << this->mb.getClassName() 
			  << "BehaviourData<hypothesis,Type,use_qt>(src1)," << endl;
      this->behaviourFile << this->mb.getClassName() 
			  << "IntegrationData<hypothesis,Type,use_qt>(src2)";
    } else {
      this->behaviourFile << this->mb.getClassName() << "("
			  << "const " << this->mb.getClassName() 
			  << "BehaviourData<hypothesis,Type,false>& src1," << endl
			  << "const " << this->mb.getClassName() 
			  << "IntegrationData<hypothesis,Type,false>& src2)"
			  << endl; 
      this->behaviourFile << ": " << this->mb.getClassName() 
			  << "BehaviourData<hypothesis,Type,false>(src1)," << endl;
      this->behaviourFile << this->mb.getClassName() 
			  << "IntegrationData<hypothesis,Type,false>(src2)";
    }
    if(!init.empty()){
      this->behaviourFile << "," << endl <<init;
    }
    this->behaviourFile << "\n{" << endl;
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::math;" << endl;
    this->behaviourFile << "using std::vector;" << endl;
    writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourConstructors",
		      this->behaviourFile,this->mb.getMaterialLaws());
    this->writeBehaviourParameterInitialisation(h);
    this->writeBehaviourLocalVariablesInitialisation(h);
    this->behaviourFile << "}" << endl << endl;
    // constructor specific to interfaces
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      if(interface->isModellingHypothesisHandled(h,this->mb)){
	interface->writeBehaviourConstructor(this->behaviourFile,this->mb,init);
	this->behaviourFile << "\n{" << endl;
	this->behaviourFile << "using namespace std;" << endl;
	this->behaviourFile << "using namespace tfel::math;" << endl;
	this->behaviourFile << "using std::vector;" << endl;
	writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourConstructors",
			  this->behaviourFile,this->mb.getMaterialLaws());		      
	this->writeBehaviourParameterInitialisation(h);
	this->writeBehaviourLocalVariablesInitialisation(h);
	this->behaviourFile << "}" << endl << endl;
      }
    }
  }

  static void
  MFrontBehaviourParserCommonWriteThermalExpansionCoefficientComputation(std::ostream& out,
									 const MaterialPropertyDescription& a,
									 const std::string& T,
									 const std::string& i,
									 const std::string& suffix)
  {
    using namespace std;
    MFrontMFrontLawInterface minterface;
    const string& mname = minterface.getFunctionName(a.material,a.law);
    out << "const thermalexpansion alpha" << suffix;
    if(!i.empty()){
      out << "_" << i;
    }
    out <<  "  = " << mname << "(";
    if(!a.inputs.empty()){
      if(a.inputs.size()!=1u){
	string msg("MFrontBehaviourParserCommonWriteThermalExpansionCoefficientComputation : "
		   "a thermal expansion shall only depend on the temperature");
	throw(runtime_error(msg));
      }
      const VariableDescription& v = a.inputs.front();
      const string& in = v.getExternalName(a.glossaryNames,a.entryNames);
      if(in!="Temperature"){
	string msg("MFrontBehaviourParserCommonWriteThermalExpansionCoefficientComputation : "
		   "a thermal expansion shall only depend on the temperature");
	throw(runtime_error(msg));
      }
      out << T;
    }
    out << ");" << endl;
  } // end of MFrontBehaviourParserCommonWriteThermalExpansionCoefficientComputation

  static void
  MFrontBehaviourParserCommonWriteThermalExpansionCoefficientsComputations(std::ostream& out,
									   const MaterialPropertyDescription& a,
									   const std::string& suffix = "")
  {
    using namespace std;
    if(a.inputs.empty()){
      MFrontMFrontLawInterface minterface;
      const string& mname = minterface.getFunctionName(a.material,a.law);
      out << "const thermalexpansion alpha" << suffix << "_Ti        = " << mname << "();" << endl;
      out << "const thermalexpansion alpha" << suffix << "_T_t       = " << mname << "();" << endl;
      out << "const thermalexpansion alpha" << suffix << "_T_t_dt    = " << mname << "();" << endl;
    } else {
      MFrontBehaviourParserCommonWriteThermalExpansionCoefficientComputation(out,a,"this->referenceTemperatureForThermalExpansion",
									     "",suffix+"_Ti");
      MFrontBehaviourParserCommonWriteThermalExpansionCoefficientComputation(out,a,"this->T","t",suffix+"_T");
      MFrontBehaviourParserCommonWriteThermalExpansionCoefficientComputation(out,a,"this->T+this->dT",
									     "t_dt",suffix+"_T");
    }
  } // end of MFrontBehaviourParserCommonWriteThermalExpansionCoefficientComputation

  static void
  MFrontBehaviourParserCommonWriteThermalExpansionComputation(std::ostream& out,
							      const MaterialPropertyDescription& a,
							      const std::string& t,
							      const std::string& c,
							      const std::string& suffix = "")
  {
    using namespace std;
    string Tref;
    string T;
    if(a.staticVars.contains("ReferenceTemperature")){
      ostringstream Tref_value;
      Tref_value << a.staticVars.get("ReferenceTemperature").value;
      Tref = Tref_value.str();
    } else {
      Tref = "293.15";
    }
    if(t=="t"){
      out << "dl_l0";
      T = "this->T";
    } else {
      out << "dl_l1";
      T = "this->T+this->dT";
    }
    out << "[" << c << "] = 1/(1+alpha" << suffix << "_Ti * (this->referenceTemperatureForThermalExpansion-" << Tref << "))*("
	<< "alpha" << suffix << "_T_"  << t << " * (" << T << "-" << Tref << ")-"
	<< "alpha" << suffix << "_Ti * (this->referenceTemperatureForThermalExpansion-" << Tref << "));" << endl;
  }
  
  void MFrontBehaviourParserCommon::writeBehaviourComputeStressFreeExpansion(void)
  {    
    using namespace std;
    using tfel::utilities::shared_ptr;
    bool b = this->mb.areThermalExpansionCoefficientsDefined();
    if(b){
      if(!((this->mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	   (this->mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR))){
	this->throwRuntimeError("MFrontBehaviourParserCommon::writeBehaviourComputeStressFreeExpansion",
				"only finite strain or small strain behaviour are supported");
      }
      this->checkBehaviourFile();
      this->behaviourFile << "void" << endl
			  << "computeStressFreeExpansion(std::pair<StressFreeExpansionType,StressFreeExpansionType>& dl_l01)\n{" << endl;
      this->behaviourFile << "using namespace std;" << endl;
      this->behaviourFile << "using namespace tfel::math;" << endl;
      this->behaviourFile << "using std::vector;" << endl;
      writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourComputeStressFreeExpansion",
			this->behaviourFile,this->mb.getMaterialLaws());		      
      this->behaviourFile << "StressFreeExpansionType& dl_l0 = dl_l01.first;" << endl;
      this->behaviourFile << "StressFreeExpansionType& dl_l1 = dl_l01.second;" << endl;
      this->behaviourFile << "dl_l0 = StressFreeExpansionType(typename StressFreeExpansionType::value_type(0));" << endl;
      this->behaviourFile << "dl_l1 = StressFreeExpansionType(typename StressFreeExpansionType::value_type(0));" << endl;
      const vector<shared_ptr<MaterialPropertyDescription> >& acs =
	this->mb.getThermalExpansionCoefficients();
      if(this->mb.getSymmetryType()==mfront::ISOTROPIC){
	if(acs.size()!=1u){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::writeBehaviourComputeStressFreeExpansion",
				  "invalid number of thermal expansion coefficients");
	}
	MFrontBehaviourParserCommonWriteThermalExpansionCoefficientsComputations(this->behaviourFile,
										 *(acs.front()));
	MFrontBehaviourParserCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs.front()),"t","0");
	this->behaviourFile << "dl_l0[1] = dl_l0[0];" << endl
			    << "dl_l0[2] = dl_l0[0];" << endl;
	MFrontBehaviourParserCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs.front()),"t_dt","0");
	this->behaviourFile << "dl_l1[1] = dl_l1[0];" << endl
			    << "dl_l1[2] = dl_l1[0];" << endl;
      } else if(this->mb.getSymmetryType()==mfront::ORTHOTROPIC){
	if(acs.size()!=3u){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::writeBehaviourComputeStressFreeExpansion",
				  "invalid number of thermal expansion coefficients");
	}
	MFrontBehaviourParserCommonWriteThermalExpansionCoefficientsComputations(this->behaviourFile,
										 *(acs[0]),"0");
	MFrontBehaviourParserCommonWriteThermalExpansionCoefficientsComputations(this->behaviourFile,
										 *(acs[1]),"1");
	MFrontBehaviourParserCommonWriteThermalExpansionCoefficientsComputations(this->behaviourFile,
										 *(acs[2]),"2");
	MFrontBehaviourParserCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs[0]),"t","0","0");
	MFrontBehaviourParserCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs[0]),"t_dt","0","0");
	MFrontBehaviourParserCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs[1]),"t","1","1");
	MFrontBehaviourParserCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs[1]),"t_dt","1","1");
	MFrontBehaviourParserCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs[2]),"t","2","2");
	MFrontBehaviourParserCommonWriteThermalExpansionComputation(this->behaviourFile,*(acs[2]),"t_dt","2","2");
      } else {
	this->throwRuntimeError("MFrontBehaviourParserCommon::writeBehaviourComputeStressFreeExpansion",
				"unsupported behaviour symmetry");
      }
      this->behaviourFile << "}" << endl << endl;
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourComputeStressFreeExpansion

  void MFrontBehaviourParserCommon::writeBehaviourInitializeMethod(const Hypothesis h)
  {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!" << endl
			<< " * \\ brief initialize the behaviour with user code" << endl
			<< " */" << endl
			<< "void initialize(void){" << endl;
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::math;" << endl;
    this->behaviourFile << "using std::vector;" << endl;
    writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourInitializeMethod",
		      this->behaviourFile,this->mb.getMaterialLaws());		      
    if(this->mb.hasCode(h,MechanicalBehaviourData::BeforeInitializeLocalVariables)){
      if(this->mb.getAttribute(MechanicalBehaviourData::profiling,false)){
	writeStandardPerformanceProfilingBegin(this->behaviourFile,
					       MechanicalBehaviourData::BeforeInitializeLocalVariables,
					       "binit");
      }
      this->behaviourFile << this->mb.getCodeBlock(h,MechanicalBehaviourData::BeforeInitializeLocalVariables).code
			  << endl;
      if(this->mb.getAttribute(MechanicalBehaviourData::profiling,false)){
	writeStandardPerformanceProfilingEnd(this->behaviourFile);
      }
    }
    if(this->mb.hasCode(h,MechanicalBehaviourData::InitializeLocalVariables)){
      if(this->mb.getAttribute(MechanicalBehaviourData::profiling,false)){
	writeStandardPerformanceProfilingBegin(this->behaviourFile,
						     MechanicalBehaviourData::InitializeLocalVariables,"init");
      }
      this->behaviourFile << this->mb.getCodeBlock(h,MechanicalBehaviourData::InitializeLocalVariables).code
			  << endl;
      if(this->mb.getAttribute(MechanicalBehaviourData::profiling,false)){
	writeStandardPerformanceProfilingEnd(this->behaviourFile);
      }
    }
    if(this->mb.hasCode(h,MechanicalBehaviourData::AfterInitializeLocalVariables)){
      if(this->mb.getAttribute(MechanicalBehaviourData::profiling,false)){
	writeStandardPerformanceProfilingBegin(this->behaviourFile,
					       MechanicalBehaviourData::AfterInitializeLocalVariables,"ainit");
      }
      this->behaviourFile << this->mb.getCodeBlock(h,MechanicalBehaviourData::AfterInitializeLocalVariables).code
			  << endl;
      if(this->mb.getAttribute(MechanicalBehaviourData::profiling,false)){
	writeStandardPerformanceProfilingEnd(this->behaviourFile);
      }
    }
    if(this->mb.hasCode(h,MechanicalBehaviourData::ComputePredictor)){
      this->behaviourFile << this->mb.getCode(h,MechanicalBehaviourData::ComputePredictor) << endl;
    }
    this->writeBehaviourParserSpecificInitializeMethodPart(h);
    this->behaviourFile << "}" << endl << endl;
  } // end of void MFrontBehaviourParserCommon::writeBehaviourInitializeMethod

  void
  MFrontBehaviourParserCommon::writeBehaviourLocalVariablesInitialisation(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    this->checkBehaviourFile();
    for(p=md.getLocalVariables().begin();p!=md.getLocalVariables().end();++p){
      if(this->useDynamicallyAllocatedVector(p->arraySize)){
	this->behaviourFile << "this->" << p->name << ".resize(" << p->arraySize << ");" << endl;
      }
    }  
  }
  
  void MFrontBehaviourParserCommon::writeBehaviourParameterInitialisation(const Hypothesis h)
  {    
    using namespace std;
    this->checkBehaviourFile();
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& params = d.getParameters();
    VariableDescriptionContainer::const_iterator p;
    for(p=params.begin();p!=params.end();++p){
      if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	this->behaviourFile << "this->" << p->name << " = " << this->mb.getClassName() 
			    << "ParametersInitializer::get()." << p->name << ";" << endl;  
      } else {
	if(this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)){
	  this->behaviourFile << "this->" << p->name << " = " << this->mb.getClassName() 
			      << "ParametersInitializer::get()." << p->name << ";" << endl;  
	} else {
	  this->behaviourFile << "this->" << p->name << " = " << this->mb.getClassName() 
			      << ModellingHypothesis::toString(h) 
			      << "ParametersInitializer::get()." << p->name << ";" << endl;
	}
      }
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourParameterInitialisation

  void MFrontBehaviourParserCommon::writeBehaviourDataMainVariablesSetters(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    vector<string>::const_iterator i;
    this->checkBehaviourDataFile();
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->writeBehaviourDataMainVariablesSetters(this->behaviourDataFile,
							this->mb);
      this->behaviourDataFile << endl;
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourDataMainVariablesSetters

  void MFrontBehaviourParserCommon::writeIntegrationDataMainVariablesSetters(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    vector<string>::const_iterator i;
    this->checkIntegrationDataFile();
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->writeIntegrationDataMainVariablesSetters(this->integrationDataFile,
							  this->mb);
      this->integrationDataFile << endl;
    }
  } // end of MFrontBehaviourParserCommon::writeIntegrationDataMainVariablesSetters

  void MFrontBehaviourParserCommon::writeBehaviourGetModellingHypothesis()
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief set the policy for \"out of bounds\" conditions" << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << "ModellingHypothesis::Hypothesis\ngetModellingHypothesis(void) const{" << endl;
    this->behaviourFile << "return hypothesis;" << endl;
    this->behaviourFile << "} // end of getModellingHypothesis" << endl << endl;
  } // end of MFrontBehaviourParserCommon::writeBehaviourGetModellingHypothesis();

  void MFrontBehaviourParserCommon::writeBehaviourLocalVariables(const Hypothesis h)
  {    
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    this->checkBehaviourFile();
    this->writeVariablesDeclarations(this->behaviourFile,
				     md.getLocalVariables(),
				     "","",this->fileName,false);
    this->behaviourFile << endl;
  }

  void
  MFrontBehaviourParserCommon::writeBehaviourIntegrationVariables(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& v = md.getIntegrationVariables();
    this->checkBehaviourFile();
    VariableDescriptionContainer::const_iterator p;
    for(p=v.begin();p!=v.end();++p){
      if(!md.isStateVariableName(p->name)){
	if(md.isVariableUsedInCodeBlocks(p->name)){
	  this->writeVariableDeclaration(this->behaviourFile,
					 *p,"","",this->fileName,false);
	}
      }
    }
    this->behaviourFile << endl;
  } // end od MFrontBehaviourParserCommon::writeBehaviourIntegrationVariables

  void MFrontBehaviourParserCommon::writeBehaviourParameters(const Hypothesis h)
  {    
    using namespace std;
    this->checkBehaviourFile();
    const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& params = d.getParameters();
    VariableDescriptionContainer::const_iterator p;
    for(p=params.begin();p!=params.end();++p){
      if(!getDebugMode()){
	if(p->lineNumber!=0u){
	  this->behaviourFile << "#line " << p->lineNumber << " \"" 
			      << this->fileName << "\"" << endl;
	}
      }
      this->behaviourFile << p->type << " " << p->name << ";" << endl;  
    }
    this->behaviourFile << endl;
  }

  
  void MFrontBehaviourParserCommon::writeBehaviourPolicyVariable(void)
  {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "//! policy for treating out of bounds conditions" << endl;
    this->behaviourFile << "OutOfBoundsPolicy policy;" << endl;  
  } // end of MFrontBehaviourParserCommon::writeBehaviourPolicyVariable

  void MFrontBehaviourParserCommon::writeBehaviourStaticVariables(const Hypothesis h)
  {    
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    this->checkBehaviourFile();
    StaticVariableDescriptionContainer::const_iterator p;
    for(p=md.getStaticVariables().begin();p!=md.getStaticVariables().end();++p){
      if(!getDebugMode()){
	if(p->lineNumber!=0u){
	  this->behaviourFile << "#line " << p->lineNumber << " \"" 
			      << this->fileName << "\"" << endl;
	}
      }
      this->behaviourFile << "static " << p->type << " " << p->name << ";" << endl;  
    }
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourIntegrationVariablesIncrements(const Hypothesis h)
  {    
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    this->checkBehaviourFile();
    this->writeVariablesDeclarations(this->behaviourFile,
				     md.getIntegrationVariables(),
				     "d","",this->fileName,
				     this->useStateVarTimeDerivative);
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourOutputOperator(const Hypothesis h)
  {    
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p2;
    this->checkBehaviourFile();
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){        
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
	this->behaviourFile << "std::ostream&" << endl;
	this->behaviourFile << "operator <<(std::ostream& os,";
	this->behaviourFile << "const " << this->mb.getClassName() << "<hypothesis,Type,use_qt>& b)" << endl;
      } else {
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
	this->behaviourFile << "std::ostream&" << endl;
	this->behaviourFile << "operator <<(std::ostream& os,";
	this->behaviourFile << "const " << this->mb.getClassName() << "<hypothesis,Type,false>& b)" << endl;
      }
    } else {
      if(this->mb.useQt()){        
	this->behaviourFile << "template<typename Type,bool use_qt>" << endl;
	this->behaviourFile << "std::ostream&" << endl;
	this->behaviourFile << "operator <<(std::ostream& os,";
	this->behaviourFile << "const " << this->mb.getClassName()
			    << "<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>& b)" << endl;
      } else {
	this->behaviourFile << "template<typename Type>" << endl;
	this->behaviourFile << "std::ostream&" << endl;
	this->behaviourFile << "operator <<(std::ostream& os,";
	this->behaviourFile << "const " << this->mb.getClassName() 
			    << "<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>& b)" << endl;
      }
    }
    this->behaviourFile << "{" << endl;
    this->behaviourFile << "using namespace std;" << endl;
    this->behaviourFile << "using namespace tfel::utilities;" << endl;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){        
	this->behaviourFile << "os << Name<" << this->mb.getClassName() 
			    << "<hypothesis,Type,use_qt> >::getName() << endl;" << endl;
      } else {
	this->behaviourFile << "os << Name<" << this->mb.getClassName() 
			    << "<hypothesis,Type,false> >::getName() << endl;" << endl;
      }
    } else {
      if(this->mb.useQt()){        
	this->behaviourFile << "os << Name<" << this->mb.getClassName() 
			    << "<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt> >::getName() << endl;" << endl;
      } else {
	this->behaviourFile << "os << Name<" << this->mb.getClassName() 
			    << "<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,false> >::getName() << endl;" << endl;
      }
    }
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->behaviourFile << "os << \""  << p2->first.name << " : \" << b." << p2->first.name << " << endl;" << endl;
	this->behaviourFile << "os << \"d" << p2->first.name << " : \" << b.d" << p2->first.name << " << endl;" << endl;
      } else {
	this->behaviourFile << "os << \"" << p2->first.name << "0 : \" << b." << p2->first.name << "0 << endl;" << endl;
	this->behaviourFile << "os << \"" << p2->first.name << "1 : \" << b." << p2->first.name << "1 << endl;" << endl;
      }
      this->behaviourFile << "os << \"" << p2->second.name << " : \" << b." << p2->second.name << " << endl;" << endl;
    }
    this->behaviourFile << "os << \"dt : \" << b.dt << endl;" << endl;
    this->behaviourFile << "os << \"T : \" << b.T << endl;" << endl;
    this->behaviourFile << "os << \"dT : \" << b.dT << endl;" << endl;
    for(p=md.getMaterialProperties().begin();p!=md.getMaterialProperties().end();++p){
      this->behaviourFile <<  "os << \"" << p->name << " : \" << b." << p->name <<  " << endl;" << endl;  
    }
    for(p=md.getStateVariables().begin();
	p!=md.getStateVariables().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name <<  " << endl;" << endl;  
      this->behaviourFile << "os << \"d" << p->name << " : \" << b.d" 
			  << p->name <<  " << endl;" << endl;  
    }    
    for(p=md.getAuxiliaryStateVariables().begin();
	p!=md.getAuxiliaryStateVariables().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name <<  " << endl;" << endl;  
    }
    for(p=md.getExternalStateVariables().begin();
	p!=md.getExternalStateVariables().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name << " << endl;" << endl;
      this->behaviourFile << "os << \"d" << p->name << " : \" << b.d" 
			  << p->name << " << endl;" << endl;  
    }
    for(p=md.getLocalVariables().begin();p!=md.getLocalVariables().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name <<  " << endl;" << endl;  
    }
    for(p=md.getParameters().begin();p!=md.getParameters().end();++p){
      this->behaviourFile << "os << \"" << p->name << " : \" << b." 
			  << p->name << " << endl;" << endl;  
    }
    this->behaviourFile << "return os;" << endl;
    this->behaviourFile << "}" << endl << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourDestructor(void) 
  {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief Destructor" << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << "~" << this->mb.getClassName() << "()" << endl << "{}" << endl << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourUpdateExternalStateVariables(const Hypothesis h) 
  {    
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p2;
    this->checkBehaviourFile();
    this->behaviourFile << "void updateExternalStateVariables(void){" << endl;
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->behaviourFile << "this->" << p2->first.name  << "  += this->d" << p2->first.name << ";" << endl;
      } else {
	this->behaviourFile << "this->" << p2->first.name  << "0  = this->"  << p2->first.name << "1;" << endl;
      }
    }
    this->behaviourFile << "this->T   += this->dT;" << endl;
    for(p=md.getExternalStateVariables().begin();p!=md.getExternalStateVariables().end();++p){
      this->behaviourFile << "this->" << p->name << " += this->d" << p->name << ";" << endl;
    }
    this->behaviourFile << "}" << endl << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourIncludes(void) {    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "#include<string>" << endl;
    this->behaviourFile << "#include<iostream>" << endl;
    this->behaviourFile << "#include<limits>" << endl;
    this->behaviourFile << "#include<stdexcept>" << endl;
    this->behaviourFile << "#include<algorithm>" << endl << endl;
    this->behaviourFile << "#include\"TFEL/Config/TFELConfig.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/Config/TFELTypes.hxx\""  << endl;
    this->behaviourFile << "#include\"TFEL/Metaprogramming/StaticAssert.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/TypeTraits/IsReal.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/Material/MechanicalBehaviour.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/Material/MechanicalBehaviourTraits.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/Material/OutOfBoundsPolicy.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/Material/BoundsCheck.hxx\"" << endl;
    this->behaviourFile << "#include\"TFEL/Material/";
    this->behaviourFile << this->behaviourDataFileName << "\"" << endl;
    this->behaviourFile << "#include\"TFEL/Material/";
    this->behaviourFile << this->integrationDataFileName << "\"" << endl;
    if(this->mb.getAttribute<bool>(MechanicalBehaviourData::profiling,false)){
      this->behaviourFile << "#include\"MFront/MFrontBehaviourProfiler.hxx\"" << endl;
    }
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourMembersFunc(void) {    
    using namespace std;
    const string& m = this->mb.getMembers();
    this->checkBehaviourFile();
    if(!m.empty()){
      this->behaviourFile << m << endl << endl;
    }
  }

  void MFrontBehaviourParserCommon::writeBehaviourPrivate(void) {    
    using namespace std;
    const string& c = this->mb.getPrivateCode();
    this->checkBehaviourFile();
    if(!c.empty()){
      this->behaviourFile << c << endl << endl;
    }
  } // end of void MFrontBehaviourParserCommon::writeBehaviourPrivate

  void MFrontBehaviourParserCommon::writeBehaviourGetName(void){    
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "namespace tfel{" << endl;
    this->behaviourFile << "namespace utilities{" << endl;
    this->behaviourFile << "//! Partial specialisation of the Name class" << endl;
    if(this->mb.useQt()){        
      this->behaviourFile << "template<tfel::material::ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
      this->behaviourFile << "struct Name<tfel::material::" << this->mb.getClassName() << "<hypothesis,Type,use_qt> >" << endl;
    } else {
      this->behaviourFile << "template<tfel::material::ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
      this->behaviourFile << "struct Name<tfel::material::" << this->mb.getClassName() << "<hypothesis,Type,false> >" << endl;
    }
    this->behaviourFile << "{" << endl;
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* \\brief  Return the name of the class." << endl;
    this->behaviourFile << "* \\return the name of the class." << endl;
    this->behaviourFile << "* \\see    Name." << endl;
    this->behaviourFile << "*/" << endl;
    this->behaviourFile << "static std::string" << endl;
    this->behaviourFile << "getName(void){" << endl;
    this->behaviourFile << "return std::string(\"";
    this->behaviourFile << this->mb.getClassName() << "\");" << endl;
    this->behaviourFile << "}" << endl;
    this->behaviourFile << "}; // end of struct Name" << endl;
    this->behaviourFile << "} // end of namespace utilities" << endl;
    this->behaviourFile << "} // end of namespace tfel" << endl << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourStandardTFELTypedefs(void)
  {
    using namespace std;
    using namespace tfel::material;
    this->checkBehaviourFile();
    const string btype = this->mb.getBehaviourTypeFlag();
    this->behaviourFile << "static const unsigned short TVectorSize = N;" << endl;
    this->behaviourFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;" << endl;
    this->behaviourFile << "static const unsigned short StensorSize = ";
    this->behaviourFile << "StensorDimeToSize::value;" << endl;
    this->behaviourFile << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;" << endl;
    this->behaviourFile << "static const unsigned short TensorSize = ";
    this->behaviourFile << "TensorDimeToSize::value;" << endl;
    this->behaviourFile << endl;
    this->writeStandardTFELTypedefs(this->behaviourFile);
    this->behaviourFile << endl;
    this->behaviourFile << "public :" << endl << endl;
    const string qt = this->mb.useQt() ? "use_qt" : "false";
    this->behaviourFile << "typedef " << this->mb.getClassName() 
			<< "BehaviourData<hypothesis,Type," << qt << "> BehaviourData;" << endl;
    this->behaviourFile << "typedef " << this->mb.getClassName() 
			<< "IntegrationData<hypothesis,Type," << qt << "> IntegrationData;" << endl;
    this->behaviourFile << "typedef typename MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::SMFlag SMFlag;" << endl;
    this->behaviourFile << "typedef typename MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::SMType SMType;" << endl;
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::ELASTIC;" << endl;
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::SECANTOPERATOR;" << endl;
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::TANGENTOPERATOR;" << endl;
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::CONSISTENTTANGENTOPERATOR;" << endl;
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::NOSTIFFNESSREQUESTED;" << endl;
    if((this->mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
       (this->mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL)){
      this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::STANDARDTANGENTOPERATOR;" << endl;
    } else if (this->mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      const vector<FiniteStrainBehaviourTangentOperatorBase::Flag> toflags = getFiniteStrainBehaviourTangentOperatorFlags();
      for(vector<FiniteStrainBehaviourTangentOperatorBase::Flag>::const_iterator pf=toflags.begin();
	  pf!=toflags.end();++pf){
	this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::"
			    << convertFiniteStrainBehaviourTangentOperatorFlagToString(*pf) << ";" << endl;
      }
    }
    this->behaviourFile << "typedef typename MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::IntegrationResult IntegrationResult;" << endl << endl;
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::SUCCESS;" << endl;
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::FAILURE;" << endl;
    this->behaviourFile << "using MechanicalBehaviour<" << btype << ",hypothesis,Type," << qt << ">::UNRELIABLE_RESULTS;" << endl << endl;
    if(this->mb.areThermalExpansionCoefficientsDefined()){
      this->behaviourFile << "typedef " << this->mb.getStressFreeExpansionType()  << " StressFreeExpansionType;" << endl << endl;
    }
    this->behaviourFile << "private :" << endl << endl;
  } // end of MFrontBehaviourParserCommon::writeBehaviourStandardTFELTypedefs

  void MFrontBehaviourParserCommon::writeBehaviourTraits(void)
  {
    using namespace std;
    this->checkBehaviourFile();
    const vector<ModellingHypothesis::Hypothesis>& ah = ModellingHypothesis::getModellingHypotheses();
    // writing partial specialisations
    if(this->mb.getModellingHypotheses().size()>=4u){
      // on définit toutes les hypothèses par défaut
      this->writeBehaviourTraitsSpecialisation(ModellingHypothesis::UNDEFINEDHYPOTHESIS,true);
      // unsupported hypothesis
      vector<ModellingHypothesis::Hypothesis>::const_iterator p;
      for(p=ah.begin();p!=ah.end();++p){
    	if(this->mb.isModellingHypothesisSupported(*p)){
	  if(this->mb.hasSpecialisedMechanicalData(*p)){
	    this->writeBehaviourTraitsSpecialisation(*p,true);
	  }
    	} else {
    	  this->writeBehaviourTraitsSpecialisation(*p,false);
	}
      }
    } else {
      // on exclut toutes les hypothèses par défaut
      this->writeBehaviourTraitsSpecialisation(ModellingHypothesis::UNDEFINEDHYPOTHESIS,false);
      // unsupported hypothesis
      set<ModellingHypothesis::Hypothesis>::const_iterator p;
      for(p=this->mb.getModellingHypotheses().begin();p!=this->mb.getModellingHypotheses().end();++p){
    	this->writeBehaviourTraitsSpecialisation(*p,true);
      }
    }
  }

  void
  MFrontBehaviourParserCommon::writeBehaviourTraitsSpecialisation(const tfel::material::ModellingHypothesis::Hypothesis h,
								  const bool b)
  {
    using namespace std;
    SupportedTypes::TypeSize coefSize;
    SupportedTypes::TypeSize stateVarsSize;
    SupportedTypes::TypeSize externalStateVarsSize;
    if(b){
      VariableDescriptionContainer::const_iterator p;
      const MechanicalBehaviourData& d = this->mb.getMechanicalBehaviourData(h);
      for(p=d.getMaterialProperties().begin();p!=d.getMaterialProperties().end();++p){
	coefSize+=this->getTypeSize(p->type,p->arraySize);
      }
      for(p=d.getPersistentVariables().begin();p!=d.getPersistentVariables().end();++p){
	stateVarsSize+=this->getTypeSize(p->type,p->arraySize);
      }
      for(p  = d.getExternalStateVariables().begin();
	  p != d.getExternalStateVariables().end();++p){
	externalStateVarsSize+=this->getTypeSize(p->type,p->arraySize);
      }
    }
    this->behaviourFile << "/*!" << endl;
    this->behaviourFile << "* Partial specialisation for ";
    this->behaviourFile << this->mb.getClassName() << "." << endl;
    this->behaviourFile << "*/" << endl;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
	this->behaviourFile << "class MechanicalBehaviourTraits<";
	this->behaviourFile << this->mb.getClassName() << "<hypothesis,Type,use_qt> >" << endl;
      } else {
	this->behaviourFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
	this->behaviourFile << "class MechanicalBehaviourTraits<";
	this->behaviourFile << this->mb.getClassName() << "<hypothesis,Type,false> >" << endl;
      }
    } else {
      if(this->mb.useQt()){
	this->behaviourFile << "template<typename Type,bool use_qt>" << endl;
	this->behaviourFile << "class MechanicalBehaviourTraits<";
	this->behaviourFile << this->mb.getClassName() << "<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt> >" << endl;
      } else {
	this->behaviourFile << "template<typename Type>" << endl;
	this->behaviourFile << "class MechanicalBehaviourTraits<";
	this->behaviourFile << this->mb.getClassName() << "<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,false> >" << endl;
      }
    }
    this->behaviourFile << "{" << endl;
    if(b){
      if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	this->behaviourFile << "static const unsigned short N = ModellingHypothesisToSpaceDimension<hypothesis>::value;" << endl;
      } else {
	this->behaviourFile << "static const unsigned short N = ModellingHypothesisToSpaceDimension<"
			    << "ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ">::value;" << endl;
      }
      this->behaviourFile << "static const unsigned short TVectorSize = N;" << endl;
      this->behaviourFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;" << endl;
      this->behaviourFile << "static const unsigned short StensorSize = ";
      this->behaviourFile << "StensorDimeToSize::value;" << endl;
      this->behaviourFile << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;" << endl;
      this->behaviourFile << "static const unsigned short TensorSize = ";
      this->behaviourFile << "TensorDimeToSize::value;" << endl;
    }
    this->behaviourFile << "public:" << endl;
    if(b){
      this->behaviourFile << "static const bool is_defined = true;" << endl;
    } else {
      this->behaviourFile << "static const bool is_defined = false;" << endl;
    }
    if(this->mb.useQt()){
      this->behaviourFile << "static const bool use_quantities = use_qt;" << endl;
    } else {
      this->behaviourFile << "static const bool use_quantities = false;" << endl;
    }
    if(this->mb.areThermalExpansionCoefficientsDefined()){
      this->behaviourFile << "static const bool hasStressFreeExpansion = true;" << endl;    
    } else {
      this->behaviourFile << "static const bool hasStressFreeExpansion = false;" << endl;
    }
    if(this->mb.areThermalExpansionCoefficientsDefined()){
      this->behaviourFile << "static const bool handlesThermalExpansion = true;" << endl;    
    } else {
      this->behaviourFile << "static const bool handlesThermalExpansion = false;" << endl;
    }
    if(b){
      this->behaviourFile << "static const unsigned short dimension = N;" << endl;
    } else {
      this->behaviourFile << "static const unsigned short dimension = 0u;" << endl;
    }
    this->behaviourFile << "typedef Type NumType;" << endl;
    this->behaviourFile << "static const unsigned short material_properties_nb = ";
    this->behaviourFile << coefSize << ";" << endl;
    this->behaviourFile << "static const unsigned short internal_variables_nb  = ";
    this->behaviourFile << stateVarsSize << ";" << endl;
    this->behaviourFile << "static const unsigned short external_variables_nb  = ";
    this->behaviourFile << externalStateVarsSize << ";" << endl;
    this->behaviourFile << "static const bool hasConsistentTangentOperator = ";
    if(b){
      if(this->mb.getAttribute<bool>(h,MechanicalBehaviourData::hasConsistentTangentOperator,false)){
	this->behaviourFile << "true;" << endl;
      } else {
	this->behaviourFile << "false;" << endl;
      }
    } else {
      this->behaviourFile << "false;" << endl;
    }
    this->behaviourFile << "static const bool isConsistentTangentOperatorSymmetric = ";
    if(b){
      if(this->mb.getAttribute<bool>(h,MechanicalBehaviourData::isConsistentTangentOperatorSymmetric,false)){
	this->behaviourFile << "true;" << endl;
      } else {
	this->behaviourFile << "false;" << endl;
      }
    } else {
	this->behaviourFile << "false;" << endl;
    }
    this->behaviourFile << "static const bool hasPredictionOperator = ";
    if(b){
      if(this->mb.getAttribute<bool>(h,MechanicalBehaviourData::hasPredictionOperator,false)){
	this->behaviourFile << "true;" << endl;
      } else {
	this->behaviourFile << "false;" << endl;
      }
    } else {
      this->behaviourFile << "false;" << endl;
    }
    this->behaviourFile << "static const bool hasTimeStepScalingFactor = ";
    if(b){
      if(this->mb.getAttribute<bool>(h,MechanicalBehaviourData::hasTimeStepScalingFactor,false)){
	this->behaviourFile << "true;" << endl;
      } else {
	this->behaviourFile << "false;" << endl;
      }
    } else {
      this->behaviourFile << "false;" << endl;
    }
    this->behaviourFile << "};" << endl << endl;
  }

  void 
  MFrontBehaviourParserCommon::writeBehaviourParserSpecificInheritanceRelationship(void)
  {
    using namespace std;
    this->behaviourFile << endl;
  }

  void MFrontBehaviourParserCommon::writeBehaviourParserSpecificTypedefs(void)
  {
    // Empty member meant to be overriden in Child if necessary
  }

  void MFrontBehaviourParserCommon::writeBehaviourParserSpecificMembers(const Hypothesis)
  {
    // Empty member meant to be overriden in Child if necessary
  }

  void MFrontBehaviourParserCommon::writeBehaviourParserSpecificIncludes(void)
  {
    // Empty member meant to be overriden in Child if necessary
  }

  void MFrontBehaviourParserCommon::writeBehaviourParametersInitializers(void)
  {
    using namespace std;
    if(!this->mb.hasParameters()){
      return;
    }
    set<Hypothesis> h = this->mb.getDistinctModellingHypotheses();
    h.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    set<Hypothesis>::const_iterator p;
    for(p=h.begin();p!=h.end();++p){
      if(this->mb.hasParameters(*p)){
	this->writeBehaviourParametersInitializer(*p);
      }
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourParametersInitializers

  void 
  MFrontBehaviourParserCommon::writeBehaviourParametersInitializer(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    const VariableDescriptionContainer& params = md.getParameters();
    string cname(this->mb.getClassName());
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      cname += ModellingHypothesis::toString(h);
    }
    cname += "ParametersInitializer";
    VariableDescriptionContainer::const_iterator p;
    bool rp = false;
    bool ip = false;
    bool up = false;
    bool rp2 = false;
    bool ip2 = false;
    bool up2 = false;
    this->checkBehaviourFile();
    this->behaviourFile << "struct " << cname << endl
			<< "{" << endl
			<< "static " << cname << "&" << endl
			<< "get();" << endl << endl;
    for(p=params.begin();p!=params.end();++p){
      if(p->type=="real"){
	rp = true;
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  rp2 = true;
	  this->behaviourFile << "double " << p->name << ";" << endl; 
	}
      } else if(p->type=="int"){
	ip = true;
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  ip2 = true;
	  this->behaviourFile << "double " << p->name << ";" << endl; 
	}
      } else 	if(p->type=="ushort"){
	up = true;
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  up2 = true;
	  this->behaviourFile << "unsigned short " << p->name << ";" << endl; 
	}
      } else {
	string msg("MFrontBehaviourParserCommon::writeBehaviourParametersInitializer : ");
	msg += "invalid type for parameter '"+p->name+"' ('"+p->type+"')";
	throw(runtime_error(msg));
      }
    }
    if(!params.empty()){
      this->behaviourFile << endl; 
    }
    if(rp){
      this->behaviourFile << "void set(const char* const,const double);" << endl << endl;
    }
    if(ip){
      this->behaviourFile << "void set(const char* const,const int);" << endl << endl;
    }
    if(up){
      this->behaviourFile << "void set(const char* const,const unsigned short);" << endl << endl;
    }
    if(rp2){
      this->behaviourFile << "/*!" << endl
			  << " * \\brief convert a string to double" << endl
			  << " * \\param[in] p : parameter" << endl
			  << " * \\param[in] v : value" << endl
			  << " */" << endl
			  << "static double getDouble(const std::string&,const std::string&);" << endl;
    }
    if(ip2){
      this->behaviourFile << "/*!" << endl
			  << " * \\brief convert a string to int" << endl
			  << " * \\param[in] p : parameter" << endl
			  << " * \\param[in] v : value" << endl
			  << " */" << endl
			  << "static int getInt(const std::string&,const std::string&);" << endl;
    }
    if(up2){
      this->behaviourFile << "/*!" << endl
			  << " * \\brief convert a string to unsigned short" << endl
			  << " * \\param[in] p : parameter" << endl
			  << " * \\param[in] v : value" << endl
			  << " */" << endl
			  << "static unsigned short getUnsignedShort(const std::string&,const std::string&);" << endl;
    }
    this->behaviourFile << "private :" << endl << endl
			<< cname << "();" << endl << endl
			<< cname << "(const " << cname << "&);" << endl << endl
			<< cname << "&" << endl
			<< "operator=(const " << cname << "&);" << endl
			<< "/*!" << endl
			<< " * \\brief read the parameters from the given file" << endl
			<< " * \\param[in] fn : file name" << endl
			<< " */" << endl
			<< "void readParameters(const char* const);" << endl;
    this->behaviourFile << "};" << endl << endl;
  } // end of MFrontBehaviourParserCommon::writeBehaviourParametersInitializer

  void MFrontBehaviourParserCommon::writeBehaviourParserSpecificInitializeMethodPart(const Hypothesis)
  {
    // Empty member meant to be overriden in Child if necessary
  }

  void
  MFrontBehaviourParserCommon::writeBehaviourFileBegin(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    this->checkBehaviourFile();
    this->writeBehaviourFileHeader();
    this->writeBehaviourFileHeaderBegin();
    this->writeBehaviourIncludes();
    this->writeBehaviourParserSpecificIncludes();
    this->writeIncludes(this->behaviourFile);
    // includes specific to interfaces
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->writeInterfaceSpecificIncludes(this->behaviourFile,this->mb);
    }
    this->writeNamespaceBegin(this->behaviourFile);
    this->writeBehaviourParametersInitializers();
    this->writeBehaviourForwardDeclarations();
    this->writeBehaviourProfiler();
  } // end of MFrontBehaviourParserCommon::writeBehaviourFileBegin

  void MFrontBehaviourParserCommon::writeBehaviourProfiler()
  {
    using namespace std;
    if(this->mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      this->checkBehaviourFile();
      this->behaviourFile << "/*!" << endl
			  << " * " << this->mb.getClassName() << " profiler" << endl
			  << " */" << endl
			  << "struct " << this->mb.getClassName() << "Profiler" << endl
			  << "{" << endl
			  << "//! return the profiler associated with the behaviour" << endl
			  << "static mfront::MFrontBehaviourProfiler&" << endl
			  << "getProfiler(void);" << endl
			  << "}; // end of struct " << this->mb.getClassName() << "Profiler"
			  << endl << endl;
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourProfiler

  void MFrontBehaviourParserCommon::writeBehaviourClass(const Hypothesis h)
  {
    using namespace std;
    this->checkBehaviourFile();
    this->writeBehaviourClassBegin(h);
    this->writeBehaviourStandardTFELTypedefs();
    this->writeBehaviourParserSpecificTypedefs();
    this->writeBehaviourStaticVariables(h);
    this->writeBehaviourIntegrationVariables(h);
    this->writeBehaviourIntegrationVariablesIncrements(h);
    this->writeBehaviourLocalVariables(h);
    this->writeBehaviourParameters(h);
    this->writeBehaviourParserSpecificMembers(h);
    this->writeBehaviourUpdateIntegrationVariables(h);
    this->writeBehaviourUpdateStateVariables(h);
    this->writeBehaviourUpdateAuxiliaryStateVariables(h);
    this->writeBehaviourMembersFunc();
    this->writeBehaviourPrivate();
    this->writeBehaviourDisabledConstructors();
    // from this point, all is public
    this->behaviourFile << "public:" << endl << endl;
    this->writeBehaviourConstructors(h);
    this->writeBehaviourComputeStressFreeExpansion();
    this->writeBehaviourInitializeMethod(h);
    this->writeBehaviourSetOutOfBoundsPolicy();
    this->writeBehaviourGetModellingHypothesis();
    this->writeBehaviourCheckBounds(h);
    this->writeBehaviourComputePredictionOperator(h);
    this->writeBehaviourIntegrator(h);
    this->writeBehaviourComputeTangentOperator(h);
    this->writeBehaviourGetTangentOperator();
    this->writeBehaviourGetTimeStepScalingFactor();
    this->writeBehaviourUpdateExternalStateVariables(h);
    this->writeBehaviourDestructor();
    this->checkBehaviourFile();
    this->behaviourFile << "private:" << endl << endl;
    this->writeBehaviourTangentOperator();
    this->writeBehaviourPolicyVariable();
    this->writeBehaviourClassEnd();
    this->writeBehaviourOutputOperator(h);
  }

  void
  MFrontBehaviourParserCommon::writeBehaviourFileEnd(void)
  {
    this->checkBehaviourFile();
    this->writeBehaviourTraits();
    this->writeNamespaceEnd(this->behaviourFile);
    this->writeBehaviourGetName();
    this->writeBehaviourFileHeaderEnd();
  } // end of MFrontBehaviourParserCommon::writeBehaviourFileBegin

  void MFrontBehaviourParserCommon::writeBehaviourComputePredictionOperator(const Hypothesis h)
  {
    using namespace std;
    const string btype = this->mb.getBehaviourTypeFlag();
    if((!this->mb.getAttribute<bool>(h,MechanicalBehaviourData::hasPredictionOperator,false))&&
       (this->mb.hasCode(h,MechanicalBehaviourData::ComputePredictionOperator))){
      this->throwRuntimeError("MFrontBehaviourParserCommon::writeBehaviourComputePredictionOperator : ",
			      "attribute 'hasPredictionOperator' is set but no associated code defined");
    }
    if(this->mb.hasCode(h,MechanicalBehaviourData::ComputePredictionOperator)){
      this->behaviourFile << "IntegrationResult" << endl
			  << "computePredictionOperator(const SMFlag smflag,const SMType smt){" << endl;
      this->behaviourFile << "using namespace std;" << endl;
      this->behaviourFile << "using namespace tfel::math;" << endl;
      writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourComputePredictionOperator",
			this->behaviourFile,this->mb.getMaterialLaws());
      if((this->mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR)||
	 (this->mb.getBehaviourType()==MechanicalBehaviourDescription::COHESIVEZONEMODEL)){
	if(mb.useQt()){
	  this->behaviourFile << "if(smflag!=MechanicalBehaviour<" << btype 
			      << ",hypothesis,Type,use_qt>::STANDARDTANGENTOPERATOR){" << endl
			      << "throw(runtime_error(\"invalid tangent operator flag\"));" << endl
			      << "}" << endl;
	} else {
	  this->behaviourFile << "if(smflag!=MechanicalBehaviour<" << btype 
			      << ",hypothesis,Type,false>::STANDARDTANGENTOPERATOR){" << endl
			      << "throw(runtime_error(\"invalid tangent operator flag\"));" << endl
			      << "}" << endl;
	}
      }
      this->behaviourFile << this->mb.getCode(h,MechanicalBehaviourData::ComputePredictionOperator);
      this->behaviourFile << "return SUCCESS;" << endl;
      this->behaviourFile << "}" << endl << endl;
    } else {
      this->behaviourFile << "IntegrationResult computePredictionOperator(const SMFlag,const SMType){" << endl;
      this->behaviourFile << "using namespace std;" << endl;
      this->behaviourFile << "string msg(\"" << this->mb.getClassName() << "::computePredictionOperator : \");" << endl;
      this->behaviourFile << "msg +=\"unimplemented feature\";" << endl;
      this->behaviourFile << "throw(runtime_error(msg));" << endl;
      this->behaviourFile << "return FAILURE;" << endl;
      this->behaviourFile << "}" << endl << endl;
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourComputePredictionOperator(void)

  void MFrontBehaviourParserCommon::writeBehaviourComputeTangentOperator(const Hypothesis h)
  {
    using namespace std;
    using namespace tfel::material;
    if(this->mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      // all available tangent operators for finite strain behaviours
      const vector<FiniteStrainBehaviourTangentOperatorBase::Flag> tos(getFiniteStrainBehaviourTangentOperatorFlags());
      vector<FiniteStrainBehaviourTangentOperatorBase::Flag>::const_iterator pt;
      // all known converters
      const vector<FiniteStrainBehaviourTangentOperatorConversion> converters =
	FiniteStrainBehaviourTangentOperatorConversion::getAvailableFiniteStrainBehaviourTangentOperatorConversions();
      // tangent operators defined by the user
      vector<FiniteStrainBehaviourTangentOperatorBase::Flag> ktos;
      for(pt=tos.begin();pt!=tos.end();++pt){
	const string ktype=convertFiniteStrainBehaviourTangentOperatorFlagToString(*pt);
	if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeTangentOperator+'-'+ktype)){
	  ktos.push_back(*pt);
	}
      }
      if(!ktos.empty()){
	// computing all the conversion paths starting from user defined ones
	vector<FiniteStrainBehaviourTangentOperatorConversionPath> paths;
	vector<FiniteStrainBehaviourTangentOperatorBase::Flag>::const_iterator pk;
	for(pk=ktos.begin();pk!=ktos.end();++pk){
	  const vector<FiniteStrainBehaviourTangentOperatorConversionPath> kpaths = 
	    FiniteStrainBehaviourTangentOperatorConversionPath::getConversionsPath(*pk,ktos,converters);
	  paths.insert(paths.end(),kpaths.begin(),kpaths.end());
	}
	for(pt=tos.begin();pt!=tos.end();++pt){
	  const string ktype=convertFiniteStrainBehaviourTangentOperatorFlagToString(*pt);
	  if(find(ktos.begin(),ktos.end(),*pt)!=ktos.end()){
	    this->behaviourFile << "bool computeConsistentTangentOperator_" << ktype << "(const SMType smt){" << endl;
	    this->behaviourFile << "using namespace std;" << endl;
	    this->behaviourFile << "using namespace tfel::math;" << endl;
	    this->behaviourFile << "using std::vector;" << endl;
	    writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourComputeTangentOperator",
			      this->behaviourFile,this->mb.getMaterialLaws());
	    this->behaviourFile << this->mb.getCode(h,MechanicalBehaviourData::ComputeTangentOperator+"-"+ktype) << '\n';
	    this->behaviourFile << "return true;" << endl;
	    this->behaviourFile << "}" << endl << endl;
	  } else {
	    const FiniteStrainBehaviourTangentOperatorConversionPath path =
	      FiniteStrainBehaviourTangentOperatorConversionPath::getShortestPath(paths,*pt);
	    if(path.empty()){
	      this->behaviourFile << "bool computeConsistentTangentOperator_" << ktype << "(const SMType){" << endl;
	      this->behaviourFile << "using namespace std;" << endl;
	      this->behaviourFile << "string msg(\"" << this->mb.getClassName()
				  << "::computeConsistentTangentOperator_" << ktype << " : \");" << endl
				  << "msg += \"computing the tangent operator '"
				  << ktype << "' is not supported\";" << endl
				  << "throw(runtime_error(msg));" << endl;
	      this->behaviourFile << "return false;" << endl;
	      this->behaviourFile << "}" << endl << endl;
	    } else {
	      this->behaviourFile << "bool computeConsistentTangentOperator_" << ktype << "(const SMType smt){" << endl;
	      FiniteStrainBehaviourTangentOperatorConversionPath::const_iterator pc = path.begin();
	      this->behaviourFile << "using namespace tfel::math;" << endl;
	      this->behaviourFile << "// computing " << convertFiniteStrainBehaviourTangentOperatorFlagToString(pc->from()) << endl;
	      const string k = convertFiniteStrainBehaviourTangentOperatorFlagToString(pc->from());
	      this->behaviourFile << "this->computeConsistentTangentOperator_" << k << "(smt);" << endl;
	      this->behaviourFile << "const " << getFiniteStrainBehaviourTangentOperatorFlagType(pc->from()) << "<N,stress>"
				  << " tangentOperator_" << convertFiniteStrainBehaviourTangentOperatorFlagToString(pc->from())
				  << " = this->Dt.template get<"
				  << getFiniteStrainBehaviourTangentOperatorFlagType(pc->from())
				  << "<N,stress> >();" << endl;
	      for(;pc!=path.end();){
		const FiniteStrainBehaviourTangentOperatorConversion converter = *pc;
		if(++pc==path.end()){
		  this->behaviourFile << converter.getFinalConversion() << endl;
		} else {
		  this->behaviourFile << converter.getIntermediateConversion() << endl;
		}
	      }
	      this->behaviourFile << "return true;" << endl;
	      this->behaviourFile << "}" << endl << endl;
	    }
	  }
	}
	this->behaviourFile << "bool computeConsistentTangentOperator(const SMFlag& smflag,const SMType smt){" << endl;
	this->behaviourFile << "using namespace std;" << endl;
	this->behaviourFile << "switch(smflag){" << endl;
	for(pt=tos.begin();pt!=tos.end();++pt){
	  const string ktype=convertFiniteStrainBehaviourTangentOperatorFlagToString(*pt);
	  this->behaviourFile << "case " << ktype << ":" << endl;
	  this->behaviourFile << "return this->computeConsistentTangentOperator_" << ktype << "(smt);" << endl;
	}
	this->behaviourFile << "}" << endl;
	this->behaviourFile << "string msg(\"" << this->mb.getClassName()
			    << "::computeConsistentTangentOperator : \");" << endl
			    << "msg += \"unsupported tangent operator flag\";" << endl
			    << "throw(runtime_error(msg));" << endl;
	this->behaviourFile << "return false;" << endl;
	this->behaviourFile << "}" << endl << endl;
      }
    } else {
      if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeTangentOperator)){
	this->behaviourFile << "bool computeConsistentTangentOperator(const SMType smt){" << endl;
	this->behaviourFile << "using namespace std;" << endl;
	this->behaviourFile << "using namespace tfel::math;" << endl;
	this->behaviourFile << "using std::vector;" << endl;
	writeMaterialLaws("MFrontBehaviourParserCommon::writeBehaviourComputeTangentOperator",
			  this->behaviourFile,this->mb.getMaterialLaws());
	this->behaviourFile << this->mb.getCode(h,MechanicalBehaviourData::ComputeTangentOperator) << '\n';
	this->behaviourFile << "return true;" << endl;
	this->behaviourFile << "}" << endl << endl;
      }
    }
  } // end of MFrontBehaviourParserCommon::writeBehaviourComputeTangentOperator

  void MFrontBehaviourParserCommon::writeBehaviourGetTangentOperator()
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "const TangentOperator&" << endl;
    this->behaviourFile << "getTangentOperator(void) const{" << endl;
    this->behaviourFile << "return this->Dt;" << endl;
    this->behaviourFile << "}" << endl << endl;
  } // end of MFrontBehaviourParserCommon::writeBehaviourComputeTangentOperator(void)

  void MFrontBehaviourParserCommon::writeBehaviourGetTimeStepScalingFactor()
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "real" << endl;
    this->behaviourFile << "getTimeStepScalingFactor(void) const{" << endl;
    this->behaviourFile << "return real(1);" << endl;
    this->behaviourFile << "}" << endl << endl;
  } // end of MFrontBehaviourParserCommon::writeBehaviourGetTimeStepScalingFactor(void)

  void MFrontBehaviourParserCommon::writeBehaviourTangentOperator()
  {
    using namespace std;
    this->checkBehaviourFile();
    this->behaviourFile << "//! Tangent operator;" << endl;
    this->behaviourFile << "TangentOperator Dt;" << endl;
  } // end of MFrontBehaviourParserCommon::writeBehaviourTangentOperator()

  void MFrontBehaviourParserCommon::checkIntegrationDataFile() const {
    using namespace std;
    if((!this->integrationDataFile)||
       (!this->integrationDataFile.good())){
      string msg("MFrontBehaviourParserCommon::checkIntegrationDataOutputFile : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataFileHeader(){
    using namespace std;

    this->checkIntegrationDataFile();

    this->integrationDataFile << "/*!" << endl;
    this->integrationDataFile << "* \\file   " << this->integrationDataFileName << endl;
    this->integrationDataFile << "* \\brief  " << "this file implements the " 
			      << this->mb.getClassName() << "IntegrationData" << " class." << endl;
    this->integrationDataFile << "*         File generated by ";
    this->integrationDataFile << MFrontHeader::getVersionName() << " ";
    this->integrationDataFile << "version " << MFrontHeader::getVersionNumber();
    this->integrationDataFile << endl;
    if(!this->authorName.empty()){
      this->integrationDataFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->integrationDataFile << "* \\date   " << this->date       << endl;
    }
    this->integrationDataFile << " */" << endl;
    this->integrationDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataFileHeaderBegin(){
    using namespace std;
    this->checkIntegrationDataFile();

    this->integrationDataFile << "#ifndef _LIB_TFELMATERIAL_";
    this->integrationDataFile << makeUpperCase(this->mb.getClassName());
    this->integrationDataFile << "_INTEGRATION_DATA_HXX_" << endl;
    this->integrationDataFile << "#define _LIB_TFELMATERIAL_";
    this->integrationDataFile << makeUpperCase(this->mb.getClassName());
    this->integrationDataFile << "_INTEGRATION_DATA_HXX_" << endl;
    this->integrationDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataFileHeaderEnd()
  {
    using namespace std;

    this->checkIntegrationDataFile();

    this->integrationDataFile << "#endif /* _LIB_TFELMATERIAL_";
    this->integrationDataFile << makeUpperCase(this->mb.getClassName());
    this->integrationDataFile << "_INTEGRATION_DATA_HXX_ */" << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataStandardTFELIncludes(void)
  {
    using namespace std;
    bool b1 = false;
    bool b2 = false;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "#include<string>" << endl;
    this->integrationDataFile << "#include<iostream>" << endl;
    this->integrationDataFile << "#include<limits>" << endl;
    this->integrationDataFile << "#include<stdexcept>" << endl;
    this->integrationDataFile << "#include<algorithm>" << endl << endl;
    this->integrationDataFile << "#include\"TFEL/Config/TFELConfig.hxx\"" << endl;
    this->integrationDataFile << "#include\"TFEL/Config/TFELTypes.hxx\"" << endl;
    this->integrationDataFile << "#include\"TFEL/Metaprogramming/StaticAssert.hxx\"" << endl;
    this->integrationDataFile << "#include\"TFEL/Metaprogramming/IsSameType.hxx\"" << endl;
    this->integrationDataFile << "#include\"TFEL/Metaprogramming/EnableIf.hxx\"" << endl;
    this->integrationDataFile << "#include\"TFEL/TypeTraits/IsFundamentalNumericType.hxx\"" << endl;
    this->integrationDataFile << "#include\"TFEL/TypeTraits/IsScalar.hxx\"" << endl;
    this->integrationDataFile << "#include\"TFEL/TypeTraits/IsReal.hxx\"" << endl;
    this->integrationDataFile << "#include\"TFEL/TypeTraits/Promote.hxx\"" << endl;
    this->integrationDataFile << "#include\"TFEL/Utilities/Name.hxx\"" << endl;
    this->mb.requiresTVectorOrVectorIncludes(b1,b2);
    if(b1){
      this->integrationDataFile << "#include\"TFEL/Math/tvector.hxx\"" << endl;
    }
    if(b2){
      this->integrationDataFile << "#include\"TFEL/Math/vector.hxx\"" << endl;
    }
    this->integrationDataFile << "#include\"TFEL/Math/stensor.hxx\"" << endl;
    this->integrationDataFile << "#include\"TFEL/Math/st2tost2.hxx\"" << endl;
    if(this->mb.getBehaviourType()==MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      this->integrationDataFile << "#include\"TFEL/Math/tensor.hxx\"" << endl;
      this->integrationDataFile << "#include\"TFEL/Math/t2tot2.hxx\"" << endl;
      this->integrationDataFile << "#include\"TFEL/Math/t2tost2.hxx\"" << endl;
      this->integrationDataFile << "#include\"TFEL/Math/st2tot2.hxx\"" << endl;
    }
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataDefaultMembers(void)
  {
    using namespace std;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "protected: " << endl << endl;
    for(p=this->mb.getMainVariables().begin();p!=this->mb.getMainVariables().end();++p){
      if(p->first.increment_known){
	this->integrationDataFile << "/*!" << endl;
	this->integrationDataFile << " * \\brief " << p->first.name << " increment" << endl;
	this->integrationDataFile << " */" << endl;
	this->integrationDataFile << p->first.type  << " d" << p->first.name << ";" << endl << endl;
      } else {
	this->integrationDataFile << "/*!" << endl;
	this->integrationDataFile << " * \\brief " << p->first.name << " at the end of the time step" << endl;
	this->integrationDataFile << " */" << endl;
	this->integrationDataFile << p->first.type  << " " << p->first.name << "1;" << endl << endl;
      }
    }
    this->integrationDataFile << "/*!" << endl;
    this->integrationDataFile << " * \\brief time increment" << endl;
    this->integrationDataFile << " */" << endl;
    this->integrationDataFile << "time dt;" << endl;
    this->integrationDataFile << "/*!" << endl;
    this->integrationDataFile << " * \\brief temperature increment" << endl;
    this->integrationDataFile << " */" << endl;
    this->integrationDataFile << "temperature dT;" << endl;
    this->integrationDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataStandardTFELTypedefs(void)
  {
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "static const unsigned short TVectorSize = N;" << endl;
    this->integrationDataFile << "typedef tfel::math::StensorDimeToSize<N> StensorDimeToSize;" << endl;
    this->integrationDataFile << "static const unsigned short StensorSize = ";
    this->integrationDataFile << "StensorDimeToSize::value;" << endl;
    this->integrationDataFile << "typedef tfel::math::TensorDimeToSize<N> TensorDimeToSize;" << endl;
    this->integrationDataFile << "static const unsigned short TensorSize = ";
    this->integrationDataFile << "TensorDimeToSize::value;" << endl;
    this->integrationDataFile << endl;
    this->writeStandardTFELTypedefs(this->integrationDataFile);
    this->integrationDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataGetName(void){    
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "namespace tfel{" << endl;
    this->integrationDataFile << "namespace utilities{" << endl;
    this->integrationDataFile << "//! Partial specialisation of the Name class" << endl;
    if(this->mb.useQt()){        
      this->integrationDataFile << "template<tfel::material::ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
      this->integrationDataFile << "struct Name<tfel::material::" << this->mb.getClassName() << "IntegrationData<hypothesis,Type,use_qt> >" << endl;
    } else {
      this->integrationDataFile << "template<tfel::material::ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
      this->integrationDataFile << "struct Name<tfel::material::" << this->mb.getClassName() << "IntegrationData<hypothesis,Type,false> >" << endl;
    }
    this->integrationDataFile << "{" << endl;
    this->integrationDataFile << "/*!" << endl;
    this->integrationDataFile << "* \\brief  Return the name of the class." << endl;
    this->integrationDataFile << "* \\return the name of the class." << endl;
    this->integrationDataFile << "* \\see    Name." << endl;
    this->integrationDataFile << "*/" << endl;
    this->integrationDataFile << "static std::string" << endl;
    this->integrationDataFile << "getName(void){" << endl;
    this->integrationDataFile << "return std::string(\"";
    this->integrationDataFile << this->mb.getClassName() << "IntegrationData\");" << endl;
    this->integrationDataFile << "}" << endl << endl;
    this->integrationDataFile << "}; // end of struct Name" << endl;
    this->integrationDataFile << "} // end of namespace utilities" << endl;
    this->integrationDataFile << "} // end of namespace tfel" << endl << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataDisabledConstructors(void)
  {
    using namespace std;
    this->checkIntegrationDataFile();
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataConstructors(const Hypothesis h)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p2;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "/*!" << endl;
    this->integrationDataFile << "* \\brief Default constructor" << endl;
    this->integrationDataFile << "*/" << endl;
    this->integrationDataFile << this->mb.getClassName() << "IntegrationData()" << endl;
    this->integrationDataFile << "{}" << endl << endl;
    this->integrationDataFile << "/*!" << endl;
    this->integrationDataFile << "* \\brief Copy constructor" << endl;
    this->integrationDataFile << "*/" << endl;
    this->integrationDataFile << this->mb.getClassName() << "IntegrationData(const ";
    this->integrationDataFile << this->mb.getClassName() << "IntegrationData& src)" << endl;
    this->integrationDataFile << ": ";
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->integrationDataFile << "d" <<p2->first.name  << "(src.d" << p2->first.name << ")," << endl;
      } else {
	this->integrationDataFile << p2->first.name  << "1(src." << p2->first.name << "1)," << endl;
      }
    }
    this->integrationDataFile << "dt(src.dt)," << endl;
    this->integrationDataFile << "dT(src.dT)";
    if(!md.getExternalStateVariables().empty()){
      for(p =md.getExternalStateVariables().begin();
	  p!=md.getExternalStateVariables().end();++p){
	this->integrationDataFile << "," << endl;
	this->integrationDataFile << "d" << p->name << "(src.d" << p->name << ")";
      }
    }
    this->integrationDataFile << "\n{}" << endl << endl;
    // Creating constructor for external interfaces
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin(); i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      if(interface->isModellingHypothesisHandled(h,this->mb)){
	interface->writeIntegrationDataConstructor(this->integrationDataFile,h,this->mb);
      }
    }
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataScaleOperators(const Hypothesis h)
  {
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p2;
    bool iknown = true;
    for(p2=this->mb.getMainVariables().begin();(p2!=this->mb.getMainVariables().end())&&(iknown);++p2){
      iknown = p2->first.increment_known;
    }
    this->checkIntegrationDataFile();
    this->integrationDataFile << "/*" << endl;
    this->integrationDataFile << "* Multiplication by a scalar." << endl;
    this->integrationDataFile << "*/" << endl;
    this->integrationDataFile << "template<typename Scal>" << endl;
    this->integrationDataFile << "typename tfel::meta::EnableIf<" << endl;
    this->integrationDataFile << "tfel::typetraits::IsFundamentalNumericType<Scal>::cond&&" << endl;
    this->integrationDataFile << "tfel::typetraits::IsScalar<Scal>::cond&&" << endl;
    this->integrationDataFile << "tfel::typetraits::IsReal<Scal>::cond&&" << endl;
    this->integrationDataFile << "tfel::meta::IsSameType<Type," 
			      << "typename tfel::typetraits::Promote"
			      << "<Type,Scal>::type>::cond," << endl;
    this->integrationDataFile << this->mb.getClassName() << "IntegrationData&" << endl
			      << ">::type" << endl;
    if(!iknown){
      if(this->mb.useQt()){
	this->integrationDataFile << "scale(const " << this->mb.getClassName() << "BehaviourData<hypothesis,Type,use_qt>& behaviourData, const Scal time_scaling_factor){" << endl;
      } else {
	this->integrationDataFile << "scale(const " << this->mb.getClassName() << "BehaviourData<hypothesis,Type,false>& behaviourData, const Scal time_scaling_factor){" << endl;
      }
    } else {
      if(this->mb.useQt()){
	this->integrationDataFile << "scale(const " << this->mb.getClassName() << "BehaviourData<hypothesis,Type,use_qt>&, const Scal time_scaling_factor){" << endl;
      } else {
	this->integrationDataFile << "scale(const " << this->mb.getClassName() << "BehaviourData<hypothesis,Type,false>&, const Scal time_scaling_factor){" << endl;
      }
    }
    this->integrationDataFile << "this->dt   *= time_scaling_factor;" << endl;
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->integrationDataFile << "this->d" <<p2->first.name  << " *= time_scaling_factor;" << endl;
      } else {
	this->integrationDataFile << "this->" <<p2->first.name  << "1 = (1-time_scaling_factor)*(behaviourData." <<p2->first.name  << "0)+time_scaling_factor*(this->" <<p2->first.name  << "1);" << endl;
      }
    }
    this->integrationDataFile << "this->dT   *= time_scaling_factor;" << endl;
    for(p=md.getExternalStateVariables().begin();p!=md.getExternalStateVariables().end();++p){
      this->integrationDataFile << "this->d" << p->name << " *= time_scaling_factor;" << endl;
    }
    this->integrationDataFile << "return *this;" << endl;
    this->integrationDataFile << "}" << endl << endl;
  } // end of MFrontBehaviourParserCommon::writeIntegrationDataScaleOpeartors

  void MFrontBehaviourParserCommon::writeIntegrationDataClassHeader(void) 
  {
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "/*!" << endl;
    this->integrationDataFile << "* \\class " << this->mb.getClassName() << "IntegrationData"  << endl;
    this->integrationDataFile << "* \\brief This class implements the " 
			      << this->mb.getClassName() << "IntegrationData" << " behaviour." << endl;
    this->integrationDataFile << "* \\param unsigned short N, space dimension." << endl;
    this->integrationDataFile << "* \\param typename Type, numerical type." << endl;
    this->integrationDataFile << "* \\param bool use_qt, conditional saying if quantities are use." << endl;
    if(!this->authorName.empty()){
      this->integrationDataFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->integrationDataFile << "* \\date   " << this->date << endl;
    }
    this->integrationDataFile << "*/" << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataForwardDeclarations(void)
  {
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "// Forward Declaration" << endl;
    this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
    this->integrationDataFile << "class " << this->mb.getClassName() << "IntegrationData;" << endl << endl;

    if(this->mb.useQt()){
      this->integrationDataFile << "// Forward Declaration" << endl;
      this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
      this->integrationDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->integrationDataFile << "const " << this->mb.getClassName() 
				<< "IntegrationData<hypothesis,Type,use_qt>&);" << endl << endl;
    } else {
      this->integrationDataFile << "// Forward Declaration" << endl;
      this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
      this->integrationDataFile << "std::ostream&\n operator <<(std::ostream&,";
      this->integrationDataFile << "const " << this->mb.getClassName() 
				<< "IntegrationData<hypothesis,Type,false>&);" << endl << endl;
    }
    // maintenant, il faut déclarer toutes les spécialisations partielles...
    const set<Hypothesis>& h = this->mb.getModellingHypotheses();
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      if(this->mb.hasSpecialisedMechanicalData(*ph)){
	if(this->mb.useQt()){
	  this->integrationDataFile << "// Forward Declaration" << endl;
	  this->integrationDataFile << "template<typename Type,bool use_qt>" << endl;
	  this->integrationDataFile << "std::ostream&\n operator <<(std::ostream&,";
	  this->integrationDataFile << "const " << this->mb.getClassName() 
				    << "IntegrationData<ModellingHypothesis::"
				    << ModellingHypothesis::toUpperCaseString(*ph) << ",Type,use_qt>&);" << endl << endl;
	} else {
	  this->integrationDataFile << "// Forward Declaration" << endl;
	  this->integrationDataFile << "template<typename Type>" << endl;
	  this->integrationDataFile << "std::ostream&\n operator <<(std::ostream&,";
	  this->integrationDataFile << "const " << this->mb.getClassName() 
				    << "IntegrationData<ModellingHypothesis::"
				    << ModellingHypothesis::toUpperCaseString(*ph) << ",Type,false>&);" << endl << endl;
	}
      }
    }
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataClassBegin(const Hypothesis h) {
    using namespace std;
    this->checkIntegrationDataFile();
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){
	this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
	this->integrationDataFile << "class " << this->mb.getClassName() << "IntegrationData" << endl;
      } else {
	this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
	this->integrationDataFile << "class " << this->mb.getClassName() 
				  << "IntegrationData<hypothesis,Type,false>" << endl;
      }
    } else {
      if(this->mb.useQt()){
	this->integrationDataFile << "template<typename Type,bool use_qt>" << endl;
	this->integrationDataFile << "class " << this->mb.getClassName()
				  << "IntegrationData<ModellingHypothesis::"
				  << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>" << endl;
      } else {
	this->integrationDataFile << "template<typename Type>" << endl;
	this->integrationDataFile << "class " << this->mb.getClassName() 
				  << "IntegrationData<ModellingHypothesis::"
				  << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>" << endl;
      }
    }
    this->integrationDataFile << "{" << endl;
    this->integrationDataFile << endl;
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->integrationDataFile << "static const ModellingHypothesis::Hypothesis hypothesis = " 
				<< "ModellingHypothesis::"
				<< ModellingHypothesis::toUpperCaseString(h) << ";" << endl;
    }
    this->integrationDataFile << "static const unsigned short N = ModellingHypothesisToSpaceDimension<hypothesis>::value;" << endl;
    this->integrationDataFile << "TFEL_STATIC_ASSERT(N==1||N==2||N==3);" << endl;
    this->integrationDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::"
			      << "IsFundamentalNumericType<Type>::cond);" << endl;
    this->integrationDataFile << "TFEL_STATIC_ASSERT(tfel::typetraits::IsReal<Type>::cond);" << endl << endl;
    this->integrationDataFile << "friend std::ostream& operator<< <>(std::ostream&,const ";
    this->integrationDataFile << this->mb.getClassName() << "IntegrationData&);" << endl << endl;
    this->writeIntegerConstants(this->integrationDataFile);
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataOutputOperator(const Hypothesis h)
  {    
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    VariableDescriptionContainer::const_iterator p;
    map<DrivingVariable,
	ThermodynamicForce>::const_iterator p2;
    this->checkBehaviourFile();
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){        
	this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type,bool use_qt>" << endl;
	this->integrationDataFile << "std::ostream&" << endl;
	this->integrationDataFile << "operator <<(std::ostream& os,";
	this->integrationDataFile << "const " << this->mb.getClassName() 
				  << "IntegrationData<hypothesis,Type,use_qt>& b)" << endl;
      } else {
	this->integrationDataFile << "template<ModellingHypothesis::Hypothesis hypothesis,typename Type>" << endl;
	this->integrationDataFile << "std::ostream&" << endl;
	this->integrationDataFile << "operator <<(std::ostream& os,";
	this->integrationDataFile << "const " << this->mb.getClassName() 
				  << "IntegrationData<hypothesis,Type,false>& b)" << endl;
      }
    } else {
      if(this->mb.useQt()){        
	this->integrationDataFile << "template<typename Type,bool use_qt>" << endl;
	this->integrationDataFile << "std::ostream&" << endl;
	this->integrationDataFile << "operator <<(std::ostream& os,";
	this->integrationDataFile << "const " << this->mb.getClassName() 
				  << "IntegrationData<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt>& b)" << endl;
      } else {
	this->integrationDataFile << "template<typename Type>" << endl;
	this->integrationDataFile << "std::ostream&" << endl;
	this->integrationDataFile << "operator <<(std::ostream& os,";
	this->integrationDataFile << "const " << this->mb.getClassName() 
				  << "IntegrationData<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,false>& b)" << endl;
      }
    }
    this->integrationDataFile << "{" << endl;
    this->integrationDataFile << "using namespace std;" << endl;
    this->integrationDataFile << "using namespace tfel::utilities;" << endl;
    if(h==ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      if(this->mb.useQt()){        
	this->integrationDataFile << "os << Name<" << this->mb.getClassName() 
				  << "IntegrationData<hypothesis,Type,use_qt> >::getName() << endl;" << endl;
      } else {
	this->integrationDataFile << "os << Name<" << this->mb.getClassName() 
				  << "IntegrationData<hypothesis,Type,false> >::getName() << endl;" << endl;
      }
    } else {
      if(this->mb.useQt()){        
	this->integrationDataFile << "os << Name<" << this->mb.getClassName() 
				  << "IntegrationData<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,use_qt> >::getName() << endl;" << endl;
      } else {
	this->integrationDataFile << "os << Name<" << this->mb.getClassName() 
				  << "IntegrationData<ModellingHypothesis::" << ModellingHypothesis::toUpperCaseString(h) << ",Type,false> >::getName() << endl;" << endl;
      }
    }
    for(p2=this->mb.getMainVariables().begin();p2!=this->mb.getMainVariables().end();++p2){
      if(p2->first.increment_known){
	this->integrationDataFile << "os << \"d" << p2->first.name << " : \" << b.d" << p2->first.name << " << endl;" << endl;
      } else {
	this->integrationDataFile << "os << \"" << p2->first.name << "1 : \" << b." << p2->first.name << "1 << endl;" << endl;
      }
    }
    this->integrationDataFile << "os << \"dt : \" << b.dt << endl;" << endl;
    this->integrationDataFile << "os << \"dT : \" << b.dT << endl;" << endl;
    for(p=md.getExternalStateVariables().begin();p!=md.getExternalStateVariables().end();++p){
      this->integrationDataFile << "os << \"d" << p->name << " : \" << b.d" 
				<< p->name << " << endl;" << endl;  
    }
    this->integrationDataFile << "return os;" << endl;
    this->integrationDataFile << "}" << endl << endl;
  } // end of MFrontBehaviourParserCommon::writeIntegrationDataOutputOperator()

  void MFrontBehaviourParserCommon::writeIntegrationDataClassEnd() {    
    using namespace std;
    this->checkIntegrationDataFile();
    this->integrationDataFile << "}; // end of " << this->mb.getClassName() << "IntegrationData" 
			      << "class" << endl;
    this->integrationDataFile << endl;
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataExternalStateVariables(const Hypothesis h) {
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    this->checkIntegrationDataFile();
    this->writeVariablesDeclarations(this->integrationDataFile,
				     md.getExternalStateVariables(),
				     "d","",this->fileName,false);
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataFileBegin(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    this->checkIntegrationDataFile();
    this->writeIntegrationDataFileHeader();
    this->writeIntegrationDataFileHeaderBegin();
    this->writeIntegrationDataStandardTFELIncludes();
    this->writeIncludes(this->integrationDataFile);
    // includes specific to interfaces
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      interface->writeInterfaceSpecificIncludes(this->integrationDataFile,this->mb);
    }
    this->writeNamespaceBegin(this->integrationDataFile);
    this->writeIntegrationDataForwardDeclarations();
  } // end of MFrontBehaviourParserCommon::writeIntegrationDataFile

  void MFrontBehaviourParserCommon::writeIntegrationDataClass(const Hypothesis h){
    using namespace std;
    this->checkIntegrationDataFile();
    this->writeIntegrationDataClassBegin(h);
    this->writeIntegrationDataStandardTFELTypedefs();
    this->writeIntegrationDataDefaultMembers();
    this->writeIntegrationDataExternalStateVariables(h);
    this->writeIntegrationDataDisabledConstructors();
    this->integrationDataFile << "public:" << endl << endl;
    this->writeIntegrationDataConstructors(h);
    this->writeIntegrationDataMainVariablesSetters();
    this->writeIntegrationDataScaleOperators(h);
    this->writeIntegrationDataClassEnd();
    this->writeIntegrationDataOutputOperator(h);
  }

  void MFrontBehaviourParserCommon::writeIntegrationDataFileEnd(void)
  {
    this->checkIntegrationDataFile();
    this->writeNamespaceEnd(this->integrationDataFile);
    this->writeIntegrationDataGetName();
    this->writeIntegrationDataFileHeaderEnd();
  } // end of MFrontBehaviourParserCommon::writeIntegrationDataFileEnd

  void MFrontBehaviourParserCommon::checkSrcFile(void) const {
    using namespace std;
    if((!this->integrationDataFile)||
       (!this->integrationDataFile.good())){
      string msg("MFrontBehaviourParserCommon::checkSrcOutputFile : ");
      msg += "ouput file is not valid";
      throw(runtime_error(msg));
    }
  }

  void MFrontBehaviourParserCommon::writeSrcFileHeader(void){
    using namespace std;
    this->checkSrcFile();
    this->srcFile << "/*!" << endl;
    this->srcFile << "* \\file   " << this->srcFileName << endl;
    this->srcFile << "* \\brief  " << "this file implements the " 
		  << this->mb.getClassName() << " Behaviour." << endl;
    this->srcFile << "*         File generated by ";
    this->srcFile << MFrontHeader::getVersionName() << " ";
    this->srcFile << "version " << MFrontHeader::getVersionNumber();
    this->srcFile << endl;
    if(!this->authorName.empty()){
      this->srcFile << "* \\author " << this->authorName << endl;
    }
    if(!this->date.empty()){
      this->srcFile << "* \\date   " << this->date       << endl;
    }
    this->srcFile << " */" << endl;
    this->srcFile << endl;
    if(this->mb.hasParameters()){
      this->srcFile << "#include<string>" << endl;
      this->srcFile << "#include<cstring>" << endl;
      this->srcFile << "#include<fstream>" << endl;
      this->srcFile << "#include<stdexcept>" << endl;
      this->srcFile << endl;
    }
    this->srcFile << "#include\"TFEL/Material/" << this->behaviourDataFileName   << "\"" << endl;
    this->srcFile << "#include\"TFEL/Material/" << this->integrationDataFileName << "\"" << endl;
    this->srcFile << "#include\"TFEL/Material/" << this->behaviourFileName       << "\"" << endl;
    this->srcFile << endl;
  } // end of MFrontBehaviourParserCommon::writeSrcFileHeader()

  void MFrontBehaviourParserCommon::writeSrcFileStaticVariables(const Hypothesis h){
    using namespace std;
    const MechanicalBehaviourData& md = this->mb.getMechanicalBehaviourData(h);
    const string m("tfel::material::ModellingHypothesis::"+
		   ModellingHypothesis::toUpperCaseString(h));
    this->checkSrcFile();
    StaticVariableDescriptionContainer::const_iterator p;
    for(p=md.getStaticVariables().begin();p!=md.getStaticVariables().end();++p){
      if(this->mb.useQt()){
	this->srcFile << "template<>" << endl;
	this->srcFile << this->mb.getClassName() << "<" << m << ",float,true>::" 
		      << p->type << endl << this->mb.getClassName() 
		      << "<" << m << ",float,true>::" 
		      << p->name << " = " << this->mb.getClassName() 
		      << "<" << m << ",float,true>::" << p->type 
		      << "(static_cast<float>(" << p->value <<"));" << endl << endl;
      }
      this->srcFile << "template<>" << endl;
      this->srcFile << this->mb.getClassName() << "<" << m << ",float,false>::" 
		    << p->type << endl << this->mb.getClassName() 
		    << "<" << m << ",float,false>::" 
		    << p->name << " = " << this->mb.getClassName() 
		    << "<" << m << ",float,false>::" << p->type 
		    << "(static_cast<float>(" << p->value <<"));" << endl << endl;
      if(this->mb.useQt()){
	this->srcFile << "template<>" << endl;
	this->srcFile << this->mb.getClassName() << "<" << m << ",double,true>::" 
		      << p->type << endl << this->mb.getClassName() 
		      << "<" << m << ",double,true>::" 
		      << p->name << " = " << this->mb.getClassName() 
		      << "<" << m << ",double,true>::" << p->type 
		      << "(static_cast<double>(" << p->value <<"));" << endl << endl;
      }
      this->srcFile << "template<>" << endl;
      this->srcFile << this->mb.getClassName() << "<" << m << ",double,false>::" 
		    << p->type << endl << this->mb.getClassName() 
		    << "<" << m << ",double,false>::" 
		    << p->name << " = " << this->mb.getClassName() 
		    << "<" << m << ",double,false>::" << p->type 
		    << "(static_cast<double>(" << p->value <<"));" << endl << endl;
      if(this->mb.useQt()){
	this->srcFile << "template<>" << endl;
	this->srcFile << this->mb.getClassName() << "<" << m << ",long double,true>::" 
		      << p->type << endl << this->mb.getClassName() 
		      << "<" << m << ",long double,true>::" 
		      << p->name << " = " << this->mb.getClassName() << "<" 
		      << m << ",long double,true>::" << p->type 
		      << "(static_cast<long double>(" << p->value <<"));" << endl << endl;
      }
      this->srcFile << "template<>" << endl;
      this->srcFile << this->mb.getClassName() << "<" << m << ",long double,false>::" 
		    << p->type << endl << this->mb.getClassName() 
		    << "<" << m << ",long double,false>::" 
		    << p->name << " = " << this->mb.getClassName() 
		    << "<" << m << ",long double,false>::" << p->type 
		    << "(static_cast<long double>(" << p->value <<"));" << endl << endl;
    }
  } // end of MFrontBehaviourParserCommon::writeSrcFileStaticVariables
  
  void
  MFrontBehaviourParserCommon::writeSrcFileUserDefinedCode(void)
  {
    using namespace std;
    const string& s = this->mb.getSources();
    this->checkBehaviourFile();
    if(!s.empty()){
      this->srcFile << s << endl << endl;
    }
  } // end of MFrontBehaviourParserCommon::writeSrcFileUserDefinedCode

  void
  MFrontBehaviourParserCommon::writeSrcFileParametersInitializers(void)
  {
    using namespace std;
    if(!this->mb.hasParameters()){
      return;
    }
    set<Hypothesis> h = this->mb.getDistinctModellingHypotheses();
    h.insert(ModellingHypothesis::UNDEFINEDHYPOTHESIS);
    for(set<Hypothesis>::const_iterator p=h.begin();p!=h.end();++p){
      if(this->mb.hasParameters(*p)){
	this->writeSrcFileParametersInitializer(*p);
      }
    }
  } // end of MFrontBehaviourParserCommon::writeSrcFileParametersInitializer

  static void
  MFrontBehaviourParserCommon_writeConverter(std::ostream& f,
					     const std::string& cname,
					     const std::string& type,
					     const std::string& type2)
  {
    using std::endl;
    f << type  << endl
      << cname << "::get" << type2 << "(const std::string& n," << endl
      <<                               "const std::string& v)" << endl
      << "{" << endl
      << "using namespace std;" << endl
      << type << " value;" << endl
      << "istringstream converter(v);" << endl
      << "converter >> value;" << endl
      << "if(!converter&&(!converter.eof())){" << endl
      << "string msg(\"" << cname << "::get" << type2 << " : \"" << endl
      << "           \"can't convert '\"+v+\"' to " << type
      << " for parameter '\"+ n+\"'\");" << endl
      << "throw(runtime_error(msg));" << endl
      << "}" << endl
      << "return value;" << endl
      << "}" << endl << endl;
  }

  void
  MFrontBehaviourParserCommon::writeSrcFileParametersInitializer(const Hypothesis h)
  {
    using namespace std;
    this->checkBehaviourFile();
    // treating the default case
    bool rp  = false; // real    parameter found
    bool ip  = false; // integer parameter found
    bool up  = false; // unsigned short parameter found
    bool rp2 = false;
    bool ip2 = false;
    bool up2 = false;
    VariableDescriptionContainer::const_iterator p;
    const string dcname(this->mb.getClassName()+"ParametersInitializer");
    string cname(this->mb.getClassName());
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      cname += ModellingHypothesis::toString(h);
    }
    cname += "ParametersInitializer";
    this->srcFile << cname << "&" << endl
		  << cname << "::get()" << endl
		  <<"{" << endl
		  << "static " << cname << " i;" << endl
		  << "return i;" << endl
		  << "}" << endl << endl;
    this->srcFile << cname << "::" 
		  << cname << "()" << endl
    		    <<"{" << endl;
    const VariableDescriptionContainer& params =
      this->mb.getMechanicalBehaviourData(h).getParameters();
    for(p=params.begin();p!=params.end();++p){
      if(p->type=="real"){
	rp = true;
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  rp2 = true;
	  this->srcFile << "this->" << p->name << " = " 
			<< this->mb.getFloattingPointParameterDefaultValue(h,p->name) << ";" << endl; 
	}
      } else if(p->type=="int"){
	ip = true;
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  ip2 = true;
	  this->srcFile << "this->" << p->name << " = " 
			<< this->mb.getIntegerParameterDefaultValue(h,p->name) << ";" << endl; 
	}
      } else if(p->type=="ushort"){
	up = true;
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  up2 = true;
	  this->srcFile << "this->" << p->name << " = " 
			<< this->mb.getUnsignedShortParameterDefaultValue(h,p->name) << ";" << endl; 
	}
      }
    }
    this->srcFile << "// Reading parameters from a file" << endl;
    this->srcFile << "this->readParameters(\"" << this->mb.getClassName() << "-parameters.txt\");" << endl;
    if(h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
      this->srcFile << "this->readParameters(\"" << this->mb.getClassName() << ModellingHypothesis::toString(h) << "-parameters.txt\");" << endl;
    }
    this->srcFile <<"}" << endl << endl;
    if(rp){
      this->srcFile <<"void" << endl
		    << cname << "::set(const char* const key,\nconst double v)" 
		    << "{" << endl
		    << "using namespace std;" << endl;
      bool first = true;
      for(p=params.begin();p!=params.end();++p){
	if(p->type=="real"){
	  if(first){
	    this->srcFile << "if(";
	    first = false;
	  } else {
	    this->srcFile << "} else if(";
	  }
	  this->srcFile << "::strcmp(\""+this->mb.getExternalName(h,p->name)+"\",key)==0){" << endl;
	  if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	     ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	      (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	    this->srcFile << "this->" << p->name << " = v;" << endl;
	  } else {
	    this->srcFile << dcname << "::get().set(\"" << this->mb.getExternalName(h,p->name) << "\",v);" << endl;
	  }
	}
      }
      this->srcFile << "} else {" << endl;
      this->srcFile << "string msg(\"" << cname << "::set : \");" << endl
		    << "msg += \" no parameter named '\";" << endl
		    << "msg += key;" << endl
		    << "msg += \"'\";" << endl
		    << "throw(runtime_error(msg));" << endl
		    << "}" << endl
		    << "}" << endl << endl;
    }
    if(ip){
      this->srcFile <<"void" << endl
		    << cname << "::set(const char* const key,\nconst int v)" 
		    << "{" << endl
		    << "using namespace std;" << endl;
      bool first = true;
      for(p=params.begin();p!=params.end();++p){
	if(p->type=="int"){
	  if(first){
	    this->srcFile << "if(";
	    first = false;
	  } else {
	    this->srcFile << "} else if(";
	  }
	  this->srcFile << "::strcmp(\""+this->mb.getExternalName(h,p->name)+"\",key)==0){" << endl;
	  if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	     ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	      (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	    this->srcFile << "this->" << p->name << " = v;" << endl;
	  } else {
	    this->srcFile << dcname << "::get().set(\"" << this->mb.getExternalName(h,p->name) << "\",v);" << endl;
	  }
	}
      }
      this->srcFile << "} else {" << endl;
      this->srcFile << "string msg(\"" << cname << "::set : \");" << endl
		    << "msg += \" no parameter named '\";" << endl
		    << "msg += key;" << endl
		    << "msg += \"'\";" << endl
		    << "throw(runtime_error(msg));" << endl
		    << "}" << endl
		    << "}" << endl << endl;
    }
    if(up){
      this->srcFile <<"void" << endl
		    << cname << "::set(const char* const key,\nconst unsigned short v)" 
		    << "{" << endl
		    << "using namespace std;" << endl;
      bool first = true;
      for(p=params.begin();p!=params.end();++p){
	if(p->type=="ushort"){
	  if(first){
	    this->srcFile << "if(";
	    first = false;
	  } else {
	    this->srcFile << "} else if(";
	  }
	  this->srcFile << "::strcmp(\""+this->mb.getExternalName(h,p->name)+"\",key)==0){" << endl;
	  if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	     ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	      (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	    this->srcFile << "this->" << p->name << " = v;" << endl;
	  } else {
	    this->srcFile << dcname << "::get().set(\"" << this->mb.getExternalName(h,p->name) << "\",v);" << endl;
	  }
	}
      }
      this->srcFile << "} else {" << endl;
      this->srcFile << "string msg(\"" << cname << "::set : \");" << endl
		    << "msg += \" no parameter named '\";" << endl
		    << "msg += key;" << endl
		    << "msg += \"'\";" << endl
		    << "throw(runtime_error(msg));" << endl
		    << "}" << endl
		    << "}" << endl << endl;
    }
    if(rp2){
      MFrontBehaviourParserCommon_writeConverter(this->srcFile,cname,"double","Double");
    }
    if(ip2){
      MFrontBehaviourParserCommon_writeConverter(this->srcFile,cname,"int","Int");
    }
    if(up2){
      MFrontBehaviourParserCommon_writeConverter(this->srcFile,cname,"unsigned short","UnsignedShort");
    }
    this->srcFile << "void" << endl
		  << cname << "::readParameters(const char* const fn)" 
		  << "{" << endl
		  << "using namespace std;" << endl
		  << "ifstream f(fn);" << endl
		  << "string p;" << endl
		  << "string v;" << endl
		  << "if(!f){" << endl
      		  << "return;" << endl
      		  << "}" << endl
		  << "while(!f.eof()){" << endl
		  << "f >> p;" << endl
		  << "if(p.empty()){" << endl
		  << "if(!f.eof()){" << endl
		  << "string msg(\"" << cname << "::readParameters : \");" << endl
		  << "msg+=\"Error while parsing file '\";" << endl
		  << "msg+=fn;" << endl
		  << "msg+=\"'\";" << endl
		  << "throw(runtime_error(msg));" << endl
		  << "}" << endl
		  << "break;" << endl
		  << "}" << endl
		  << "f >> v;" << endl;
      for(p=params.begin();p!=params.end();++p){
	if(p==params.begin()){
	  this->srcFile << "if(";
	} else {
	  this->srcFile << "} else if(";
	}
	this->srcFile << "\"" <<  this->mb.getExternalName(h,p->name) << "\"==p){" << endl;
	if((h==ModellingHypothesis::UNDEFINEDHYPOTHESIS)||
	   ((h!=ModellingHypothesis::UNDEFINEDHYPOTHESIS)&&
	    (!this->mb.hasParameter(ModellingHypothesis::UNDEFINEDHYPOTHESIS,p->name)))){
	  this->srcFile << "this->" << p->name << " = ";
	  if(p->type=="real"){
	    this->srcFile <<  cname << "::getDouble(p,v);" << endl;
	  } else if(p->type=="int"){
	    this->srcFile << cname << "::getInt(p,v);" << endl;
	  } else if(p->type=="ushort"){
	    this->srcFile << cname << "::getUnsignedShort(p,v);" << endl;
	  } else {
	    this->throwRuntimeError("BehaviourDSLCommon::writeSrcFileParametersInitializer",
				    "invalid parameter type '"+p->type+"'");
	  }
	} else {
	  this->srcFile << dcname << "::get().set(\"" << this->mb.getExternalName(h,p->name) << "\"," << endl;
	  if(p->type=="real"){
	    this->srcFile << dcname << "::getDouble(p,v)" << endl;
	  } else if(p->type=="int"){
	    this->srcFile << dcname << "::getInt(p,v)" << endl;
	  } else if(p->type=="ushort"){
	    this->srcFile << dcname << "::getUnsignedShort(p,v)" << endl;
	  } else {
	    this->throwRuntimeError("BehaviourDSLCommon::writeSrcFileParametersInitializer",
				    "invalid parameter type '"+p->type+"'");
	  }
	  this->srcFile << ");" << endl;
	}
      }
      this->srcFile << "} else {" << endl
		    << "string msg(\"" << cname << "::readParameters : \");" << endl
		    << "msg+=\"Error while parsing file '\";" << endl
		    << "msg+=fn;" << endl
		    << "msg+=\"'. Invalid parameter '\"+p+\"'\";" << endl
		    << "throw(runtime_error(msg));" << endl
		    << "}" << endl
		    << "}" << endl
		    << "}" << endl << endl;
  } // end of MFrontBehaviourParserCommon::writeSrcFileParametersInitializer

  void
  MFrontBehaviourParserCommon::writeSrcFileBehaviourProfiler(void)
  {
    using namespace std;
    if(this->mb.getAttribute(MechanicalBehaviourData::profiling,false)){
      this->checkSrcFile();
      this->srcFile << "mfront::MFrontBehaviourProfiler&" << endl
		    << this->mb.getClassName() << "Profiler::getProfiler()" << endl
		    << "{" << endl
		    << "static mfront::MFrontBehaviourProfiler profiler(\""
		    << this->mb.getClassName() << "\");\n;"
		    << "return profiler;" << endl
		    << "}" << endl << endl;
    }
  } // end of MFrontBehaviourParserCommon::writeSrcFileBehaviourProfiler

  void MFrontBehaviourParserCommon::writeSrcFile(void)
  {
    using namespace std;
    this->writeSrcFileHeader();
    this->writeSrcFileUserDefinedCode();
    this->writeNamespaceBegin(this->srcFile);
    this->writeSrcFileBehaviourProfiler();
    this->writeSrcFileParametersInitializers();
    // modelling hypotheses handled by the behaviour
    const set<Hypothesis>& h = this->mb.getModellingHypotheses();
    for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
      this->writeSrcFileStaticVariables(*ph);
    }
    this->writeNamespaceEnd(this->srcFile);
  } // end of MFrontBehaviourParserCommon::writeSrcFile(void)

  std::map<std::string,std::vector<std::string> >
  MFrontBehaviourParserCommon::getGlobalIncludes(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    typedef map<string,vector<string> > Map;
    Map incs;
    vector<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      const Map& iincs = interface->getGlobalIncludes(this->mb);
      for(p=iincs.begin();p!=iincs.end();++p){
	copy(p->second.begin(),p->second.end(),back_inserter(incs[p->first]));
      }
    }
    return incs;
  } // end of MFrontBehaviourParserCommon::getGlobalIncludes

  std::map<std::string,std::vector<std::string> >
  MFrontBehaviourParserCommon::getGlobalDependencies(void)
  {
    using namespace std;
    typedef map<string,vector<string> > Map;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    Map deps;
    vector<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      const Map& ideps = interface->getGlobalDependencies(this->mb);
      for(p=ideps.begin();p!=ideps.end();++p){
	copy(p->second.begin(),p->second.end(),back_inserter(deps[p->first]));
      }
    }
    return deps;
  } // end of MFrontBehaviourParserCommon::getGlobalDependencies

  std::map<std::string,std::vector<std::string> >
  MFrontBehaviourParserCommon::getGeneratedSources(void)
  {
    using namespace std;
    typedef map<string,vector<string> > Map;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    Map osources;
    vector<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    vector<string>::const_iterator p2;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      const Map& isources = interface->getGeneratedSources(this->mb);
      for(p=isources.begin();p!=isources.end();++p){
	copy(p->second.begin(),p->second.end(),back_inserter(osources[p->first]));
	osources[p->first].push_back(this->srcFileName);
      }
    }
    for(p=osources.begin();p!=osources.end();++p){
      for(p2=this->librariesDependencies.begin();
	  p2!=this->librariesDependencies.end();++p2){
	if("-l"+p->first!=*p2){
	  this->sourcesLibrairiesDependencies[p->first].push_back(*p2);
	}
      }
    }
    return osources;
  } // end of MFrontBehaviourParserCommonCommon::getGeneratedSources

  std::vector<std::string>
  MFrontBehaviourParserCommon::getGeneratedIncludes(void)
  {
    using namespace std;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    vector<string> incs;
    vector<string>::const_iterator i;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      const vector<string>& iincs = interface->getGeneratedIncludes(this->mb);
      copy(iincs.begin(),iincs.end(),back_inserter(incs));
    }
    incs.push_back(this->behaviourFileName);
    incs.push_back(this->behaviourDataFileName);
    incs.push_back(this->integrationDataFileName);
    return incs;
  } // end of MFrontBehaviourParserCommon::getGeneratedIncludes(void)

  std::map<std::string,std::vector<std::string> >
  MFrontBehaviourParserCommon::getLibrariesDependencies(void)
  {
    using namespace std;
    typedef map<string,vector<string> > Map;
    typedef MFrontBehaviourInterfaceFactory MBIF;
    MBIF& mbif = MBIF::getMFrontBehaviourInterfaceFactory();
    vector<string>::const_iterator i;
    map<string,vector<string> >::const_iterator p;
    vector<string>::const_iterator p2;
    for(i  = this->interfaces.begin();
	i != this->interfaces.end();++i){
      MFrontBehaviourVirtualInterface *interface = mbif.getInterfacePtr(*i);
      const Map& ideps = interface->getLibrariesDependencies(this->mb);
      for(p=ideps.begin();p!=ideps.end();++p){
	for(p2=p->second.begin();p2!=p->second.end();++p2){
	  if(find(this->sourcesLibrairiesDependencies[p->first].begin(),
		  this->sourcesLibrairiesDependencies[p->first].end(),
		  *p2)==this->sourcesLibrairiesDependencies[p->first].end()){
	    this->sourcesLibrairiesDependencies[p->first].push_back(*p2);
	  }
	}
      }
    }
    return this->sourcesLibrairiesDependencies;
  } // end of MFrontBehaviourParserCommon::getLibrariesDependencies

  std::string
  MFrontBehaviourParserCommon::predictionOperatorVariableModifier(const Hypothesis h,
								  const std::string& var,
								  const bool addThisPtr)
  {
    if(this->mb.isIntegrationVariableIncrementName(h,var)){
      this->throwRuntimeError("MFrontBehaviourParserCommon::predictionOperatorVariableModifier : ",
			      "integration variable '"+var+"' can't be used in @PredictionOperator");
    }
    if(addThisPtr){
      return "(this->"+var+")";
    } else {
      return var;
    }
  } // end of MFrontBehaviourParserCommon::predictionOperatorVariableModifier

  void MFrontBehaviourParserCommon::treatProfiling(void)
  {
    using namespace std;
    const bool b = 
      this->readBooleanValue("MFrontBehaviourParserCommon::treatProfiling");
    this->readSpecifiedToken("MFrontBehaviourParserCommon::treatProfiling",";");
#ifdef HAVE_CXX11
    this->mb.setAttribute(MechanicalBehaviourData::profiling,b,false);
#else
    static_cast<void>(b);
    if(getVerboseMode()>=VERBOSE_QUIET){
      ostream& log = getLogStream();
      log << "MFrontBehaviourParserCommon::treatProfiling : performances measurements "
	"are only available if C++-11 support have been enabled";
    }
#endif
  } // end of MFrontBehaviourParserCommon::treatProfiling

  void MFrontBehaviourParserCommon::treatPredictionOperator(void)
  {
    using namespace std;
    const CodeBlockOptions& o = this->readCodeBlock(*this,MechanicalBehaviourData::ComputePredictionOperator,
						    &MFrontBehaviourParserCommon::predictionOperatorVariableModifier,
						    true,true);
    for(set<Hypothesis>::const_iterator p=o.hypotheses.begin();p!=o.hypotheses.end();++p){
      this->mb.setAttribute(*p,MechanicalBehaviourData::hasPredictionOperator,true);
    }
  } // end of MFrontBehaviourParserCommon::treatPredictionOperator

  void
  MFrontBehaviourParserCommon::treatParameter(void)
  {
    using namespace std;
    set<Hypothesis> h;
    this->readHypothesesList(h);
    bool endOfTreatement=false;
    while((this->current!=this->fileTokens.end())&&
	  (!endOfTreatement)){
      if(!isValidIdentifier(this->current->value)){
	this->throwRuntimeError("ParserBase::handleParameter : ",
				"variable given is not valid (read '"+this->current->value+"').");
      }
      const string n = this->current->value;
      const unsigned short lineNumber = this->current->line;
      ++(this->current);
      this->checkNotEndOfFile("ParserBase::handleParameter");
      if((this->current->value=="=")||
	 (this->current->value=="{")||
	 (this->current->value=="(")){
	string ci; // closing initializer
	double value;
	if(this->current->value=="{"){
	  ci="}";
	}
	if(this->current->value=="("){
	  ci=")";
	}
	++(this->current);
	this->checkNotEndOfFile("ParserBase::handleParameter");
	istringstream converter(this->current->value);
	converter >> value;
	if(!converter&&(!converter.eof())){
	  this->throwRuntimeError("ParserBase::handleParameter",
				  "could not read default value for parameter '"+n+"'");
	}
	++(this->current);
	this->checkNotEndOfFile("ParserBase::handleParameter");
	if(!ci.empty()){
	  this->readSpecifiedToken("ParserBase::handleParameter",ci);
	}
	this->registerVariable(n,true);
	for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
	  this->mb.addParameter(*ph,VariableDescription("real",n,1u,lineNumber));
	  this->mb.setParameterDefaultValue(*ph,n,value);
	}
      } else {
	this->registerVariable(n,true);
	for(set<Hypothesis>::const_iterator ph=h.begin();ph!=h.end();++ph){
	  this->mb.addParameter(*ph,VariableDescription("real",n,1u,lineNumber));
	}
      }
      if(this->current->value==","){
	++(this->current);
      } else if (this->current->value==";"){
	endOfTreatement=true;
	++(this->current);
      } else {
	this->throwRuntimeError("ParserBase::handleParameter",
				", or ; expected afer '"+n+"'");
      }
    }
    if(!endOfTreatement){
      --(this->current);
      this->throwRuntimeError("ParserBase::handleParameter",
			      "Expected ';' before end of file");
    }
  } // end of MFrontBehaviourParserCommon::treatParameter

  void
  MFrontBehaviourParserCommon::treatInitLocalVariables(void)
  {
    this->readCodeBlock(*this,MechanicalBehaviourData::InitializeLocalVariables,
			&MFrontBehaviourParserCommon::standardModifier,true,true);
  } // end of MFrontBehaviourParserCommon:treatInitLocalVariables

  std::map<std::string,
	   std::pair<std::vector<std::string>,
		     std::vector<std::string> > >
  MFrontBehaviourParserCommon::getSpecificTargets(void)
  {
    using namespace std;
    return map<string,pair<vector<string>,vector<string> > >();
  } // end of MFrontBehaviourParserCommon::getSpecificTargets(void)

  void
  MFrontBehaviourParserCommon::disableVariableDeclaration(const Hypothesis h)
  {
    if(!this->mb.hasAttribute(h,MechanicalBehaviourData::allowsNewUserDefinedVariables)){
      this->mb.setAttribute(h,MechanicalBehaviourData::allowsNewUserDefinedVariables,false);
    }
  }

  void
  MFrontBehaviourParserCommon::setMinimalTangentOperator(void)
  {
    using namespace std;
    if(this->mb.getBehaviourType()!=MechanicalBehaviourDescription::FINITESTRAINSTANDARDBEHAVIOUR){
      const set<Hypothesis>& mh = this->mb.getDistinctModellingHypotheses();
      for(set<Hypothesis>::const_iterator ph=mh.begin();ph!=mh.end();++ph){
	// basic check
	if(this->mb.hasAttribute(*ph,MechanicalBehaviourData::hasConsistentTangentOperator)){
	  if(!this->mb.hasCode(*ph,MechanicalBehaviourData::ComputeTangentOperator)){
	  this->throwRuntimeError("MFrontBehaviourParserCommon::setMinimalTangentOperator",
				  "behaviour has attribute 'hasConsistentTangentOperator' but "
				  "no associated code");
	  }
	}
      }
      if(this->mb.getAttribute(MechanicalBehaviourDescription::requiresStiffnessTensor,false)){
	if(this->mb.getBehaviourType()==MechanicalBehaviourDescription::SMALLSTRAINSTANDARDBEHAVIOUR){
	  const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
	  // if the user provided a tangent operator, it won't be
	  // overriden
	  CodeBlock tangentOperator;
	  ostringstream code; 
	  code << "if(smt==ELASTIC){" << endl
	       << "this->Dt = this->D;" << endl
	       << "} else {" << endl
	       << "return false;" << endl
	       << "}" << endl;
	  tangentOperator.code = code.str();
	  this->mb.setCode(h,MechanicalBehaviourData::ComputeTangentOperator,
			   tangentOperator,
			   MechanicalBehaviourData::CREATEBUTDONTREPLACE,
			   MechanicalBehaviourData::BODY);
	  this->mb.setAttribute(h,MechanicalBehaviourData::hasConsistentTangentOperator,true,true);
	}
      }
    }
  } // end of MFrontBehaviourParserCommon::setMinimalTangentOperator

  void
  MFrontBehaviourParserCommon::setComputeFinalStressFromComputeFinalStressCandidateIfNecessary(void)
  {
    using namespace std;
    const set<Hypothesis>& mh = this->mb.getDistinctModellingHypotheses();
    // first treating specialised mechanical data
    for(set<Hypothesis>::const_iterator ph=mh.begin();ph!=mh.end();++ph){
      if(*ph!=ModellingHypothesis::UNDEFINEDHYPOTHESIS){
	if(!this->mb.hasCode(*ph,MechanicalBehaviourData::ComputeFinalStress)){
	  if(this->mb.hasCode(*ph,MechanicalBehaviourData::ComputeFinalStressCandidate)){
	    this->mb.setCode(*ph,MechanicalBehaviourData::ComputeFinalStress,
			     this->mb.getCodeBlock(*ph,MechanicalBehaviourData::ComputeFinalStressCandidate),
			     MechanicalBehaviourData::CREATE,
			     MechanicalBehaviourData::BODY);
	  }
	}
      }
    }
    // now treating the default hypothesis case
    if(!this->mb.areAllMechanicalDataSpecialised()){
      const Hypothesis h = ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      if(!this->mb.hasCode(h,MechanicalBehaviourData::ComputeFinalStress)){
	if(this->mb.hasCode(h,MechanicalBehaviourData::ComputeFinalStressCandidate)){
	  this->mb.setCode(h,MechanicalBehaviourData::ComputeFinalStress,
			   this->mb.getCodeBlock(h,MechanicalBehaviourData::ComputeFinalStressCandidate),
			   MechanicalBehaviourData::CREATEBUTDONTREPLACE,
			   MechanicalBehaviourData::BODY);
	}
      }
    }
  } // end of MFrontBehaviourParserCommon::setComputeFinalStressFromComputeFinalStressCandidateIfNecessary

} // end of namespace mfront
