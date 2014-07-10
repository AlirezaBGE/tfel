/*! 
 * \file  MTestUmatBehaviourBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTUMATBEHAVIOURBASE_H_
#define _LIB_MFRONT_MTESTUMATBEHAVIOURBASE_H_ 

#include"TFEL/System/ExternalFunctionsPrototypes.hxx"
#include"MFront/MTestBehaviour.hxx"

namespace mfront
{

  /*!
   * A class to handle mechanical beheaviours written using the umat
   * interface
   */
  struct MTestUmatBehaviourBase
    : public MTestBehaviour
  {
    /*!
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    MTestUmatBehaviourBase(const std::string&,
			   const std::string&);
    /*!
     * \return the type of the behaviour
     */
    virtual tfel::material::MechanicalBehaviourBase::BehaviourType
    getBehaviourType(void) const;
    /*!
     * \param[in] h : modelling hypothesis
     */
    virtual unsigned short
    getProblemSize(const tfel::material::ModellingHypothesis::Hypothesis) const;
    /*!
     * \param[in] c : components
     * \param[in] h : modelling hypothesis
     */
    virtual void
    getStensorComponentsSuffixes(std::vector<std::string>&,
				 const tfel::material::ModellingHypothesis::Hypothesis) const;
    /*!
     * \param[in] c : components
     * \param[in] h : modelling hypothesis
     */
    virtual void
    getDrivingVariablesComponents(std::vector<std::string>&,
				  const tfel::material::ModellingHypothesis::Hypothesis) const;
    /*!
     * \param[in] c : components
     * \param[in] h : modelling hypothesis
     */
    virtual void
    getThermodynamicForcesComponents(std::vector<std::string>&,
				     const tfel::material::ModellingHypothesis::Hypothesis) const;
    /*!
     * \param[in] h     : modelling hypothesis
     * \param[in] cname : component name
     */
    virtual unsigned short
    getDrivingVariableComponentPosition(const tfel::material::ModellingHypothesis::Hypothesis,
					const std::string&) const;
    /*!
     * \param[in] h     : modelling hypothesis
     * \param[in] cname : component name
     */
    virtual unsigned short
    getThermodynamicForceComponentPosition(const tfel::material::ModellingHypothesis::Hypothesis,
					   const std::string&) const;
    /*!
     * \return the type of the behaviour
     * 0 means that the behaviour is isotropic.
     * 1 means that the behaviour is orthotropic.
     */
    virtual unsigned short
    getSymmetryType(void) const;
    /*!
     * \return the names of material properties
     */
    virtual std::vector<std::string>
    getMaterialPropertiesNames(void) const;
    /*!
     * \return the names of internal variables
     */
    virtual std::vector<std::string>
    getInternalStateVariablesNames(void) const;
    /*!
     * \return the size of the array of internal variables
     * \param[in] h : modelling hypothesis
     */
    virtual size_t
    getInternalStateVariablesSize(const tfel::material::ModellingHypothesis::Hypothesis) const;
    /*!
     * \return the descriptions the internal variables
     * \param[in] h : modelling hypothesis
     */
    virtual std::vector<std::string>
    getInternalStateVariablesDescriptions(const tfel::material::ModellingHypothesis::Hypothesis) const;
    /*!
     * \return the type of an internal variable
     * \param[in] n : internal variable name
     */
    virtual unsigned short
    getInternalStateVariableType(const std::string&) const;
    /*!
     * \return the position of an internal variable
     * \param[in] h : modelling hypothesis
     * \param[in] n : internal variable name
     */
    virtual unsigned short
    getInternalStateVariablePosition(const tfel::material::ModellingHypothesis::Hypothesis,
				     const std::string&) const;
    /*!
     * \return the names of external variables
     */
    virtual std::vector<std::string>
    getExternalStateVariablesNames(void) const;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    virtual void
    setParameter(const std::string&,
		 const real) const;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    virtual void
    setIntegerParameter(const std::string&,
			const int) const;
    /*!
     * \brief set the value of a parameter
     * \param[in] n : name of the parameter
     * \param[in] v : value
     */
    virtual void
    setUnsignedIntegerParameter(const std::string&,
				const unsigned short) const;
    //! destructor
    virtual ~MTestUmatBehaviourBase();
  protected:
    //! library
    std::string library;
    //! function
    std::string behaviour;
    //! names of the material properties
    std::vector<std::string> mpnames;
    //! names of the internal state variables
    std::vector<std::string> ivnames;
    //! types of the internal state variables
    std::vector<int> ivtypes;
    //! names of the external state variables
    std::vector<std::string> evnames;
    //! type of of law 
    unsigned short type;
    //! symmetry of of behaviour (isotropic or orthotropic)
    unsigned short stype;
  }; // end of struct MTestBehaviour
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTUMATBEHAVIOUR_H */

