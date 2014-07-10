/*! 
 * \file  UMATInterfaceBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 janv. 2013
 */

#ifndef _LIB_MFRONT_UMAT_UMATINTERFACEBASE_H_
#define _LIB_MFRONT_UMAT_UMATINTERFACEBASE_H_ 

#include<string>

#include"TFEL/Exception/TFELException.hxx"
#include"TFEL/Material/MaterialException.hxx"

#include"MFront/UMAT/UMAT.hxx"
#include"MFront/UMAT/UMATTraits.hxx"
#include"MFront/UMAT/UMATConfig.hxx"
#include"MFront/UMAT/UMATException.hxx"

namespace umat{

  /*!
   * This structure is called when we fall in a case that the umat
   * interface is not able to handle. Normally, this case shall have
   * been handled during the code generation (see the
   * MFrontUMATInterface class).
   */
  struct MFRONT_UMAT_VISIBILITY_EXPORT UMATUnSupportedCaseHandler
  {
    /*!
     * \brief throw an exception
     */
    static void
    exe(const UMATReal *const ,
	const UMATReal *const,
	const UMATReal *const,
	const UMATReal *const,
	const UMATReal *const,
	const UMATReal *const,
	const UMATReal *const,
	const UMATReal *const,
	const UMATInt  *const,
	const UMATReal *const,
	const UMATReal *const,
	UMATReal *const,
	const UMATInt  *const,
	UMATReal *const); // end of exe
  }; // end of struct UMATUnSupportedCaseHandler

  /*!
   * \class  UMATInterfaceBase
   * \brief  Base class for umat interfaces
   * \author Helfer Thomas
   * \date   12/12/2011
   */
  struct MFRONT_UMAT_VISIBILITY_EXPORT UMATInterfaceBase
  {
    /*!
     * \brief throw an UMATException. This method shall be called when
     * the number of materials properties declared by the beahviour and the
     * number of  materials properties declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of material properties declared by the behaviour
     * \param[in] n2 : number of material properties declared by the interface
     */
    static void
    throwUnMatchedNumberOfMaterialProperties(const std::string&,
					     const unsigned short n1,
					     const UMATInt n2);
    
    /*!
     * \brief throw an UMATException. This method shall be called when
     * the number of state variables declared by the beahviour and the
     * number of state variables declared by the interface does not
     * match.
     * \param[in] b  : behaviour name
     * \param[in] n1 : number of state variables declared by the behaviour
     * \param[in] n2 : number of state variables declared by the interface
     */
    static void
    throwUnMatchedNumberOfStateVariables(const std::string&,
					 const unsigned short n1,
					 const UMATInt n2);

    /*!
     * \brief display the error message out of an UMATException to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the UMATException to be treated
     */
    static void
    treatUmatException(const std::string&,
		       const UMATException&);

    /*!
     * \brief display the error message out of a material exception to the
     * standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the material exception to be treated
     */
    static void
    treatMaterialException(const std::string&,
			   const tfel::material::MaterialException&);
    
    /*!
     * \brief display the error message out of a generic tfel
     * exception to the standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the exception to be treated
     */
    static void
    treatTFELException(const std::string&,
		       const tfel::exception::TFELException&);

    /*!
     * \brief display the error message out of a generic standard
     * exception to the standard output.
     * \param[in] b : behaviour name
     * \param[in] e : the exception to be treated
     */
    static void
    treatStandardException(const std::string&,
			   const std::exception&);

    /*!
     * \brief display the error message when an unknown exception is caught
     * \param[in] b : behaviour name
     */
    static void
    treatUnknownException(const std::string&);

    /*!
     * \brief throw an UMATException if the time step is negative
     * \param[in] b : behaviour name
     */
    static void
    throwNegativeTimeStepException(const std::string&);

    /*!
     * \brief throw an UMATException if the behaviour integration
     * failed
     * \param[in] b : behaviour name
     */
    static void
    throwBehaviourIntegrationFailedException(const std::string&);

    /*!
     * \brief throw an UMATException if the maximum number of sub
     * stepping has been reached
     * \param[in] b : behaviour name
     */
    static void
    throwMaximumNumberOfSubSteppingReachedException(const std::string&);

    /*!
     * \brief throw an UMATException if the maximum number of sub
     * stepping has been reached
     * \param[in] b : behaviour name
     */
    static void
    throwPlaneStressMaximumNumberOfIterationsReachedException(const std::string&);

    /*!
     * \brief throw an error message if the value of the NTENS
     * parameter is not valid
     * \param[in] NTENS : NTENS value
     * \param[in] s     : expected size
     */
    static void
    checkNTENSValue(const UMATInt,const unsigned short);
    /*!
     * \brief throw an error message if the behaviour type is not supported for the given hypothesis
     * \param[in] type : behaviour type
     * \param[in] H    : hypothesis
     */
    static void
    throwInvalidBehaviourTypeAndModellingHypothesis(const UMATBehaviourType,
						    const std::string&);
    /*!
     * \brief display an error message if the value of the NDI
     * parameter is not valid
     */
    static void
    displayInvalidModellingHypothesisErrorMessage(void);

  }; // end of struct UMATInterfaceBase
  
} // end of namespace umat

#endif /* _LIB_MFRONT_UMAT_UMATINTERFACEBASE_H */

