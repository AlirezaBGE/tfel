/*!
 * \file   mfront/include/MFront/VariableBoundsDescription.hxx
 * \brief  This file declares the VariableBoundsDescription class
 * \author Helfer Thomas
 * \date   06 mai 2008
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifndef LIB_VARIABLEBOUNDSDESCRIPTION_HXX
#define LIB_VARIABLEBOUNDSDESCRIPTION_HXX 

#include<string>
#include<limits>
#include"MFront/MFrontConfig.hxx"

namespace mfront
{

  /*!
   * \brief a structure in charge of describing bounds on a variable.
   * Two kinds of bounds are considered:
   * - physical bounds: give the bounds out of which this variable is
   *   not physically defined. For example, a temperature must be
   *   positive.
   * - (standard) bounds: give the bounds out of which the correlation
   *   has not been identified.
   */
  struct MFRONT_VISIBILITY_EXPORT VariableBoundsDescription
  {
    //! \brief bounds type
    enum BoundsType{
      LOWER,
      UPPER,
      LOWERANDUPPER
    }; // end of enum BoundsType
    //! default constructor
    VariableBoundsDescription() = default;
    //! copy constructor
    VariableBoundsDescription(VariableBoundsDescription&&) = default;
    //! move constructor
    VariableBoundsDescription(const VariableBoundsDescription&) = default;
    //! move assignement
    VariableBoundsDescription& operator=(VariableBoundsDescription&&) = default;
    //! standard assignement
    VariableBoundsDescription& operator=(const VariableBoundsDescription&) = default;
    //! \return true if the bounds type is either LOWER or LOWERANDUPPER
    bool hasLowerBound() const;
    //! \return true if the bounds type is either UPPER or LOWERANDUPPER
    bool hasUpperBound() const;
    //! destructor
    ~VariableBoundsDescription() noexcept;
    //! type of the bound
    BoundsType boundsType;
    size_t     lineNumber = 0u;
    // component. If component is egal to -1, bounds acts of all
    // components
    short component   = -1;
    long double lowerBound = std::numeric_limits<long double>::min();
    long double upperBound = std::numeric_limits<long double>::max();
  }; // end of class VariableBoundsDescription

} // end of namespace mfront

#endif /* LIB_VARIABLEBOUNDSDESCRIPTION_HXX */

