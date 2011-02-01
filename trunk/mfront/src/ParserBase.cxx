/*!
 * \file   ParserBase.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 jun 2007
 */

#include<stdexcept>
#include<sstream>
#include<algorithm>
#include<iterator>

#include"TFEL/System/ProcessManager.hxx"

#include"MFront/MFront.hxx"
#include"MFront/ParserBase.hxx"
#include"MFront/ParserUtilities.hxx"
#include"MFront/MFrontMFrontLawInterface.hxx"

namespace mfront
{

  std::string
  ParserBase::variableModifier1(const std::string& var,const bool)
  {
    return var;
  } // end of ParserBase::variableModifier1

  std::string
  ParserBase::variableModifier2(const std::string& var,const bool)
  {
    return var;
  } // end of ParserBase::variableModifier2

  std::string
  ParserBase::variableModifier3(const std::string& var,const bool)
  {
    return var;
  } // end of ParserBase::variableModifier3

  std::string
  ParserBase::variableModifier4(const std::string& var,const bool)
  {
    return var;
  } // end of ParserBase::variableModifier4

  std::string
  ParserBase::variableModifier5(const std::string& var,const bool)
  {
    return var;
  } // end of ParserBase::variableModifier5

  ParserBase::ParserBase()
    : debugMode(false),
      verboseMode(false),
      warningMode(false)
  {
    this->reserveName("std");
    this->reserveName("real");
    this->reserveName("policy");
  }

  ParserBase::~ParserBase()
  {} // end of ParserBase::~ParserBase

  void
  ParserBase::throwRuntimeError(const std::string& method,
				const std::string& m)
  {
    using namespace std;
    if(this->current==this->fileTokens.end()){
      --(this->current);
    }
    string msg(method);
    if(!m.empty()){
      msg +=" : " + m;
    }
    msg += "\nError at line " + toString(this->current->line);
    throw(runtime_error(msg));
  } // end of ParserBase::throwRuntimeError

  void
  ParserBase::checkNotEndOfFile(const std::string& method,
				const std::string& error){
    using namespace std;
    if(this->current==this->fileTokens.end()){
      --(this->current);
      string msg(method+" : ");
      msg += "unexpected end of file.";
      if(!error.empty()){
	msg += "\n"+error;
      }
      msg += "\nError at line " + toString(this->current->line);
      throw(runtime_error(msg));
    }
  } // end of ParserBase::checkNotEndOfFile

  void
  ParserBase::readSpecifiedToken(const std::string& method,
				 const std::string& token)
  {
    using namespace std;
    this->checkNotEndOfFile(method,"Expected '"+token+"'.");
    if(this->current->value!=token){
      string msg(method+" : ");
      msg += "expected '"+token+"', read ";
      msg += this->current->value;
      msg += ".\n";
      msg += "Error at line : ";
      msg += toString(this->current->line);
      throw(runtime_error(msg));
    }
    ++(this->current);
  } // end of ParserBase::readSpecifiedToken

  std::string
  ParserBase::readUntilEndOfInstruction(void)
  {
    using namespace std;
    string res;
    while((this->current!=this->fileTokens.end())&&
	  (this->current->value != ";")){
      res+=this->current->value;
      res+=" ";
      ++(this->current);
    }
    this->checkNotEndOfFile("ParserBase::readUntilEndOfInstruction",
			    "Missing ';' delimiter.");
    if(!res.empty()){
      res.erase(res.length()-1);
    }
    ++(this->current);
    return res;
  }

  std::string
  ParserBase::readOnlyOneToken(void)
  {
    using namespace std;
    string res;
    this->checkNotEndOfFile("ParserBase::readOnlyOneToken",
			    "Expected a word.");
    if(this->current->value==";"){
      string msg("ParserBase::readOnlyOneToken : ");
      msg += "No word read";
      throw(runtime_error(msg));
    }
    res = this->current->value;
    ++(this->current);
    this->readSpecifiedToken("ParserBase::readOnlyOneToken",";");
    return res;
  } // end of ParserBase::readOnlyOneToken

