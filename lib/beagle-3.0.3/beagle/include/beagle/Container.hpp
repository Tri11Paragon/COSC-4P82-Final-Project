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
 *  \file   beagle/Container.hpp
 *  \brief  Container class for implementing the Bag data type.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.2 $
 *  $Date: 2007/09/10 18:24:11 $
 */

/*!
 *  \defgroup DataStruct Data Structures
 *  \ingroup OOF
 *  \brief Object oriented data structures, part of the Object Oriented Foundations.
 *
 *  It is natural to design data structures that take advantage of all the mechanisms exposed in the 
 *  current section. Working with the new features of the C++, it is essential to exploit the
 *  \e Standard \e Template \e Library (STL) which provides common, well known and portable data 
 *  structures and algorithms. An Open BEAGLE specific, but STL compliant, data structures is 
 *  extensively used: the container.
 *
 *  The Open BEAGLE container is a random access table of heterogeneous objects. It is implemented as 
 *  a dynamic array of specified smart pointers. Any objects that is a specialization of the contained
 *  type could be put in a container. The constraints are the same as those of the smart pointers: the
 *  objects must be allocated on the heap (via the \c new operator) and these objects must not be
 *  unallocated explicitly (i.e. \c delete of the objects are forbidden). Our container is 
 *  implemented as a Beagle object that is also a STL vector of object handles.
 *  \code
 *  class Container : public Object, public std::vector<Object::Handle> { ... };
 *  \endcode
 *  Built this way, our container could be manipulated by the STL generic algorithms. It is also possible
 *  to make container of containers, since a container is an object that can be smart pointed.
 *
 *  A drawback of this data structure is that every contained objects must 
 *  have been allocated (on the heap) and thereafter inserted into the container. Knowing so, we add 
 *  an allocator handle member attribute to the class Beagle::Container. This member automatically 
 *  allocates instance of objects when resizing. If no allocator are given to a container (the member
 *  value of the allocator handle is null), resizing will simply resizes the vector of smart pointer 
 *  and the newly added pointers are initialized to the null value. It is then possible to dynamically
 *  type containers by giving them allocator instances of the desired type.
 *
 *  As with the smart pointers and allocators, every Open BEAGLE type has a nested type that define a
 *  container of the specified type, the nested type named \c Bag. For example, the type that is a 
 *  container of floating-point values, Beagle::Float, is the type Beagle::Float::Bag. The nested type 
 *  is declared in classes as usual for the nested types \c Handle and \c Alloc, using a \c typedef 
 *  to a template, in this case to template class Beagle::ContainerT.
 *  \code 
 *  class MyClass : public SuperClass {
 *  public:
 *    typedef AllocatorT<MyClass,SuperClass::Alloc> Alloc;
 *    typedef PointerT<MyClass,SuperClass::Handle> Handle;
 *    typedef ContainerT<MyClass,SuperClass::Bag> Bag;
 *  };
 *  \endcode
 *  The use of dynamically typed containers raises a problem when it is necessary to allocate 
 *  containers of objects not from the basic usual associated type. Supposing a container of genotype,
 *  namely a Beagle::Genotype::Bag. It may be possible for the container to contain a different type 
 *  of genotype, for example genotypes of the type \c SpecializedGenotype, which is derived from 
 *  the type Beagle::Genotype. Then, the type allocator member of Beagle::Genotype::Bag should be an 
 *  instance of the type \c SpecializedGenotype::Alloc. This can be easily done by giving as 
 *  argument an \c SpecializedGenotype::Alloc to the constructor of Beagle::Genotype::Bag. But 
 *  supposing now that a Beagle::Genotype::Bag::Bag is created and that it should contain these 
 *  containers dynamically typed, it is necessary to defined a new allocator of Beagle::Genotype::Bag 
 *  that instantiate the allocator of Beagle::Genotype as instances of allocator of
 *  \c SpecializedGenotype. This can be very tricky to implement for a novice user.
 *
 *  This issue is solved by designing a new template of allocators specific to the Open BEAGLE
 *  containers. This specialized allocator has an attribute that is an object allocator handle 
 *  compatible with the type contained in the associated bag. This allocator is implemented in the
 *  basic class Beagle::ContainerAllocator.
 *  \code
 *  class ContainerAllocator : public Object::Alloc {
 *  public:
 *    ContainerAllocator(Object::Alloc::Handle);
 *  protected:
 *    Object::Alloc::Handle mContainerTypeAlloc;
 *  };
 *  \endcode
 *  The type \c Alloc of Beagle::ContainerT is declared as \c typedef of a templated allocator of 
 *  container (Beagle::ContainerAllocatorT). This template takes a third type, the type of the member
 *  allocator type of the container, in addition to the two first usual types.
 *  \code
 *  template <class T, class BaseType>
 *  class ContainerT : public BaseType {
 *  public:
 *    typedef ContainerAllocatorT<ContainerT<T,BaseType>,BaseType,T::Alloc> Alloc;
 *    ...
 *  }; 
 *  \endcode
 *  Doing so, allocators of bags that are dynamically typed can be instantiated. A second template for 
 *  the case of abstract bag type has also been declared, Beagle::AbstractContainerAllocT. This is 
 *  coherent with the discussion about the type Beagle::AbstractAllocT, which is presented in
 *  the section on the allocators module.
 *
 */


