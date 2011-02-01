/*!
 * \file   ScalarStensorExprWithoutConstIteratorSpecialisation.ixx
 * \brief  This file partially specialises the StensorStensorExprWithoutConstIterator classe for usual operations.
 * \see    NO_EXPRWITHOUTCONSTITERATORESSION_TEMPLATE_SPECIALISATION 
 * \author Helfer Thomas
 * \date   11 Sep 2006
 */

#ifndef _LIB_TFEL_SCALARSTENSOREXPRWITHOUTCONSTITERATORSPECIALISATION_I_
#define _LIB_TFEL_SCALARSTENSOREXPRWITHOUTCONSTITERATORSPECIALISATION_I_ 

namespace tfel{

  namespace math {

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<typename A, typename B>
    class ScalarStensorExprWithoutConstIterator<A,B,OpMult>
    {
      
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<A>::cond));
      TFEL_STATIC_ASSERT((tfel::meta::Implements<B,StensorConcept>::cond));
      
      typedef typename StensorTraits<B>::NumType   NumTypeB;
      
      static const bool IsBTemporary = tfel::typetraits::IsTemporary<B>::cond;

      const A  a;
      typename tfel::meta::IF<IsBTemporary,const B,const B&>::type b;
      
    public:
      
      typedef typename ComputeBinaryResult<A,NumTypeB,OpMult>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("ScalarStensorExprWithoutConstIterator<")+Name<A>::getName()
	  +string(",")+Name<B>::getName()+string(",")+Name<OpMult>::getName()+string(">");
      }

    protected:

      typedef A first_arg;
      typedef B second_arg;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef unsigned short size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE ScalarStensorExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE 
      ScalarStensorExprWithoutConstIterator(const ScalarStensorExprWithoutConstIterator& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i) const 
      {
	return a*b(i);
      }

    };

    /*
     * \brief Partial Specialisation for OpMult.
     */
    template<typename A, typename B>
    class StensorScalarExprWithoutConstIterator<A,B,OpMult>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,StensorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename StensorTraits<A>::NumType   NumTypeA;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

    public:

      typedef typename ComputeBinaryResult<NumTypeA,B,OpMult>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("StensorScalarExprWithoutConstIterator<")+Name<A>::getName()
	  +string(",")+Name<B>::getName()+string(",")+Name<OpMult>::getName()+string(">");
      }

    protected:

      typedef A first_arg;
      typedef B second_arg;
      
      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef unsigned short size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE StensorScalarExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE 
      StensorScalarExprWithoutConstIterator(const StensorScalarExprWithoutConstIterator& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i) const 
      {
	return a(i)*b;
      }

    };

    /*
     * \brief Partial Specialisation for OpDiv.
     */
    template<typename A, typename B>
    class StensorScalarExprWithoutConstIterator<A,B,OpDiv>
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,StensorConcept>::cond));
      TFEL_STATIC_ASSERT((tfel::typetraits::IsScalar<B>::cond));
      
      typedef typename StensorTraits<A>::NumType   NumTypeA;
      
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const B b;

    public:

      typedef typename ComputeBinaryResult<NumTypeA,B,OpDiv>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("StensorScalarExprWithoutConstIterator<")+Name<A>::getName()+string(",")
	  +Name<B>::getName()+string(",")+Name<OpDiv>::getName()+string(">");
      }

    protected:

      typedef A first_arg;
      typedef B second_arg;
      
      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef unsigned short size_type;	    						
      typedef ptrdiff_t      difference_type;

      TFEL_MATH_INLINE StensorScalarExprWithoutConstIterator(const A& l, const B& r)
	: a(l), b(r)
      {}

      TFEL_MATH_INLINE 
      StensorScalarExprWithoutConstIterator(const StensorScalarExprWithoutConstIterator& src)
	: a(src.a), b(src.b)
      {}
      
      TFEL_MATH_INLINE const NumType 
      operator()(const unsigned short i) const 
      {
	return a(i)/b;
      }

    };

    
  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_SCALARSTENSOREXPRWITHOUTCONSTITERATORSPECIALISATION_I_ */

