/*
 * \file   shared_ptr.hxx
 * \brief  this file describes the implementation of shared_ptr.
 * \author Thomas Helfer
 * \date   december 2005
 */

#ifndef _LIB_TFEL_UTILITIES_SMARTPTR_HPP_
#define _LIB_TFEL_UTILITIES_SMARTPTR_HPP_

#include<string>
#include<cassert>

#ifdef TFEL_HAVE_BOOST
#include<boost/shared_ptr.hpp>
#endif /* TFEL_HAVE_BOOST */

#include"TFEL/Config/TFELConfig.hxx"

namespace tfel{
  
  namespace utilities{

#ifdef TFEL_HAVE_BOOST

    using boost::shared_ptr;

#else /* TFEL_HAVE_BOOST */

    /*    ! \class shared_ptr
     *  \brief La classe \c shared_ptr se substitue aux pointeurs
     *  traditionnels auxquels elle ajoute la d�sallocation
     *  automatique de la m�moire.
     *  \author  Helfer Thomas
     *  \date    20 F�vrier 2006
     *  \warning Cette classe ne doit pas �tre utilis�e sur un tableau
     *
     *  La classe \c shared_ptr propose une implantation d'un pointeur
     *  intelligent bas� sur un compteur de r�f�rence. Cette classe
     *  est adapt�e � son utilisation avec les conteneurs de la STL.
     */
    template<typename T>
    struct shared_ptr{

      typedef T type;

      /*! \fn shared_ptr::shared_ptr() 
       *  \brief Constructeur par d�faut
       *  
       *  Par d�faut, les membres \c p et \c i sont initialis�s au
       *  pointeur \c 0
       *
       */
      TFEL_INLINE shared_ptr() throw()
	: p(0), i(0) {}
  
      /*! \fn shared_ptr::shared_ptr(T *src) 
       *  \brief Constructeur � partir d'un pointeur
       *  \warning Cette classe ne doit pas �tre utilis�e sur un tableau
       *
       *  La classe \c shared_ptr se charge de d�sallouer la m�moire sur
       *  laquelle pointe le pointeur en argument.
       *
       *  \latexonly
       *  Une utilisation typique de cette m�thode est~:
       *  \begin{center}
       *  shared_ptr<double> p(new double);
       *  \end{center}
       *  \endlatexonly
       *
       *  \param src : pointeur servant � initialiser la classe 
       */
      TFEL_INLINE explicit shared_ptr(T *src)
	: p(src), i(0) {
	if(p!=0){
	  this->i = new unsigned int(1);
	}
      }

      /*! \fn shared_ptr::shared_ptr(const shared_ptr<T>& src) 
       *  \brief  Constructeur par copie
       *  
       *  \param[in] src \c shared_ptr source utilis� pour initialiser la
       *  classe. 
       */
      TFEL_INLINE shared_ptr(const shared_ptr<T>& src) throw()
	: p(src.p), i(src.i) 
      {
	if(i!=0){
	  ++(*(this->i));
	}
      }

      /*! \fn    shared_ptr::~shared_ptr() 
       *  \brief Destructeur
       *  
       */
      TFEL_INLINE
      ~shared_ptr()  throw()
      {
	this->decreaseCounter();
      }

      /*! \fn      const shared_ptr<T>& operator=(const shared_ptr<T>& src)
       *  \brief  Op�rateur d'affectation
       *  \param[in] src \c shared_ptr source auquel la classe
       *  est affect�.        
       */
      TFEL_INLINE
      shared_ptr<T>&
      operator=(const shared_ptr<T>& src)
	throw()
      {
	// Traitement d'une affectation � soi-m�me
	if(this==&src){
	  return *this;
	}
	if(this->i!=0){
	  this->decreaseCounter();
	}
	this->i=src.i;
	this->p=src.p;
	if(i!=0){
	  *(this->i) += 1;
	}
	return *this;
      }

      /*! \fn     const T& operator*(void) 
       *  \brief  Op�rateur de d�f�rencement (version \c const).
       *  \warning Il est a priori n�cessaire de v�rifier que l'objet r�f�renc� par la classe � bien �t� initialis�.
       *  \return Retourne une r�f�rence constante vers l'objet
       *  contenu par la classe.
       */
      TFEL_INLINE const T& operator*(void) const 
	throw()
      {
	assert(p!=0);
	return *(this->p);
      }

