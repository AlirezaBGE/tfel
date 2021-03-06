/*!
 * \file   mfront/src/CMaterialPropertyInterfaceBase.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<sstream>
#include<stdexcept>
#include"TFEL/Raise.hxx"
#include"MFront/DSLUtilities.hxx"
#include"MFront/MFrontHeader.hxx"
#include"MFront/FileDescription.hxx"
#include"MFront/MaterialPropertyDescription.hxx"
#include"MFront/CMaterialPropertyInterfaceBase.hxx"

#ifndef _MSC_VER
static const char * const constexpr_c = "constexpr";
#else
static const char * const constexpr_c = "const";
#endif

namespace mfront
{

  static std::string transformHeaderName(const std::string& h) {
    auto replace = [](std::string& s,
		      const std::string::value_type a,
		      const std::string::value_type b){
      std::string::size_type p = 0;
      while((p=s.find(a))!=std::string::npos){
	s[p]=b;
      }
    };
    auto header = makeUpperCase(h);
    replace(header,'.','_');
    replace(header,'-','_');
    replace(header,'/','_');
    return header;
  } // end of transformHeaderName
  
  CMaterialPropertyInterfaceBase::CMaterialPropertyInterfaceBase() = default;

  void CMaterialPropertyInterfaceBase::writeHeaderPreprocessorDirectives(
      std::ostream& os, const MaterialPropertyDescription&) const {
    writeExportDirectives(os);
  } // end of CMaterialPropertyInterfaceBase::writeSrcPreprocessorDirectives

  void CMaterialPropertyInterfaceBase::writeSrcPreprocessorDirectives(
      std::ostream&, const MaterialPropertyDescription&) const {
  }  // end of CMaterialPropertyInterfaceBase::writeSrcPreprocessorDirectives

  void CMaterialPropertyInterfaceBase::writeOutputFiles(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    this->writeHeaderFile(mpd,fd);
    this->writeSrcFile(mpd,fd);
  } // end of CMaterialPropertyInterfaceBase::writeOutputFiles

  std::string CMaterialPropertyInterfaceBase::getCallingConvention() const {
    return "";
  }

  void CMaterialPropertyInterfaceBase::writeHeaderFile(
      const MaterialPropertyDescription& mpd, const FileDescription& fd) const {
    // the fortran interface does not need any header...
    const auto header = this->getHeaderFileName(mpd.material,mpd.className);
    if(header.empty()){
      return;
    }
    const auto fn  = "include/" + header +".hxx";
    std::ofstream os(fn);
    tfel::raise_if(!os,"CMaterialPropertyInterfaceBase::writeOutputFiles : "
		   "unable to open '"+fn+"' for writing output file.");
    os.exceptions(std::ios::badbit|std::ios::failbit);
    const auto headerGard = transformHeaderName(header)+"_HH";
    os << "/*!\n"
	       << "* \\file   " << fn  << "\n"
	       << "* \\brief  " << "this file declares the " 
	       << mpd.className << " MaterialLaw.\n"
	       << "*         File generated by "
	       << MFrontHeader::getVersionName() << " "
	       << "version " << MFrontHeader::getVersionNumber()
	       << "\n";
    if(!fd.authorName.empty()){
      os << "* \\author " << fd.authorName << "\n";
    }
    if(!fd.date.empty()){
      os << "* \\date   " << fd.date       << "\n";
    }
    if(!fd.description.empty()){
      os << fd.description << "\n";
    }
    os << " */\n\n"
	       << "#ifndef " << headerGard << "\n"
	       << "#define " << headerGard << "\n\n";
    writeExportDirectives(os);
    this->writeHeaderPreprocessorDirectives(os,mpd);
    this->writeBeginHeaderNamespace(os);
    os << "MFRONT_SHAREDOBJ double "
	       << this->getCallingConvention() << '\n'
	       << this->getFunctionName(mpd);
    os << "(";
    this->writeParameterList(os,mpd.inputs);
    os << ");\n\n";
    if(((hasBounds(mpd.inputs))||(hasPhysicalBounds(mpd.inputs)))||
       (this->requiresCheckBoundsFunction())){
      os << "MFRONT_SHAREDOBJ int "
		 << this->getCallingConvention() << '\n'
		 << this->getCheckBoundsFunctionName(mpd);
      os << "(";
      this->writeParameterList(os,mpd.inputs);
      os << ");\n\n";
    }
    this->writeEndHeaderNamespace(os);
    os << "#endif /* " << headerGard << " */\n";
    os.close();
  } // end of CMaterialPropertyInterfaceBase::writeHeaderFile()

  void CMaterialPropertyInterfaceBase::writeInterfaceSpecificVariables(
      std::ostream&, const VariableDescriptionContainer&) const {
  }  // end of CMaterialPropertyInterfaceBase::writeInterfaceSpecificVariables

  void CMaterialPropertyInterfaceBase::writeParameterList(
      std::ostream& os, const VariableDescriptionContainer& inputs) const {
    if(!inputs.empty()){
      for(auto p=inputs.begin();p!=inputs.end();){
	os << "const double " << p->name;
	if((++p)!=inputs.end()){
	  os << ",";
	}
      }
    } else {
      os << "void";
    }
  } // end of CMaterialPropertyInterfaceBase::writeParameterList

  VariableDescriptionContainer::size_type
  CMaterialPropertyInterfaceBase::getVariableNumber(
      const MaterialPropertyDescription& mpd, const std::string& n) {
    VariableDescriptionContainer::size_type nbr = 1u;
    for(const auto& i:mpd.inputs){
      if(i.name==n){
	return nbr;
      }
      ++nbr;
    }
    tfel::raise("CMaterialPropertyInterfaceBase::getVariableNumber: "
		"no inputs named '"+n+"'");
  } // end of CMaterialPropertyInterfaceBase::getVariableNumber

  static void writePhysicalBounds(std::ostream& out,
                                  const MaterialPropertyDescription& mpd) {
    for(const auto& i : mpd.inputs){
      if(!i.hasPhysicalBounds()){
	continue;
      }
      const auto& b = i.getPhysicalBounds();
      const auto nbr = CMaterialPropertyInterfaceBase::getVariableNumber(mpd,i.name);
      if(b.boundsType==VariableBoundsDescription::LOWER){
	out << "if(" << i.name << " < "<< b.lowerBound << "){\n"
	    << "return -" << nbr << ";\n"
	    << "}\n";
      } else if(b.boundsType==VariableBoundsDescription::UPPER){
	out << "if(" << i.name << " > "<< b.upperBound << "){\n"
	    << "return -" << nbr << ";\n"
	    << "}\n";
      } else {
	out << "if((" << i.name << " < "<< b.lowerBound << ")||"
	    << "(" << i.name<< " > "<< b.upperBound << ")){\n"
	    << "return -" << nbr << ";\n"
	    << "}\n";
      }
    }
  }

  static void writeBounds(std::ostream& out,
                          const MaterialPropertyDescription& mpd) {
    for(const auto& i : mpd.inputs){
      if(!i.hasBounds()){
	continue;
      }
      const auto& b = i.getBounds();
      const auto nbr = CMaterialPropertyInterfaceBase::getVariableNumber(mpd,i.name);
      if(b.boundsType==VariableBoundsDescription::LOWER){
	out << "if(" << i.name << " < "<< b.lowerBound << "){\n"
	    << "return " << nbr << ";\n"
	    << "}\n";
      } else if(b.boundsType==VariableBoundsDescription::UPPER){
	out << "if(" << i.name << " > "<< b.upperBound << "){\n"
	    << "return " << nbr << ";\n"
	    << "}\n";
      } else {
	out << "if((" << i.name << " < "<< b.lowerBound << ")||"
	    << "(" << i.name<< " > "<< b.upperBound << ")){\n"
	    << "return " << nbr << ";\n"
	    << "}\n";
      }
    }
  }

  void CMaterialPropertyInterfaceBase::writeEntryPointSymbol(
      std::ostream& os, const MaterialPropertyDescription& mpd) const {
    mfront::writeEntryPointSymbol(os,this->getFunctionName(mpd));
  } // end of CMaterialPropertyInterfaceBase::writeEntryPointSymbol

  void CMaterialPropertyInterfaceBase::writeTFELVersionSymbol(
      std::ostream& os, const MaterialPropertyDescription& mpd) const {
    mfront::writeTFELVersionSymbol(os,this->getFunctionName(mpd));
  } // end of CMaterialPropertyInterfaceBase::writeTFELVersionSymbol

  void CMaterialPropertyInterfaceBase::writeMaterialSymbol(
      std::ostream& os, const MaterialPropertyDescription& mpd) const {
    mfront::writeMaterialSymbol(os,this->getFunctionName(mpd),mpd.material);
  } // end of CMaterialPropertyInterfaceBase::writeMaterialSymbol

  void CMaterialPropertyInterfaceBase::writeMaterialKnowledgeTypeSymbol(
      std::ostream& os, const MaterialPropertyDescription& mpd) const {
    mfront::writeMaterialKnowledgeTypeSymbol(os,this->getFunctionName(mpd),
					     MATERIALPROPERTY);
  } // end of CMaterialPropertyInterfaceBase::writeMaterialKnowledgeTypeSymbol

  void CMaterialPropertyInterfaceBase::writeVariablesNamesSymbol(std::ostream& os,
								 const std::string& name,
								 const MaterialPropertyDescription& mpd) const
  {
    mfront::writeVariablesNamesSymbol(os,name,mpd);    
  } // end of CMaterialPropertyInterfaceBase::writeVariablesNamesSymbol

  void CMaterialPropertyInterfaceBase::writeVariablesBoundsSymbols(std::ostream& os,
								   const std::string& name,
								   const MaterialPropertyDescription& mpd) const
  {
    mfront::writeVariablesBoundsSymbols(os,name,mpd);
  } // end of CMaterialPropertyInterfaceBase::writeVariablesBoundsSymbols


  
  void CMaterialPropertyInterfaceBase::writeSrcFile(const MaterialPropertyDescription& mpd,
						    const FileDescription& fd) const
  {
    // opening the source file
    const auto src = this->getSrcFileName(mpd.material,mpd.className);
    const auto fn  = "src/" + src +".cxx";
    const auto name  = (!mpd.material.empty()) ? mpd.material+"_"+mpd.law : mpd.law;
    const auto& file=fd.fileName;
    std::ofstream os(fn);
    tfel::raise_if(!os,"CMaterialPropertyInterfaceBase::writeOutputFiles : "
		   "unable to open '"+fn+"' for writing output file.");
    os.exceptions(std::ios::badbit|std::ios::failbit);
    os << "/*!\n"
	    << " * \\file   " << fn  << '\n'
	    << " * \\brief  " << "this file implements the " 
	    << mpd.className << " MaterialLaw.\n"
	    << " *         File generated by "
	    << MFrontHeader::getVersionName() << " "
	    << "version " << MFrontHeader::getVersionNumber()
	    << '\n';
    if(!fd.authorName.empty()){
      os << "* \\fd.author " << fd.authorName << '\n';
    }
    if(!fd.date.empty()){
      os << "* \\date   " << fd.date       << '\n';
    }
    os << " */\n\n"
	    << "#include<cmath>\n"
	    << "#include<cerrno>\n"
	    << "#include<algorithm>\n"
	    << "#include\"TFEL/Math/General/IEEE754.hxx\"\n\n";
    if(!mpd.includes.empty()){
      os << mpd.includes << "\n\n";
    }
    const auto header = this->getHeaderFileName(mpd.material,mpd.className);
    if(!header.empty()){
      os << "#include\""+header+".hxx\"\n\n";
    }
    this->writeSrcPreprocessorDirectives(os,mpd);
    this->writeVariablesNamesSymbol(os,name,mpd);
    this->writeVariablesBoundsSymbols(os,name,mpd);
    this->writeEntryPointSymbol(os,mpd);
    this->writeTFELVersionSymbol(os,mpd);
    this->writeInterfaceSymbol(os,mpd);
    this->writeMaterialSymbol(os,mpd);
    this->writeMaterialKnowledgeTypeSymbol(os,mpd);
    this->writeBeginSrcNamespace(os);
    os << "double " << this->getFunctionName(mpd) << "(";
    this->writeParameterList(os,mpd.inputs);
    os << ")\n{\n"
	    << "using namespace std;\n"
	    << "using real = double;\n";
    // material laws
    writeMaterialLaws(os,mpd.materialLaws);
    // static variables
    writeStaticVariables(os,mpd.staticVars,fd.fileName);
    // parameters
    if(!mpd.parameters.empty()){
      for(const auto& p : mpd.parameters){
	tfel::raise_if(!p.hasAttribute(VariableDescription::defaultValue),
		       "CMaterialPropertyInterfaceBase::writeSrcFile : "
		       "internal error (can't find value of "
		       "parameter '"+p.name+"')");
	os << "static " << constexpr_c << " real " << p.name << " = "
		<< p.getAttribute<double>(VariableDescription::defaultValue) << ";\n";
      }
    }
    this->writeInterfaceSpecificVariables(os,mpd.inputs);
    if(!mpd.inputs.empty()){
      os << "#ifndef MFRONT_NOERRNO_HANDLING\n"
	      << "const auto mfront_errno_old = errno;\n"
	      << "errno=0;\n"
	      << "#endif /* MFRONT_NOERRNO_HANDLING */\n";
    }
    os << "real " << mpd.output.name << ";\n"
	    << mpd.f.body << "\n";
    if(!mpd.inputs.empty()){
      os << "#ifndef MFRONT_NOERRNO_HANDLING\n"
	// can't use std::swap here as errno might be a macro
	      << "const auto mfront_errno = errno;\n"
	      << "errno = mfront_errno_old;\n"
	      << "if((mfront_errno!=0)||(!tfel::math::ieee754::isfinite("
	      << mpd.output.name << "))){\n";
      this->writeCErrorTreatment(os,mpd);
      os << "}\n"
	      << "#endif /* MFRONT_NOERRNO_HANDLING */\n";
    }
    os << "return " << mpd.output.name << ";\n"
	    << "} /* end of " << mpd.className << " */\n\n";
    if(((hasBounds(mpd.inputs))||(hasPhysicalBounds(mpd.inputs)))||
       (this->requiresCheckBoundsFunction())){
      os << "int " << this->getCheckBoundsFunctionName(mpd);
      os << "(";
      this->writeParameterList(os,mpd.inputs);
      os << ")\n{\n";
      this->writeInterfaceSpecificVariables(os,mpd.inputs);
      for(const auto& i : mpd.inputs){
        os << "static_cast<void>(" << i.name << ");\n";
      }
      if(hasPhysicalBounds(mpd.inputs)){
        os << "/* treating mpd.physical bounds */\n";
        writePhysicalBounds(os,mpd);
      }
      if(hasBounds(mpd.inputs)){
	os << "/* treating standard bounds */\n";
	writeBounds(os,mpd);
      }
      os << "return 0;\n} /* end of " << mpd.className << "_checkBounds */\n\n";
    }
    this->writeEndSrcNamespace(os);
    os.close();
  } // end of CMaterialPropertyInterfaceBase::writeSrcFile()

  void
  CMaterialPropertyInterfaceBase::writeCErrorTreatment(std::ostream& os,
						       const MaterialPropertyDescription& mpd) const{
    os << "return std::nan(\"" << this->getFunctionName(mpd)
       << ": invalid call to a C function (errno is not null)\");\n";
  } // CMaterialPropertyInterfaceBase::writeCErrorTreatment

  CMaterialPropertyInterfaceBase::~CMaterialPropertyInterfaceBase() = default;
  
} // end of namespace mfront
