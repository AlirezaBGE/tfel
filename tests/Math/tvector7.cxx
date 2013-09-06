/*! 
 * \file  tvector7.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 janv. 2013
 */

#include<cstdlib>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/FSAlgorithm/FSAlgorithm.hxx"
#include"TFEL/Math/tvector.hxx"

struct TVector7
  : public tfel::tests::TestCase
{
  TVector7()
    : tfel::tests::TestCase("TFEL/Math",
			    "TVector7")
  {} // end of TVector7

  tfel::tests::TestResult
  execute()
  {
    using namespace tfel::fsalgo;
    using namespace tfel::math;
    int vx[3u] = {1,2,3};
    tvector<3u,int> x(vx);
    tvector<2u,int> y;
    tvector<2u,int> z;
    y = x.slice<1>();
    z = x.slice<0,2>();
    TFEL_TESTS_ASSERT(y(0)==2);
    TFEL_TESTS_ASSERT(y(1)==3);
    TFEL_TESTS_ASSERT(z(0)==1);
    TFEL_TESTS_ASSERT(z(1)==2);
    slice<1>(x)[0]   = 4;
    slice<0,2>(x)[0] = 5;
    y = slice<1>(x);
    z = slice<0,2>(x);
    TFEL_TESTS_ASSERT(y(0)==4);
    TFEL_TESTS_ASSERT(y(1)==3);
    TFEL_TESTS_ASSERT(z(0)==5);
    TFEL_TESTS_ASSERT(z(1)==4);
    return this->result;
  } // end of execute
};

TFEL_TESTS_GENERATE_PROXY(TVector7,
			  "TVector7");

int main(void)
{
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("tvector7.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main
