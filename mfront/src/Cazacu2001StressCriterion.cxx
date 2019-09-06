/*!
 * \file   mfront/src/Cazacu2001StressCriterion.cxx
 * \brief
 * \author Thomas Helfer
 * \date   15/03/2018
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#include "TFEL/Raise.hxx"
#include "MFront/BehaviourBrick/BrickUtilities.hxx"
#include "MFront/BehaviourBrick/StressPotential.hxx"
#include "MFront/BehaviourBrick/OptionDescription.hxx"
#include "MFront/BehaviourBrick/Cazacu2001StressCriterion.hxx"

namespace mfront {

  namespace bbrick {

    std::vector<OptionDescription> Cazacu2001StressCriterion::getOptions()
        const {
      auto opts = StressCriterionBase::getOptions();
      opts.emplace_back("a", "coefficients of J20",
                        OptionDescription::ARRAYOFMATERIALPROPERTIES);
      opts.emplace_back("b", "coefficients of J30",
                        OptionDescription::ARRAYOFMATERIALPROPERTIES);
      opts.emplace_back("c", "", OptionDescription::MATERIALPROPERTY);
      return opts;
    }  // end of Cazacu2001StressCriterion::getOptions()

    std::vector<mfront::BehaviourSymmetryType>
    Cazacu2001StressCriterion::getSupportedBehaviourSymmetries() const {
      return {mfront::ISOTROPIC, mfront::ORTHOTROPIC};
    }  // end of Cazacu2001StressCriterion::getSupportedBehaviourSymmetries()

    void Cazacu2001StressCriterion::initialize(BehaviourDescription& bd,
                                               AbstractBehaviourDSL& dsl,
                                               const std::string& id,
                                               const DataMap& d,
                                               const Role r) {
      using ConstantMaterialProperty =
          mfront::BehaviourDescription::ConstantMaterialProperty;
      tfel::raise_if(bd.getSymmetryType() != mfront::ORTHOTROPIC,
                     "Cazacu2004OrthotropicStressCriterion::initialize: "
                     "the behaviour must be orthotropic");
      StressCriterionBase::initialize(bd, dsl, id, d, r);
      const auto an = StressCriterion::getVariableId("a", id, r);
      const auto bn = StressCriterion::getVariableId("b", id, r);
      const auto cn = StressCriterion::getVariableId("c", id, r);
      bd.appendToIncludes(
          "#include\"TFEL/Material/Cazacu2001YieldCriterion.hxx\"");
      tfel::raise_if(d.count("c") == 0,
                     "Cazacu2001StressCriterion::initialize: "
                     "material property 'c' is not defined");
      // J2O coefficients
      if (d.count("a") == 0) {
        tfel::raise("Cazacu2001StressCriterion::initialize: entry '" +
                    std::string("a") + "' is not defined");
      }
      this->a = getArrayOfBehaviourDescriptionMaterialProperties<6u>(dsl, "a",
                                                                     d.at("a"));
      if (this->a[0].is<ConstantMaterialProperty>()) {
        std::vector<double> values(6u);
        for (unsigned short i = 0; i != 6; ++i) {
          tfel::raise_if(!this->a[i].is<ConstantMaterialProperty>(),
                         "Cazacu2001StressCriterion::initialize: "
                         "if one component of '" +
                             std::string("a") +
                             "' is a constant value, all components must be "
                             "constant values");
          values[i] = this->a[i].get<ConstantMaterialProperty>().value;
        }
        const auto ean = [&r] {
          if (r == StressCriterion::STRESSANDFLOWCRITERION) {
            return "J2OCoefficients";
          } else if (r == StressCriterion::STRESSCRITERION) {
            return "StressCriterionJ2OCoefficients";
          }
          return "FlowCriterionJ2OCoefficients";
        }();
        addParameter(bd, an, ean, 6, values);
      } else {
        addLocalVariable(bd, "real", an, 6);
      }
      // J3O coefficients
      if (d.count("b") == 0) {
        tfel::raise("Cazacu2001StressCriterion::initialize: entry '" +
                    std::string("b") + "' is not defined");
      }
      this->b = getArrayOfBehaviourDescriptionMaterialProperties<11u>(
          dsl, "b", d.at("b"));
      if (this->b[0].is<ConstantMaterialProperty>()) {
        std::vector<double> values(11u);
        for (unsigned short i = 0; i != 11; ++i) {
          tfel::raise_if(!this->b[i].is<ConstantMaterialProperty>(),
                         "Cazacu2001StressCriterion::initialize: "
                         "if one component of '" +
                             std::string("b") +
                             "' is a constant value, all components must be "
                             "constant values");
          values[i] = this->b[i].get<ConstantMaterialProperty>().value;
        }
        const auto ebn = [&r] {
          if (r == StressCriterion::STRESSANDFLOWCRITERION) {
            return "J3OCoefficients";
          } else if (r == StressCriterion::STRESSCRITERION) {
            return "StressCriterionJ3OCoefficients";
          }
          return "FlowCriterionJ3OCoefficients";
        }();
        addParameter(bd, bn, ebn, 11, values);
      } else {
        addLocalVariable(bd, "real", bn, 11);
      }
      this->cp = getBehaviourDescriptionMaterialProperty(dsl, "c", d.at("c"));
      declareParameterOrLocalVariable(bd, this->cp, "real", cn);
    }  // end of Cazacu2001StressCriterion::initialize

    void Cazacu2001StressCriterion::endTreatment(
        BehaviourDescription& bd,
        const AbstractBehaviourDSL& dsl,
        const std::string& id,
        const Role r) {
      constexpr const auto uh =
          tfel::material::ModellingHypothesis::UNDEFINEDHYPOTHESIS;
      const auto an = StressCriterion::getVariableId("a", id, r);
      const auto bn = StressCriterion::getVariableId("b", id, r);
      const auto cn = StressCriterion::getVariableId("c", id, r);
      if (bd.getOrthotropicAxesConvention() ==
          tfel::material::OrthotropicAxesConvention::PIPE) {
        for (const auto mh : bd.getModellingHypotheses()) {
          tfel::raise_if(
              ((mh != ModellingHypothesis::TRIDIMENSIONAL) &&
               (mh != ModellingHypothesis::AXISYMMETRICAL) &&
               (mh !=
                ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRAIN) &&
               (mh !=
                ModellingHypothesis::AXISYMMETRICALGENERALISEDPLANESTRESS)),
              "Cazacu2001Criterion::endTreatment: "
              "with the Pipe orthotropic axis convention, "
              "only the following modelling hypotheses are supported:\n"
              "- Tridimensional\n"
              "- Axisymmetrical\n"
              "- AxisymmetricalGeneralisedPlaneStrain\n"
              "- AxisymmetricalGeneralisedPlaneStress.");
        }
      }
      if (bd.getOrthotropicAxesConvention() ==
          tfel::material::OrthotropicAxesConvention::PLATE) {
        for (const auto mh : bd.getModellingHypotheses()) {
          tfel::raise_if(
              ((mh != ModellingHypothesis::TRIDIMENSIONAL) &&
               (mh != ModellingHypothesis::PLANESTRESS)),
              "Cazacu2001Criterion::endTreatment: "
              "with the Plate orthotropic axis convention, "
              "only the following modelling hypotheses are supported:\n"
              "- Tridimensional\n"
              "- Plane stress");
        }
      } else {
        tfel::raise_if(
            bd.getOrthotropicAxesConvention() !=
                tfel::material::OrthotropicAxesConvention::DEFAULT,
            "Cazacu2001Criterion::endTreatment: "
            "internal error, unsupported orthotropic axes convention");
        for (const auto mh : bd.getModellingHypotheses()) {
          tfel::raise_if(mh != ModellingHypothesis::TRIDIMENSIONAL,
                         "Cazacu2001Criterion::endTreatment: "
                         "an orthotropic axes convention must be choosen when "
                         "defining a stress free expansion in behaviours "
                         "which shall be valid in other modelling hypothesis "
                         "than 'Tridimensional'.\n"
                         "Either restrict the validity of the behaviour to "
                         "'Tridimensional' (see @ModellingHypothesis) or "
                         "choose and orthotropic axes convention as on option "
                         "to the @OrthotropicBehaviour keyword");
        }
      }
      auto c = std::string{};
      c += generateMaterialPropertiesInitializationCode(dsl, bd, an, this->a);
      c += generateMaterialPropertiesInitializationCode(dsl, bd, bn, this->b);
      c += generateMaterialPropertyInitializationCode(dsl, bd, cn, this->cp);
      if (!c.empty()) {
        CodeBlock i;
        i.code = c;
        bd.setCode(uh, BehaviourData::BeforeInitializeLocalVariables, i,
                   BehaviourData::CREATEORAPPEND, BehaviourData::AT_BEGINNING);
      }
    }  // end of Cazacu2001StressCriterion::endTreatment

    std::string Cazacu2001StressCriterion::computeElasticPrediction(
        const std::string& id,
        const BehaviourDescription&,
        const StressPotential&) const {
      const auto an = StressCriterion::getVariableId(
          "a", id, StressCriterion::STRESSCRITERION);
      const auto bn = StressCriterion::getVariableId(
          "b", id, StressCriterion::STRESSCRITERION);
      const auto cn = StressCriterion::getVariableId(
          "c", id, StressCriterion::STRESSCRITERION);
      return "const auto seqel" + id +
             " = computeCazacu2001StressCriterion(sel" + id + ",this->" + an +
             ",this->" + bn + ",this->" + cn + ");\n";
    }  // end of Cazacu2001StressCriterion::computeElasticPrediction

    std::string Cazacu2001StressCriterion::computeCriterion(
        const std::string& id,
        const BehaviourDescription&,
        const StressPotential&) const {
      const auto an = StressCriterion::getVariableId(
          "a", id, StressCriterion::STRESSCRITERION);
      const auto bn = StressCriterion::getVariableId(
          "b", id, StressCriterion::STRESSCRITERION);
      const auto cn = StressCriterion::getVariableId(
          "c", id, StressCriterion::STRESSCRITERION);
      return "const auto seq" + id + " = computeCazacu2001StressCriterion(s" +
             id + ",this->" + an + ",this->" + bn + ",this->" + cn + ");\n";
    }  // end of Cazacu2001StressCriterion::computeCriterion

    std::string Cazacu2001StressCriterion::computeNormal(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const Role r) const {
      const auto an = StressCriterion::getVariableId(
          "a", id, StressCriterion::STRESSCRITERION);
      const auto bn = StressCriterion::getVariableId(
          "b", id, StressCriterion::STRESSCRITERION);
      const auto cn = StressCriterion::getVariableId("c", id, r);
      auto c = std::string{};
      if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
#if __cplusplus >= 201703L
        c += "const auto [seq" + id + ",dseq" + id + "_ds" + id + "] = ";
        c += "computeCazacu2001StressCriterionNormal(s" + id + ", this->" + an +
             ", this->" + bn + ", this->" + cn + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
#else  /* __cplusplus >= 201703L */
        c += "stress seq" + id + ";\n";
        c += "Stensor dseq" + id + "_ds" + id + ";\n";
        c += "std::tie(seq" + id + ",dseq" + id + "_ds" + id + ") = ";
        c += "computeCazacu2001StressCriterionNormal(s" + id + ", this->" + an +
             ", this->" + bn + ", this->" + cn + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      if (r == STRESSANDFLOWCRITERION) {
        c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
      }
      if (r == FLOWCRITERION) {
#if __cplusplus >= 201703L
        c += "const auto [seqf" + id + ",dseq" + id + "_ds" + id + "] = ";
        c += "computeCazacu2001StressCriterionNormal(s" + id + ", this->" + an +
             ", this->" + bn + ", this->" + cn + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
#else  /* __cplusplus >= 201703L */
        c += "stress seqf" + id + ";\n";
        c += "Stensor n" + id + ";\n";
        c += "std::tie(seqf" + id + ",n" + id + ") = ";
        c += "computeCazacu2001StressCriterionNormal(s" + id + ", this->" + an +
             ", this->" + bn + ", this->" + cn + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      return c;
    }  // end of Cazacu2001StressCriterion::computeNormal

    std::string Cazacu2001StressCriterion::computeNormalDerivative(
        const std::string& id,
        const BehaviourDescription& bd,
        const StressPotential& sp,
        const Role r) const {
      const auto an = StressCriterion::getVariableId(
          "a", id, StressCriterion::STRESSCRITERION);
      const auto bn = StressCriterion::getVariableId(
          "b", id, StressCriterion::STRESSCRITERION);
      const auto cn = StressCriterion::getVariableId("c", id, r);
      auto c = std::string{};
      if ((r == STRESSCRITERION) || (r == STRESSANDFLOWCRITERION)) {
#if __cplusplus >= 201703L
        c += "const auto [seq" + id + ",dseq" + id + "_ds" + id + ",d2seq" +
             id + "_ds" + id + "ds" + id + "] = ";
        c += "computeCazacu2001StressCriterionSecondDerivative(s" + id +
             ", this->" + an + ", this->" + bn + ", this->" + cn + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
#else  /* __cplusplus >= 201703L */
        c += "stress seq" + id + ";\n";
        c += "Stensor dseq" + id + "_ds" + id + ";\n";
        c += "Stensor4 d2seq" + id + "_ds" + id + "ds" + id + ";\n";
        c += "std::tie(seq" + id + ",dseq" + id + "_ds" + id + ",d2seq" + id +
             "_ds" + id + "ds" + id + ") = ";
        c += "computeCazacu2001StressCriterionSecondDerivative(s" + id +
             ", this->" + an + ", this->" + bn + ", this->" + cn + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      if (r == STRESSANDFLOWCRITERION) {
        c += "const auto& n" + id + " = dseq" + id + "_ds" + id + ";\n";
        c += "const auto& dn" + id + "_ds" + id + " = ";
        c += "d2seq" + id + "_ds" + id + "ds" + id + ";\n";
      }
      if (r == FLOWCRITERION) {
#if __cplusplus >= 201703L
        c += "const auto [seq" + id + ", n" + id + ", dn" + id + "_ds" + id +
             "] = ";
        c += "computeCazacu2001StressCriterionSecondDerivative(s" + id +
             ", this->" + an + ", this->" + bn + ",this->" + cn + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
#else  /* __cplusplus >= 201703L */
        c += "stress seqf" + id + ";\n";
        c += "Stensor n" + id + ";\n";
        c += "Stensor4 dn" + id + "_ds" + id + ";\n";
        c +=
            "std::tie(seqf" + id + ",n" + id + ",dn" + id + "_ds" + id + ") = ";
        c += "computeCazacu2001StressCriterionSecondDerivative(s" + id +
             +", this->" + an + ", this->" + bn + ",this->" + cn + "," +
             sp.getEquivalentStressLowerBound(bd) + ");\n";
#endif /* __cplusplus >= 201703L */
      }
      return c;
    }  // end of Cazacu2001StressCriterion::computeNormalDerivative

    bool Cazacu2001StressCriterion::isNormalDeviatoric() const {
      return true;
    }  // end of Cazacu2001StressCriterion::isNormalDeviatoric

    Cazacu2001StressCriterion::~Cazacu2001StressCriterion() = default;

  }  // end of namespace bbrick

}  // end of namespace mfront
