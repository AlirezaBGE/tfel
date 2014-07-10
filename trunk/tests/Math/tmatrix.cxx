/*!
 * \file   tmatrix.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   03 jui 2006
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>

#include"TFEL/Math/tmatrix.hxx"

int main(void){
  
  using namespace std;
  using namespace tfel::math;

  tmatrix<2,2,double> m;
  tmatrix<2,2,double> m2;

  m(0,0)=0.;
  m(0,1)=1.;
  m(1,0)=2.;
  m(1,1)=3.;
  
  m2 = 2*m;

  return EXIT_SUCCESS;

}


