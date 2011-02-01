/*!
 * \file   MFrontInitParsers.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 sep 2008
 */

#include"MFront/MFrontParserProxy.hxx"

#include"MFront/MFrontDefaultParser.hxx"
#include"MFront/MFrontIsotropicMisesCreepParser.hxx"
#include"MFront/MFrontIsotropicMisesPlasticFlowParser.hxx"
#include"MFront/MFrontIsotropicStrainHardeningMisesCreepParser.hxx"
#include"MFront/MFrontMaterialLawParser.hxx"
#include"MFront/MFrontMultipleIsotropicMisesFlowsParser.hxx"
#include"MFront/MFrontRungeKuttaParser.hxx"
#include"MFront/MFrontImplicitParser.hxx"

#include"MFront/MFrontModelParser.hxx"

namespace mfront
{

  void
  initParsers(void)
  {
    MFrontParserProxy<MFrontDefaultParser> proxy1;
    MFrontParserProxy<MFrontIsotropicMisesCreepParser>  proxy2;
    MFrontParserProxy<MFrontIsotropicMisesPlasticFlowParser>  proxy3;
    MFrontParserProxy<MFrontIsotropicStrainHardeningMisesCreepParser>  proxy4;
    MFrontParserProxy<MFrontMaterialLawParser>  proxy5;
    MFrontParserProxy<MFrontMultipleIsotropicMisesFlowsParser>  proxy6;
    MFrontParserProxy<MFrontRungeKuttaParser>  proxy7;
    MFrontParserProxy<MFrontImplicitParser>  proxy8;
    MFrontParserProxy<MFrontModelParser>  proxy9;
  } // end of initParsers 

} // end of namespace mfront
