/*!
 * \file   mfront/include/MFront/BehaviourBrick/HookeStressPotentialBase.hxx
 * \brief
 * \author Thomas Helfer
 * \date   20/03/2018
 */

#ifndef LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIALBASE_HXX
#define LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIALBASE_HXX

#include "MFront/BehaviourBrick/StressPotential.hxx"

namespace mfront {

  // forward declaration
  struct LocalDataStructure;

  namespace bbrick {

    /*!
     * \brief class describing the computation of the stress through the
     * standard Hooke law.
     */
    struct HookeStressPotentialBase : StressPotential {
      /*!
       * \return the options associated with material properties which are valid
       * for either an isotropic or an orthotropic behaviour.
       * The following options are defined:
       * - `young_modulus`
       * - `poisson_ratio`
       * - `thermal_expansion`
       */
      static std::vector<OptionDescription> getIsotropicBehaviourOptions();
      /*!
       * \return the options associated with material properties which are only
       * valid for an orthotropic behaviour.
       * The following options are defined:
       * - `young_modulus1`
       * - `young_modulus2`
       * - `young_modulus3`
       * - `poisson_ratio12`
       * - `poisson_ratio23`
       * - `poisson_ratio13`
       * - `thermal_expansion1`
       * - `thermal_expansion2`
       * - `thermal_expansion3`
       */
      static std::vector<OptionDescription> getOrthotropicBehaviourOptions();
      //! get options only valid for all behaviours
      static std::vector<OptionDescription> getGeneralOptions();
      //! constructor
      HookeStressPotentialBase();
      std::string getName() const override;
      std::vector<OptionDescription> getOptions() const override;
      void initialize(BehaviourDescription&,
                      AbstractBehaviourDSL&,
                      const DataMap&) override;
      std::vector<Hypothesis> getSupportedModellingHypotheses(
          const BehaviourDescription&,
          const AbstractBehaviourDSL&) const override;
      void completeVariableDeclaration(
          BehaviourDescription&, const AbstractBehaviourDSL&) const override;
      void endTreatment(BehaviourDescription&,
                        const AbstractBehaviourDSL&) const override;
      void writeStressDerivatives(BehaviourDescription&) const override;
      void computeElasticPrediction(BehaviourDescription&) const override;
      std::string getStressNormalisationFactor(
          const BehaviourDescription&) const override;
      std::string getEquivalentStressLowerBound(
          const BehaviourDescription&) const override;
      //! destructor
      ~HookeStressPotentialBase() override;

     protected:
      /*!
       * \brief declared the `BehaviourData::ComputeStress` and
       * `BehaviourData::ComputeFinalStress` code blocks when either the
       * `BehaviourDescription::requiresStiffnessTensor` attribute or the
       * `BehaviourDescription::computesStiffnessTensor` attribute
       * has been set.
       * \param[in] bd: behaviour description
       */
      virtual void declareComputeStressWhenStiffnessTensorIsDefined(
          BehaviourDescription&) const = 0;
      /*!
       * \brief complete the variable description in the case of isotropic
       * behaviours when the stiffness tensor is not defined.
       * This method is meant to define the `BehaviourData::ComputeStress`
       * and `BehaviourData::ComputeFinalStress` code blocks.
       * \param[in] bd: behaviour description
       * \param[in] d: local data structure
       */
      virtual void declareComputeStressForIsotropicBehaviour(
          BehaviourDescription&, LocalDataStructure&) const = 0;
      /*!
       * \brief treat the case of orthotropic behaviours when the stiffness
       * tensor is not defined.
       * This case is generally not supported and lead to an error.
       * \param[in] bd: behaviour description
       */
      virtual void declareComputeStressForOrthotropicBehaviour(
          BehaviourDescription&) const;
      /*!
       * \brief add support for the AXISYMMETRICALGENERALISEDPLANESTRESS
       * modelling hypothesis
       * \param[in] bd: behaviour description
       */
      virtual void addAxisymmetricalGeneralisedPlaneStressSupport(
          BehaviourDescription&, const AbstractBehaviourDSL&) const;
      /*!
       * \brief add support for the PLANESTRESS modelling hypothesis
       * \param[in] bd: behaviour description
       */
      virtual void addPlaneStressSupport(BehaviourDescription&,
                                         const AbstractBehaviourDSL&) const;
      /*!
       * \brief add the generic tangent operator computation
       * \param[in] bd: behaviour description
       */
      virtual void addGenericTangentOperatorSupport(
          BehaviourDescription&, const AbstractBehaviourDSL&) const = 0;
      /*!
       * \brief add the generic prediction operator computation
       * \param[in] bd: behaviour description
       */
      virtual void addGenericPredictionOperatorSupport(
          BehaviourDescription&) const = 0;
      /*!
       * \brief declare the compute elastic prediction method
       * \param[in] bd: behaviour description
       */
      virtual void declareComputeElasticPredictionMethod(
          BehaviourDescription&) const = 0;

      //! plane stress support;
      bool pss = true;
      //! generic prediction operator support
      bool gto = true;
      //! generic tangent operator support
      bool gpo = true;
    };  // end of struct HookeStressPotentialBase

  }  // end of namespace bbrick

}  // end of namespace mfront

#endif /* LIB_MFRONT_BEHAVIOURBRICK_HOOKESTRESSPOTENTIALBASE_HXX */
