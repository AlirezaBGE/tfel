/*!
 * \file   MFrontInitInterfaces.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   12 sep 2008
 */

#include"MFront/MFrontBehaviourInterfaceProxy.hxx"
#include"MFront/MFrontLawInterfaceProxy.hxx"
#include"MFront/MFrontModelInterfaceProxy.hxx"

#include"MFront/MFrontMFrontLawInterface.hxx"
#include"MFront/MFrontCLawInterface.hxx"
#include"MFront/MFrontExcelLawInterface.hxx"

#ifdef HAVE_FORTRAN
#include"MFront/MFrontFortranLawInterface.hxx"
#endif /* HAVE_FORTRAN */

#include"MFront/MFrontCppLawInterface.hxx"
#include"MFront/MFrontCppTestLawInterface.hxx"

#ifdef HAVE_PYTHON
#include"MFront/MFrontPythonLawInterface.hxx"
#endif /* HAVE_PYTHON */

#ifdef HAVE_GNUPLOT
#include"MFront/MFrontGnuplotLawInterface.hxx"
#endif

#ifdef HAVE_PLEIADES
#include"MFront/MFrontPleiadesLawInterface.hxx"
#include"MFront/MFrontCelaenoModelInterface.hxx"
#include"MFront/MFrontGerminalModelInterface.hxx"
#include"MFront/MFrontCelaenoModelInterface_v1.3.hxx"
#endif /* HAVE_PLEIADES */

#ifdef HAVE_CASTEM
#include"MFront/MFrontCastemLawInterface.hxx"
#include"MFront/MFrontUMATInterface.hxx"
#endif

namespace mfront
{

  void
  initInterfaces(void)
  {

    MFrontLawInterfaceProxy<MFrontCLawInterface> cLawProxy;

#ifdef HAVE_FORTRAN
    MFrontLawInterfaceProxy<MFrontFortranLawInterface> fLawProxy;
#endif /* HAVE_FORTRAN */

    MFrontLawInterfaceProxy<MFrontMFrontLawInterface>  mLawProxy;

    MFrontLawInterfaceProxy<MFrontExcelLawInterface> excelLawProxy(MFrontExcelLawInterface::getName(),
								   MFrontCLawInterface::getName());
    
    static const char * const cppLawProxyNames [6] = {"c++","C++",
						      "cxx","Cxx",
						      "cpp","Cpp"};
    MFrontLawInterfaceProxy<MFrontCppLawInterface> cppLawProxy(cppLawProxyNames,cppLawProxyNames+6);

    MFrontLawInterfaceProxy<MFrontCppTestLawInterface> cppTestLawProxy(MFrontCppTestLawInterface::getName(),
								       MFrontCppLawInterface::getName());

#ifdef HAVE_GNUPLOT
    MFrontLawInterfaceProxy<MFrontGnuplotLawInterface> gnuplotLawProxy(MFrontGnuplotLawInterface::getName(),
								       MFrontCppTestLawInterface::getName());
#endif /* HAVE_GNUPLOT */

#ifdef HAVE_PLEIADES
    MFrontLawInterfaceProxy<MFrontPleiadesLawInterface>         pleiadesLawProxy;
    MFrontLawInterfaceProxy<MFrontPleiadesLawInterface>         pleiadesLawProxy2("Pleiades");
    MFrontModelInterfaceProxy<MFrontCelaenoModelInterface>      celaenoModelProxy;
    MFrontModelInterfaceProxy<MFrontGerminalModelInterface>      germinalModelProxy;
    MFrontModelInterfaceProxy<MFrontCelaenoModelInterface_v1_3> celaenoModelProxy_v13;
#endif /* HAVE_PLEIADES */

#ifdef HAVE_PYTHON
    static const char * const pythonProxyNames[2] = {"python","Python"};
    MFrontLawInterfaceProxy<MFrontPythonLawInterface> pythonLawProxy(pythonProxyNames,pythonProxyNames+2);
#endif /* HAVE_PYTHON */

#ifdef HAVE_CASTEM
    static const char * const castemProxyNames[3] = {"castem","Castem","Cast3M"};
    MFrontLawInterfaceProxy<MFrontCastemLawInterface> castemLawProxy(castemProxyNames,castemProxyNames+3);
    MFrontBehaviourInterfaceProxy<MFrontUMATInterface> umatProxy;
#endif /* HAVE_CASTEM */

  } // end of initInterfaces(void)

} // end of namespace mfront
