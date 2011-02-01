/*!
 * \file   UMATRotationMatrix.cxx
 * \brief    
 * \author �. Castelier
 * \date   17 mar 2008
 */

#include"MFront/UMAT/UMATRotationMatrix.hxx"

namespace umat
{

  // Constructeur
  // V : Matrice de passage mat�riau/�lement
  //     (deux premiers vecteurs)
  // drot : Matrice de passage �lement/global
  UMATRotationMatrix2D::UMATRotationMatrix2D(const UMATReal *const V,
					     const UMATReal *const drot)
  {
    // Matrice de passage mat�riau/global
    UMATReal a[4];

    // Premier vecteur
    // a[1,1]
    a[0]=drot[0]*V[0]+drot[2]*V[1];
    // a[2,1]
    a[1]=drot[1]*V[0]+drot[3]*V[1];
    // Deuxi�me vecteur :
    //   Produit vectoriel
    // a[1,2]
    a[2]=-a[1];
    // a[2,2]
    a[3]=a[0];

    // Contruction de la matrice de passage N (pour les tenseurs)
    // Premi�re ligne
    MN[0][0]=a[0]*a[0];
    MN[0][1]=a[1]*a[1];
    MN[0][2]=a[0]*a[1];

    // Deuxi�me ligne
    MN[1][0]=a[2]*a[2];
    MN[1][1]=a[3]*a[3];
    MN[1][2]=a[2]*a[3];

    // Troisi�me ligne
    MN[2][0]=a[0]*a[2];
    MN[2][1]=a[1]*a[3];
    MN[2][2]=a[0]*a[3]+a[1]*a[2];
  } // end of UMATRotationMatrix2D::UMATRotationMatrix2D

  // Calcul des d�formations dans le rep�re mat�riau
  // e^m=N:e^g
  // eg : D�formations dans le rep�re global
  // em : D�formations dans le rep�re mat�riau
  void
  UMATRotationMatrix2D::rotateStrainsForward(const UMATReal *const eg,
					     UMATReal *const em) {
    // e11
    em[0]=MN[0][0]*eg[0]+MN[0][1]*eg[1]
      +MN[0][2]*eg[3];
    // e22
    em[1]=MN[1][0]*eg[0]+MN[1][1]*eg[1]
      +MN[1][2]*eg[3];
    // g12
    em[3]=2*(MN[2][0]*eg[0]+MN[2][1]*eg[1])
      +MN[2][2]*eg[3];

    // e33
    em[2]=eg[2];
  } // end of UMATRotationMatrix2D::rotateStrainsForward


  // Calcul des contraintes dans le rep�re global
  // s^g=N^T:s^m
  // sm : Contraintes dans le rep�re mat�riau
  // sg : Contraintes dans le rep�re global
  void
  UMATRotationMatrix2D::rotateStressesBackward(const UMATReal *const sm,
					       UMATReal *const sg){
    // s11
    sg[0]=MN[0][0]*sm[0]+MN[1][0]*sm[1]
      +2*MN[2][0]*sm[3];
    // s22
    sg[1]=MN[0][1]*sm[0]+MN[1][1]*sm[1]
      +2*MN[2][1]*sm[3];
    // s12
    sg[3]=MN[0][2]*sm[0]+MN[1][2]*sm[1]
      +MN[2][2]*sm[3];

    // s33
    sg[2]=sm[2];
  } // end of UMATRotationMatrix2D::rotateStressesBackward

  // Calcul des contraintes dans le rep�re mat�riau
  // s^m=M:s^g
  // sg : Contraintes dans le rep�re global
  // sm : Contraintes dans le rep�re mat�riau
  void
  UMATRotationMatrix2D::rotateStressesForward(const UMATReal *const sg,
					      UMATReal *const sm){
    // s11
    sm[0]=MN[0][0]*sg[0]+MN[0][1]*sg[1]
      +2*MN[0][2]*sg[3];
    // s22
    sm[1]=MN[1][0]*sg[0]+MN[1][1]*sg[1]
      +2*MN[1][2]*sg[3];
    // s12
    sm[3]=MN[2][0]*sg[0]+MN[2][1]*sg[1]
      +MN[2][2]*sg[3];

    // s33
    sm[2]=sg[2];
  } // end of UMATRotationMatrix2D::rotateStressesForward

