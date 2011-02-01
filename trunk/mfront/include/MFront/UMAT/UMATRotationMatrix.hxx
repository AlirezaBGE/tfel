/*!
 * \file   UMATRotationMatrix.hxx
 * \brief  This file declares the UMATRotationMatrix class
 * \author �. Castelier
 * \date   17 mar 2008
 */

#ifndef _LIB_MFRONT_UMATROTATIONMATRIX_H_
#define _LIB_MFRONT_UMATROTATIONMATRIX_H_ 

#include"TFEL/Config/TFELConfig.hxx"
#include"MFront/UMAT/UMAT.hxx"

namespace umat
{

  struct TFEL_VISIBILITY_EXPORT UMATRotationMatrix2D
  {
    UMATRotationMatrix2D(const UMATReal *const,
			 const UMATReal *const);
    
    // Compute stresses in the material space
    void rotateStressesForward(const UMATReal *const,
			             UMATReal *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const UMATReal *const,
                                      UMATReal *const);

    // Compute strain in the material space
    void rotateStrainsForward(const UMATReal *const,
                                    UMATReal *const);
    // Compute stresses back in the global space
    void rotateStrainsBackward(const UMATReal *const,
                                     UMATReal *const);

  private:

    UMATReal MN[4][4];
  
  }; // end of struct UMATRotationMatrix2D
  
  struct TFEL_VISIBILITY_EXPORT UMATRotationMatrix3D
  {
    
    UMATRotationMatrix3D(const UMATReal *const,
			 const UMATReal *const);
    
    // Compute stresses in the material space
    void rotateStressesForward(const UMATReal *const,
			             UMATReal *const);
    // Compute stresses back in the global space
    void rotateStressesBackward(const UMATReal *const,
                                      UMATReal *const);

    // Compute strain in the material space
    void rotateStrainsForward(const UMATReal *const,
                                    UMATReal *const);
    // Compute stresses back in the global space
    void rotateStrainsBackward(const UMATReal *const,
                                     UMATReal *const);

  private:

    UMATReal MN[6][6];
  
  }; // end of struct UMATRotationMatrix3D

} // end of namespace umat

#endif /* _LIB_MFRONT_UMATROTATIONMATRIX_H */