  void
  ParserBase::readNextBlock(std::string& res1,
			    std::string& res2,
			    std::string (ParserBase::* modifier1)(const std::string&,const bool),
			    std::string (ParserBase::* modifier2)(const std::string&,const bool),
			    const bool addThisPtr,
			    const std::string delim1,
			    const std::string delim2,
			    const bool allowSemiColon,
			    const bool registerLine)
  {
    TokensContainer::const_iterator pb = this->current;
    res1 = this->readNextBlock(addThisPtr,delim1,delim2,allowSemiColon,registerLine,modifier1);
    this->current = pb;
    res2 = this->readNextBlock(addThisPtr,delim1,delim2,allowSemiColon,registerLine,modifier2);
  } // end of ParserBase::readNextBlock

  std::string
  ParserBase::readNextBlock(std::string (ParserBase::* modifier)(const std::string&,const bool),
			    const bool addThisPtr,
			    const std::string delim1,
			    const std::string delim2,
			    const bool allowSemiColon,
			    const bool registerLine)
  {
    return this->readNextBlock(addThisPtr,delim1,delim2,allowSemiColon,registerLine,modifier);
  } // end of ParserBase::readNextBlock

  std::string
  ParserBase::readNextBlock(const bool addThisPtr,
			    const std::string delim1,
			    const std::string delim2,
			    const bool allowSemiColon,
			    const bool registerLine,
			    std::string (ParserBase::* modifier)(const std::string&,const bool))
  {
    using namespace std;
    string res;
    unsigned short currentLine;
    unsigned short openedBlock;
    TokensContainer::const_iterator previous;
    openedBlock=0;
    this->readSpecifiedToken("ParserBase::readNextBlock",delim1);
    this->checkNotEndOfFile("ParserBase::readNextBlock",
			    "Expected a '"+delim2+"'.");
    if((this->current->value==";")&&(!allowSemiColon)){
      string msg("ParserBase::readNextBlock : ");
      msg += "read ';' before the end of block.\n";
      msg += "Number of block opened : ";
      msg += toString(openedBlock);
      throw(runtime_error(msg));
    }
    if(this->current->value==delim1){
      ++openedBlock;
    }
    if(this->current->value==delim2){
      ++(this->current);
      return res;
    }
    currentLine = this->current->line;
    if((registerLine)&&(!this->debugMode)){
      res  = "#line ";
      res += toString(currentLine);
      res += " \"";
      res += this->fileName;
      res += "\"\n";
    }
    if(this->staticVarNames.find(this->current->value)!=this->staticVarNames.end()){
      res += this->className;
      res += "::";
      res += this->current->value;
    } else if(this->varNames.find(this->current->value)!=this->varNames.end()){
      string currentValue;
      if(modifier!=0){
	currentValue = (this->*modifier)(this->current->value,addThisPtr);
      } else {
	if(addThisPtr){
	  currentValue = "this->"+this->current->value;
	} else {
	  currentValue = this->current->value;
	}
      }
      previous = this->current;
      --previous;
      if(previous->value=="*"){
	res += "("+currentValue+")";
      } else {
	res += currentValue;
      }
    } else {
      res += this->current->value;
    }
    res += " ";
    ++(this->current);
    while((!((this->current->value==delim2)&&(openedBlock==0)))&&
	  (this->current!=this->fileTokens.end())){
      
      if(currentLine!=this->current->line){
	currentLine = this->current->line;
	if((registerLine)&&(!this->debugMode)){
	  res += "\n";
	  res += "#line ";
	  res += toString(currentLine);
	  res += " \"";
	  res += this->fileName;
	  res += "\"\n";
	}  else {
	  res += "\n";
	}
      }
      if((this->current->value==";")&&(!allowSemiColon)){
	string msg("ParserBase::readNextBlock : ");
	msg += "read ';' before the end of block.\n";
	msg += "Number of block opened : ";
	msg += toString(openedBlock);
	throw(runtime_error(msg));
      }
      if(this->staticVarNames.find(this->current->value)!=this->staticVarNames.end()){
	previous = this->current;
	--previous;
	if((previous->value!="->")&&
	   (previous->value!=".")&&
	   (previous->value!="::")){
	  res += this->className;
	  res += "::";
	}
	res += this->current->value;
      } else if(this->varNames.find(this->current->value)!=this->varNames.end()){
	string currentValue;
	previous = this->current;
	--previous;
	if(modifier!=0){
	  currentValue = (this->*modifier)(this->current->value,addThisPtr);
	} else {
	  if(addThisPtr){
	    if((previous->value=="->")||
	       (previous->value==".")||
	       (previous->value=="::")){
	      currentValue = this->current->value;
	    } else {
	      if(previous->value=="*"){
		currentValue = "(this->"+this->current->value+')';
	      } else {
		currentValue = "this->"+this->current->value;
	      }
	    }
	  } else {
	    currentValue = this->current->value;
	  }
	}
	res += currentValue;
      } else {
	res += this->current->value;
      }
      res+=" ";
      if(this->current->value==delim1){
	++openedBlock;
      }
      if(this->current->value==delim2){
	--openedBlock;
      }
      ++(this->current);
    }
    if(this->current==this->fileTokens.end()){
      --(this->current);
      string msg("ParserBase::readNextBlock : ");
      msg += "Expected the end of a block.\n";
      msg += "Number of block opened : ";
      msg += toString(openedBlock);
      throw(runtime_error(msg));
    }
    ++(this->current);
    return res;
  } // end of ParserBase::readNextBlock

