/*! 
 * \file  matrix.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 avr 2009
 */

#ifndef _LIB_TFEL_MATH_MATRIX_H_
#define _LIB_TFEL_MATH_MATRIX_H_ 

#include"TFEL/Math/vector.hxx"
#include"TFEL/Math/Matrix/MatrixConcept.hxx"
#include"TFEL/Math/Forward/matrix.hxx"

#ifdef NO_RUNTIME_CHECK_BOUNDS
#include"TFEL/Math/General/EmptyRunTimeProperties.hxx"
#endif /* NO_RUNTIME_CHECK_BOUNDS */

namespace tfel
{

  namespace math
  {


    template<typename T>
    struct MatrixProperties;

    //! Partial specialisation for matrixs.
    /*
     * This is a MatrixConcept requirement.
     * \see MatrixTraits.
     */
    template<typename T>
    struct TFEL_VISIBILITY_LOCAL MatrixTraits<matrix<T> >{
      //! the type holded by the tmatrix.
      typedef typename tfel::math::vector<T>::value_type NumType;
      //! the type of the index used by the tmatrix.
      typedef typename tfel::math::vector<T>::size_type IndexType;
      /*!
       * RunTime Properties
       */
#ifdef NO_RUNTIME_CHECK_BOUNDS
      typedef EmptyRunTimeProperties RunTimeProperties;
#else /* NO_RUNTIME_CHECK_BOUNDS */
      typedef MatrixProperties<T> RunTimeProperties;
#endif /* NO_RUNTIME_CHECK_BOUNDS */
    };

    template<typename T>
    struct TFEL_VISIBILITY_LOCAL MatrixProperties
    {
      //! a simple alias
      typedef typename MatrixTraits<matrix<T> >::IndexType IndexType;

      MatrixProperties(const IndexType,
		       const IndexType);

      MatrixProperties(const MatrixProperties&);

      MatrixProperties&
      operator=(const MatrixProperties&);

      bool
      operator == (const MatrixProperties&) const;

      bool
      operator != (const MatrixProperties&) const;

    protected:

      typename MatrixTraits<matrix<T> >::IndexType nb_rows;

      typename MatrixTraits<matrix<T> >::IndexType nb_cols;

    }; // end of MatrixProperties

