/*! 
 * \file  system.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 31 aoû 2010
 */

#include<boost/python.hpp>

void declareTestResult(void);

BOOST_PYTHON_MODULE(tests)
{
  declareTestResult();
}
