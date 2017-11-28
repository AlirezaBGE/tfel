/*!
 * \file   src/System/SystemError.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   07 Nov 2007
 * \copyright Copyright (C) 2006-2018 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include"TFEL/System/SystemError.hxx"

namespace tfel
{
  namespace system
  {

    SystemError::SystemError(const std::string& msg_)
      : msg(msg_)
    {} // end of SystemError::SystemError

    const char*
    SystemError::what() const noexcept
    {
      return this->msg.c_str();
    }

    SystemError::~SystemError() noexcept = default;

  } // end of namespace system

} // end of namespace tfel
