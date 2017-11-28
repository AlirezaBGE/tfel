/*! 
 * \file  mtest/include/MTest/SteffensenAccelerationAlgorithm.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_MTEST_MTEST_MTESTSTEFFENSENACCELERATIONALGORITHM_HXX
#define LIB_MTEST_MTEST_MTESTSTEFFENSENACCELERATIONALGORITHM_HXX 

#include"MTest/Config.hxx"
#include"MTest/AccelerationAlgorithm.hxx"

namespace mtest
{

  /*!
   * \brief This class implements the Steffensen acceleration algorithm.
   */
  struct MTEST_VISIBILITY_EXPORT SteffensenAccelerationAlgorithm
    : public AccelerationAlgorithm
  {
    //! default constructor
    SteffensenAccelerationAlgorithm();
    /*!
     * \param[in] psz : total number of unknowns (problem size)
     */
    virtual void
    initialize(const unsigned short) override;
    /*!
     * \return the name of the acceleration algorithm
     */
    virtual std::string
    getName() const override;
    /*!
     * set a parameter of the acceleration algorithm
     * \param[in] p : parameter name
     * \param[in] v : parameter value
     */
    virtual void
    setParameter(const std::string&,
		 const std::string&) override;
    /*!
     * called at each time step, before the beginning of the Newton
     * algorithm
     */
    virtual void
    preExecuteTasks() override;
    /*!
     * \param[in,out] u1   : current estimate of the unknowns
     * \param[in]     u0   : current estimate of the unknowns
     * \param[in]     r    : Newton residual
     * \param[in]     ueps : criterium on the unknowns
     * \param[in]     seps : criterium on the thermodynamic forces
     * \param[in]     iter : current iteration number
     */
    virtual void
    execute(tfel::math::vector<real>&,
	    const tfel::math::vector<real>&,
	    const tfel::math::vector<real>&,
	    const real,
	    const real,
	    const unsigned short) override;
    /*!
     * called at each time step, once convergence is reached
     */
    virtual void
    postExecuteTasks() override;
    //! destructor
    virtual ~SteffensenAccelerationAlgorithm();
  protected:
    tfel::math::vector<real> sta_u0;
    tfel::math::vector<real> sta_u1;
    tfel::math::vector<real> sta_u2;
    tfel::math::vector<real> sta_du2;
    tfel::math::vector<real> sta_du1;
    //! trigger
    int stat;
  }; // end of struct SteffensenAccelerationAlgorithm

} // end of namespace mtest

#endif /* LIB_MTEST_MTEST_MTESTSTEFFENSENACCELERATIONALGORITHM_HXX */

