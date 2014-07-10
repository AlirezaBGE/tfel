/*!
 * \file   UMATOrthotropicBehaviour.hxx
 * \brief  This file declares the UMATOrthotropicBehaviour class
 * \author Helfer Thomas
 * \date   18 mar 2008
 */

#ifndef _LIB_MFRONT_UMATORTHOTROPICBEHAVIOUR_H_
#define _LIB_MFRONT_UMATORTHOTROPICBEHAVIOUR_H_ 

#include"MFront/UMAT/UMATConfig.hxx"
#include"MFront/UMAT/UMATTraits.hxx"

namespace umat
{

  /*!
   * This behaviour tells which behaviour is used
   * \param[in] Type : behaviour type
   * \param[in] N    : spatial dimension
   */
  template<UMATBehaviourType Type,
	   unsigned short N>
  struct UMATOrthotropicOffset;

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,1u>
  {
    // 6 elastic coefficients
    //   mass density
    // 3 thermal expansion coefficients
    static const unsigned short value = 10u;
  }; // end of struct UMATOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,1u>

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,2u>
  {
    // 7 elastic coefficients
    // 2 components of direction
    //   mass density
    // 3 thermal expansion coefficients
    static const unsigned short value = 13u;
  }; // end of struct UMATOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,2u>

  template<>
  struct MFRONT_UMAT_VISIBILITY_EXPORT
  UMATOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,3u>
  {
    // 9 elastic coefficients
    // 6 components of direction
    //   mass density
    // 3 thermal expansion coefficients
    static const unsigned short value = 19u;
  }; // end of struct UMATOrthotropicOffset<SMALLSTRAINSTANDARDBEHAVIOUR,3u>

} // end of namespace umat

#endif /* _LIB_MFRONT_UMATORTHOTROPICBEHAVIOUR_H */

