/*!
 * \file   ASTERTraits.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   13 f�v 2007
 */

#ifndef _LIB_MFRONT_ASTERTRAITS_HXX_
#define _LIB_MFRONT_ASTERTRAITS_HXX_ 

namespace aster{

  enum AsterSymmetryType{
    ISOTROPIC,
    ORTHOTROPIC
  }; // end of enum AsterSymmetryType

  enum AsterErrorReportPolicy{
    ASTER_WRITEONSTDOUT,
    ASTER_NOERRORREPORT
  }; // end of enum AsterErrorReportPolicy

  template<typename Behaviour>
  struct AsterTraits{
    static const AsterErrorReportPolicy errorReportPolicy = ASTER_WRITEONSTDOUT;
    static const bool requiresStiffnessTensor             = false;
    static const bool requiresThermalExpansionTensor      = false;
    static const unsigned short propertiesOffset          = 0u;
    // Type of the behaviour of the material (isotropic or orthotropic)
    static const AsterSymmetryType type   = aster::ISOTROPIC;
    // Type of the elastic behaviour of the material (isotropic or orthotropic)
    static const AsterSymmetryType  etype = aster::ISOTROPIC;
  }; // end of class ASTERTraits

} // end of namespace aster  

#endif /* _LIB_MFRONT_ASTERTRAITS_HXX */