  void ParserBase::readVarList(const std::string& type,
			       VarContainer& cont,
			       const bool addIncrementVar)
  {
    using namespace std;
    string varName;
    unsigned short lineNumber;
    bool endOfTreatement=false;
    while((this->current!=this->fileTokens.end())&&
	  (!endOfTreatement)){
      if(!isValidIdentifier(this->current->value)){
	this->throwRuntimeError("ParserBase::readVarList : ",
				"variable given is not valid (read '"+this->current->value+"').");
      }
      varName = this->current->value;
      lineNumber = this->current->line;
      ++(this->current);
      this->checkNotEndOfFile("ParserBase::readVarList");
      if(this->current->value==","){
	++(this->current);
      } else if (this->current->value==";"){
	endOfTreatement=true;
	++(this->current);
      } else {
	this->throwRuntimeError("ParserBase::readVarList",
				", or ; expected afer '"+varName+"'");
      }
      this->registerVariable(varName);
      cont.push_back(VarHandler(type,varName,lineNumber));
      if(addIncrementVar){
	string incrVarName("d");
	incrVarName += varName;	
	this->registerVariable(incrVarName);
      }
    }
    if(!endOfTreatement){
      --(this->current);
      this->throwRuntimeError("ParserBase::readVarList",
			      "Expected ';' before end of file");
    }
  }

  void ParserBase::readVarList(VarContainer& cont,
			       const bool addIncrementVar)
  {
    using namespace std;
    string type;
    set<string>::iterator p;
    this->checkNotEndOfFile("ParserBase::readVarList",
			    "Cannot read type of varName.\n");
    type=this->current->value;
    if(!isValidIdentifier(type,false)){
      --(this->current);
      this->throwRuntimeError("ParserBase::readVarList",
			      "Type given is not valid.");
    }
    ++(this->current);
    this->readVarList(type,cont,addIncrementVar);
  } // end of ParserBase::readVarList

