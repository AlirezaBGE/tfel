#ifdef NDEBUG
#undef NDEBUG
#endif

#include<iostream>
#include<cstdlib>
#include<cmath>
#include<cassert>

#include<string>
#include<vector>

#include"TFEL/Metaprogramming/StaticAssert.hxx"
#include"TFEL/Metaprogramming/EnableIf.hxx"
#include"TFEL/Metaprogramming/TypeList.hxx"
#include"TFEL/Math/General/LogicalOperations.hxx"

template<typename T, template<class> class U>
class generic_container_;

template<template<class> class U>
class generic_container_< tfel::meta::TLE,U>{};

template<typename T, template<class> class U>
class generic_container_
  : public U<typename T::Current>,
    public generic_container_<typename T::Next, U>
{
};

template<typename T>
struct holder
{
  T value;
  
  void set(const T& src)
  {
    value=src;
  }

};

template<typename T>
class generic_container
  : public generic_container_<T,holder>
{

private:
  
  TFEL_STATIC_ASSERT((tfel::meta::TLElementsAreUnique<T>::cond));

public:
  
  template<unsigned int N>
  typename tfel::meta::EnableIf< 
    (N < tfel::meta::TLSize<T>::value),  
      typename tfel::meta::TLFindNthElt<T,N>::type >::type
  get(void) const {
    return static_cast<const holder<typename tfel::meta::TLFindNthElt<T,N>::type> *>(this)->value;
  }
  
  template<unsigned int N>
  typename tfel::meta::EnableIf< 
    (N < tfel::meta::TLSize<T>::value), 
      void >::type
  set(const typename tfel::meta::TLFindNthElt<T,N>::type& src){
    static_cast<holder<typename tfel::meta::TLFindNthElt<T,N>::type> *>(this)->set(src);
  }

};

template<class T>
struct Is_same{

  template<class T2>
  struct test{
    static const bool cond = tfel::meta::IsSameType<T,T2>::cond;
  };

};

template<typename T, class Test>
class Mytest{

  typedef typename Test::template test<T> my_test_class;

public:

  static const bool cond = my_test_class::cond;

};

int main(void){

  using namespace tfel::meta;

  typedef GenerateTypeList<std::string,double,int>::type my_list;
  assert(TLElementsAreUnique<my_list>::cond);

  generic_container<TLNode<double,TLNode<std::string, TLE> > > test;
  
  test.set<0>(13.254);
  test.set<1>("Coucou");
  assert(std::abs(test.get<0>()-13.254)<1.e-14);
  assert(test.get<1>()=="Coucou");

  assert(Is_same<double>::test<double>::cond);
  assert(!Is_same<double>::test<int>::cond);
  assert((Mytest<double, Is_same<double> >::cond));

  return EXIT_SUCCESS;
  
}
