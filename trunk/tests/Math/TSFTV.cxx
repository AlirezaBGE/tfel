/*! 
 * \file  tsftv.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 08 févr. 2013
 */

#include<cmath>
#include<cassert>
#include<cstdlib>
#include<limits>

#include"TFEL/Tests/Test.hxx"
#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/Vector/TSFTV.hxx"

struct TSFTVTest
  : public tfel::tests::TestCase
{
  
  TSFTVTest()
    : tfel::tests::TestCase("TFEL/Math",
			    "TSFTV")
  {} // end of TSFTVTest
  
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    using namespace tfel::math;
    const double eps = 100*std::numeric_limits<double>::epsilon();
    
    double values[10] = {1.1,10.1,
			 2.9,9.2,3.8,8.3,
			 4.7,7.4,5.6,6.5};
    tvector<10,double> v(values);
    TSFTV<2,10,2,2,double>::type tsftv(v);
    const stensor<2,double>& s1 = tsftv(0);
    stensor<2,double>& s2 = tsftv(1);
    stensor<2,double>::size_type i;

    for(i=0;i!=4u;++i){
      unsigned short idx;
      idx = static_cast<unsigned short>(i+2u);
      TFEL_TESTS_ASSERT(abs(s1(i)-v(idx))<eps);
      idx = static_cast<unsigned short>(i+6u);
      TFEL_TESTS_ASSERT(abs(s2(i)-v(idx))<eps);
    }
    s2 += stensor<2,double>::Id();
    for(i=0;i!=3u;++i){
      unsigned short idx;
      idx = static_cast<unsigned short>(i+6u);
      TFEL_TESTS_ASSERT(abs(s2(i)-values[idx]-1)<eps);
      TFEL_TESTS_ASSERT(abs(v(idx)-values[idx]-1)<eps);
    }
    TFEL_TESTS_ASSERT(abs(s2(3)-values[9u])<eps);

    return this->result;

  } // end of execute

}; // end of TSFTVTest

TFEL_TESTS_GENERATE_PROXY(TSFTVTest,"TSFTVTest");


int main(void)
{
  using namespace std;
  using namespace std;
  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("TSFTV.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
} // end of main