  // Calcul des d�formations dans le rep�re global
  // e^g=M^T:e^m
  // em : Contraintes dans le rep�re mat�riau
  // eg : Contraintes dans le rep�re global
  void
  UMATRotationMatrix2D::rotateStrainsBackward(const UMATReal *const em,
					      UMATReal *const eg)
  {
    // e11
    eg[0]=MN[0][0]*em[0]+MN[1][0]*em[1]
      +MN[2][0]*em[3];
    // e22
    eg[1]=MN[0][1]*em[0]+MN[1][1]*em[1]
      +MN[2][1]*em[3];
    // g12
    eg[3]=2*(MN[0][2]*em[0]+MN[1][2]*em[1])
      +MN[2][2]*em[3];

    // e33
    eg[2]=em[2];
  } // end of UMATRotationMatrix2D:::rotateStrainsBackward

  // Constructeur
  // V : Matrice de passage mat�riau/�lement
  //     (deux premiers vecteurs)
  // drot : Matrice de passage �lement/global
  UMATRotationMatrix3D::UMATRotationMatrix3D(const UMATReal *const V,
					     const UMATReal *const drot)
  {
    // Matrice de passage mat�riau/global
    UMATReal a[9];
    
    // Premier vecteur
    a[0]=drot[0]*V[0]+drot[3]*V[1]+drot[6]*V[2];
    a[1]=drot[1]*V[0]+drot[4]*V[1]+drot[7]*V[2];
    a[2]=drot[2]*V[0]+drot[5]*V[1]+drot[8]*V[2];
    // Deuxi�me vecteur
    a[3]=drot[0]*V[3]+drot[3]*V[4]+drot[6]*V[5];
    a[4]=drot[1]*V[3]+drot[4]*V[4]+drot[7]*V[5];
    a[5]=drot[2]*V[3]+drot[5]*V[4]+drot[8]*V[5];
    // Troisi�me vecteur :
    //   produit vectoriel des deux premiers
    a[6]=a[1]*a[5]-a[4]*a[2];
    a[7]=a[2]*a[3]-a[5]*a[0];
    a[8]=a[0]*a[4]-a[3]*a[1];

    // Contruction de la matrice de passage N (pour les tenseurs)
    // Premi�re ligne
    MN[0][0]=a[0]*a[0];
    MN[0][1]=a[1]*a[1];
    MN[0][2]=a[2]*a[2];
    MN[0][5]=a[1]*a[2];
    MN[0][4]=a[2]*a[0];
    MN[0][3]=a[0]*a[1];

    // Deuxi�me ligne
    MN[1][0]=a[3]*a[3];
    MN[1][1]=a[4]*a[4];
    MN[1][2]=a[5]*a[5];
    MN[1][5]=a[4]*a[5];
    MN[1][4]=a[5]*a[3];
    MN[1][3]=a[3]*a[4];

    // Troisi�me ligne
    MN[2][0]=a[6]*a[6];
    MN[2][1]=a[7]*a[7];
    MN[2][2]=a[8]*a[8];
    MN[2][5]=a[7]*a[8];
    MN[2][4]=a[8]*a[6];
    MN[2][3]=a[6]*a[7];

    // Quatri�me ligne
    MN[5][0]=a[3]*a[6];
    MN[5][1]=a[4]*a[7];
    MN[5][2]=a[5]*a[8];
    MN[5][5]=a[4]*a[8]+a[5]*a[7];
    MN[5][4]=a[5]*a[6]+a[3]*a[8];
    MN[5][3]=a[3]*a[7]+a[4]*a[6];

    // Cinqui�me ligne
    MN[4][0]=a[6]*a[0];
    MN[4][1]=a[7]*a[1];
    MN[4][2]=a[8]*a[2];
    MN[4][5]=a[7]*a[2]+a[8]*a[1];
    MN[4][4]=a[8]*a[0]+a[6]*a[2];
    MN[4][3]=a[6]*a[1]+a[7]*a[0];

    // Sixi�me ligne
    MN[3][0]=a[0]*a[3];
    MN[3][1]=a[1]*a[4];
    MN[3][2]=a[2]*a[5];
    MN[3][5]=a[1]*a[5]+a[2]*a[4];
    MN[3][4]=a[2]*a[3]+a[0]*a[5];
    MN[3][3]=a[0]*a[4]+a[1]*a[3];
  } // end of UMATRotationMatrix3D::UMATRotationMatrix3D