      /*! \fn     const T& operator*(void) 
       *  \brief  Op�rateur de d�f�rencement.
       *  \warning Il est a priori n�cessaire de v�rifier que l'objet r�f�renc� par la classe � bien �t� initialis�.
       *  \return Retourne une r�f�rence vers l'objet contenu par la
       *  classe.
       */
      TFEL_INLINE T& operator*(void) throw()
      {
	assert(p!=0);
	return *(this->p);
      }

      /*! \fn const T* operator->(void) 
       *  \brief  Op�rateur de d'indirection (version constante).
       *  \warning Il est a priori n�cessaire de v�rifier que l'objet r�f�renc� par la classe � bien �t� initialis�.
       *  \return Retourne un pointeur vers l'objet contenu par la
       *  classe. L'objet point� ne peut �tre modifi�.
       */
      TFEL_INLINE
      const T*
      operator->(void) const throw()
      {
	assert(p!=0);
	return this->p;
      }
      
      /*! 
       *  \brief  Op�rateur de d'indirection.
       *  \warning Il est a priori n�cessaire de v�rifier que l'objet r�f�renc� par la classe � bien �t� initialis�.
       *  \return Retourne un pointeur vers l'objet contenu par la
       *  classe. L'objet point� ne peut �tre modifi�.
       */
      TFEL_INLINE
      T* operator->(void) throw()
      {
	assert(p!=0);
	return this->p;
      }
      
      /*! 
       *  \brief   Permet de savoir si la classe pointe sur un objet valide
       *  \return \c true si la classe pointe sur un objet valide, \c false sinon.
       */
      TFEL_INLINE
      bool
      isValid(void) const throw()
      {
	return !(this->p==0);
      }

      /*! \fn      unsigned int count(void)
       *  \brief   Permet de conna�tre le nombre de r�f�r�nces sur
       *           l'objet r�f�renc� par la classe.
       *  \warning Cette m�thode n'est fournie que pour le d�bogage~:
       *           elle est d�sactiv�e si le symbole NDEBUG est d�fini.  
       *  \return  Le nombre de r�f�rences
       */
      TFEL_INLINE
      unsigned int
      count(void) const throw()
      {
	if(i!=0){
	  return *(this->i);
	}
	return 0;
      }

      /*! \fn      bool unique(void)
       *  \brief   permet de savoir si un seul objet est r�f�renc�
       *  \return  true ou false suivant le cas
       */
      TFEL_INLINE
      bool
      unique(void) const throw()
      {
	if(i!=0){
	  return (*(this->i)==1);
	}
	return false;
      }

      /*! \fn T* getPtr(void)
       */
      TFEL_INLINE T * get(void){
	return this->p;
      }
      
      /*! \fn T* getPtr(void) const
       */
      TFEL_INLINE
      const T *
      get(void) const{
	return this->p;
      }

    protected:

      T *p;
      
    private:

      mutable unsigned int *i; 
     
      /*! \fn shared_ptr::decreaseCounter() 
       *  \brief   m�thode charg�e de d�cr�menter le compteur de r�f�rence
       *  
       *  Cette m�thode est charg�e de d�cr�menter le compteur de
       *  r�f�rence i et de d�sallouer la m�moire si celui-ci est nul.
       */
      TFEL_INLINE void decreaseCounter(void) throw() {
	if(this->i!=0){
	  if((--*(this->i))==0){
	    delete this->i;
	    delete this->p;
	    this->i=0;
	    this->p=0;
	  }
	}
      }

    };

#endif /* TFEL_HAVE_BOOST */

    /*!
     * A small metafunction which returns the shared_ptr class
     * associated with the given type
     * \param T : type to be transformed
     */
    template<typename T>
    struct PtrType
    {
      //! the result
      typedef shared_ptr<T> type;
    }; // end of struct PtrType

  } // end of namespace utilities

} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_SMARTPTR_HPP_ */
