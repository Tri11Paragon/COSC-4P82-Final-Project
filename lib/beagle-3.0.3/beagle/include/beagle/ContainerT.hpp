/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2007 by Christian Gagne and Marc Parizeau
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *  Contact:
 *  Laboratoire de Vision et Systemes Numeriques
 *  Departement de genie electrique et de genie informatique
 *  Universite Laval, Quebec, Canada, G1K 7P4
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/ContainerT.hpp
 *  \brief  Definition of template ContainerT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.2 $
 *  $Date: 2007/09/10 18:24:11 $
 */

#ifndef Beagle_ContainerT_hpp
#define Beagle_ContainerT_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/ContainerAllocatorT.hpp"
#include "beagle/RunTimeException.hpp"


namespace Beagle {

template <class T, class BaseType, class ContainerTypeAllocType> class ContainerAllocatorT;

/*!
 *  \class ContainerT beagle/ContainerT.hpp "beagle/ContainerT.hpp"
 *  \brief Templated class defining a T-type uniform bag, inheriting from BaseType uniform bag.
 *  \param T Type of the values contained.
 *  \param BaseType Base type from which the actual uniform bag type is derived.
 *  \ingroup OOF
 *  \ingroup DataStruct
 *  \warning Containers can have a type allocator for managing new elements.
 *    If this allocator is NULL, then the container is heterogeneous. It can contain any type
 *    of object. Otherwise, the container is said to be typed, and resizing will allocate
 *    new objects of the specified type. Note however, that the uniformity of the container is not
 *    insured for mutating methods inherited from std::vector (e.g. push_back and insert). The user
 *    should restrain to method resize to modify (enlarge or reduce) the size of a typed container.
 *    The user should also never insert of modify handles of the typed container unless he really
 *    knows what he's doing.
 */
template <class T, class BaseType>
class ContainerT : public BaseType {

public:

  //! ContainerT allocator type.
  typedef ContainerAllocatorT<ContainerT<T,BaseType>,typename BaseType::Alloc,typename T::Alloc>
          Alloc;
  //! ContainerT handle type.
  typedef PointerT<ContainerT<T,BaseType>,typename BaseType::Handle>
          Handle;
  //! ContainerT bag type.
  typedef ContainerT<ContainerT<T,BaseType>,typename BaseType::Bag>
          Bag;

  explicit ContainerT(typename T::Alloc::Handle inTypeAlloc=NULL,
                      typename BaseType::size_type inN=0);
  explicit ContainerT(typename T::Alloc::Handle inTypeAlloc,
                      typename BaseType::size_type inN,
                      const T& inModel);
  virtual ~ContainerT() { }

