/*! 
 * \file  tfel-doc/src/MTestDocParser.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 28 avril 2013
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<iostream>

#include"MTest/MTestParser.hxx"
#include"TFEL/Utilities/MTestDocParser.hxx"

namespace tfel{

  namespace utilities {
  
    MTestDocParser::MTestDocParser(const std::string& f)
      : MTest(),
	file(f)
    {
      auto parser = this->getParser();
      parser->execute(f);
    } // end of MTestDocParser::MTestDocParser
    
    void
    MTestDocParser::addDocumentation(std::map<std::string,
					      std::vector<TestDocumentation> >& r)
    {
      using namespace std;
      const auto c = "Mechanical behaviour unary testing";
      auto ptest = r.find(c);
      if(r.find(c)==r.end()){
	ptest = r.insert({c,vector<TestDocumentation>()}).first;
      }
      ptest->second.push_back(TestDocumentation());
      auto& t = ptest->second.back();
      t.name    = this->file;
      t.date    = this->date;
      t.author  = this->author;
      t.descriptions.insert({"english",this->description});
    } // end of MTestDocParser::execute

    MTestDocParser::~MTestDocParser()
    {} // end of MTestDocParser::~MTestDocParser

  } // end of namespace utilities

} // end of namespace tfel
