/*!
 * \file   MFrontAsterInterface.hxx
 * \brief  This file declares the MFrontAsterInterface class
 * \author Helfer Thomas
 * \date   17 Jan 2007
 */

#ifndef _LIB_MFRONTASTERINTERFACE_H_
#define _LIB_MFRONTASTERINTERFACE_H_ 

#include<string>
#include<fstream>

#include"TFEL/Utilities/CxxTokenizer.hxx"
#include"MFront/MFrontUMATInterfaceBase.hxx"

namespace mfront{

  struct MFrontAsterInterface
    : public MFrontUMATInterfaceBase
  {
    static std::string 
    getName(void);
    
    MFrontAsterInterface();
    
    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const std::string&,
		      const std::string&,
		      const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const std::string&,
			const std::string&,
			const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::vector<std::string>
    getGeneratedIncludes(const std::string&,
			 const std::string&,
			 const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    virtual std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const std::string&,
			     const std::string&,
			     const std::string&);

    virtual std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);

    virtual void
    endTreatement(const std::string&,
		  const std::string&,
		  const std::string&,
		  const std::string&,
		  const std::string&,
		  const std::string&,
		  const std::map<std::string,std::string>&,
		  const std::map<std::string,std::string>&,
		  const MechanicalBehaviourDescription&);

    virtual void
    reset(void);

    virtual ~MFrontAsterInterface();
    
  protected:

    virtual std::string
    getLibraryName(const std::string&,
		   const std::string&) const;

    virtual std::string
    getInterfaceName(void) const;

    virtual std::string
    getFunctionName(const std::string&) const;

    virtual bool
    hasMaterialPropertiesOffset(const MechanicalBehaviourDescription&) const;

    /*!
     * \param[in] out : output file
     */
    virtual void
    writeMTestFileGeneratorSetModellingHypothesis(std::ostream&) const;

    virtual void
    writeMTestFileGeneratorAdditionalMaterialPropertiesInitialisation(std::ostream&,
								      const MechanicalBehaviourDescription&) const;

    /*!
     * \param[in] out : output file
     */
    virtual void
    writeUMATxxMaterialPropertiesSymbols(std::ostream&,
					 const std::string&,
					 const MechanicalBehaviourDescription&,
					 const std::map<std::string,std::string>&,
					 const std::map<std::string,std::string>&) const;

    /*!
     * \param[out] hasElasticMaterialPropertiesOffset          : true if an offset is required for elastic properties
     * \param[out] hasThermalExpansionMaterialPropertiesOffset : true if an offset is required for thermal expansion
     * \param[out] massDensityOffsetForThermalExpansion        : true if an offset is required for mass density
     * \param[in] mb                                           : mechanical description
     */
    void
    getMaterialPropertiesOffset(bool&,bool&,bool&,
				const MechanicalBehaviourDescription&) const;
    /*!
     * \param[in] out           : output file
     * \param[in] name          : name of the behaviour 
     * \param[in] file          : source file
     * \param[in] mb            : behaviour description
     * \param[in] glossaryNames : glossary names
     * \param[in] entrNames     : entry    names
     */
    virtual void
    writeUMATxxAdditionalSymbols(std::ostream&,
				 const std::string&,
				 const std::string&,
				 const MechanicalBehaviourDescription&,
				 const std::map<std::string,std::string>&,
				 const std::map<std::string,std::string>&) const;

    bool   compareToNumericalTangentOperator;

    double strainPerturbationValue;

    double tangentOperatorComparisonCriterium;

    bool savesTangentOperator;

    bool errorReport;

  }; // end of MFrontAsterInterface

} // end of namespace mfront

#endif /* _LIB_MFRONTASTERINTERFACE_H */
