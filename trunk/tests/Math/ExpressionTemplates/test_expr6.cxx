/*!
 * \file   test.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Oct 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cassert>
#include<cstdlib>

#include"TFEL/Utilities/Name.hxx"

#include"TFEL/Math/tvector.hxx"

int main(void)
{
  using namespace std;
  using namespace tfel::math;
  using namespace tfel::utilities;

  tvector<3,tvector<2,double> > v1(tvector<2,double>(1.4));
  tvector<3,tvector<2,double> > v2(tvector<2,double>(3.));
  tvector<3,tvector<2,double> > v3(tvector<2,double>(1.2));

  assert(abs(v1(0)(0)-1.4)<1.e-14);
  assert(abs(v1(0)(1)-1.4)<1.e-14);
  assert(abs(v1(1)(0)-1.4)<1.e-14);
  assert(abs(v1(1)(1)-1.4)<1.e-14);
  assert(abs(v1(2)(0)-1.4)<1.e-14);
  assert(abs(v1(2)(1)-1.4)<1.e-14);

  assert(abs(v2(0)(0)-3.)<1.e-14);
  assert(abs(v2(0)(1)-3.)<1.e-14);
  assert(abs(v2(1)(0)-3.)<1.e-14);
  assert(abs(v2(1)(1)-3.)<1.e-14);
  assert(abs(v2(2)(0)-3.)<1.e-14);
  assert(abs(v2(2)(1)-3.)<1.e-14);

  assert(abs(v3(0)(0)-1.2)<1.e-14);
  assert(abs(v3(0)(1)-1.2)<1.e-14);
  assert(abs(v3(1)(0)-1.2)<1.e-14);
  assert(abs(v3(1)(1)-1.2)<1.e-14);
  assert(abs(v3(2)(0)-1.2)<1.e-14);
  assert(abs(v3(2)(1)-1.2)<1.e-14);

  assert(abs((v1+v2)(0)(0)-4.4)<1.e-14);
  assert(abs((v1+v2)(0)(1)-4.4)<1.e-14);
  assert(abs((v1+v2)(1)(0)-4.4)<1.e-14);
  assert(abs((v1+v2)(1)(1)-4.4)<1.e-14);
  assert(abs((v1+v2)(2)(0)-4.4)<1.e-14);
  assert(abs((v1+v2)(2)(1)-4.4)<1.e-14);

  assert(abs((v1+v2+v3)(0)(0)-5.6)<1.e-14);
  assert(abs((v1+v2+v3)(0)(1)-5.6)<1.e-14);
  assert(abs((v1+v2+v3)(1)(0)-5.6)<1.e-14);
  assert(abs((v1+v2+v3)(1)(1)-5.6)<1.e-14);
  assert(abs((v1+v2+v3)(2)(0)-5.6)<1.e-14);
  assert(abs((v1+v2+v3)(2)(1)-5.6)<1.e-14);

  return EXIT_SUCCESS;
}
