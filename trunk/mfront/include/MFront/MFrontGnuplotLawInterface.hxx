/*!
 * \file   MFrontGnuplotLawInterface.hxx
 * \brief  This file declares the MFrontGnuplotLawInterface class
 * \author P. GOLDBRONN
 * \date   14 mai 2008
 */

#ifndef _LIB_MFRONTGNUPLOTLAWINTERFACE_H_
#define _LIB_MFRONTGNUPLOTLAWINTERFACE_H_ 

#include<map>
#include<vector>
#include<string>
#include<fstream>

#include"TFEL/Utilities/CxxTokenizer.hxx"

#include"MFront/VarHandler.hxx"
#include"MFront/StaticVarHandler.hxx"
#include"MFront/LawFunction.hxx"
#include"MFront/VariableBoundsDescription.hxx"
#include"MFront/MFrontLawVirtualInterface.hxx"

namespace mfront{

  struct MFrontGnuplotLawInterface
    : public MFrontLawVirtualInterface
  {
    static std::string 
    getName(void);
    
    MFrontGnuplotLawInterface();
    
    void setVerboseMode(void);

    void setDebugMode(void);

    void setWarningMode(void);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::map<std::string,std::vector<std::string> >
    getGlobalIncludes(const std::string&,
		      const std::string&,
		      const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::map<std::string,std::vector<std::string> >
    getGlobalDependencies(const std::string&,
			  const std::string&,
			  const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::map<std::string,std::vector<std::string> >
    getGeneratedSources(const std::string&,
			const std::string&,
			const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::vector<std::string>
    getGeneratedIncludes(const std::string&,
			 const std::string&,
			 const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     */
    std::map<std::string,std::vector<std::string> >
    getLibrariesDependencies(const std::string&,
			     const std::string&,
			     const std::string&);

    /*!
     * \param const std::string&, library
     * \param const std::string&, material
     * \param const std::string&, class
     * \param const std::vector<std::string>&, library links
     */
    std::map<std::string,
	     std::pair<std::vector<std::string>,
		       std::vector<std::string> > >
    getSpecificTargets(const std::string&,
		       const std::string&,
		       const std::string&,
		       const std::vector<std::string>&);

    /*
     * \param const std::string&, name of the original file
     * \param const std::string&, name of the output library
     * \param const std::string&, name of the material
     * \param const std::string&, name of the class
     * \param const std::string&, author
     * \param const std::string&, date
     * \param const std::string&, description
     * \param const std::string&, includes
     * \param const std::string&, output name
     * \param const VarContainer&, inputs
     * \param const std::vector<std::string>&, material laws
     * \param const StaticVarContainer&, static variables
     * \param const std::map<std::string,std::string>&, glossary names,
     * \param const std::map<std::string,std::string>&, entry names,
     * \param const std::vector<std::string>&, parameters
     * \param const std::map<std::string,double>&, parameters values
     * \param const LawFunction&, function definition
     * \param const std::vector<VariableBoundsDescription>&, bounds of the law
     * \param const std::vector<VariableBoundsDescription>&, physical bounds of the law
     * \param const bool, use template
     * \param const std::vector<std::string>&, namespaces
     */
    void writeOutputFiles(const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const std::string&,
			  const VarContainer&,
			  const std::vector<std::string>&,
			  const std::map<std::string,std::string>&,
			  const std::map<std::string,std::string>&,
			  const StaticVarContainer& staticVars,
			  const std::vector<std::string>&,
			  const std::map<std::string,double>&,
			  const LawFunction&,
			  const std::vector<VariableBoundsDescription>&,
			  const std::vector<VariableBoundsDescription>&,
			  const bool,
			  const std::vector<std::string>&);
    
    
    void
    reset(void);

    ~MFrontGnuplotLawInterface();
    
  private:

    std::string
    eraseQuote(const std::string&);

    std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    treatKeyword(const std::string&,
		 tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		 const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);
    
    std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    registerTestBounds(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
			const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);

    std::pair<bool,tfel::utilities::CxxTokenizer::TokensContainer::const_iterator>
    registerGraph(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
		  const tfel::utilities::CxxTokenizer::TokensContainer::const_iterator);

    tfel::utilities::CxxTokenizer::TokensContainer::const_iterator
    nextToken(tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
	      tfel::utilities::CxxTokenizer::TokensContainer::const_iterator,
	      const std::string &msg="");

    /*
     * \param const std::string&, name of the original file
     * \param const std::string&, className
     * \param const std::string&, author
     * \param const std::string&, date
     * \param const VarContainer&, inputs
     * \param const StaticVarContainer&, static variables
     * \param const std::vector<std::string>&, parameters
     * \param const std::map<std::string,double>&, parameters values
     * \param const LawFunction&, function definition
     * \param const std::vector<VariableBoundsDescription>&, bounds of the law
     * \param const std::vector<VariableBoundsDescription>&, physical bounds of the law
     * \param const bool, use template
     * \param const std::vector<std::string>&, namespaces
     */
    void writeSrcFile(const std::string&,
		      const std::string&,
		      const std::string&,
		      const std::string&,
		      const VarContainer&,
		      const StaticVarContainer& staticVars,
		      const std::vector<std::string>&,
		      const std::map<std::string,double>&,
		      const LawFunction&,
		      const std::vector<VariableBoundsDescription>&,
		      const std::vector<VariableBoundsDescription>&,
		      const bool useTemplate,
		      const std::vector<std::string>& namespaces);

    std::ofstream srcFile;

    std::string srcFileName;
    
    bool verboseMode;

    bool debugMode;

    bool warningMode;

    std::vector<VariableBoundsDescription> testBounds ;

    std::string xLabel ;
    std::string yLabel ;
    //map : variable,datafile
    struct dataFile {
      std::string name;
      std::string legend;
    };
    std::multimap< std::string, dataFile > experimentalData ;
    
  }; // end of MfrontGnuplotLawInterface

} // end of namespace mfront

#endif /* _LIB_MFRONTGNUPLOTLAWINTERFACE_H */
