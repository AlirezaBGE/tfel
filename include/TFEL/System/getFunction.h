/*!
 * \file   getFunction.h
 * \brief  this file declares some C wrappers around the dlsym
 * system call.
 * \author Helfer Thomas
 * \date   13 déc 2008
 */

#ifndef _LIB_TFEL_GETFUNCTION_H_
#define _LIB_TFEL_GETFUNCTION_H_

#if defined _WIN32 || defined _WIN64 ||defined __CYGWIN__
#include <windows.h>
typedef HINSTANCE__* LibraryHandlerPtr;
#else
typedef void*        LibraryHandlerPtr;
#endif /* defined _WIN32 || defined _WIN64 ||defined __CYGWIN__ */

#include"TFEL/System/ExternalFunctionsPrototypes.hxx"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

  /*!
   * \brief this function get if the number of argument of the
   * specified function
   *
   * This function search a symbol called name+'_nargs' and interpret
   * it as and unsigned short. The value of that unsigned short
   * compared to the number given in argument.
   *
   * The name 'castem' comes from an extension we wrote for that
   * finite element code that uses function with the following
   * prototype double (*)(const double* const);
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return int, the number of variables (-1 if on error).
   */
  int tfel_getCastemFunctionNumberOfVariables(LibraryHandlerPtr,
					      const char * const);
  /*!
   * \brief this function returns true if the given behaviour requires
   * an offset for the elastic properties.
   * \param l: link to library opened through dlopen
   * \param f: name of the behaviour to be checked
   * \return int, 0 if false, 1 if true, -1 if an error occured.
   */
  int tfel_getUMATRequiresStiffnessTensor(LibraryHandlerPtr,
					  const char * const);
  /*!
   * \brief this function returns true if the given behaviour requires
   * an offset of the thermal expansions coefficients.
   * \param l: link to library opened through dlopen
   * \param f: name of the behaviour to be checked
   * \return int, 0 if false, 1 if true, -1 if an error occured.
   */
  int tfel_getUMATRequiresThermalExpansionCoefficientTensor(LibraryHandlerPtr,
							    const char * const);

  /*!
   * \brief this function returns true if the given behaviour saves the tangent operator
   * \param l: link to library opened through dlopen
   * \param f: name of the behaviour to be checked
   * \return int, 0 if false, 1 if true, -1 if an error occured.
   */
  int tfel_checkIfAsterBehaviourSavesTangentOperator(LibraryHandlerPtr,
						     const char * const);

  int tfel_getBool(LibraryHandlerPtr,
		   const char * const);

  int tfel_getUnsignedShort(LibraryHandlerPtr,
			    const char * const);


  char **
  tfel_getArrayOfStrings(LibraryHandlerPtr,
			 const char * const);

  int *
  tfel_getArrayOfInt(LibraryHandlerPtr,
		     const char * const);

  char **
  tfel_getCastemFunctionVariables(LibraryHandlerPtr,
				  const char * const);
  /*!
   * \return a function giving access the behaviour integration error
   * report.
   * \param l: link to library opened through dlopen
   * \param b: name of the behaviour
   */
  void * (TFEL_ADDCALL_PTR tfel_getBehaviourIntegrationErrorReport(LibraryHandlerPtr,
								   const char * const))(void);
  
  int (TFEL_ADDCALL_PTR tfel_getSetOutOfBoundsPolicyFunction(LibraryHandlerPtr lib,
							     const char * const name))(const int);

  int (TFEL_ADDCALL_PTR tfel_getSetParameterFunction(LibraryHandlerPtr lib,
						     const char * const name))(const char* const,
									       const double);

  int (TFEL_ADDCALL_PTR tfel_getSetIntegerParameterFunction(LibraryHandlerPtr lib,
							    const char * const name))(const char* const,
										      const int);

  int (TFEL_ADDCALL_PTR tfel_getSetUnsignedShortParameterFunction(LibraryHandlerPtr lib,
								  const char * const name))(const char* const,
											    const unsigned short);
  /*!
   * \brief this function returns a function of type
   * void (*)(const CyranoIntegerType  *const,
   *          const CyranoRealType *const,
   *          const CyranoRealType *const,
   *          CyranoRealType *const,
   * 	      const CyranoRealType *const,
   * 	      const CyranoRealType *const,
   * 	      const CyranoRealType *const,
   * 	      const CyranoRealType *const,
   * 	      const CyranoRealType *const,
   * 	      const CyranoIntegerType  *const,
   * 	      const CyranoRealType *const,
   * 	      const CyranoRealType *const,
   * 	      CyranoRealType *const,
   * 	      const CyranoIntegerType  *const,
   * 	      CyranoRealType *const,
   * 	      const CyranoIntegerType  *const,
   * 	      CyranoIntegerType  *const);
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  void (TFEL_ADDCALL_PTR tfel_getCyranoFunction(LibraryHandlerPtr,const char * const))(const CyranoIntegerType  *const,
										       const CyranoRealType *const,
										       const CyranoRealType *const,
										       CyranoRealType *const,
										       const CyranoRealType *const,
										       const CyranoRealType *const,
										       const CyranoRealType *const,
										       const CyranoRealType *const,
										       const CyranoRealType *const,
										       const CyranoIntegerType  *const,
										       const CyranoRealType *const,
										       const CyranoRealType *const,
										       CyranoRealType *const,
										       const CyranoIntegerType  *const,
										       CyranoRealType *const,
										       const CyranoIntegerType  *const,
										       CyranoIntegerType  *const);
  /*!
   * \brief this function returns a function of type
   *
   * The name 'umat' comes from the Cast3M finite element code.
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  void (TFEL_ADDCALL_PTR tfel_getUMATFunction(LibraryHandlerPtr,const char * const))(UMATRealType *const,
										     UMATRealType *const,
										     UMATRealType *const,
										     UMATRealType *const,
										     UMATRealType *const,
										     UMATRealType *const,
										     UMATRealType *const,
										     UMATRealType *const,
										     UMATRealType *const,
										     UMATRealType *const,
										     const UMATRealType *const,
										     const UMATRealType *const,
										     const UMATRealType *const,
										     const UMATRealType *const,
										     const UMATRealType *const,
										     const UMATRealType *const,
										     const UMATRealType *const,
										     const UMATRealType *const,
										     const char      *const,
										     const UMATIntegerType  *const,
										     const UMATIntegerType  *const,
										     const UMATIntegerType  *const,
										     const UMATIntegerType  *const,
										     const UMATRealType *const,
										     const UMATIntegerType  *const,
										     const UMATRealType *const,
										     const UMATRealType *const,
										     UMATRealType *const,
										     const UMATRealType *const,
										     const UMATRealType *const,
										     const UMATRealType *const,
										     const UMATIntegerType  *const,
										     const UMATIntegerType  *const,
										     const UMATIntegerType  *const,
										     const UMATIntegerType  *const,
										     const UMATIntegerType  *const,
										     UMATIntegerType  *const,
										     const int);

 /*!
   * \brief this function returns a function of type
   *  void (*)(AsterRealType *const,
   *	       AsterRealType *const,
   *	       AsterRealType *const,
   *	       const AsterRealType *const,
   *	       const AsterRealType *const,
   *	       const AsterRealType *const,
   *	       const AsterRealType *const,
   *	       const AsterRealType *const,
   *	       const AsterRealType *const,
   *	       const AsterRealType *const,
   *	       const AsterIntegerType  *const,
   *	       const AsterIntegerType  *const,
   *	       const AsterRealType *const,
   *	       const AsterIntegerType  *const,
   *	       const AsterRealType *const,
   *	       AsterRealType *const,
   *	       const AsterIntegerType  *const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  void (TFEL_ADDCALL_PTR tfel_getAsterFunction(LibraryHandlerPtr,const char * const))(AsterRealType *const,
										      AsterRealType *const,
										      AsterRealType *const,
										      const AsterRealType *const,
										      const AsterRealType *const,
										      const AsterRealType *const,
										      const AsterRealType *const,
										      const AsterRealType *const,
										      const AsterRealType *const,
										      const AsterRealType *const,
										      const AsterIntegerType  *const,
										      const AsterIntegerType  *const,
										      const AsterRealType *const,
										      const AsterIntegerType  *const,
										      const AsterRealType *const,
										      AsterRealType *const,
										      const AsterIntegerType  *const);
 
  /*!
   * \brief this function returns a function of type
   * double (*)(const double * const)
   *
   * The name 'castem' comes from an extension we wrote for that
   * finite element code that uses function with the following
   * prototype double (*)(const double* const);
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCastemFunction(LibraryHandlerPtr,const char * const))(const double*const);

  /*!
   * \brief this function returns a function of type
   * double (*)(void)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction0(LibraryHandlerPtr,const char * const))(void);

  /*!
   * \brief this function returns a function of type
   * double (*)(double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction1(LibraryHandlerPtr,const char * const))(double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction2(LibraryHandlerPtr,const char * const))(double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction3(LibraryHandlerPtr,const char * const))(double,double,
										     double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction4(LibraryHandlerPtr,const char * const))(double,double,
										     double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction5(LibraryHandlerPtr,const char * const))(double,double,
										     double,double,
										     double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction6(LibraryHandlerPtr,const char * const))(double,double,
										     double,double,
										     double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction7(LibraryHandlerPtr,const char * const))(double,double,
										     double,double,
										     double,double,
										     double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction8(LibraryHandlerPtr,const char * const))(double,double,
										     double,double,
										     double,double,
										     double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,double,double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction9(LibraryHandlerPtr,const char * const))(double,double,
										     double,double,
										     double,double,
										     double,double,
										     double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,double,double,double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction10(LibraryHandlerPtr,const char * const))(double,double,
										      double,double,
										      double,double,
										      double,double,
										      double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,
   *          double,double,double,double,
   *          double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction11(LibraryHandlerPtr,const char * const))(double,double,
										      double,double,
										      double,double,
										      double,double,
										      double,double,
										      double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,
   *          double,double,double,double,
   *          double,double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction12(LibraryHandlerPtr,const char * const))(double,double,
										      double,double,
										      double,double,
										      double,double,
										      double,double,
										      double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,
   *          double,double,double,double,
   *          double,double,double,double,
   *          double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction13(LibraryHandlerPtr,const char * const))(double,double,
										      double,double,
										      double,double,
										      double,double,
										      double,double,
										      double,double,
										      double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,
   *          double,double,double,double,
   *          double,double,double,double,
   *          double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction14(LibraryHandlerPtr,const char * const))(double,double,
										      double,double,
										      double,double,
										      double,double,
										      double,double,
										      double,double,
										      double,double);

  /*!
   * \brief this function returns a function of type
   * double (*)(double,double,double,double,
   *          double,double,double,double,
   *          double,double,double,double,
   *          double,double,double)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getCFunction15(LibraryHandlerPtr,const char * const))(double,double,
										      double,double,
										      double,double,
										      double,double,
										      double,double,
										      double,double,
										      double,double,
										      double);

  /*!
   * \brief this function returns a function of type
   * double (*)(void)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction0(LibraryHandlerPtr,const char * const))(void);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction1(LibraryHandlerPtr,const char * const))(const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction2(LibraryHandlerPtr,const char * const))(const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction3(LibraryHandlerPtr,const char * const))(const double* const,const double* const,
											   const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction4(LibraryHandlerPtr,const char * const))(const double* const,const double* const,
											   const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction5(LibraryHandlerPtr,const char * const))(const double* const,const double* const,
											   const double* const,const double* const,
											   const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction6(LibraryHandlerPtr,const char * const))(const double* const,const double* const,
											   const double* const,const double* const,
											   const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction7(LibraryHandlerPtr,const char * const))(const double* const,const double* const,
											   const double* const,const double* const,
											   const double* const,const double* const,
											   const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction8(LibraryHandlerPtr,const char * const))(const double* const,const double* const,
											   const double* const,const double* const,
											   const double* const,const double* const,
											   const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction9(LibraryHandlerPtr,const char * const))(const double* const,const double* const,
											   const double* const,const double* const,
											   const double* const,const double* const,
											   const double* const,const double* const,
											   const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction10(LibraryHandlerPtr,const char * const))(const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction11(LibraryHandlerPtr,const char * const))(const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction12(LibraryHandlerPtr,const char * const))(const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction13(LibraryHandlerPtr,const char * const))(const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction14(LibraryHandlerPtr,const char * const))(const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const);

  /*!
   * \brief this function returns a function of type
   * double (*)(const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const,const double* const,
   *          const double* const,const double* const,const double* const)
   *
   * This function is a simple wrapper around dlsym. We can check if a
   * symbol with the given name exists but cannot check if it points to
   * a function with that prototype.
   *
   * \param LibraryHandlerPtr, link to library opened through dlopen
   * \param const char * const, name of the function to be checked
   * \return a function pointer if the call succeed, the NULL pointer if not.
   */
  double (TFEL_ADDCALL_PTR tfel_getFortranFunction15(LibraryHandlerPtr,const char * const))(const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const,const double* const,
											    const double* const);
#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif 	/* _LIB_TFEL_GETFUNCTION_H_ */
