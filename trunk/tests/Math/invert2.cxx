/*! 
 * \file  invert2.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 05 févr. 2013
 */

#include<cmath>
#include<cassert>
#include<cstdlib>
#include<limits>

#include"TFEL/Tests/Test.hxx"
#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Math/tvector.hxx"
#include"TFEL/Math/tmatrix.hxx"
#include"TFEL/Math/TinyMatrixInvert.hxx"

template<typename T>
struct TinyMatrixInvertTest2D
  : public tfel::tests::TestCase
{
  
  TinyMatrixInvertTest2D()
    : tfel::tests::TestCase("TFEL/Math",
			    "TinyMatrixInvertTest2D<"+tfel::utilities::Name<T>::getName()+">")
  {} // end of TinyMatrixInvertTest2D
  
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::math;
    const T eps = 100*std::numeric_limits<T>::epsilon();
    tmatrix<2,2,T> m;
    m(0,0)=1.;
    m(0,1)=2.;
    m(1,0)=3.;
    m(1,1)=4.;
    TinyMatrixInvert<2,T>::exe(m);
    TFEL_TESTS_ASSERT(abs(m(0,0)+2)<eps);
    TFEL_TESTS_ASSERT(abs(m(0,1)-1)<eps);
    TFEL_TESTS_ASSERT(abs(m(1,0)-1.5)<eps);
    TFEL_TESTS_ASSERT(abs(m(1,1)+0.5)<eps);
    return this->result;
  } // end of execute

}; // end of TinyMatrixInvertTest2D

template<typename T>
struct TinyMatrixInvertTest3D
  : public tfel::tests::TestCase
{
  
  TinyMatrixInvertTest3D()
    : tfel::tests::TestCase("TFEL/Math",
			    "TinyMatrixInvertTest3D<"+tfel::utilities::Name<T>::getName()+">")
  {} // end of TinyMatrixInvertTest3D
  
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::math;
    const T eps = 100*std::numeric_limits<T>::epsilon();
    tmatrix<3,3,T> m;
    m(0,0)=0.;
    m(0,1)=2.;
    m(0,2)=1.;
    m(1,0)=1.;
    m(1,1)=0.;
    m(1,2)=0.;
    m(2,0)=3.;
    m(2,1)=0.;
    m(2,2)=1.;
    TinyMatrixInvert<3,T>::exe(m);
    TFEL_TESTS_ASSERT(abs(m(0,0))<eps);
    TFEL_TESTS_ASSERT(abs(m(0,1)-1)<eps);
    TFEL_TESTS_ASSERT(abs(m(0,2)-0)<eps);

    TFEL_TESTS_ASSERT(abs(m(1,0)-0.5)<eps);
    TFEL_TESTS_ASSERT(abs(m(1,1)-1.5)<eps);
    TFEL_TESTS_ASSERT(abs(m(1,2)+0.5)<eps);

    TFEL_TESTS_ASSERT(abs(m(2,0))<eps);
    TFEL_TESTS_ASSERT(abs(m(2,1)+3)<eps);
    TFEL_TESTS_ASSERT(abs(m(2,2)-1)<eps);

    return this->result;

  } // end of execute

}; // end of TinyMatrixInvertTest3D

typedef TinyMatrixInvertTest2D<float>       FloatTinyMatrixInvertTest2D;
typedef TinyMatrixInvertTest2D<double>      DoubleTinyMatrixInvertTest2D;
typedef TinyMatrixInvertTest2D<long double> LongDoubleTinyMatrixInvertTest2D;
typedef TinyMatrixInvertTest3D<float>       FloatTinyMatrixInvertTest3D;
typedef TinyMatrixInvertTest3D<double>      DoubleTinyMatrixInvertTest3D;
typedef TinyMatrixInvertTest3D<long double> LongDoubleTinyMatrixInvertTest3D;

TFEL_TESTS_GENERATE_PROXY(FloatTinyMatrixInvertTest2D,
			  "TinyMatrixInvertTest2D<float>");
TFEL_TESTS_GENERATE_PROXY(DoubleTinyMatrixInvertTest2D,
			  "TinyMatrixInvertTest2D<double>");
TFEL_TESTS_GENERATE_PROXY(LongDoubleTinyMatrixInvertTest2D,
			  "TinyMatrixInvertTest2D<long double>");
TFEL_TESTS_GENERATE_PROXY(FloatTinyMatrixInvertTest3D,
			  "TinyMatrixInvertTest3D<float>");
TFEL_TESTS_GENERATE_PROXY(DoubleTinyMatrixInvertTest3D,
			  "TinyMatrixInvertTest3D<double>");
TFEL_TESTS_GENERATE_PROXY(LongDoubleTinyMatrixInvertTest3D,
			  "TinyMatrixInvertTest3D<long double>");


int main(void)
{
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("Invert.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main

