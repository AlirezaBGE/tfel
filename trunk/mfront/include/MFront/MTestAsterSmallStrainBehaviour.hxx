/*! 
 * \file  MTestAsterBehaviour.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 avril 2013
 */

#ifndef _LIB_MFRONT_MTESTASTERBEHAVIOUR_H_
#define _LIB_MFRONT_MTESTASTERBEHAVIOUR_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/System/ExternalFunctionsPrototypes.hxx"
#include"MFront/MTestUmatBehaviourBase.hxx"

namespace mfront
{

  /*!
   * A class to handle mechanical beheaviours written using the aster
   * interface
   */
  struct TFEL_VISIBILITY_LOCAL MTestAsterSmallStrainBehaviour
    : public MTestUmatBehaviourBase
  {
    /*!
     * \param[in] h : modelling hypothesis
     * \param[in] l : library name
     * \param[in] b : behaviour name
     */
    MTestAsterSmallStrainBehaviour(const tfel::material::ModellingHypothesis::Hypothesis h,
				   const std::string&,
				   const std::string&);
    /*!
     * \return the size of the array of internal variables
     * \param[in] d : space dimension
     */
    size_t
    getInternalStateVariablesSize(const tfel::material::ModellingHypothesis::Hypothesis h) const;
    /*!
     * \return the descriptions the internal variables
     * \param[in] d : space dimension
     */
    std::vector<std::string>
    getInternalStateVariablesDescriptions(const tfel::material::ModellingHypothesis::Hypothesis) const;
    /*!
     * \brief allocate internal workspace
     * \param[in] h : modelling hypothesis
     */
    virtual void
    allocate(const tfel::material::ModellingHypothesis::Hypothesis);
    /*!
     * \return the default type of stiffness matrix used by the behaviour
     */
    virtual MTestStiffnessMatrixType::mtype
    getDefaultStiffnessMatrixType(void) const;
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return true if the integration was successfull, false otherwise
     * \param[out] Kt    : tangent operator
     * \param[in]  r     : rotation matrix
     * \param[in]  e0    : strain at the beginning of the time step
     * \param[in]  s0    : stresses at the beginning of the time step
     * \param[in]  mp    : material properties at the beginning of the time step
     * \param[in]  iv0   : internal state variables at the beginning of the time step
     * \param[in]  ev0   : external state variables at the beginning of the time step
     * \param[in]  h     : modelling hypothesis
     * \param[in]  dt    : time increment
     * \param[in]  ktype : type of the stiffness matrix
     */
    virtual bool
    computePredictionOperator(tfel::math::matrix<real>&,
			      const tfel::math::tmatrix<3u,3u,real>&,
			      const tfel::math::vector<real>&,
			      const tfel::math::vector<real>&,
			      const tfel::math::vector<real>&,
			      const tfel::math::vector<real>&,
			      const tfel::math::vector<real>&,
			      const tfel::material::ModellingHypothesis::Hypothesis,
			      const MTestStiffnessMatrixType::mtype) const;
    /*!
     * \brief integrate the mechanical behaviour over the time step
     * \return true if the integration was successfull, false otherwise
     * \param[out] Kt    : tangent operator
     * \param[out] s1    : stresses at the end of the time step
     * \param[out] iv1   : internal state variables at the end of the time step
     * \param[in]  r     : rotation matrix
     * \param[in]  e0    : strain at the beginning of the time step
     * \param[in]  de    : strain increment
     * \param[in]  s0    : stresses at the beginning of the time step
     * \param[in]  mp    : material properties
     * \param[in]  iv0   : internal state variables at the beginning of the time step
     * \param[in]  ev0   : external state variables at the beginning of the time step
     * \param[in]  dev   : external state variables increments
     * \param[in]  h     : modelling hypothesis
     * \param[in]  dt    : time increment
     * \param[in]  ktype : type of the stiffness matrix
     */
    virtual bool
    integrate(tfel::math::matrix<real>&,
	      tfel::math::vector<real>&,
	      tfel::math::vector<real>&,
	      const tfel::math::tmatrix<3u,3u,real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::math::vector<real>&,
	      const tfel::material::ModellingHypothesis::Hypothesis,
	      const real,
	      const MTestStiffnessMatrixType::mtype) const;
    //! destructor
    virtual ~MTestAsterSmallStrainBehaviour();
  protected:
    /*!
     * \brief call the mechanical behaviour
     * \param[out] s1    : stresses at the end of the time step
     * \param[out] iv1   : internal state variables at the end of the time step
     * \param[in]  r     : rotation matrix
     * \param[in]  e0    : strain at the beginning of the time step
     * \param[in]  de    : strain increment
     * \param[in]  s0    : stresses at the beginning of the time step
     * \param[in]  mp    : material properties
     * \param[in]  iv0   : internal state variables at the beginning of the time step
     * \param[in]  ev0   : external state variables at the beginning of the time step
     * \param[in]  dev   : external state variables increments
     * \param[in]  h     : modelling hypothesis
     * \param[in]  dt    : time increment
     * \param[in]  ktype : type of the stiffness matrix
     * \param[in] b : if true, integrate the behaviour over the time
     * step, if false compute a prediction of the stiffness matrix
     */
    virtual bool
    call_behaviour(tfel::math::matrix<real>&,
		   tfel::math::vector<real>&,
		   tfel::math::vector<real>&,
		   const tfel::math::tmatrix<3u,3u,real>&,
		   const tfel::math::vector<real>&,
		   const tfel::math::vector<real>&,
		   const tfel::math::vector<real>&,
		   const tfel::math::vector<real>&,
		   const tfel::math::vector<real>&,
		   const tfel::math::vector<real>&,
		   const tfel::math::vector<real>&,
		   const tfel::material::ModellingHypothesis::Hypothesis,
		   const real,
		   const MTestStiffnessMatrixType::mtype,
		   const bool) const;
    //! the aster fonction
    tfel::system::AsterFctPtr fct;
    //! internal array
    mutable tfel::math::vector<real> iv;
    //! internal workspace
    mutable tfel::math::matrix<real> D;
    //! save tangent operator
    bool savesTangentOperator;
  }; // end of struct MTestBehaviour
  
} // end of namespace mfront

#endif /* _LIB_MFRONT_MTESTASTERBEHAVIOUR_H */

