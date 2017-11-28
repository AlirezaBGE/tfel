/*!
 * \file   mfront/src/MFront.cxx
 * \brief  
 * \author Thomas Helfer
 * \date   09 nov 2006
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<stdexcept>
#include<algorithm>
#include<stdexcept>
#include<iostream>
#include<iterator>
#include<cstdlib>
#include<cassert>
#include<fstream>
#include<sstream>
#include<cstring>
#include<string>
#include<cerrno>
#include<memory>

#include"TFEL/Raise.hxx"
#include"TFEL/Config/GetInstallPath.hxx"
#include"TFEL/Utilities/TerminalColors.hxx"
#include"TFEL/Utilities/StringAlgorithms.hxx"
#include"TFEL/System/System.hxx"
#include"TFEL/System/ExternalLibraryManager.hxx"

#include"MFront/MFrontHeader.hxx"
#include"MFront/MFrontLogStream.hxx"
#include"MFront/TargetsDescription.hxx"
#include"MFront/DSLFactory.hxx"
#include"MFront/MaterialPropertyInterfaceFactory.hxx"
#include"MFront/BehaviourInterfaceFactory.hxx"
#include"MFront/ModelInterfaceFactory.hxx"
#include"MFront/MFrontLock.hxx"
#include"MFront/MFrontDebugMode.hxx"
#include"MFront/MFrontUtilities.hxx"
#include"MFront/CMakeGenerator.hxx"
#include"MFront/MakefileGenerator.hxx"
#include"MFront/MFront.hxx"

namespace mfront{

  /*!
   * \return the path to the documentation file if available.
   * If not, an empty string is returned
   * \param[in] pn : parser name
   * \param[in] k  : keyword
   */
  static std::string
  getDocumentationFilePath(const std::string& pn,
			   const std::string& k)
  {
    const auto root = tfel::getInstallPath();
#ifdef TFEL_APPEND_VERSION
    auto fn = root+"/share/doc/mfront-" VERSION "/"+pn+"/"+k.substr(1)+".md";
#else /* TFEL_APPEND_VERSION */
    auto fn = root+"/share/doc/mfront/"+pn+"/"+k.substr(1)+".md";
#endif /* TFEL_APPEND_VERSION */
    std::ifstream desc{fn};
    if(desc){
      return fn;
    }
#ifdef TFEL_APPEND_VERSION
    fn = root+"/share/doc/mfront-" VERSION "/"+k.substr(1)+".md";
#else  /* TFEL_APPEND_VERSION */
    fn = root+"/share/doc/mfront/"+k.substr(1)+".md";
#endif /* TFEL_APPEND_VERSION */
    desc.open(fn);
    if(desc){
      return fn;
    }
    return "";
  }

  std::string MFront::getVersionDescription() const
  {
    return MFrontHeader::getHeader();
  }

  std::string MFront::getUsageDescription() const
  {
    return "Usage: "+this->programName+" [options] [files]";
  }

  const tfel::utilities::Argument&
  MFront::getCurrentCommandLineArgument() const
  {
    return *(this->currentArgument);
  }

  void MFront::doNothing()
  {} // end of MFront::doNothing
  
  void MFront::treatUnknownArgument()
  {
    if(!MFrontBase::treatUnknownArgumentBase()){
#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
      ArgumentParserBase<MFront>::treatUnknownArgument();
#else
      auto a = static_cast<const std::string&>(this->getCurrentCommandLineArgument());
      std::cerr << "mfront: unsupported option '" 
		<< a << '\'' << std::endl;
      exit(EXIT_FAILURE);
#endif /* __CYGWIN__ */
    }
  } // end of MFront::treatUnknownArgument()

  void MFront::treatGenerator()
  {
    const auto g = this->currentArgument->getOption();
    if(g=="make"){
      this->generator=MAKE;
    } else if(g=="cmake"){
      this->generator=CMAKE;
    } else {
      tfel::raise("MFront::treatGenerator: "
		  "unsupported generator '"+g+"'");
    }
  } // end of MFront::treatGenerator
  
  void MFront::treatMake()
  {
    this->genMake = true;
  } // end of MFront::treatMake

  void MFront::treatBuild()
  {
    this->genMake   = true;
    this->buildLibs = true;
  } // end of MFront::treatBuild

  void MFront::treatClean()
  {
    this->genMake   = true;
    this->cleanLibs = true;
  } // end of MFront::treatBuild

  void MFront::treatOMake()
  {
    this->genMake = true;
    const auto level = this->currentArgument->getOption();
    if(!level.empty()){
      if(level=="level2"){
	this->opts.olevel = GeneratorOptions::LEVEL2;
      } else if(level=="level0"){
	this->opts.olevel = GeneratorOptions::LEVEL0;
      } else if(level=="level1"){
	this->opts.olevel = GeneratorOptions::LEVEL1;
      } else {
	tfel::raise("MFront::treatOMake: "
		    "unsupported value '"+level+
		    "' for the --omake option");
      }
    } else {
      this->opts.olevel = GeneratorOptions::LEVEL1;
    }
  } // end of MFront::treatOMake

  void MFront::treatOBuild()
  {
    this->genMake   = true;
    this->buildLibs = true;
    const auto level = this->currentArgument->getOption();
    if(!level.empty()){
      if(level=="level2"){
	this->opts.olevel = GeneratorOptions::LEVEL2;
      } else if(level=="level0"){
	this->opts.olevel = GeneratorOptions::LEVEL0;
      } else if(level=="level1"){
	this->opts.olevel = GeneratorOptions::LEVEL1;
      } else {
	tfel::raise("MFront::treatOBuild: "
		    "unsupported value '"+level+
		    "' for the --obuild option");
      }
    } else {
      this->opts.olevel = GeneratorOptions::LEVEL1;
    }
  } // end of MFront::treatOBuild

  void MFront::treatListParsers()
  {
    std::cout << "available dsl: \n";
    auto& parserFactory = DSLFactory::getDSLFactory();
    const auto& parsers = parserFactory.getRegistredParsers();
    auto p = parsers.begin();
    while(p!=parsers.end()){
      auto tmp = "- " + *p;
      if(tmp.size()<=32){
	tmp.insert(tmp.size(),32-tmp.size(),' ');
      }
      std::cout << tmp << ": " << parserFactory.getParserDescription(*p) << ".\n";
      ++p;
    }
    exit(EXIT_SUCCESS);
  } // end of MFront::treatListParsers

  void MFront::treatSilentBuild()
  {
    const auto& o = this->currentArgument->getOption();
    tfel::raise_if(o.empty(),"MFront::treatSilentBuild: "
		   "no argument given to the "
		   "--silentBuild option");
    if(o=="on"){
      this->opts.silentBuild=true;
    } else if(o=="off"){
      this->opts.silentBuild=false;
    } else {
      tfel::raise("MFront::treatSilentBuild: "
		  "unsupported argument '"+o+
		  "' given to the --silentBuild option");
    }
  } // end of MFront::treatSilentBuild

  
  void MFront::treatTarget()
  {
    using tfel::utilities::tokenize;
    const auto& t = tokenize(this->currentArgument->getOption(),',');
    tfel::raise_if(t.empty(),"MFront::treatTarget: "
		   "no argument given to the "
		   "--target option");
    this->specifiedTargets.insert(t.begin(),t.end());
    this->genMake   = true;
    this->buildLibs = true;
  } // end of MFront::treatTarget

  void MFront::treatOTarget()
  {
    tfel::raise_if(this->currentArgument==this->args.end(),
		   "MFront::treatTarget: "
		   "no argument given to the "
		   "--otarget option");
    this->opts.olevel = GeneratorOptions::LEVEL1;
    this->treatTarget();
  } // end of MFront::treatTarget
  
#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
  void MFront::treatWin32()
  {
    this->opts.sys = "win32";
  } // end of MFront::treatWin32
#endif /* __CYGWIN__ */

  void MFront::registerArgumentCallBacks()
  {
    this->registerNewCallBack("--no-terminate-handler",&MFront::doNothing,
			      "don't set a terminate handler");
    this->registerNewCallBack("--no-gui",&MFront::doNothing,
			      "does not report errors by a message box");
    this->registerNewCallBack("--verbose",&MFront::treatVerbose,
			      "set verbose output",true);
    this->registerNewCallBack("--list-parsers",&MFront::treatListParsers,"list all available domain specific languages (deprecated)");
    this->registerNewCallBack("--list-dsl",&MFront::treatListParsers,"list all available domain specific languages");
    this->registerNewCallBack("--help-commands",&MFront::treatHelpCommands,
			      "display the help associated with all the keywords for the given domain specific language and exits",true);
    this->registerNewCallBack("--help-keywords",&MFront::treatHelpCommands,
			      "display the help associated with all the keywords for the given domain specific language and exits",true);
    this->registerNewCallBack("--help-commands-list",&MFront::treatHelpCommandsList,
			      "list all keywords for the given domain specific language and exits",true);
    this->registerNewCallBack("--help-keywords-list",&MFront::treatHelpCommandsList,
			      "list all keywords for the given domain specific language and exits",true);
    this->registerNewCallBack("--define","-D",&MFront::treatDefine,
			      "define a macro from the command line",true);
    this->registerNewCallBack("--include","-I",&MFront::treatSearchPath,
			      "add a new path at the beginning of the search paths",true);
    this->registerNewCallBack("--search-path",&MFront::treatSearchPath,
			      "add a new path at the beginning of the search paths",true);
    this->registerNewCallBack("--help-keyword",&MFront::treatHelpCommand,
			      "display the help associated for the given domain specific language and exits",true);
    this->registerNewCallBack("--help-command",&MFront::treatHelpCommand,
			      "display the help associated for the given domain specific language and exits",true);
    this->registerNewCallBack("--debug",&MFront::treatDebug,
			      "set debug mode (remove references to initial file)");
    this->registerNewCallBack("--warning","-W",&MFront::treatWarning,"print warnings");
    this->registerNewCallBack("--pedantic",&MFront::treatPedantic,
			      "print pedantic warning message");
    this->registerNewCallBack("--interface","-i",&MFront::treatInterface,
			      "specify which interface to use",true);
#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    this->registerNewCallBack("--def-file",&MFront::treatDefFile,
			      "outputs def file associated with the libraries given in arguments (separated by commas)",true);
