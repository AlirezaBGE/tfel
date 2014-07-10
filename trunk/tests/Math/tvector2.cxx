#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>

#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"

int main(void){
  
  using namespace std;
  using namespace tfel::math;
  tvector<2>   v;
  tvector<2>   v2;
  tmatrix<2,2> m(0.);

  v(0) = 1.2;
  v(1) = 2.;
  m(0,0) = 1.;
  m(0,1) = 0.5;
  m(1,0) = 0.5;
  m(1,1) = 1.;

  v2 = m*v;

  return EXIT_SUCCESS;

}