  /*!
   *  \brief  Access to a constant handle to the Nth element of the ContainerT.
   *  \param  inN Indice of the constant handle (T::Handle) to get.
   *  \return Constant handle (T::Handle) at the indice N.
   */
  inline const typename T::Handle& operator[](typename BaseType::size_type inN) const
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,BaseType::size()-1);
    return castHandleT<T>(std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::operator[](inN));
    Beagle_StackTraceEndM("const T::Handle& ContainerT<T,BaseType>::operator[](BaseType::size_type inN) const");
  }

  /*!
   *  \brief  Acces to an handle the Nth element of the ContainerT.
   *  \param  inN Indice of the handle (T::Handle) to get.
   *  \return Pointer (T::Handle) at the indice N.
   */
  inline typename T::Handle& operator[](typename BaseType::size_type inN)
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,BaseType::size()-1);
    return castHandleT<T>(std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::operator[](inN));
    Beagle_StackTraceEndM("T::Handle& ContainerT<T,BaseType>::operator[](BaseType::size_type inN)");
  }

  /*!
   *  \brief  Access to a handle to the Nth element of the ContainerT.
   *  \param  inN Index of the handle (T::Handle) to get.
   *  \return Pointer (T::Handle) at the index N.
   */
  inline typename T::Handle& at(typename BaseType::size_type inN)
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,BaseType::size()-1);
    return castHandleT<T>(std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::at(inN));
    Beagle_StackTraceEndM("T::Handle& ContainerT<T,BaseType>::at(BaseType::size_type inN)");
  }

  /*!
   *  \brief  Access to a handle to the Nth element of the ContainerT.
   *  \param  inN Index of the handle (T::Handle) to get.
   *  \return const Pointer (T::Handle) at the index N.
   */
  inline const typename T::Handle& at(typename BaseType::size_type inN) const
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,BaseType::size()-1);
    return castHandleT<T>(std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::at(inN));
    Beagle_StackTraceEndM("const T::Handle& ContainerT<T,BaseType>::at(BaseType::size_type inN) const");
  }

  /*!
   *  \brief  Access to a handle to the last element of the ContainerT.
   *  \return Pointer (T::Handle) at the index N.
   */
  inline typename T::Handle& back()
  {
    Beagle_StackTraceBeginM();
    if(BaseType::empty()) {
      throw Beagle_RunTimeExceptionM(string("Container named \"")+BaseType::getName()+
        string("\" is empty. You've called the back() method but there is nothing to return."));
    }
    return castHandleT<T>(std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::back());
    Beagle_StackTraceEndM("T::Handle& ContainerT<T,BaseType>::back()");
  }

  /*!
   *  \brief  Access to a handle to the last element of the ContainerT.
   *  \return Pointer (T::Handle) at the index N.
   */
  inline const typename T::Handle& back() const
  {
    Beagle_StackTraceBeginM();
    if(BaseType::empty()) {
      throw Beagle_RunTimeExceptionM(string("Container named \"")+BaseType::getName()+
        string("\" is empty. You've called the back() method but there is nothing to return."));
    }
    return castHandleT<T>(std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::back());
    Beagle_StackTraceEndM("const T::Handle& ContainerT<T,BaseType>::back() const");
  }

  /*!
   *  \brief  Get the allocator of the instance contained in the container.
   *  \return An handle to the type allocator.
   */
  inline const typename T::Alloc::Handle getTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return castHandleT<typename T::Alloc>(Beagle::Container::getTypeAlloc());
    Beagle_StackTraceEndM("const T::Alloc::Handle ContainerT<T,BaseType>::getTypeAlloc() const");
  }

  /*!
   *  \brief Get the allocator of the instance contained in the container.
   *  \return An handle to the type allocator.
   */
  inline typename T::Alloc::Handle getTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return castHandleT<typename T::Alloc>(Container::getTypeAlloc());
    Beagle_StackTraceEndM("T::Alloc::Handle ContainerT<T,BaseType>::getTypeAlloc()");
  }

  /*!
   *  \brief Resize the container by allocating object if necessary
   *  \param inN New size of the container.
   */
  inline void resize(typename BaseType::size_type inN)
  {
    Beagle_StackTraceBeginM();
    Container::resize(inN);
    Beagle_StackTraceEndM("void ContainerT<T,BaseType>::resize(typename BaseType::size_type inN)");
  }

  /*!
   *  \brief Resize the container by allocating object if necessary
   *  \param inN New size of the container.
   *  \param inModel Model of object to use to allocate the new value of the container.
   */
  inline void resize(typename BaseType::size_type inN, const T& inModel)
  {
    Beagle_StackTraceBeginM();
    Container::resize(inN,inModel);
    Beagle_StackTraceEndM("void ContainerT<T,BaseType>::resize(typename BaseType::size_type inN, const T& inModel)");
  }

};

}


/*!
 *  \brief Construct a container of the size given.
 *  \param inTypeAlloc Allocator use to fill the container.
 *  \param inN Initial size of the container.
 */
template <class T, class BaseType>
Beagle::ContainerT<T,BaseType>::ContainerT(typename T::Alloc::Handle inTypeAlloc,
                                           typename BaseType::size_type inN) :
  BaseType(inTypeAlloc, inN)
{ }


/*!
 *  \brief Construct a container of the size given.
 *  \param inTypeAlloc Allocator use to fill the container.
 *  \param inN Initial size of the container.
 *  \param inModel Model of object to use to allocate the new value of the container.
 */
template <class T, class BaseType>
Beagle::ContainerT<T,BaseType>::ContainerT(typename T::Alloc::Handle inTypeAlloc,
                                           typename BaseType::size_type inN,
                                           const T& inModel) :
  BaseType(inTypeAlloc, inN, inModel)
{ }


#endif // Beagle_ContainerT_hpp

