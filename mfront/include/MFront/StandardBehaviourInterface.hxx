/*!
 * \file  mfront/include/MFront/StandardBehaviourInterface.hxx
 * \brief
 * \author Thomas Helfer
 * \brief 10 juil. 2013
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights
 * reserved.
 * This project is publicly released under either the GNU GPL Licence
 * or the CECILL-A licence. A copy of thoses licences are delivered
 * with the sources of TFEL. CEA or EDF may also distribute this
 * project under specific licensing conditions.
 */

#ifndef LIB_MFRONT_STANDARDBEHAVIOURINTERFACE_HXX
#define LIB_MFRONT_STANDARDBEHAVIOURINTERFACE_HXX

#include "MFront/SupportedTypes.hxx"
#include "MFront/AbstractBehaviourInterface.hxx"
#include "MFront/BehaviourMaterialProperty.hxx"

namespace mfront {

  /*!
   * This class provides some helper function for behaviours
   * interfaces based on C-functions as entry points, which is
   * currently the most current case (to the very exception of the
   * `ZMAT` interface).
   */
  struct MFRONT_VISIBILITY_EXPORT StandardBehaviourInterface
      : public AbstractBehaviourInterface,
        public SupportedTypes {
    //! attribute used to store if an MTest file shall be generated
    static const char* const generateMTestFileAttribute;
    //! a simple alias
    using ModellingHypothesis = tfel::material::ModellingHypothesis;
    //! a simple alias
    using Hypothesis = ModellingHypothesis::Hypothesis;
    // constructor
    StandardBehaviourInterface();
    bool isBehaviourConstructorRequired(
        const Hypothesis, const BehaviourDescription&) const override;
    /*!
     * \return the basis for the function(s)' names implementing the
     * behaviour
     * generated by the interface. This can be different from the name of
     * the
     * behaviour to comply with the targeted code restrictions. For example,
     * the
     * `Abaqus` interfaces require the function name to be upper-cased.
     * \note Some interfaces generate one function per behaviour. The name
     * of
     * this function shall be the one returned by this method. The other
     * interfaces generate one function per hypothesis: the function to be
     * called the appends a suffix associated with the modelling hypothesis
     * to
     * the name returned by this method. In any case, the `UMAT++` symbols'
     * names are build using the name returned by this method.
     * \param[in] n : name of the behaviour as defined by interface
     *                (generally taking into account the material
     *                 and the behaviour name)
     */
    virtual std::string getFunctionNameBasis(const std::string &) const = 0;
    /*!
     * \return an header guard based on the name of the interface, as returned
     * by `getInterfaceName`, and the name of the behaviour.
     * \param[in] bd: behaviour description
     */
    virtual std::string getHeaderGuard(const BehaviourDescription&) const;
    /*!
     * \return a pair which first member gives the position of the
     * material properties in the values given through the interface
     * and which second member is an offset giving the number of
     * imposed material properties.
     * \param[in] mb: behaviour description
     * \param[in] h:  modelling hypothesis
     */
    virtual std::pair<std::vector<BehaviourMaterialProperty>,
                      SupportedTypes::TypeSize>
    buildMaterialPropertiesList(const BehaviourDescription &,
                                const Hypothesis) const;
    /*!
     * \brief include the appropriate headers and write the definition
     * of the `MFRONT_SHAREDOBJ` macro which is used to define the
     * visibility of the generated functions.
     * \param[out] out: output file
     */
    virtual void writeVisibilityDefines(std::ostream&) const;

    virtual void writeSetOutOfBoundsPolicyFunctionDeclaration(
        std::ostream&, const std::string&) const;

    virtual void writeGetOutOfBoundsPolicyFunctionImplementation(
        std::ostream&, const std::string&) const;

    virtual void writeSetOutOfBoundsPolicyFunctionImplementation(
        std::ostream&, const std::string&) const;

    virtual void writeSetParametersFunctionsDeclarations(
        std::ostream&, const BehaviourDescription&, const std::string&) const;
    /*!
     * \brief generate the implementations of the functions allowing
     * to modify the parameters of a behaviour.
     * \param[out] out: output stream
     */
    virtual void writeSetParametersFunctionsImplementations(std::ostream&, const BehaviourDescription&,const std::string&) const;
    /*!
     * \brief set an attribute stating that a `MTest` file shall be generated on
     * integration failure.
     * \param[in,out] bd: behavour description
     * \param[in] b: attribute value
     */
    virtual void setGenerateMTestFileOnFailureAttribute(BehaviourDescription&,
                                                        const bool) const;
    /*!
     * \return if a behaviour shall generate a `MTest` file
     * on integration failure.
     * \param[in] bd: behavour description
     */
    virtual bool shallGenerateMTestFileOnFailure(
        const BehaviourDescription&) const;
    /*!
     * \return a pair. If the first entry is true, the "axial strain"
     * was found and the second contains its offset
     * \param[in] mb : material description
     */
    virtual std::pair<bool, SupportedTypes::TypeSize>
    checkIfAxialStrainIsDefinedAndGetItsOffset(
        const BehaviourDescription&) const;
    //! destructor
    ~StandardBehaviourInterface() override;
  };  // end of struct StandardBehaviourInterface

}  // end of namespace mfront

#endif /* LIB_MFRONT_STANDARDBEHAVIOURINTERFACE_HXX */
