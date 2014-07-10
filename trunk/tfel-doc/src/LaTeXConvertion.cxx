/*! 
 * \file  LaTeXConvertion.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 03 mai 2011
 */

#include"TFEL/Utilities/LaTeXConvertion.hxx"

namespace tfel
{

  namespace utilities
  {

    std::string
    LaTeXConvertion::capitalize(const std::string& s)
    {
      using namespace std;
      string r(s);
      if(r[0]=='�'){
	r[0]='�';
      } else if(r[0]=='�'){
	r[0]='�';
      } else if(r[0]=='�'){
	r[0]='�';
      } else if(r[0]=='�'){
	r[0]='�';
      } else if(r[0]=='�'){
	r[0]='�';
      } else {
	r[0] = static_cast<char>(toupper(r[0]));
      }
      return r;
    } // end of capitalize

  } // end of namespace utilities

} // end of namespace tfel