#endif    
    this->registerNewCallBack("--silent-build",&MFront::treatSilentBuild,
			      "active or desactivate silent build",true);
    this->registerNewCallBack("--make",&MFront::treatMake,
			      "generate MakeFile (see also --build)");
    this->registerNewCallBack("--build",&MFront::treatBuild,
			      "generate MakeFile and build libraries");
    this->registerNewCallBack("--omake","-m",&MFront::treatOMake,
			      "generate build file with optimized compilations flags (see also --obuild)",true);
    this->registerNewCallBack("--obuild","-b",&MFront::treatOBuild,
			      "generate build file with optimized compilations flags and build libraries",true);
    this->registerNewCallBack("--target","-t",&MFront::treatTarget,
			      "generate build file and build the specified target",true);
    this->registerNewCallBack("--otarget",&MFront::treatOTarget,
			      "generate build file with optimized compilations flags "
			      "and build the specified target",true);
    this->registerNewCallBack("--clean",&MFront::treatClean,
			      "generate build file and clean libraries");
    this->registerNewCallBack("--generator","-G",&MFront::treatGenerator,
			      "choose build system",true);
#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    this->registerNewCallBack("--nodeps",&MFront::treatNoDeps,
			      "don't generate compilation dependencies");
#endif /* __CYGWIN__ */
    this->registerNewCallBack("--nomelt",&MFront::treatNoMelt,
			      "don't melt librairies sources");
#if !(defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    this->registerNewCallBack("--win32",&MFront::treatWin32,
			      "specify that the target system is win32");
