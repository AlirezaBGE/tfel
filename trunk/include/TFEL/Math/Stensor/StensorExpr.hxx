#ifndef _MATH_STENSOR_EXPR_LIB_
#define _MATH_STENSOR_EXPR_LIB_ 1

#include <string>
#include <ostream>

#include"TFEL/Config/TFELConfig.hxx"

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Metaprogramming/IF.hxx"
#include"TFEL/Metaprogramming/EmptyClass.hxx"
#include"TFEL/Metaprogramming/HasIterator.hxx"
#include"TFEL/Metaprogramming/HasConstIterator.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/BasicOperations.hxx"
#include"TFEL/Math/Stensor/StensorConcept.hxx"

namespace tfel{

  namespace math{

    template<typename T_type, typename Expr>
    class StensorExpr;

    template<typename T_type, typename Expr>
    struct StensorTraits<StensorExpr<T_type,Expr> >{
      typedef typename StensorTraits<T_type>::NumType   NumType;
      static const unsigned short dime = StensorTraits<T_type>::dime;
    };

    template<typename T_type, typename Expr>
    struct StensorExpr
      : public StensorConcept<StensorExpr<T_type,Expr> >,
	public Expr
    {

      typedef typename Expr::value_type      value_type;      
      typedef typename Expr::pointer	     pointer;	    
      typedef typename Expr::const_pointer   const_pointer; 
      typedef typename Expr::reference	     reference;	    
      typedef typename Expr::const_reference const_reference;
      typedef typename Expr::size_type 	     size_type;	    
      typedef typename Expr::difference_type difference_type;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("StensorExpr<")+Name<T_type>::getName()
	  +string(",")+Name<Expr>::getName()+string(">");
      }

      explicit TFEL_MATH_INLINE StensorExpr()
	: Expr()
      {}

      explicit TFEL_MATH_INLINE StensorExpr(const typename Expr::first_arg& a_)
	: Expr(a_)
      {}

      explicit TFEL_MATH_INLINE StensorExpr(const typename Expr::first_arg& a_, 
					    const typename Expr::second_arg& b_)
	: Expr(a_,b_)
      {}

      using Expr::operator();

    };

    template<typename T_type, typename Expr>
    struct StensorType<StensorExpr<T_type,Expr> >
    {
      typedef T_type type;
    };

  } // end of namespace math
  
  namespace typetraits{

    template<typename T_type, typename Expr>
    struct IsTemporary<tfel::math::StensorExpr<T_type,Expr> >
    {
      static const bool cond = true;
    };

  }// end of namespace typetraits

} // end of TFEL namespace

#endif /* _MATH_STENSOR_EXPR_LIB_ */
