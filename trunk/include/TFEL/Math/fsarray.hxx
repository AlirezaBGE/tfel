/*! 
 * \file  fsarray.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 août 2013
 */

#ifndef _LIB_TFEL_MATH_FSARRAY_H_
#define _LIB_TFEL_MATH_FSARRAY_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Metaprogramming/StaticAssert.hxx"

namespace tfel
{

  namespace math
  {
    
    /*!
     * \brief a container for finite size array.
     * This is mainly equivalent to the array class of C++11.
     * \param N : size of the array
     * \param T : value holded
     */
    template<unsigned short N,
	     typename T>
    struct TFEL_VISIBILITY_LOCAL fsarray
    {
      /*!
       * type of the tvector's values.
       * (this is a stl requirement).
       */
      typedef T value_type;
      /*!
       * type of a pointer to the value contained.
       * (this is a stl requirement).
       */
      typedef value_type* pointer;
      /*!
       * type of a const pointer to the value contained.
       * (this is a stl requirement).
       */
      typedef const value_type* const_pointer;
      /*!
       * type of a reference to the value contained.
       * (this is a stl requirement).
       */
      typedef value_type& reference;
      /*!
       * type of a const reference to the value contained.
       * (this is a stl requirement).
       */
      typedef const value_type& const_reference;
      /*!
       * type of the size of the container.
       * (this is a stl requirement).
       */
      typedef unsigned short size_type;
      /*!
       * type of the difference between two iterators.
       * (this is a stl requirement).
       */
      typedef ptrdiff_t difference_type;
      /*!
       * type of the tvector's iterator.
       * (provided for stl compatibility).
       */
      typedef pointer iterator;
      /*!
       * type of the tvector's const iterator.
       * (provided for stl compatibility).
       */
      typedef const_pointer const_iterator;
      /*!
       * type of the tvector's reverse iterator.
       * (provided for stl compatibility).
       */
#ifndef __SUNPRO_CC
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator; 
#else
      typedef std::reverse_iterator<const_iterator,T,
      				    const_reference,
      				    difference_type> const_reverse_iterator;
#endif
      /*!
       * type of the tvector's const reverse iterator.
       * (provided for stl compatibility).
       */
#ifndef __SUNPRO_CC
      typedef std::reverse_iterator<iterator> reverse_iterator;
#else
      typedef std::reverse_iterator<iterator,T,
      				    reference,
      				    difference_type> reverse_iterator;
#endif
      /*!
       * return an iterator to the first element of the vector
       * (provided for stl compatibility)
       * \return iterator, an iterator to the first element
       */
      TFEL_MATH_INLINE iterator
      begin(void);
      /*!
       * return an const iterator to the first element of the vector
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator to the first element
       */
      TFEL_MATH_INLINE const_iterator 
      begin(void) const;
      /*!
       * return an iterator after the last element of the vector
       * (provided for stl compatibility)
       * \return iterator, an iterator after the last element
       */
      TFEL_MATH_INLINE iterator
      end(void);
      /*!
       * return an const iterator after the last element of the vector
       * (provided for stl compatibility)
       * \return const_iterator, a const iterator after the last element
       */
      TFEL_MATH_INLINE const_iterator
      end(void) const;
      /*!
       * return an reverse iterator to the last element of the vector
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator to the last element
       */
      TFEL_MATH_INLINE reverse_iterator
      rbegin(void);
      /*!
       * return an const reverse iterator to the last element of the vector
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator to the last element
       */
      TFEL_MATH_INLINE const_reverse_iterator
      rbegin(void) const;
      
      /*!
       * return an  reverse iterator before the first element of the vector
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator before the first element
       */
      TFEL_MATH_INLINE reverse_iterator
      rend(void);
      /*!
       * return an const reverse iterator before the first element of the vector
       * (provided for stl compatibility)
       * \return const_reverse_iterator, a const reverse iterator before the first element
       */
      TFEL_MATH_INLINE const_reverse_iterator
      rend(void) const;
      /*!
       * size of the tiny vector
       * (compatibility with vector)
       */
      TFEL_MATH_INLINE unsigned short
      size(void) const;
    protected:
      // values holded by the tvector.
      T v[N];
    private:
      //! a simple assertion stating that the dimension is valid.
      TFEL_STATIC_ASSERT(N!=0);
    };

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/General/fsarray.ixx"

#endif /* _LIB_TFEL_MATH_FSARRAY_H */