#endif /* __CYGWIN__ */
  } // end of MFront::registerArgumentCallBacks

  MFront::MFront() = default;
  
  MFront::MFront(const int argc, const char *const *const argv)
    : MFront()
  {
    this->setArguments(argc,argv);
    this->registerArgumentCallBacks();
    this->parseArguments();
  } // end of MFront::MFront

  void MFront::treatDefine()
  {
    const auto& o = this->currentArgument->getOption();
    tfel::raise_if(o.empty(),"MFront::treatDefine: "
		   "no macro definition given");
    this->defines.insert(o);
  } // end of MFront::treatDefine
  
  void MFront::treatHelpCommandsList()
  {
    using tfel::utilities::TerminalColors;
    auto& f = DSLFactory::getDSLFactory();
    const auto& o = this->currentArgument->getOption();
    tfel::raise_if(o.empty(),"MFront::treatHelpCommandsList: "
		   "no parser name given");
    std::shared_ptr<AbstractDSL> p{f.createNewParser(o)};
    std::vector<std::string> keys;
    p->getKeywordsList(keys);
    std::string::size_type msize = 0;
    for(const auto& k:keys){
      msize = std::max(msize,k.size());
    }
    for(const auto& key: keys){
      if(key==";"){continue;}
      const auto fp = getDocumentationFilePath(o,key);
      auto k = key;
      k.resize(msize,' ');
      std::cout << k << "  ";
      if(!fp.empty()){
	std::cout.write(TerminalColors::Green,sizeof(TerminalColors::Green));
	std::cout << "(documented)";
      } else {
	std::cout.write(TerminalColors::Red,sizeof(TerminalColors::Red));
	std::cout << "(undocumented)";
      }
      std::cout.write(TerminalColors::Reset,sizeof(TerminalColors::Reset));
      std::cout << std::endl;
    }
    exit(EXIT_SUCCESS);
  } // end of MFront::treatHelpCommandsList

  void MFront::treatHelpCommands()
  {
    auto& f = DSLFactory::getDSLFactory();
    const auto& o = this->currentArgument->getOption();
    tfel::raise_if(o.empty(),"MFront::treatHelpCommandsList: "
		   "no parser name given");
    auto keys = std::vector<std::string>{};
    f.createNewParser(o)->getKeywordsList(keys);
    std::cout << "% `" << o << "` keywords\n\n";
    for(const auto& k : keys){
      const auto fp = getDocumentationFilePath(o,k);
      std::cout << "\n# The `" <<  k << "` keyword\n\n";
      if(!fp.empty()){
	std::ifstream desc{fp};
	if(!desc){
	  // note, this shall never append...
	  std::cout << "Internal error: can't access to the description of keyword '"
		    << k << '\n';
	} else {
	  std::cout << desc.rdbuf();
	}
      } else {
	std::cout << "The keyword `" <<  k << "` is not documented yet\n";
      }
    }
    exit(EXIT_SUCCESS);
  } // end of MFront::treatHelpCommands
  
  void MFront::treatHelpCommand()
  {
    auto& f = DSLFactory::getDSLFactory();
    const auto& o = this->currentArgument->getOption();
    tfel::raise_if(o.empty(),"MFront::treatHelpCommand: "
		   "no argument given");
    const auto pos = o.rfind(':');
    tfel::raise_if((pos==std::string::npos)||(pos+1==o.size()),
		   "MFront::treatHelpCommand: "
		   "ill-formed argument, expected "
		   "'parser:@keyword'");
    const auto pn = o.substr(0,pos); // parser name
    const auto k  = o.substr(pos+1); // key
    tfel::raise_if((pn.empty())||(k.empty()),
		   "MFront::treatHelpCommand: "
		   "ill-formed argument, expected "
		   "'parser:@keyword'");
    tfel::raise_if(k[0]!='@',"MFront::treatHelpCommand: "
		   "ill-formed argument, expected "
		   "'parser:@keyword'");
    auto p = f.createNewParser(pn);
    std::vector<std::string> keys;
    p->getKeywordsList(keys);
    tfel::raise_if(std::find(keys.begin(),keys.end(),k)==keys.end(),
		   "MFront::treatHelpCommand: "
		   "keyword '"+k+"' is not declared ");
    const auto fp = getDocumentationFilePath(pn,k);
    if(fp.empty()){
      std::cout << "no description available for keyword '"
		<< k << "'" << std::endl;
    } else {
      std::ifstream desc(fp);
      if(!desc){
	// note, this shall never append...
	std::cout << "can't access to the description of keyword '"
		  << k << "'" << std::endl;
      } else {
	std::cout << desc.rdbuf();
      }
    }
    exit(EXIT_SUCCESS);
  } // end of MFront::treatHelpCommand

  void MFront::treatNoDeps()
  {
    this->opts.nodeps = true;
  } // end of MFront::treatNoDeps

  void MFront::treatNoMelt()
  {
    this->opts.melt = false;
  } // end of MFront::treatNoMelt