  // Calcul des d�formations dans le rep�re mat�riau
  // e^m=N:e^g
  // eg : D�formations dans le rep�re global
  // em : D�formations dans le rep�re mat�riau
  void
  UMATRotationMatrix3D::rotateStrainsForward(const UMATReal *const eg,
					     UMATReal *const em)
  {
    em[0]=MN[0][0]*eg[0]+MN[0][1]*eg[1]+MN[0][2]*eg[2]
      +MN[0][3]*eg[3]+MN[0][4]*eg[4]+MN[0][5]*eg[5];
    em[1]=MN[1][0]*eg[0]+MN[1][1]*eg[1]+MN[1][2]*eg[2]
      +MN[1][3]*eg[3]+MN[1][4]*eg[4]+MN[1][5]*eg[5];
    em[2]=MN[2][0]*eg[0]+MN[2][1]*eg[1]+MN[2][2]*eg[2]
      +MN[2][3]*eg[3]+MN[2][4]*eg[4]+MN[2][5]*eg[5];
    em[3]=2*(MN[3][0]*eg[0]+MN[3][1]*eg[1]+MN[3][2]*eg[2])
      +MN[3][3]*eg[3]+MN[3][4]*eg[4]+MN[3][5]*eg[5];
    em[4]=2*(MN[4][0]*eg[0]+MN[4][1]*eg[1]+MN[4][2]*eg[2])
      +MN[4][3]*eg[3]+MN[4][4]*eg[4]+MN[4][5]*eg[5];
    em[5]=2*(MN[5][0]*eg[0]+MN[5][1]*eg[1]+MN[5][2]*eg[2])
      +MN[5][3]*eg[3]+MN[5][4]*eg[4]+MN[5][5]*eg[5];
  } // end of UMATRotationMatrix3D::rotateStrainForward

  // Calcul des contraintes dans le rep�re global
  // s^g=N^T:s^m
  // sm : Contraintes dans le rep�re mat�riau
  // sg : Contraintes dans le rep�re global
  void
  UMATRotationMatrix3D::rotateStressesBackward(const UMATReal *const sm,
					       UMATReal *const sg){
    sg[0]=MN[0][0]*sm[0]+MN[1][0]*sm[1]+MN[2][0]*sm[2]
      +2*(MN[3][0]*sm[3]+MN[4][0]*sm[4]+MN[5][0]*sm[5]);
    sg[1]=MN[0][1]*sm[0]+MN[1][1]*sm[1]+MN[2][1]*sm[2]
      +2*(MN[3][1]*sm[3]+MN[4][1]*sm[4]+MN[5][1]*sm[5]);
    sg[2]=MN[0][2]*sm[0]+MN[1][2]*sm[1]+MN[2][2]*sm[2]
      +2*(MN[3][2]*sm[3]+MN[4][2]*sm[4]+MN[5][2]*sm[5]);
    sg[3]=MN[0][3]*sm[0]+MN[1][3]*sm[1]+MN[2][3]*sm[2]
      +MN[3][3]*sm[3]+MN[4][3]*sm[4]+MN[5][3]*sm[5];
    sg[4]=MN[0][4]*sm[0]+MN[1][4]*sm[1]+MN[2][4]*sm[2]
      +MN[3][4]*sm[3]+MN[4][4]*sm[4]+MN[5][4]*sm[5];
    sg[5]=MN[0][5]*sm[0]+MN[1][5]*sm[1]+MN[2][5]*sm[2]
      +MN[3][5]*sm[3]+MN[4][5]*sm[4]+MN[5][5]*sm[5];
  } // end of UMATRotationMatrix3D::rotateStressesBackward