  std::vector<std::string>
  ParserBase::readArrayOfString(const std::string& method)
  {
    using namespace std;
    using namespace tfel::utilities;
    vector<string> res;
    this->readSpecifiedToken(method,"{");
    this->checkNotEndOfFile(method,"Expected '}'");
    while(this->current->value!="}"){
      if(this->current->flag!=Token::String){
	this->throwRuntimeError(method,"Expected a string");
      }
      res.push_back(this->current->value.substr(1,this->current->value.size()-2));
      ++(this->current);
      this->checkNotEndOfFile(method,"Expected '}'");
      if(!((this->current->value=="}")||
	   (this->current->value==","))){
	this->throwRuntimeError(method,
				"Expected ',' or '}', read '"+this->current->value+"'");
      }
      if(this->current->value==","){
	++(this->current);
	this->checkNotEndOfFile(method,"Expected '}'");
	if(this->current->value=="}"){
	  this->throwRuntimeError(method,"Expected a string");
	}
      }
    }
    ++(this->current);
    return res;
  } // end of ParserBase::readArrayOfString

  std::vector<std::string>
  ParserBase::readStringOrArrayOfString(const std::string& method)
  {
    using namespace std;
    using namespace tfel::utilities;
    vector<string> res;
    this->checkNotEndOfFile(method,"Expected a string or '{'");
    if(this->current->value=="{"){
      return this->readArrayOfString(method);
    }
    if(this->current->flag!=Token::String){
      this->throwRuntimeError(method,"Expected a string");
    }
    res.push_back(this->current->value.substr(1,this->current->value.size()-2));
    ++(this->current);
    return res;
  } // end of ParserBase::readStringOrArrayOfString

  void
  ParserBase::treatLink(void){
    using namespace std;
    vector<string> nlink;
    vector<string>::const_iterator p;
    nlink = readStringOrArrayOfString("ParserBase::treatLink");
    this->readSpecifiedToken("ParserBase::treatLink",";");
    for(p=nlink.begin();p!=nlink.end();++p){
      if(find(this->librariesDependencies.begin(),
	      this->librariesDependencies.end(),*p)==this->librariesDependencies.end()){
	this->librariesDependencies.push_back(*p);
      }
    }
  } // end of ParserBase::treatLink

  void
  ParserBase::treatMFront(void){
    using namespace std;
    using namespace tfel::system;
    ProcessManager m;
    vector<string> vfiles;
    vector<string> vinterfaces;
    vector<string>::const_iterator p;
    ostringstream cmd;
    this->readSpecifiedToken("ParserBase::treatMfront","{");
    vfiles = this->readStringOrArrayOfString("ParserBase::treatMfront");
    this->checkNotEndOfFile("ParserBase::treatMfront","Expected '}'");
    if(!((this->current->value=="}")||
	 (this->current->value==","))){
      this->throwRuntimeError("ParserBase::treatMfront",
			      "Expected ',' or '}', read '"+this->current->value+"'");
    }
    if(this->current->value==","){
      ++(this->current);
     vinterfaces = this->readStringOrArrayOfString("ParserBase::treatMfront");
    }
    this->readSpecifiedToken("ParserBase::treatMfront","}");
    this->readSpecifiedToken("ParserBase::treatMfront",";");
    cmd << MFront::getCallingName() << " --interface=";
    for(p=vinterfaces.begin();p!=vinterfaces.end();){
      cmd << *p;
      if((++p)!=vinterfaces.end()){
	cmd << ",";
      }
    }
    cmd << " ";
    copy(vfiles.begin(),vfiles.end(),ostream_iterator<string>(cmd," "));
    m.execute(cmd.str());
  } // end of ParserBase::treatMfront

  std::string
  ParserBase::readSpecifiedValue(const std::string& file,
				 const std::string& value)
  {
    using namespace std;
    vector<string> values(1,value);
    return this->readSpecifiedValues(file,values)[1];
  } // end of ParserBase::readSpecifiedValue

  std::vector<std::string>
  ParserBase::readSpecifiedValues(const std::string& file,
				  const std::string& value1,
				  const std::string& value2)
  {
    using namespace std;
    vector<string> values(2);
    values[0] = value1;
    values[1] = value2;
    return this->readSpecifiedValues(file,values);
  } // end of ParserBase::readSpecifiedValues

