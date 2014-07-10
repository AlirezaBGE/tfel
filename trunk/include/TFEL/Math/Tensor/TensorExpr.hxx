#ifndef _MATH_TENSOR_EXPR_LIB_
#define _MATH_TENSOR_EXPR_LIB_ 1

#include <string>
#include <ostream>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/EmptyClass.hxx"
#include"TFEL/Metaprogramming/HasIterator.hxx"
#include"TFEL/Metaprogramming/HasConstIterator.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Tensor/TensorConcept.hxx"

namespace tfel{

  namespace math{

    template<typename T_type, typename Expr>
    struct TensorExpr;

    template<typename T_type, typename Expr>
    struct TensorTraits<TensorExpr<T_type,Expr> >{
      typedef typename TensorTraits<T_type>::NumType   NumType;
      typedef typename TensorTraits<T_type>::IndexType IndexType;
      static const unsigned short dime = TensorTraits<T_type>::dime;
    };

    template<typename T_type, typename Expr>
    struct TensorExpr
      : public TensorConcept<TensorExpr<T_type,Expr> >,
	public Expr
    {

      typedef typename Expr::RunTimeProperties RunTimeProperties;
      typedef typename Expr::value_type      value_type;      
      typedef typename Expr::pointer	     pointer;	    
      typedef typename Expr::const_pointer   const_pointer; 
      typedef typename Expr::reference	     reference;	    
      typedef typename Expr::const_reference const_reference;
      typedef typename Expr::size_type 	     size_type;	    
      typedef typename Expr::difference_type difference_type;

      explicit TFEL_MATH_INLINE TensorExpr()
	: Expr()
      {}

      explicit TFEL_MATH_INLINE TensorExpr(typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE TensorExpr(typename Expr::first_arg& a_, 
					   typename Expr::second_arg& b_)
	: Expr(a_,b_)
      {}

      using Expr::operator();

      using Expr::operator=;

    };

    template<typename T_type, typename Expr>
    struct TensorType<TensorExpr<T_type,Expr> >
    {
      typedef T_type type;
    };

  } // end of namespace math
  
  namespace typetraits{

    template<typename T_type, typename Expr>
    struct IsTemporary<tfel::math::TensorExpr<T_type,Expr> >
    {
      static const bool cond = true;
    };

  }// end of namespace typetraits

} // end of TFEL namespace

#endif /* _MATH_TENSOR_EXPR_LIB_ */