#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
  void MFront::treatDefFile(){
    const auto& o = this->getCurrentCommandLineArgument().getOption();
    tfel::raise_if(o.empty(),"MFrontBase::treatDefFile: "
		   "no option given to the "
		   "'--def-file' argument");
    for(const auto& l : tfel::utilities::tokenize(o,',')){
      tfel::raise_if(l.empty(),"MFrontBase::treatDefFile: "
		     "empty library specified.");
      this->defs.insert(l);
    }    
  } // end of void MFront::treatDefFile
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
  
  TargetsDescription MFront::treatFile(const std::string& f) const
  {
    if(getVerboseMode()>=VERBOSE_LEVEL2){
      getLogStream() << "Treating file: '" << f << "'" <<  std::endl;
    }
    auto dsl = MFrontBase::getDSL(f);
    if(!this->interfaces.empty()){
      dsl->setInterfaces(this->interfaces);
    }
    dsl->analyseFile(f,this->ecmds,this->substitutions);
    dsl->generateOutputFiles();
    auto td = dsl->getTargetsDescription();
    for(auto& l: td){
      for(const auto& d:this->defines){
#ifndef _MSC_VER
	insert_if(l.cppflags,"-D "+d);
#else   /* _MSC_VER */
	insert_if(l.cppflags,"/D "+d);
#endif  /* _MSC_VER */
      }
    }
    return td;
  } // end of MFront::treatFile()

  void MFront::analyseTargetsFile(){
    using tfel::system::dirStringSeparator;
    MFrontLockGuard lock;
    const auto file = "src"+dirStringSeparator()+"targets.lst";
    const std::ifstream test{file};
    if(!test){
      return;
    }
    try{
      tfel::utilities::CxxTokenizer tokenizer{file};
      auto c = tokenizer.begin();
      const auto t = read<TargetsDescription>(c,tokenizer.end());
      if(getVerboseMode()>=VERBOSE_LEVEL2){
	getLogStream() << t << std::endl;
      }
      mergeTargetsDescription(this->targets,t,false);
    } catch(std::exception& e){
      getLogStream() << "can't read file '" << file << "': "
		     << e.what() << '\n';
    } catch(...){
      getLogStream() << "can't read file '" << file << "'\n";
    }
  }
  
  void MFront::buildLibraries(const std::string& target)
  {
    if(this->generator==CMAKE){
      callCMake(target);
    } else {
      callMake(target);
    }
  } // end of MFront::buildLibraries

  void MFront::cleanLibraries()
  {
    if(this->generator==CMAKE){
      //      callCMake("clean");
    } else {
      callMake("clean");
    }
  } // end of MFront::cleanLibraries

