/*!
 * \file   IsReal.hxx
 * \brief  This file declares the IsReal traits class
 * and specialises it for standard numeric types.
 * \author Helfer Thomas
 * \date   04 Jul 2006
 */

#ifndef _LIB_TFEL_ISREAL_HXX_
#define _LIB_TFEL_ISREAL_HXX_ 

/*!
 * \def    TFEL_META_IS_REAL
 * \brief An helper macro to specialise IsReal for each standard
 * "real" numeric types.
 * \author Helfer Thomas
 * \date   28 jun 2006 
 */
#define TFEL_META_IS_REAL(X)               \
    /*!                                    \
     * \brief Partial specialisation for X \
     * \see   IsReal                       \
     */                                    \
    template<>                             \
    struct IsReal< X >{                    \
      /*!				   \
       *  Result			   \
       */				   \
      static const bool cond = true;       \
    };                                     \
    /*!                                    \
     * \brief Partial specialisation for X \
     * \see   IsReal                       \
     */                                    \
    template<>                             \
    struct IsReal<const X >{               \
      /*!				   \
       *  Result			   \
       */				   \
      static const bool cond = true;       \
    }                                 

namespace tfel{

  namespace typetraits{
    
    /*!
     * \brief Traits class which says it a given type stands for a
     * real numerical type.
     *
     * \param  T, type to be tested.
     * \return bool cond, true if T stands for a real numerical type,
     * false otherwise.
     *
     * \see IsScalar, IsComplex
     *
     * \author Helfer Thomas
     * \date   04 Jul 2006
     */
    template<typename T>
    struct IsReal{
      /*!
       *  Result
       */
      static const bool cond = false;
    };

    TFEL_META_IS_REAL(float);
    TFEL_META_IS_REAL(double);
    TFEL_META_IS_REAL(long double);

  } // end of namespace typetraits

} // end of namespace tfel
  
#endif /* _LIB_TFEL_ISREAL_HXX */