#ifndef Beagle_Container_hpp
#define Beagle_Container_hpp

#include <vector>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/AssertException.hpp"

namespace Beagle {

// Forward declarations.
class ContainerAllocator;
template <class T, class BaseType> class ContainerT;
template <class T, class BaseType> class PointerT;

/*!
 *  \class Container beagle/Container.hpp "beagle/Container.hpp"
 *  \brief Basic container data structure, implemented as a vector of smart pointers.
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
class Container : public Object,
                  public std::vector< Object::Handle,BEAGLE_STLALLOCATOR<Object::Handle> > {

public:

  //! Container's allocator type.
  typedef ContainerAllocator
          Alloc;
  //! Container's handle type.
  typedef PointerT<Container,Object::Handle>
          Handle;
  //! Container's bag type.
  typedef ContainerT<Container,Object::Bag>
          Bag;

  explicit Container(Object::Alloc::Handle inTypeAlloc=NULL,
                     unsigned int inN=0);
  explicit Container(Object::Alloc::Handle inTypeAlloc,
                     unsigned int inN,
                     const Object& inModel);
  virtual ~Container() { }

  virtual void copyData(const Container& inOriginalContainer);
  virtual bool isEqual(const Object& inRightObj) const;
  virtual bool isLess(const Object& inRightObj) const;
  virtual void read(PACC::XML::ConstIterator inIter);
          void resize(unsigned int inN);
          void resize(unsigned int inN, const Object& inModel);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

  /*!
   *  \brief  Standard const operator for accessing elements by index.
   *  \param  inN Index of element (first is 0).
   *  \return Smart pointer to element.
   */
  inline const Object::Handle& operator[](unsigned int inN) const
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,size()-1);
    return std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::operator[](inN);
    Beagle_StackTraceEndM("const Object::Handle& Container::operator[](size_type inN) const");
  }

  /*!
   *  \brief Standard  operator for accessing elements by index.
   *  \param inN Index of element (first is 0).
   *  \return Smart pointer to element.
   */
  inline Object::Handle& operator[](unsigned int inN)
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,size()-1);
    return std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::operator[](inN);
    Beagle_StackTraceEndM("Object::Handle& Container::operator[](size_type inN)");
  }

  /*!
   *  \brief Get the container's data type allocator (const version).
   *  \return Smart pointer to type allocator.
   */
  inline const Object::Alloc::Handle getTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return mTypeAlloc;
    Beagle_StackTraceEndM("const Object::Alloc::Handle Container::getTypeAlloc() const");
  }

  /*!
   *  \brief Get the container's data type allocator.
   *  \return Smart pointer to type allocator.
   */
  inline Object::Alloc::Handle getTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return mTypeAlloc;
    Beagle_StackTraceEndM("Object::Alloc::Handle Container::getTypeAlloc()");
  }

  /*!
   *  \brief Set the container's data type allocator.
   *  \param inTypeAlloc Smart pointer to the new type allocator.
   */
  inline void setTypeAlloc(Object::Alloc::Handle inTypeAlloc)
  {
    Beagle_StackTraceBeginM();
    mTypeAlloc = inTypeAlloc;
    Beagle_StackTraceEndM("void Container::setTypeAlloc(Object::Alloc::Handle inTypeAlloc)");
  }

private:
  Object::Alloc::Handle mTypeAlloc;  //!< Smart pointer to allocator for new elements.

};

}

#endif // Beagle_Container_hpp
