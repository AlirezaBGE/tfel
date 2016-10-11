/*! 
 * \file  mfront/mtest/FAndersonAccelerationAlgorithm.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2014
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#include<limits>
#include<ostream>
#include<iterator>
#include<stdexcept>

#include"MFront/MFrontLogStream.hxx"
#include"MTest/FAndersonAccelerationAlgorithm.hxx"

namespace mtest
{

  FAndersonAccelerationAlgorithm::FAndersonAccelerationAlgorithm() = default;

  std::string
  FAndersonAccelerationAlgorithm::getName() const
  {
    return "FAnderson";
  } // end of FAndersonAccelerationAlgorithm::getName

  void
  FAndersonAccelerationAlgorithm::setParameter(const std::string& p,
						 const std::string& v)
  {
    const std::string m = "FAndersonAccelerationAlgorithm::"
      "setParameterAbaqusExplicitBehaviour";
    auto throw_if = [&m](const bool c, const std::string& msg){
      if(c){throw(std::runtime_error(m+": "+msg));}
    };
    if(p=="MethodOrder"){
      throw_if(this->Nmax!=-1,"the method order has already been defined");
      const auto i = AccelerationAlgorithm::convertToUnsignedShort(m,v);
      throw_if(i==0,"invalid method order value.");
      this->Nmax = i;
    } else if(p=="AccelerationPeriod"){
      throw_if(this->alMax!=-1,"the acceleration period has already been defined");
      const auto i = AccelerationAlgorithm::convertToUnsignedShort(m,v);
      throw_if(i==0,"invalid acceleration period value.");
      this->alMax = i;
    } else {
      throw_if(true,"invalid parameter '"+p+"'.");
    }
  } // end of FAndersonAccelerationAlgorithm::setParameter

  void
  FAndersonAccelerationAlgorithm::initialize(const unsigned short psz)
  {
    this->a = std::unique_ptr<FAnderson>(new FAnderson([psz](){
	  return new tfel::math::vector<real>(psz);
	}));
    if(this->Nmax==-1){this->Nmax = 4;}
    if(this->alMax==-1){this->alMax = 2;}
    mfront::getLogStream() << "FAndersonAccelerationAlgorithm::initialize: " << this->Nmax << " " << this->alMax << std::endl;
    this->a->setAnderson(this->Nmax,this->alMax);
  } // end of FAndersonAccelerationAlgorithm::initialize

  void FAndersonAccelerationAlgorithm::preExecuteTasks()
  {
    this->a->restart(this->uO,this->uN,this->Df);
  } // end of FAndersonAccelerationAlgorithm::preExecuteTasks

  void
  FAndersonAccelerationAlgorithm::execute(tfel::math::vector<real>& u1,
					  const tfel::math::vector<real>& du,
					  const tfel::math::vector<real>& r,
					  const real,
					  const real,
					  const unsigned short iter)
  {
    auto print = [](const char* n,const tfel::math::vector<real>& v){
      auto& os = mfront::getLogStream();
      os << n << ": ";
      std::copy(v.begin(),v.end(),std::ostream_iterator<real>(os," "));
      os << std::endl;
    };
    // for the first iterator, this->uO==this->uN
    *(this->uN) = u1;
    *(this->Df) = r;
    this->a->newIter(this->uO,this->uN,this->Df);
    u1 = *(this->uO);
  } // end of FAndersonAccelerationAlgorithm::execute

  void FAndersonAccelerationAlgorithm::postExecuteTasks()
  {} // end of FAndersonAccelerationAlgorithm::postExecuteTasks

  FAndersonAccelerationAlgorithm::~FAndersonAccelerationAlgorithm() = default;

} // end of namespace mtest