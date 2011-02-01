/*!
 * \file   vector.ixx
 * \brief  
 * \author Helfer Thomas
 * \date   05 oct 2006
 */

#ifndef _LIB_TFEL_VECTOR_IXX_
#define _LIB_TFEL_VECTOR_IXX_ 

#include"cassert"
#include"TFEL/Math/Function/Power.hxx"


namespace tfel{

  namespace math{

    template<typename ValueType>
    vector<ValueType>::vector()
      : std::vector<ValueType>()
    {}

    template<typename ValueType>
    vector<ValueType>::vector(const vector<ValueType>& src)
      : std::vector<ValueType>(src)
    {}

    template<typename ValueType>
    vector<ValueType>::vector(const typename vector<ValueType>::size_type s)
      : std::vector<ValueType>(s)
    {}

    template<typename ValueType>
    vector<ValueType>::vector(const typename vector<ValueType>::size_type s, const ValueType& v)
      : std::vector<ValueType>(s,v)
    {}

    template<typename ValueType>
    template<typename InputIterator>
    vector<ValueType>::vector(const InputIterator begin_, const InputIterator end_)
      : std::vector<ValueType>(begin_,end_)
    {}

    template<typename ValueType>
    vector<ValueType>&
    vector<ValueType>::operator=(const vector<ValueType>& src)
    {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->size(),src.size());
#endif
      std::vector<ValueType>::operator=(src);
      return *this;
    }

    template<typename ValueType>
    vector<ValueType>&
    vector<ValueType>::operator+=(const vector<ValueType>& src)
    {
      size_type i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->size(),src.size());
#endif
      for(i=0;i<this->size();++i){
	std::vector<ValueType>::operator[](i) += src(i);
      }
      return *this;
    }

    template<typename ValueType>
    vector<ValueType>&
    vector<ValueType>::operator-=(const vector<ValueType>& src)
    {
      size_type i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->size(),src.size());
#endif
      for(i=0;i<this->size();++i){
	std::vector<ValueType>::operator[](i) -= src(i);
      }
      return *this;
    }

    template<typename ValueType>
    TFEL_MATH_INLINE2 const typename vector<ValueType>::RunTimeProperties 
    vector<ValueType>::getRunTimeProperties(void) const
    {
      return std::vector<ValueType>::size();
    }

    template<typename ValueType>
    TFEL_MATH_INLINE ValueType& 
    vector<ValueType>::operator()(const typename vector<ValueType>::size_type i)
    {
#ifndef NO_RUNTIME_CHECK_BOUNDS
	assert(i<this->size());
#endif
      return std::vector<ValueType>::operator[](i);
    }

    template<typename ValueType>
    TFEL_MATH_INLINE const ValueType& 
    vector<ValueType>::operator()(const typename vector<ValueType>::size_type i) const
    {
#ifndef NO_RUNTIME_CHECK_BOUNDS
	assert(i<this->size());
#endif
      return std::vector<ValueType>::operator[](i);
    }

    template<typename ValueType>
    template<typename ValueType2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<ValueType2,ValueType>::cond,
      vector<ValueType>&
    >::type
    vector<ValueType>::operator=(const VectorExpr<vector<ValueType2>,Expr>& expr)
    {
      size_type i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),expr.getRunTimeProperties());
#endif
      for(i=0;i<this->size();++i){
	std::vector<ValueType>::operator[](i) = expr(i);
      }
      return *this;
    }

    template<typename ValueType>
    template<typename ValueType2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<ValueType2,ValueType>::cond,
      vector<ValueType>&
    >::type
    vector<ValueType>::operator+=(const VectorExpr<vector<ValueType2>,Expr>& expr)
    {
      size_type i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),expr.getRunTimeProperties());
#endif
      for(i=0;i<this->size();++i){
	std::vector<ValueType>::operator[](i) += expr(i);
      }
      return *this;
    }

    template<typename ValueType>
    template<typename ValueType2,typename Expr>
    TFEL_MATH_INLINE2 typename tfel::meta::EnableIf<
      tfel::typetraits::IsAssignableTo<ValueType2,ValueType>::cond,
      vector<ValueType>&
    >::type
    vector<ValueType>::operator-=(const VectorExpr<vector<ValueType2>,Expr>& expr)
    {
      size_type i;
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->getRunTimeProperties(),expr.getRunTimeProperties());
#endif
      for(i=0;i<this->size();++i){
	std::vector<ValueType>::operator[](i) -= expr(i);
      }
      return *this;
    }

    template<typename ValueType>
    template<typename InputIterator>
    TFEL_MATH_INLINE2 void vector<ValueType>::copy(const InputIterator b,const InputIterator e)
    {
      std::copy(b,e,this->v);
    }

    template<typename ValueType>
    void vector<ValueType>::swap(vector<ValueType>& a)
    {
#ifndef NO_RUNTIME_CHECK_BOUNDS
      RunTimeCheck<RunTimeProperties>::exe(this->size(),a.size());
#endif
      std::vector<ValueType>::swap(a);
    }

    template<typename T>
    TFEL_MATH_INLINE2
    typename tfel::meta::EnableIf<
      tfel::typetraits::IsScalar<T>::cond,
      typename tfel::typetraits::RealPartType<T>::type
    >::type
    norm(const vector<T>& vec)
    {
      using tfel::math::stdfunctions::power;
      T n(0);
      typename vector<T>::size_type i;
      for(i=0;i!=vec.size();++i){
	n += power<2>(vec(i));
      }
      return sqrt(real(n));
    }

  } // end of namespace math

} // end of namespace tfel

#include"TFEL/Math/Vector/vectorResultType.hxx"

#endif /* _LIB_TFEL_VECTOR_IXX */