#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
  void MFront::generateDefsFiles(){
    MFrontLockGuard lock;
    for(const auto& d:this->defs){
      tfel::raise_if(!describes(this->targets,d),
		     "MFront::generateDefsFile: "
		     "libray '"+d+"' is not handled");
      const auto f = "src"+tfel::system::dirStringSeparator()+d+".def";
      std::ofstream def{f};
      def.exceptions(std::ios::badbit|std::ios::failbit);
      tfel::raise_if(!def,"MFront::generateDefsFile: "
		     "can't open file '"+f+"'");
      def << "LIBRARY " << d << "\n"
	  << "EXPORTS\n";
      std::copy(this->targets[d].epts.begin(),this->targets[d].epts.end(),
		std::ostream_iterator<std::string>(def,"\n"));
    }
  } // end of MFront::generateDefsFile
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */

  void MFront::writeTargetsDescription() const
  {
    using tfel::system::dirStringSeparator;
    MFrontLockGuard lock;
    const auto fn = "src"+dirStringSeparator()+"targets.lst";
    std::ofstream file{fn};
    file.exceptions(std::ios::badbit|std::ios::failbit);
    if(!file){
      getLogStream() << "can't open file '" << fn << "'";
      return;
    }
    file << this->targets;
  } // end of MFront::writeTargetDescription
  
  void MFront::exe()
  {
    tfel::system::systemCall::mkdir("src");
    tfel::system::systemCall::mkdir("include");
    if(this->specifiedTargets.empty()){
      this->specifiedTargets.insert("all");
    }
    bool w = !this->inputs.empty(); //< something to be done 
    w = w || this->genMake;
#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    w = w || (!this->defs.empty());
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
    tfel::raise_if(!w,"MFront::exe: no file name specified "
		   "and nothing to be done\n"+
		   this->getUsageDescription());
    // get file generated by previous sessions
    this->analyseTargetsFile();
    auto errors = std::vector<std::pair<std::string,std::string>>{};
    if(!this->inputs.empty()){
      for(const auto& i : this->inputs){
	try{
	  const auto td = this->treatFile(i);
	  mergeTargetsDescription(this->targets,td,true);
	} catch(std::exception& e){
	  errors.push_back({i,e.what()});
	}
      }
      for(auto& t : this->targets.specific_targets){
	auto tmp = std::vector<std::string>{};
	for(auto p2=t.second.first.cbegin();p2!=t.second.first.cend();++p2){
	  const auto p4 = p2+1;
	  if(find(p4,t.second.first.cend(),*p2)==t.second.first.cend()){
	    tmp.push_back(*p2);
	  }
	}
	t.second.first.swap(tmp);
	tmp.clear();
	for(auto p2=t.second.second.cbegin();
	    p2!=t.second.second.cend();++p2){
	  const auto p4 = p2+1;
	  if(find(p4,t.second.second.cend(),*p2)==t.second.second.cend()){
	    tmp.push_back(*p2);
	  }
	}
	t.second.second.swap(tmp);
      }
      // get file generated by previous sessions
      this->analyseTargetsFile();
      // save all
      this->writeTargetsDescription();
    }
    if(!errors.empty()){
      auto msg = std::string{};
      auto p6  = errors.begin();
      while(p6!=errors.end()){
	msg += "Error while treating file '"+p6->first+"'\n";
	msg += p6->second;
	if(++p6!=errors.end()){
	  msg += "\n\n";
	}
      }
      tfel::raise(msg);
    }
#if (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__)
    this->generateDefsFiles();
#endif /* (defined _WIN32 || defined _WIN64 ||defined __CYGWIN__) */
    const auto has_libs = this->targets.begin()!=this->targets.end();
    if((this->genMake)&&((has_libs)||(!this->targets.specific_targets.empty()))){
      if(this->generator==CMAKE){
	generateCMakeListsFile(this->targets,this->opts);
      } else {
	generateMakeFile(this->targets,this->opts);
      }
    }
    if(this->cleanLibs){
      this->cleanLibraries();
    }
    if((this->buildLibs)&&((has_libs)||(!this->targets.specific_targets.empty()))){
      auto& log = getLogStream();
      for(const auto& t : this->specifiedTargets){
	if(getVerboseMode()>=VERBOSE_LEVEL0){
	  log << "Treating target : " << t << std::endl;
	}
	this->buildLibraries(t);
      }
      if(getVerboseMode()>=VERBOSE_LEVEL0){
	if(has_libs){
	  if(this->targets.end()-this->targets.begin()==1){
	    log << "The following library has been built :\n";
	  } else {
	    log << "The following libraries have been built :\n";
	  }
	  for(const auto& l:this->targets){
	    if(l.name=="MFrontMaterialLaw"){
	      continue;
	    }
	    log << "- " << l.prefix << l.name << "." << l.suffix << " : ";
	    for(const auto& pts:l.epts){
	      log << " " << pts;
	    }
	    log << std::endl;
	  }
	}
	if(!this->targets.specific_targets.empty()){
	  auto pt2=this->targets.specific_targets.find("all");
	  if(pt2!=this->targets.specific_targets.end()){	      
	    if(pt2->second.first.size()==1){
	      log << "The following main target has been build :\n";
	    } else {
	      log << "The following main targets have been build :\n";
	    }
	    for(const auto& t : pt2->second.first){
	      log << "- " << t << std::endl;
	    }
	  }
	}
      }
    }
  } // end of MFront::exe

  MFront::~MFront() = default;

} // end of namespace mfront