    template<typename T>
    struct TFEL_VISIBILITY_LOCAL matrix
      : protected tfel::math::vector<T>,
	protected MatrixProperties<T>
    {
      //! a simple alias
#ifdef NO_RUNTIME_CHECK_BOUNDS
      typedef EmptyRunTimeProperties RunTimeProperties;
#else /* NO_RUNTIME_CHECK_BOUNDS */
      typedef MatrixProperties<T> RunTimeProperties;
#endif /* NO_RUNTIME_CHECK_BOUNDS */
      /*!
       * type of the matrix's values.
       * (this i<s a stl requirement).
       */
      using typename std::vector<T>::value_type;
      /*!
       * type of a pointer to the value contained.
       * (this is a stl requirement).
       */
      using typename std::vector<T>::pointer;
      /*!
       * type of a const pointer to the value contained.
       * (this is a stl requirement).
       */
      using typename std::vector<T>::const_pointer;
      /*!
       * type of the matrix's iterator.
       * (provided for stl compatibility).
       */
      using typename std::vector<T>::iterator;
      /*!
       * type of the matrix's const iterator.
       * (provided for stl compatibility).
       */
      using typename std::vector<T>::const_iterator;
      /*!
       * type of the matrix's reverse iterator.
       * (provided for stl compatibility).
       */
      using typename std::vector<T>::const_reverse_iterator;
      /*!
       * type of the matrix's const reverse iterator.
       * (provided for stl compatibility).
       */
      using typename std::vector<T>::reverse_iterator;
      /*!
       * type of a reference to the value contained.
       * (this is a stl requirement).
       */
      using typename std::vector<T>::reference;
      /*!
       * type of a const reference to the value contained.
       * (this is a stl requirement).
       */
      using typename std::vector<T>::const_reference;
      /*!
       * type of the size of the container.
       * (this is a stl requirement).
       */
      typedef typename std::vector<T>::size_type size_type;
      /*!
       * type of the difference between two iterators.
       * (this is a stl requirement).
       */
      using typename std::vector<T>::difference_type;

      TFEL_MATH_INLINE2
      matrix(void);

      TFEL_MATH_INLINE2
      matrix(const size_type,
	     const size_type);

      TFEL_MATH_INLINE2
      matrix(const size_type,
	     const size_type,
	     const T&);

      TFEL_MATH_INLINE2
      matrix(const matrix&);

      /*!
       * resize the matrix
       * \param[in] n : number of rows
       * \param[in] m : number of columns
       * \param[in] v : values of the newly inserted elements
       */
      void
      resize(const size_type,
	     const size_type,
	     const T& = T());

      /*!
       * clear the matrix
       */
      void clear();

      TFEL_MATH_INLINE T&
      operator()(const size_type,
		 const size_type);

      TFEL_MATH_INLINE const T&
      operator()(size_type,
		 size_type) const;

#ifdef NO_RUNTIME_CHECK_BOUNDS
      TFEL_MATH_INLINE const RunTimeProperties
#else
      TFEL_MATH_INLINE const RunTimeProperties&
#endif
      getRunTimeProperties(void) const;

      /*
       * return an iterator to the first element of the matrix
       * (provided for stl compatibility)
       * \return iterator, an iterator to the first element
       */
      using std::vector<T>::begin;
      
      /*
       * return an iterator after the last element of the matrix
       * (provided for stl compatibility)
       * \return iterator, an iterator after the last element
       */
      using std::vector<T>::end;

      /*
       * return an reverse iterator to the last element of the matrix
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator to the last element
       */
      using std::vector<T>::rbegin;
      
      /*
       * return an  reverse iterator before the first element of the matrix
       * (provided for stl compatibility)
       * \return reverse_iterator, a reverse iterator before the first element
       */
      using std::vector<T>::rend;

      /*
       * Assignement operator.
       * \param  const matrix&, the matrix to be copied.
       * \return matrix&, a reference to itself.
       */
      matrix& 
      operator=(const matrix&);

      /*
       * Assignement operator.
       * \param  const matrix&, the matrix to be copied.
       * \return matrix&, a reference to itself.
       */
      matrix& 
      operator+=(const matrix&);

      /*
       * Assignement operator.
       * \param  const matrix&, the matrix to be copied.
       * \return matrix&, a reference to itself.
       */
      matrix& 
      operator-=(const matrix&);

      /*
       * Assignement operator
       * \param const MatrixExpr<matrix<T2>,Expr>&, a matrix
       * expression based on matrix
       * \return matrix&, a reference to itself.
       */
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE2
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	matrix<T>&
      >::type
      operator=(const MatrixExpr<matrix<T2>,Expr>&);

      /*
       * Assignement operator
       * \param const MatrixExpr<matrix<T2>,Expr>&, a matrix
       * expression based on matrix
       * \return matrix&, a reference to itself.
       */
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE2
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	matrix<T>&
      >::type
      operator+=(const MatrixExpr<matrix<T2>,Expr>&);

      /*
       * Assignement operator
       * \param const MatrixExpr<matrix<T2>,Expr>&, a matrix
       * expression based on matrix
       * \return matrix&, a reference to itself.
       */
      template<typename T2,typename Expr>
      TFEL_MATH_INLINE2
      typename tfel::meta::EnableIf<
	tfel::typetraits::IsAssignableTo<T2,T>::cond,
	matrix<T>&
      >::type
      operator-=(const MatrixExpr<matrix<T2>,Expr>&);

      void swap(matrix&);

      TFEL_MATH_INLINE size_type
      getNbRows() const;

      TFEL_MATH_INLINE size_type
      getNbCols() const;

    };

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Matrix/matrix.ixx"

#endif /* _LIB_TFEL_MATH_MATRIX_H */

