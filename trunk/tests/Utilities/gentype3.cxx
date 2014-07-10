/*! 
 * \file  gentype3.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 janv. 2011
 */

#include<iostream>
#include<cstdlib>
#include<sstream>
#include<string>

#include"TFEL/Utilities/Name.hxx"
#include"TFEL/Utilities/GenTypeBase.hxx"

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

typedef tfel::meta::GenerateTypeList<int,std::string>::type HoldedTypes;

struct TFEL_VISIBILITY_EXPORT GenType
  : tfel::utilities::GenTypeBase<HoldedTypes>
{
  using tfel::utilities::GenTypeBase<HoldedTypes>::operator =;

  std::string getType() const;
  
  std::string dumpToString(void) const;

  void dump(std::ostream&) const;

  void load(const std::string&,
	    const std::string&);
  
 private:

  struct GetType;
  struct DumpToString;
  struct Dump;
  struct Load;

}; // end of struct TFEL_VISIBILITY_EXPORT GenType

struct GenType::GetType
{
  typedef std::string return_type;
  template<typename T>
  static return_type
  apply(const T& v)
  {
    using namespace tfel::utilities;
    return name(v);
  }
};

struct GenType::DumpToString
{
  typedef std::string return_type;

  template<typename T>
  static return_type
  apply(const T& v)
  {
    using namespace std;
    ostringstream out;
    out << v;
    return out.str();
  }
};

struct GenType::Dump
{
  typedef void return_type;

  Dump(std::ostream& s)
    : stream(s)
  {}

  template<typename T>
  return_type
  operator()(const T& v)
  {
    using namespace std;
    this->stream << v;
  }

private:
  
  std::ostream& stream;

};

struct GenType::Load
{
  typedef void return_type;

  Load(const std::string& s)
    : value(s)
  {}

  template<typename T>
  return_type
  operator()(T& v)
  {
    using namespace std;
    istringstream is(this->value);
    is >> v;
  }

private:
  
  std::string value;

};

std::string
GenType::getType() const
{
  using namespace tfel::utilities;
  return apply<GetType>(*this);
}

std::string
GenType::dumpToString(void) const
{
  using namespace tfel::utilities;
  return apply<DumpToString>(*this);
}

void
GenType::dump(std::ostream& s) const
{
  using namespace tfel::utilities;
  Dump d(s);
  return apply(d,*this);
}

void
GenType::load(const std::string& t,
	      const std::string& v)
{
  using namespace std;
  using namespace tfel::utilities;
  if(t=="int"){
    this->set<int>(0);
  } else if(t=="std::string") {
    this->set<string>(string());
  } else {
    throw("load");
  }
  Load l(v);
  return apply(l,*this);
}

struct GenTypeTest3
  : public tfel::tests::TestCase
{
  GenTypeTest3()
     : tfel::tests::TestCase("TFEL/Utilities","GenTypeTest3")
  {} // end of MyTest
  tfel::tests::TestResult
  execute()
  {
    using namespace std;
    GenType g;
    g = 21;
    ostringstream os;
    TFEL_TESTS_ASSERT(g.is<int>());
    TFEL_TESTS_CHECK_EQUAL(g.get<int>(),21);
    TFEL_TESTS_CHECK_EQUAL(g.getType(),"int");
    TFEL_TESTS_CHECK_EQUAL(g.dumpToString(),"21");
    g.dump(os);
    TFEL_TESTS_CHECK_EQUAL(os.str(),"21");

    os.str("");
    g = string("toto");
    TFEL_TESTS_ASSERT(g.is<string>());
    TFEL_TESTS_CHECK_EQUAL(g.get<string>(),"toto");
    TFEL_TESTS_CHECK_EQUAL(g.getType(),"std::string");
    TFEL_TESTS_CHECK_EQUAL(g.dumpToString(),"toto");
    g.dump(os);
    TFEL_TESTS_CHECK_EQUAL(os.str(),"toto");
    
    os.str("");
    g.load("int","21");
    TFEL_TESTS_ASSERT(g.is<int>());
    TFEL_TESTS_CHECK_EQUAL(g.get<int>(),21);
    TFEL_TESTS_CHECK_EQUAL(g.getType(),"int");
    TFEL_TESTS_CHECK_EQUAL(g.dumpToString(),"21");
    g.dump(os);
    TFEL_TESTS_CHECK_EQUAL(os.str(),"21");

    return this->result;
  } // end of execute()
};

TFEL_TESTS_GENERATE_PROXY(GenTypeTest3,"GenType");

int main(void)
{
  using namespace std;
  using namespace std;

  using namespace tfel::tests;
  using namespace tfel::utilities;
  TestManager& manager = TestManager::getTestManager();
  manager.addTestOutput(cout);
  manager.addXMLTestOutput("gentype3.xml");
  TestResult r = manager.execute();
  if(!r.success()){
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;

}
