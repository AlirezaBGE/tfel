/*!
 * \file   FiniteStrainBehaviourTangentOperator12.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   21 avril 2017
 * \copyright Copyright (C) 2006-2014 CEA/DEN, EDF R&D. All rights 
 * reserved. 
 * This project is publicly released under either the GNU GPL Licence 
 * or the CECILL-A licence. A copy of thoses licences are delivered 
 * with the sources of TFEL. CEA or EDF may also distribute this 
 * project under specific licensing conditions. 
 */

#ifdef NDEBUG
#undef NDEBUG
#endif /* NDEBUG */

#include<cstdlib>
#include<utility>
#include<iostream>

#include"TFEL/Tests/TestCase.hxx"
#include"TFEL/Tests/TestProxy.hxx"
#include"TFEL/Tests/TestManager.hxx"

#include"TFEL/Math/tensor.hxx"
#include"TFEL/Math/stensor.hxx"
#include"TFEL/Math/st2tost2.hxx"
#include"TFEL/Math/ST2toST2/ST2toST2ConceptIO.hxx"

#include"TFEL/Material/FiniteStrainBehaviourTangentOperator.hxx"

struct FiniteStrainBehaviourTangentOperator12 final
  : public tfel::tests::TestCase
{
  FiniteStrainBehaviourTangentOperator12()
    : tfel::tests::TestCase("TFEL/Material",
			    "FiniteStrainBehaviourTangentOperator12")
  {} // end of FiniteStrainBehaviourTangentOperator12
  virtual tfel::tests::TestResult
  execute() override
  {
    this->check1<1u>();
    this->check2<1u>();
    this->check1<2u>();
    this->check2<3u>();
    this->check1<3u>();
    this->check2<3u>();
    return this->result;
  }
 private:
  using real = double;
  using TangentOperator = tfel::material::FiniteStrainBehaviourTangentOperatorBase;
  // check that the perturbation approximation is ok
  template<unsigned short N>
  void check1(){
    using namespace tfel::math;
    using namespace tfel::material;
    using stensor   = tfel::math::stensor<N,real>;
    using tensor    = tfel::math::tensor<N,real>;
    using st2tost2  = tfel::math::st2tost2<N,real>;
    using size_type = unsigned short;
    const real l0  = 1.09465e+11;
    const real m0  = 5.6391e+12;
    const real eps = 1.e-7*2*m0;
    // Hencky-Biot law with PK2 result
    auto hb = [&l0,&m0](const stensor& C) -> stensor{
      auto e = stensor{};
      auto p = st2tost2{};
      const auto f   = [](const real x){return std::log1p(x-1)/2;};
      const auto df  = [](const real x){return 1/(2*x);};
      std::tie(e,p)  = C.template computeIsotropicFunctionAndDerivative<stensor::FSESJACOBIEIGENSOLVER>(f,df,1.e-14);
      const auto T   = eval(l0*trace(e)*stensor::Id()+2*m0*e);
      return 2*(T|p);
    };
    auto getD = [hb](const stensor& C,const real e){
      auto D = st2tost2{};
      auto Cp = C;
      for(size_type j=0;j!=StensorDimeToSize<N>::value;++j){
	Cp[j] += e;
	const auto Sp = hb(Cp);
	Cp[j] -= 2*e;
	const auto Sm = hb(Cp);
	const auto dS = (Sp-Sm)/(2*e);
	for(size_type i=0;i!=StensorDimeToSize<N>::value;++i){
	  D(i,j)=dS(i);
	}
	Cp[j] += e;
      }
      return D;
    };
    const real v1[9u] = {1.03,0.98,1.09,0.03,-0.012,0.04,-0.028,-0.015,0.005};
    const real v2[9u] = {0.70,1.125,1.32,-0.24,-0.32,0.15,-0.14,-0.05,0.08};
    const real v3[9u] = {1.70,0.625,0.625,0.,0.,0.,0.,0.,0.};
    const real v4[9u] = {-0.625,1.70,-0.625,0.,0.,0.,0.,0.,0.};
    const real v5[9u] = {-0.625,-0.625,1.70,0.,0.,0.,0.,0.,0.};
    const auto F0 = tensor::Id();
    for(const tensor& F1: {tensor::Id(),tensor{v1},tensor{v2},
			   tensor{v3},tensor{v4},tensor{v5}}){
      const auto  C = computeRightCauchyGreenTensor(F1);
      const auto nD = getD(C,5.e-7);
      const auto  D = eval(l0*st2tost2::IxI()+2*m0*st2tost2::Id());
      const auto  S = hb(C);
      const auto sig = convertSecondPiolaKirchhoffStressToCauchyStress(S,F1);
      auto aD = tfel::material::convert<TangentOperator::DT_DELOG,
					TangentOperator::DS_DC>(D,F0,F1,sig);
      for(size_type i=0;i!=tfel::math::StensorDimeToSize<N>::value;++i){
	for(size_type j=0;j!=tfel::math::StensorDimeToSize<N>::value;++j){
	  if(std::abs(aD(i,j)-nD(i,j))>eps){
	    std::cout << i << " " << j << " "
		      << nD(i,j) << " " << " " << aD(i,j)
		      << " " << std::abs(aD(i,j)-nD(i,j))
		      << " " << std::abs(aD(i,j)-nD(i,j))/(2*m0) << std::endl;
	  }
	  TFEL_TESTS_ASSERT(std::abs(aD(i,j)-nD(i,j))<eps);	
	}
      }
    }
  }
  // check that the perturbation approximation is ok
  template<unsigned short N>
  void check2(){
    using namespace tfel::math;
    using namespace tfel::material;
    using stensor   = tfel::math::stensor<N,real>;
    using tensor    = tfel::math::tensor<N,real>;
    using st2tost2  = tfel::math::st2tost2<N,real>;
    using size_type = unsigned short;
    const real l0  = 1.09465e+11;
    const real m0  = 5.6391e+12;
    const real eps = 1.e-7*2*m0;
    // Non symmetric behaviour from the Hencky-Biot law with PK2 result
    auto hb = [&l0,&m0](const stensor& C) -> stensor{
      auto e = stensor{};
      auto p = st2tost2{};
      const auto f   = [](const real x){return std::log1p(x-1)/2;};
      const auto df  = [](const real x){return 1/(2*x);};
      std::tie(e,p)  = C.template computeIsotropicFunctionAndDerivative<stensor::FSESJACOBIEIGENSOLVER>(f,df,1.e-14);
      auto D0 = eval(l0*st2tost2::IxI()+2*m0*st2tost2::Id());
      for(size_type i=0;i!=StensorDimeToSize<N>::value;++i){
	for(size_type j=i+1;j!=StensorDimeToSize<N>::value;++j){
	  D0(i,j) *= 2;
	}
      }
      const auto T   = D0*e;
      return 2*(T|p);
    };
    auto getD = [hb](const stensor& C,const real e){
      auto D = st2tost2{};
      auto Cp = C;
      for(size_type j=0;j!=StensorDimeToSize<N>::value;++j){
	Cp[j] += e;
	const auto Sp = hb(Cp);
	Cp[j] -= 2*e;
	const auto Sm = hb(Cp);
	const auto dS = (Sp-Sm)/(2*e);
	for(size_type i=0;i!=StensorDimeToSize<N>::value;++i){
	  D(i,j)=dS(i);
	}
	Cp[j] += e;
      }
      return D;
    };
    const real v1[9u] = {1.03,0.98,1.09,0.03,-0.012,0.04,-0.028,-0.015,0.005};
    const real v2[9u] = {0.70,1.125,1.32,-0.24,-0.32,0.15,-0.14,-0.05,0.08};
    const real v3[9u] = {1.70,0.625,0.625,0.,0.,0.,0.,0.,0.};
    const real v4[9u] = {-0.625,1.70,-0.625,0.,0.,0.,0.,0.,0.};
    const real v5[9u] = {-0.625,-0.625,1.70,0.,0.,0.,0.,0.,0.};
    const auto F0 = tensor::Id();
    for(const tensor& F1: {tensor::Id(),tensor{v1},tensor{v2},
			   tensor{v3},tensor{v4},tensor{v5}}){
      const auto  C = computeRightCauchyGreenTensor(F1);
      const auto nC = getD(C,5.e-7);
      auto  D = eval(l0*st2tost2::IxI()+2*m0*st2tost2::Id());
      for(size_type i=0;i!=StensorDimeToSize<N>::value;++i){
	for(size_type j=i+1;j!=StensorDimeToSize<N>::value;++j){
	  D(i,j) *= 2;
	}
      }
      const auto  S = hb(C);
      const auto sig = convertSecondPiolaKirchhoffStressToCauchyStress(S,F1);
      auto aC = st2tost2{};
      tfel::material::convert<TangentOperator::DT_DELOG,
			      TangentOperator::DS_DC>(aC,D,F0,F1,sig);
      for(size_type i=0;i!=tfel::math::StensorDimeToSize<N>::value;++i){
	for(size_type j=0;j!=tfel::math::StensorDimeToSize<N>::value;++j){
	  if(std::abs(aC(i,j)-nC(i,j))>eps){
	    std::cout << i << " " << j << " "
		      << nC(i,j) << " " << " " << aC(i,j)
		      << " " << std::abs(aC(i,j)-nC(i,j))
		      << " " << std::abs(aC(i,j)-nC(i,j))/(2*m0) << std::endl;
	  }
	  TFEL_TESTS_ASSERT(std::abs(aC(i,j)-nC(i,j))<eps);	
	}
      }
    }
  }
};

TFEL_TESTS_GENERATE_PROXY(FiniteStrainBehaviourTangentOperator12,
			  "FiniteStrainBehaviourTangentOperator12");

/* coverity [UNCAUGHT_EXCEPT]*/
int main()
{
  auto& m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("FiniteStrainBehaviourTangentOperator12.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
} // end of main
