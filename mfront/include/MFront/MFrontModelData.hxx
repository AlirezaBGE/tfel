/*! 
 * \file   mfront/include/MFront/MFrontModelData.hxx
 * \brief
 * \author Helfer Thomas
 * \brief  12 jun 2010
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef _LIB_MFRONT_MFRONTMODELDATA_H_
#define _LIB_MFRONT_MFRONTMODELDATA_H_ 

#include<set>
#include<map>
#include<string>

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/VariableDescription.hxx"
#include"MFront/StaticVariableDescription.hxx"
#include"MFront/VariableBoundsDescription.hxx"

namespace mfront
{

  struct TFEL_VISIBILITY_EXPORT MFrontModelData
  {

    struct Function
    {
      std::set<std::string> usedVariables;
      std::set<std::string> modifiedVariables;
      std::set<std::string> constantMaterialProperties;
      std::set<std::string> localParameters;
      std::set<std::string> globalParameters;
      std::map<std::string,unsigned short> depths;
      std::string name;
      std::string body;
      unsigned int line;
      bool useTimeIncrement;
    }; // end of struct MFrontPleiadesModelParser::Function

    typedef std::vector<Function> FunctionContainer;
    FunctionContainer functions;

    VariableDescriptionContainer outputs;
    VariableDescriptionContainer inputs;
    VariableDescriptionContainer globalParameters;
    VariableDescriptionContainer localParameters;
    VariableDescriptionContainer constantMaterialProperties;
    /*!
     * static variables
     */
    StaticVariableDescriptionContainer staticVars;

    /*!
     * material name
     */
    std::string material;
    std::string library;
    std::string className;
    /*!
     * included header files
     */
    std::string includes;
    /*!
     * specific sources
     */
    std::string sources;
    /*!
     * private code
     */
    std::string privateCode;
    /*!
     * class member
     */
    std::string members;
    std::set<std::string> domains;
    std::set<std::string> fieldNames;
    std::set<std::string> functionNames;
    std::map<std::string,std::string> entryNames;
    std::map<std::string,double>      initialValues;
    std::map<std::string,std::string> defaultValues;
    std::vector<VariableBoundsDescription> boundsDescriptions;
    std::vector<VariableBoundsDescription> physicalBoundsDescriptions;
    std::map<std::string,unsigned short> depths;
    /*!
     * list of material laws used
     */
    std::vector<std::string> materialLaws;
  }; // end of struct MFrontModelData

} // end of namespace mfront

#endif /* _LIB_MFRONT_MFRONTMODELDATA_H */
