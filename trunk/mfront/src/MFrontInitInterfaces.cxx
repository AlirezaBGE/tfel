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
#include"MFront/MFrontLaTeXLawInterface.hxx"

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

#ifdef HAVE_CASTEM
#include"MFront/MFrontCastemLawInterface.hxx"
#include"MFront/MFrontUMATInterface.hxx"
#endif

#ifdef HAVE_ASTER
#include"MFront/MFrontAsterInterface.hxx"
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

    static const char * const texLawProxyNames [2] = {"latex","LaTex"};
    MFrontLawInterfaceProxy<MFrontLaTeXLawInterface> latexLawProxy(texLawProxyNames,texLawProxyNames+2);
    
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

#ifdef HAVE_PYTHON
    static const char * const pythonProxyNames[2] = {"python","Python"};
    MFrontLawInterfaceProxy<MFrontPythonLawInterface> pythonLawProxy(pythonProxyNames,pythonProxyNames+2);
#endif /* HAVE_PYTHON */

#ifdef HAVE_CASTEM
    static const char * const castemProxyNames[3] = {"castem","Castem","Cast3M"};
    MFrontLawInterfaceProxy<MFrontCastemLawInterface> castemLawProxy(castemProxyNames,castemProxyNames+3);
    MFrontBehaviourInterfaceProxy<MFrontUMATInterface> umatProxy;
#endif /* HAVE_CASTEM */

#ifdef HAVE_ASTER
    MFrontBehaviourInterfaceProxy<MFrontAsterInterface> asterProxy;
#endif /* HAVE_ASTER */

  } // end of initInterfaces(void)

} // end of namespace mfront
