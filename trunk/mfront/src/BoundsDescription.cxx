/*!
 * \file   BoundsDescription.cxx
 * 
 * \brief    
 * \author Helfer Thomas
 * \date   06 Mar 2007
 */

#include<limits>
#include<string>
#include<sstream>
#include<stdexcept>
#include"MFront/BoundsDescription.hxx"

namespace mfront{

  BoundsDescription::BoundsDescription()
    : category(Standard),
      lowerBound(std::numeric_limits<long double>::min()),
      upperBound(std::numeric_limits<long double>::max())
  {}

  void
  BoundsDescription::writeBoundsChecks(std::ofstream& file) const
  {
    using namespace std;
    if(this->arraySize==1u){
      this->writeBoundsChecks(file,this->varName);
    } else {
      for(unsigned short i=0;i!=this->arraySize;++i){
	ostringstream n;
	n << this->varName;
	n << '[' << i << ']';
	this->writeBoundsChecks(file,n.str());
      }
    }
  } // end of BoundsDescription::writeBoundsChecks

  void
  BoundsDescription::writeBoundsChecks(std::ofstream& file,
				       const std::string& n) const
  {
    using namespace std;
    if(this->category==Standard){
      if(this->boundsType==Lower){
	if(this->varCategory==ExternalStateVarII){
	  file << "BoundsCheck<N>::lowerBoundCheck(\""
	       << n << "0\",this->" << n << "0,"
	       << "static_cast<real>(" << this->lowerBound << "),this->policy);\n";
	  file << "BoundsCheck<N>::lowerBoundCheck(\""
	       << n << "1\",this->" << n << "1,"
	       << "static_cast<real>(" << this->lowerBound << "),this->policy);\n";
	} else {
	  file << "BoundsCheck<N>::lowerBoundCheck(\""
	       << n << "\",this->" << n << ","
	       << "static_cast<real>(" << this->lowerBound << "),this->policy);\n";
	  if(this->varCategory==ExternalStateVar){
	    file << "BoundsCheck<N>::lowerBoundCheck(\""
		 << n << "+d" << n << "\",this->" 
		 << n << "+this->d" << n << ","
		 << "static_cast<real>(" << this->lowerBound << "),this->policy);\n";
	  }
	}
      } else if(this->boundsType==Upper){
	if(this->varCategory==ExternalStateVarII){
	  file << "BoundsCheck<N>::upperBoundCheck(\""
	       << n << "0\",this->" << n << "0,"
	       << "static_cast<real>(" << this->upperBound << "),this->policy);\n";
	  file << "BoundsCheck<N>::upperBoundCheck(\""
	       << n << "1\",this->" << n << "1,"
	       << "static_cast<real>(" << this->upperBound << "),this->policy);\n";
	} else {
	  file << "BoundsCheck<N>::upperBoundCheck(\""
	       << n << "\",this->" << n << ","
	     << "static_cast<real>(" << this->upperBound << "),this->policy);\n";
	  if(this->varCategory==ExternalStateVar){
	    file << "BoundsCheck<N>::upperBoundCheck(\""
		 << n << "+d" << n << "\",this->" 
		 << n << "+this->d" << n << ","
		 << "static_cast<real>(" << this->upperBound << "),this->policy);\n";
	  }
	}
      } else if(this->boundsType==LowerAndUpper){
	if(this->varCategory==ExternalStateVarII){
	  file << "BoundsCheck<N>::lowerAndUpperBoundCheck(\""
	       << n << "0\",this->" << n << "0,"
	       << "static_cast<real>("   << this->lowerBound << "),"
	       << "static_cast<real>("   << this->upperBound << "),this->policy);\n";
	  file << "BoundsCheck<N>::lowerAndUpperBoundCheck(\""
	       << n << "1\",this->" << n << "1,"
	       << "static_cast<real>("   << this->lowerBound << "),"
	       << "static_cast<real>("   << this->upperBound << "),this->policy);\n";
	} else {
	  file << "BoundsCheck<N>::lowerAndUpperBoundsChecks(\""
	       << n << "\",this->" << n    << ","
	       << "static_cast<real>("   << this->lowerBound << "),"
	       << "static_cast<real>("   << this->upperBound << "),this->policy);\n";
	  if(this->varCategory==ExternalStateVar){
	    file << "BoundsCheck<N>::lowerAndUpperBoundsChecks(\""
		 << n << "+d" << n << "\",this->" 
		 << n << "+this->d" << n << ","
		 << "static_cast<real>("   << this->lowerBound << "),"
		 << "static_cast<real>("   << this->upperBound << "),this->policy);\n";
	  }
	}
      }
    } else if(this->category==Physical){
      if(this->boundsType==Lower){
	if(this->varCategory==ExternalStateVarII){
	  file << "BoundsCheck<N>::lowerBoundCheck(\""
	       << n << "0\",this->" << n << "0,"
	       << "static_cast<real>(" << this->lowerBound << "));\n";
	  file << "BoundsCheck<N>::lowerBoundCheck(\""
	       << n << "1\",this->" << n << "1,"
	       << "static_cast<real>(" << this->lowerBound << "));\n";
	} else {
	  file << "BoundsCheck<N>::lowerBoundCheck(\""
	       << n << "\",this->" << n << ","
	       << "static_cast<real>(" << this->lowerBound << "));\n";
	  if(this->varCategory==ExternalStateVar){
	    file << "BoundsCheck<N>::lowerBoundCheck(\""
		 << n << "+d" << n << "\",this->" 
		 << n << "+this->d" << n << ","
	       << "static_cast<real>(" << this->lowerBound << "));\n";
	  }
	}
      } else if(this->boundsType==Upper){
	if(this->varCategory==ExternalStateVarII){
	  file << "BoundsCheck<N>::upperBoundCheck(\""
	       << n << "0\",this->" << n << "0,"
	       << "static_cast<real>(" << this->upperBound << "));\n";
	  file << "BoundsCheck<N>::upperBoundCheck(\""
	       << n << "1\",this->" << n << "1,"
	       << "static_cast<real>(" << this->upperBound << "));\n";
	} else {
	  file << "BoundsCheck<N>::upperBoundCheck(\""
	       << n << "\",this->" << n << ","
	       << "static_cast<real>(" << this->upperBound << "));\n";
	  if(this->varCategory==ExternalStateVar){
	    file << "BoundsCheck<N>::upperBoundCheck(\""
		 << n << "+d" << n << "\",this->" 
		 << n << "+this->d" << n << ","
		 << "static_cast<real>(" << this->upperBound << "));\n";
	  }
	}
      } else if(this->boundsType==LowerAndUpper){
	if(this->varCategory==ExternalStateVarII){
	  file << "BoundsCheck<N>::lowerAndUpperBoundCheck(\""
	       << n << "0\",this->" << n << "0,"
	       << "static_cast<real>("   << this->lowerBound << "),"
	       << "static_cast<real>("   << this->upperBound << "));\n";
	  file << "BoundsCheck<N>::lowerAndUpperBoundCheck(\""
	       << n << "1\",this->" << n << "1,"
	       << "static_cast<real>("   << this->lowerBound << "),"
	       << "static_cast<real>("   << this->upperBound << "));\n";
	} else {
	  file << "BoundsCheck<N>::lowerAndUpperBoundsChecks(\""
	       << n << "\",this->" << n    << ","
	       << "static_cast<real>("   << this->lowerBound << "),"
	       << "static_cast<real>("   << this->upperBound << "));\n";
	  if(this->varCategory==ExternalStateVar){
	    file << "BoundsCheck<N>::lowerAndUpperBoundsChecks(\""
		 << n << "+d" << n << "\",this->" 
		 << n << "+this->d" << n << ","
		 << "static_cast<real>("   << this->lowerBound << "),"
		 << "static_cast<real>("   << this->upperBound << "));\n";
	  }
	}
      }
    } else {
      throw(runtime_error("BoundsDescription::writeBoundsChecks : internal error"));
    }
  } // end of BoundsDescription::writeBoundsChecks
  
} // end of namespace mfront
