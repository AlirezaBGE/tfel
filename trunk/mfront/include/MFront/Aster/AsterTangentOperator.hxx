/*! 
 * \file  AsterTangentOperator.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 févr. 2013
 */

#ifndef _LIB_MFRONT_ASTER_ASTERTANGENTOPERATOR_H_
#define _LIB_MFRONT_ASTER_ASTERTANGENTOPERATOR_H_ 

#include"TFEL/Math/Forward/st2tost2.hxx"

#include"MFront/Aster/Aster.hxx"
#include"MFront/Aster/AsterConfig.hxx"

namespace aster
{
  
  /*!
   * \brief an helper structure introduced to normalise the tangent
   * operator to follow the umat interface
   */
  struct MFRONT_ASTER_VISIBILITY_EXPORT AsterTangentOperator
  {
    /*!
     * \brief normalize the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::st2tost2<1u,AsterReal>&);
    /*!
     * \brief normalize the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::st2tost2<2u,AsterReal>&);
    /*!
     * \brief normalize the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    normalize(tfel::math::st2tost2<3u,AsterReal>&);
    /*!
     * \brief transpose the tangent operator in 1D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::st2tost2<1u,AsterReal>&);
    /*!
     * \brief transpose the tangent operator in 2D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::st2tost2<2u,AsterReal>&);
    /*!
     * \brief transpose the tangent operator in 3D
     * \param[in] Dt : tangent operator
     */
    static void
    transpose(tfel::math::st2tost2<3u,AsterReal>&);
  }; // end of struct AsterTangentOperator

} // end of namespace aster

#endif /* _LIB_MFRONT_ASTER_ASTERTANGENTOPERATOR_H */

