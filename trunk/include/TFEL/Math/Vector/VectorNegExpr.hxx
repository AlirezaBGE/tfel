/*!
 * \file   VectorNegExpr.hxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   24 Aug 2006
 */

#ifndef _LIB_TFEL_VECTOR_NEG_EXPR_H_
#define _LIB_TFEL_VECTOR_NEG_EXPR_H_ 

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/Implements.hxx"
#include"TFEL/Metaprogramming/InvalidType.hxx"
#include"TFEL/TypeTraits/IsInvalid.hxx"
#include"TFEL/TypeTraits/IsTemporary.hxx"
#include"TFEL/Math/General/NegObjectRandomAccessConstIterator.hxx"
#include"TFEL/Math/Vector/VectorConcept.hxx"

namespace tfel{

  namespace math{

    template<typename A>
    class VectorNegExpr
    {
      
      TFEL_STATIC_ASSERT((tfel::meta::Implements<A,VectorConcept>::cond));
      static const bool IsATemporary = tfel::typetraits::IsTemporary<A>::cond;

      typename tfel::meta::IF<IsATemporary,const A,const A&>::type a;
      const typename A::RunTimeProperties RTP;

      struct invalid_argument;

      typedef typename VectorTraits<A>::NumType   NumTypeA;
      typedef typename VectorTraits<A>::IndexType IndexType;

    public:

      typedef typename ComputeUnaryResult<NumTypeA,OpNeg>::Handle NumType;

      static const std::string getName(void){
	using namespace std;
	using namespace tfel::utilities;
	return string("VectorNegExpr<")+Name<A>::getName()+string(">");
      }

    protected:

      typedef A first_arg;
      typedef invalid_argument second_arg;
      typedef typename A::RunTimeProperties RunTimeProperties;

      typedef NumType        value_type;                                                
      typedef NumType*       pointer;	    						
      typedef const NumType* const_pointer; 						
      typedef NumType&       reference;	    						
      typedef const NumType& const_reference;						
      typedef IndexType      size_type;	    						
      typedef ptrdiff_t      difference_type;                                          	

      TFEL_MATH_INLINE VectorNegExpr(const A& l)
	: a(l),RTP(l.getRunTimeProperties())
      {}

      TFEL_MATH_INLINE const NumType
      operator()(const IndexType i) const 
      {
	return -a(i);
      }

    public:

      typedef NegObjectRandomAccessConstIterator<A> const_iterator;
      typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

      TFEL_MATH_INLINE const RunTimeProperties 
      getRunTimeProperties(void) const 
      {                                                         
	return RTP;                                             
      }                                                         

      TFEL_MATH_INLINE const_iterator begin(void) const                           	
      {											
	return const_iterator(a.begin());        					
      }											
 											
      TFEL_MATH_INLINE const_iterator end(void) const	                                
      {											
	return const_iterator(a.end());   						
      }											
											
      TFEL_MATH_INLINE const_reverse_iterator rbegin(void) const		        
      {											
	return const_reverse_iterator(end());						
      }											
											
      TFEL_MATH_INLINE const_reverse_iterator rend(void) const                          
      {											
	return const_reverse_iterator(begin());						
      }                                                                                 

    };

  } // end of namespace math

} // end of namespace tfel

#endif /* _LIB_TFEL_VECTOR_NEG_EXPR_H */