  std::vector<std::string>
  ParserBase::readSpecifiedValues(const std::string& file,
				  const std::vector<std::string>& values)
  {
    using namespace std;
    using namespace tfel::utilities;
    CxxTokenizer cfile;
    vector<string> res(values.size());
    string value;
    CxxTokenizer::TokensContainer::const_iterator pt;
    CxxTokenizer::TokensContainer::const_iterator pte;
    std::vector<std::string>::const_iterator p;
    cfile.openFile(file);
    cfile.stripComments();
    pt  = cfile.begin();
    pte = cfile.end();
    while(pt!=pte){
      if((p=find(values.begin(),values.end(),pt->value))!=values.end()){
	if(pt!=cfile.begin()){
	  CxxTokenizer::TokensContainer::const_iterator ptp = pt;
	  --ptp;
	  if(ptp->value!=";"){
	    string msg("ParserBase::readSpecifiedValues : ");
	    msg += "the keyword @Parser does not begin a new instruction.";
	    throw(runtime_error(msg));
	  }
	}
	++pt;
	if(pt==pte){
	  ostringstream msg;
	  msg << "ParserBase::readSpecifiedValues : ";
	  msg << "unexepected end of file '"+file+"' (exepected "+*p+").\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	if(pt->value==";"){
	  ostringstream msg;
	  msg << "ParserBase::readSpecifiedValues : ";
	  msg << "unexepected end of line '"+file+"' (exepected "+*p+").\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	value = pt->value;
	++pt;
	if(pt==pte){
	  ostringstream msg;
	  msg << "ParserBase::readSpecifiedValues : ";
	  msg << "unexepected end of file '"+file+"' (exepected library name or ';').\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	if(pt->value!=";"){
	  ostringstream msg;
	  msg << "ParserBase::readSpecifiedValues : ";
	  msg << "unexepected token '" << pt->value << "' (exepected ';').\n";
	  msg << "Error at line " << pt->line << ".";
	  throw(runtime_error(msg.str()));
	}
	res[static_cast<vector<string>::size_type>(p-values.begin())]=value;
      }
      ++pt;
    }
    return res;
  } // end of ParserBase::readSpecifiedValues

  void
  ParserBase::registerVariable(const std::string& v)
  {
    using namespace std;
    if(!CxxTokenizer::isValidIdentifier(v)){
      string msg("ParserBase::registerVariable : ");
      msg += "variable name '"+v+"' is invalid";
      throw(runtime_error(msg));
    }
    if(!this->varNames.insert(v).second){
      string msg("ParserBase::registerVariable : ");
      msg += "variable '"+v+"' already registred";
      throw(runtime_error(msg));
    }
    this->reserveName(v);
  } // end of ParserBase::registerVariable

  void
  ParserBase::registerStaticVariable(const std::string& v)
  {
    using namespace std;
    if(!CxxTokenizer::isValidIdentifier(v)){
      string msg("ParserBase::registerStaticVariable : ");
      msg += "variable name '"+v+"' is invalid";
      throw(runtime_error(msg));
    }
    if(!this->staticVarNames.insert(v).second){
      string msg("ParserBase::registerStaticVariable : ");
      msg += "static variable '"+v+"' already registred";
      throw(runtime_error(msg));
    }
    this->reserveName(v);
  } // end of ParserBase::registerStaticVariable

  void
  ParserBase::reserveName(const std::string& w)
  {
    using namespace std;
    if(!this->reservedNames.insert(w).second){
      string msg("ParserBase::reserveName : ");
      msg += "name '"+w+"' already reserved";
      throw(runtime_error(msg));
    }
  } // end of ParserBase::reserveName

  void
  ParserBase::treatMaterialLaw(void){
    using namespace std;
    using namespace tfel::system;
    MFrontMFrontLawInterface minterface;
    ProcessManager m;
    vector<string> vfiles;
    vector<string>::const_iterator p;
    ostringstream cmd;
    vfiles = this->readStringOrArrayOfString("ParserBase::treatMfront");
    this->readSpecifiedToken("ParserBase::treatMfront",";");
    cmd << MFront::getCallingName() << " --interface=mfront ";
    copy(vfiles.begin(),vfiles.end(),ostream_iterator<string>(cmd," "));
    m.execute(cmd.str());
    for(p=vfiles.begin();p!=vfiles.end();++p){
      const vector<string>& res = this->readSpecifiedValues(*p,"@Material","@Law");
      string mname;
      if(res[0].empty()){
	mname = res[1];
      } else {
	mname = res[0]+"_"+res[1];
      }
      this->includes+= "#include\"";
      this->includes+= minterface.getHeaderFileName(res[0],res[1]);
      this->includes+= ".hxx\"\n";
      if(find(this->materialLaws.begin(),
	      this->materialLaws.end(),mname)==this->materialLaws.end()){
	this->materialLaws.push_back(mname);
      }
    }
    if(find(this->librariesDependencies.begin(),
	    this->librariesDependencies.end(),"-lMFrontMaterialLaw")==this->librariesDependencies.end()){
      this->librariesDependencies.push_back("-lMFrontMaterialLaw");
    }
  } // end of ParserBase::treatMaterialLaw

  void
  ParserBase::treatDescription(void)
  {
    using namespace std;
    unsigned short currentLine;
    if(!this->description.empty()){
      string msg("ParserBase::treatDescription : ");
      msg += "@Description shall only be called once.";
      throw(runtime_error(msg));
    }
    this->readSpecifiedToken("ParserBase::treatDescription","{");
    this->checkNotEndOfFile("ParserBase::treatDescription");
    currentLine = this->current->line;
    this->description = "* ";
    while((this->current->value!="}")&&
	  (this->current!=this->fileTokens.end())){
      if(this->current->value=="{"){
	this->throwRuntimeError("ParserBase::treatDescription",
				"'{' is not allowed in description.");
      }
      if(currentLine!=this->current->line){
	this->description+="\n* ";
	currentLine=this->current->line;
      }
      this->description+=this->current->value;
      this->description+=" ";
      ++(this->current);
    }
    if(this->current==this->fileTokens.end()){
      --(this->current);
      this->throwRuntimeError("ParserBase::treatDescription",
			      "File ended before the end of description.");
    }
    ++(this->current);
  } // end of ParserBase::Description

  void ParserBase::treatAuthor(void)
  {
    this->authorName = this->readUntilEndOfInstruction();
  } // end of ParserBase::treatAuthor

  void ParserBase::treatDate(void)
  {
    this->date = this->readUntilEndOfInstruction();
  } // end of ParserBase::treatDate

  void ParserBase::treatUnknownKeyword(void)
  {
    using namespace std;
    --(this->current);
    this->throwRuntimeError("ParserBase::treatUnknownKeyword",
			    "unknown keyword read ('"+this->current->value+"')");
  } // end of ParserBase::treatUnknownKeyword

  void ParserBase::treatIncludes(void)
  {
    if(!includes.empty()){
      this->includes += "\n";
    }
    this->includes += this->readNextBlock();
  }

  void
  ParserBase::treatMembers(void)
  {
    this->members = this->readNextBlock();
  }

  void
  ParserBase::treatParser(void)
  {
    this->readUntilEndOfInstruction();
  } // end of ParserBase::treatParser

  void
  ParserBase::treatSources(void)
  {
    this->sources = this->readNextBlock();
  } // end of ParserBase::treatSources(void)

  void
  ParserBase::treatPrivate(void)
  {
    this->privateCode = this->readNextBlock();
  } // end of ParserBase::treatSources(void)

  void
  ParserBase::treatStaticVar(void)
  {
    using namespace std;
    string type;
    string name;
    unsigned short line;
    long double value;
    this->checkNotEndOfFile("ParserBase::treatStaticVar",
			    "Cannot read type of static variable.");
    type=this->current->value;
    if(!isValidIdentifier(type,false)){
      --(this->current);
      this->throwRuntimeError("ParserBase::treatStaticVar",
			      "type given is not valid.");
    }
    ++(this->current);
    this->checkNotEndOfFile("ParserBase::treatStaticVar",
			    "Cannot read variable name.");
    name = this->current->value;
    if(!isValidIdentifier(name)){
      this->throwRuntimeError("ParserBase::treatStaticVar",
			      "Variable name '"+name+"' is not valid.");
    }
    line = this->current->line;
    ++(this->current);
    this->readSpecifiedToken("ParserBase::treatStaticVar","=");
    this->checkNotEndOfFile("ParserBase::treatStaticVar",
			    "Expected to read value of variable.");
    istringstream tmp(this->current->value);
    tmp >> value;
    if(!tmp&&(!tmp.eof())){
      this->throwRuntimeError("ParserBase::treatStaticVar",
			      "Could not read of variable '"+name+"'");
    }
    ++(this->current);
    this->readSpecifiedToken("ParserBase::treatStaticVar",";");
    this->registerStaticVariable(name);
    this->staticVars.push_back(StaticVarHandler(type,name,line,value));
  } // end of ParserBase::treatStaticVar

  void
  ParserBase::ignoreKeyWord(const std::string& key)
  {
    using namespace std;
    this->checkNotEndOfFile("ParserBase::ignoreKeyWord",
			    "error while treating keyword '"+key+"' ");
    while((this->current->value!="{")&&(this->current->value!=";")){
      ++(this->current);
      this->checkNotEndOfFile("ParserBase::ignoreKeyWord",
			      "error while treating keyword '"+key+"' ");
    }
    if(this->current->value=="{"){
      unsigned short openedBrackets = 1;
      while(!((this->current->value=="}")&&
	      (openedBrackets==0))){
	++(this->current);
	this->checkNotEndOfFile("ParserBase::ignoreKeyWord",
				"error while treating keyword '"+key+"' ");
	if(this->current->value=="{"){
	  ++openedBrackets;
	}
	if(this->current->value=="}"){
	  --openedBrackets;
	}
      }
    }
    ++(this->current);
  } // end of ParserBase::ignoreKeyWord

  void
  ParserBase::treatMaterial(void)
  {
    using namespace std;
    if(!this->material.empty()){
      string msg("ParserBase::treatMaterial : ");
      msg += "material name alreay defined";
      throw(runtime_error(msg));
    }
    this->material = this->readOnlyOneToken();
    if(!CxxTokenizer::isValidIdentifier(this->material,true)){
      string msg("ParserBase::treatMaterial : ");
      msg += "invalid material name '"+this->material+"'";
      throw(runtime_error(msg));
    }
  } // end of ParserBase::treatMaterial

  void
  ParserBase::treatLibrary(void)
  {
    using namespace std;
    if(!this->library.empty()){
      string msg("ParserBase::treatLibrary : ");
      msg += "library name alreay defined";
      throw(runtime_error(msg));
    }
    this->library = this->readOnlyOneToken();
    if(!CxxTokenizer::isValidIdentifier(this->library,true)){
      string msg("ParserBase::treatLibrary : ");
      msg += "invalid library name '"+this->library+"'";
      throw(runtime_error(msg));
    }
  } // end of ParserBase::treatLibrary

  double
  ParserBase::readDouble(void)
  {
    using namespace std;
    double value;
    this->checkNotEndOfFile("ParserBase::readDouble");
    istringstream flux(this->current->value);
    flux >> value;
    if((flux.fail())||(!flux.eof())){
      this->throwRuntimeError("ParserBase::readDouble",
			      "Failed to read double value from token '"+
			      this->current->value+"'.");
    }
    ++(this->current);
    return value;
  } // end of ParserBase::readDouble

} // end of namespace mfront
