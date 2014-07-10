/*!
 * \file   equal.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   28 Aug 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif

#include<cstdlib>
#include<cassert>
#include<limits>
#include<cmath>

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"

struct AlmostEqual
{
  template<typename T>
  bool operator()(const T a,const T b)
  {
    return (std::abs(a-b)<10*std::abs(a)*std::numeric_limits<T>::epsilon());
  }
};

int main()
{
  using namespace tfel::fsalgo;

  int A1[] = { 3, 1, 4, 1, 5, 9, 3 };
  int A2[] = { 3, 1, 4, 2, 8, 5, 7 };
  const int N = sizeof(A1) / sizeof(int);
  double B1[] = { 3., 1., 4., 1., 5., 9., 3 };
  double B2[] = { 3., 1., 4., 2., 8., 5., 7 };
  const int N2 = sizeof(B1) / sizeof(double);
  AlmostEqual a;
  
  assert(!equal<N>::exe(A1,A2));
  assert(equal<3>::exe(A1,A2));
  assert(!equal<N2>::exe(B1,B2,a));
  assert(equal<3>::exe(B1,B2,a));

  return EXIT_SUCCESS;
}
