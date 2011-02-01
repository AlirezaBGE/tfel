/*!
 * \file   parser4.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   20 déc 2008
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cmath>
#include<cassert>
#include<cstdlib>

#include"TFEL/Math/Evaluator.hxx"

#include<iostream>
#include<algorithm>
#include<iterator>

int
main(void)
{
  using namespace std;
  using namespace tfel::utilities;
  using namespace tfel::math;
  using namespace tfel::math::parser;
  vector<string> var(1,"x");
  vector<string> nvar(1,"b");
  set<string> n;
  set<string> n2;
  shared_ptr<ExternalFunctionManager> manager(new ExternalFunctionManager());
  manager->operator[]("a") = shared_ptr<ExternalFunction>(new Evaluator("12",manager));
  shared_ptr<Evaluator> f(new Evaluator(var,"2*exp(a*x)",manager));
  shared_ptr<Evaluator> g(new Evaluator(var,"2*b*exp(a*x)",manager));
  shared_ptr<ExternalFunction> h = g->createFunctionByChangingParametersIntoVariables(nvar);
  cout << "h : " << h->getNumberOfVariables() << endl;
  h->setVariableValue(0,1.5);
  h->setVariableValue(1,2.54);
  cout << "h : " << h->getValue() << endl;
  f->getParametersNames(n);
  g->getParametersNames(n2);
  copy(n.begin(),n.end(),
       ostream_iterator<string>(cout," "));
  cout << endl;
  copy(n2.begin(),n2.end(),
       ostream_iterator<string>(cout," "));
  cout << endl;
  return EXIT_SUCCESS;
}
