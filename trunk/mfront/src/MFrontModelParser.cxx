/*! 
 * \file  MFrontModelParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 jun 2010
 */

#include"MFront/MFrontModelParser.hxx"
#include"MFront/MFrontModelInterfaceFactory.hxx"

namespace mfront
{

  std::string
  MFrontModelParser::getName(void)
  {
    return "Model";
  } // end of MFrontMaterialLawParser::getName(void)

  std::string
  MFrontModelParser::getDescription(void)
  {
    return "this parser is used to define simple material models";
  } // end of MFrontModelParser::getDescription

  void
  MFrontModelParser::setInterfaces(const std::set<std::string>& f)
  {
    MFrontModelParserCommon::setInterfaces(f);
    using namespace std;
    StringContainer::const_iterator p;
    typedef MFrontModelInterfaceFactory MMIF;
    MMIF& mmif = MMIF::getMFrontModelInterfaceFactory();
    for(p=this->interfaces.begin();p!=this->interfaces.end();++p){
      MFrontModelVirtualInterface *interface = mmif.getInterfacePtr(*p);
      interface->declareReservedNames(this->reservedNames);
    }
  } // end of MFrontModelParser::setInterfaces

  void
  MFrontModelParser::treatFile(const std::string& f)
  {
    using namespace std;
    if(this->interfaces.empty()){
      string msg("MFrontModelParser::treatFile : ");
      msg += "no interface defined";
      throw(runtime_error(msg));
    }
    MFrontModelParserBase<MFrontModelParser>::treatFile(f);
  }

} // end of namespace mfront