  // Calcul des contraintes dans le rep�re mat�riau
  // s^m=M:s^g
  // sg : Contraintes dans le rep�re global
  // sm : Contraintes dans le rep�re mat�riau
  void
  UMATRotationMatrix3D::rotateStressesForward(const UMATReal *const sg,
				              UMATReal *const sm){
    sm[0]=MN[0][0]*sg[0]+MN[0][1]*sg[1]+MN[0][2]*sg[2]
      +2*(MN[0][3]*sg[3]+MN[0][4]*sg[4]+MN[0][5]*sg[5]);
    sm[1]=MN[1][0]*sg[0]+MN[1][1]*sg[1]+MN[1][2]*sg[2]
      +2*(MN[1][3]*sg[3]+MN[1][4]*sg[4]+MN[1][5]*sg[5]);
    sm[2]=MN[2][0]*sg[0]+MN[2][1]*sg[1]+MN[2][2]*sg[2]
      +2*(MN[2][3]*sg[3]+MN[2][4]*sg[4]+MN[2][5]*sg[5]);
    sm[3]=MN[3][0]*sg[0]+MN[3][1]*sg[1]+MN[3][2]*sg[2]
      +MN[3][3]*sg[3]+MN[3][4]*sg[4]+MN[3][5]*sg[5];
    sm[4]=MN[4][0]*sg[0]+MN[4][1]*sg[1]+MN[4][2]*sg[2]
      +MN[4][3]*sg[3]+MN[4][4]*sg[4]+MN[4][5]*sg[5];
    sm[5]=MN[5][0]*sg[0]+MN[5][1]*sg[1]+MN[5][2]*sg[2]
      +MN[5][3]*sg[3]+MN[5][4]*sg[4]+MN[5][5]*sg[5];
  } // end of UMATRotationMatrix3D::rotateStressesForward

  // Calcul des d�formations dans le rep�re global
  // e^g=M^T:e^m
  // em : Contraintes dans le rep�re mat�riau
  // eg : Contraintes dans le rep�re global
  void
  UMATRotationMatrix3D::rotateStrainsBackward(const UMATReal *const em,
					      UMATReal *const eg)
  {
    eg[0]=MN[0][0]*em[0]+MN[1][0]*em[1]+MN[2][0]*em[2]
      +MN[3][0]*em[3]+MN[4][0]*em[4]+MN[5][0]*em[5];
    eg[1]=MN[0][1]*em[0]+MN[1][1]*em[1]+MN[2][1]*em[2]
      +MN[3][1]*em[3]+MN[4][1]*em[4]+MN[5][1]*em[5];
    eg[2]=MN[0][2]*em[0]+MN[1][2]*em[1]+MN[2][2]*em[2]
      +MN[3][2]*em[3]+MN[4][2]*em[4]+MN[5][2]*em[5];
    eg[3]=2*(MN[0][3]*em[0]+MN[1][3]*em[1]+MN[2][3]*em[2])
      +MN[3][3]*em[3]+MN[4][3]*em[4]+MN[5][3]*em[5];
    eg[4]=2*(MN[0][4]*em[0]+MN[1][4]*em[1]+MN[2][4]*em[2])
      +MN[3][4]*em[3]+MN[4][4]*em[4]+MN[5][4]*em[5];
    eg[5]=2*(MN[0][5]*em[0]+MN[1][5]*em[1]+MN[2][5]*em[2])
      +MN[3][5]*em[3]+MN[4][5]*em[4]+MN[5][5]*em[5];
  } // end of UMATRotationMatrix3D::rotateStrainBackward


} // end of namespace umat
