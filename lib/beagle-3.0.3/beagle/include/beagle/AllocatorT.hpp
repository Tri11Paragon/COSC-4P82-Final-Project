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
 *  \file   beagle/AllocatorT.hpp
 *  \brief  Definition of class template AllocatorT.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6.2.1 $
 *  $Date: 2007/05/09 01:51:08 $
 */

#ifndef Beagle_AllocatorT_hpp
#define Beagle_AllocatorT_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/castObjectT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/InternalException.hpp"
#include "beagle/BadCastException.hpp"


namespace Beagle {

// Forward declarations.
template <class T, class BaseType> class AllocatorT;
template <class T, class BaseType> class PointerT;

/*!
 *  \class AllocatorT beagle/Allocator.hpp "beagle/Allocator.hpp"
 *  \brief Templated class defining a T-type allocator.
 *  \param T Type of object allocated.
 *  \param BaseType Base type from which the actual allocator type is derived.
 *  \ingroup OOF
 *  \ingroup Allocs
 */
template <class T, class BaseType>
class AllocatorT : public BaseType {

public:

  //! AllocatorT allocator type.
  typedef AllocatorT<AllocatorT<T,BaseType>,typename BaseType::Alloc>
          Alloc;
  //! AllocatorT handle type.
  typedef PointerT<AllocatorT<T,BaseType>,typename BaseType::Handle>
          Handle;
  //! AllocatorT bag type.
  typedef ContainerT<AllocatorT<T,BaseType>,typename BaseType::Bag>
          Bag;

           AllocatorT() { }
  virtual ~AllocatorT() { }

  virtual Object* allocate() const;
  virtual Object* clone(const Object& inOrigObj) const;
  virtual void    copy(Object& outCopyObj, const Object& inOrigObj) const;

};

}


/*!
 *  \brief  Allocate a new T-type object and the heap.
 *  \return Pointer to the allocated T-type object.
 */
template <class T, class BaseType>
Beagle::Object* Beagle::AllocatorT<T,BaseType>::allocate() const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  T* lT = new T;
  if(!lT) throw Beagle_InternalExceptionM("Out of memory!");
  return lT;
#else // BEAGLE_NDEBUG
  return new T;
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* AllocatorT<T,BaseType>::allocate() const");
}


/*!
 *  \brief  Allocate a new T-type object on the heap that is a clone an existing object.
 *  \param  inOrigObj Constant reference to the original T-type object to clone.
 *  \return Pointer to the allocated T-type object.
 *  \throw  BasCastException If the type of the parameter is not as specified.
 */
template <class T, class BaseType>
Beagle::Object* Beagle::AllocatorT<T,BaseType>::clone(const Beagle::Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigObj);
  T* lT = new T(lOrigT);
  if(!lT) throw Beagle_InternalExceptionM("Out of memory!");
  return lT;
#else // BEAGLE_NDEBUG
  const T& lOrigT = castObjectT<const T&>(inOrigObj);
  return new T(lOrigT);
#endif // BEAGLE_NDEBUG
  Beagle_StackTraceEndM("Object* AllocatorT<T,BaseType>::clone(const Object& inOrigObj) const");
}


/*!
 *  \brief Copy a T-type object into another.
 *  \param outCopyObj Constant reference to the original object to copy.
 *  \param inOrigObj  Reference to the object that is a copy of the original.
 *  \throw BasCastException If the types of the parameters are not as specified.
 */
template <class T, class BaseType>
void Beagle::AllocatorT<T,BaseType>::copy(Beagle::Object& outCopyObj,
                                          const Beagle::Object& inOrigObj) const
{
  Beagle_StackTraceBeginM();
  T& lCopyT = castObjectT<T&>(outCopyObj);
  const T& lOrigT = castObjectT<const T&>(inOrigObj);
  lCopyT = lOrigT;
  Beagle_StackTraceEndM("void AllocatorT<T,BaseType>::copy(Object& outCopyObj, const Object& inOrigObj) const");
}


#endif // Beagle_AllocatorT_hpp

