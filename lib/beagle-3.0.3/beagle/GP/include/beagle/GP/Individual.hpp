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
 *  \file   beagle/GP/Individual.hpp
 *  \brief  Definition of the class GP::Individual.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10.2.2 $
 *  $Date: 2007/09/10 18:24:11 $
 */

#ifndef Beagle_GP_Individual_hpp
#define Beagle_GP_Individual_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Individual.hpp"
#include "beagle/IndividualAllocT.hpp"
#include "beagle/GP/Tree.hpp"


namespace Beagle {
namespace GP {

/*!
 *  \class Individual beagle/GP/Individual.hpp "beagle/GP/Individual.hpp"
 *  \brief An individual inherits from a uniform bag of genotypes and has a fitness member.
 *  \ingroup GPF
 *  \ingroup GPPop
 */
class Individual : public Beagle::Individual {

public:

  //! GP::Individual allocator type.
  typedef IndividualAllocT<GP::Individual,Beagle::Individual::Alloc,GP::Tree::Alloc>
          Alloc;
  //! GP::Individual handle type.
  typedef PointerT<GP::Individual,Beagle::Individual::Handle>
          Handle;
  //! GP::Individual bag type.
  typedef ContainerT<GP::Individual,Beagle::Individual::Bag>
          Bag;

           Individual();
  explicit Individual(GP::Tree::Alloc::Handle inGenotypeAlloc);
  explicit Individual(GP::Tree::Alloc::Handle inGenotypeAlloc,
                      Fitness::Alloc::Handle inFitnessAlloc,
                      size_type inN=0);
  virtual ~Individual() { }

  unsigned int chooseRandomTree(Beagle::Context& ioContext) const;
  unsigned int chooseRandomNode(unsigned int inTree, Beagle::Context& ioContext) const;
  unsigned int chooseRandomNodeWithArgs(unsigned int inTree, Beagle::Context& ioContext) const;
  unsigned int chooseRandomNodeWithoutArgs(unsigned int inTree, Beagle::Context& ioContext) const;
  void         run(GP::Datum& outResult, GP::Context& ioContext);
  bool         validate(GP::Context& ioContext);

  /*!
   *  \brief Acces to a constant handle the nth tree of the GP::Individual.
   *  \param inN Indice of the GP::Tree to get
   *  \return Constant pointer (GP::Tree::Handle) at the indice N.
   */
  inline const GP::Tree::Handle& operator[](unsigned int inN) const
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,size()-1);
    return castHandleT<GP::Tree>(std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::operator[](inN));
    Beagle_StackTraceEndM("const GP::Tree::Handle& GP::Individual::operator[](unsigned int inN) const");
  }

  /*!
   *  \brief Acces to an handle the nth tree of the GP::Individual.
   *  \param inN Indice of the GP::Tree to get.
   *  \return Pointer (GP::Tree::Handle) at the indice N.
   */
  inline GP::Tree::Handle& operator[](unsigned int inN)
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,size()-1);
    return castHandleT<GP::Tree>(std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::operator[](inN));
    Beagle_StackTraceEndM("GP::Tree::Handle& GP::Individual::operator[](unsigned int inN)");
  }

  /*!
   *  \return Maximum tree depth of the individual.
   */
  inline unsigned int getMaxTreeDepth() const
  {
    Beagle_StackTraceBeginM();
    unsigned int lDepth = 0;
    for(unsigned int i=0; i<size(); ++i) lDepth = maxOf(lDepth, (*this)[i]->getTreeDepth());
    return lDepth;
    Beagle_StackTraceEndM("unsigned int GP::Individual::getMaxTreeDepth() const");
  }

  /*!
   *  \return Total number of nodes in the trees.
   */
  inline unsigned int getTotalNodes() const
  {
    Beagle_StackTraceBeginM();
    unsigned int lNumberNodes = 0;
    for(unsigned int i=0; i<size(); ++i) lNumberNodes += (*this)[i]->size();
    return lNumberNodes;
    Beagle_StackTraceEndM("unsigned int GP::Individual::getTotalNodes() const");
  }

  /*!
   *  \brief Get the tree data type allocator.
   *  \return An handle to the tree type allocator.
   */
  inline const GP::Tree::Alloc::Handle getTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return castHandleT<GP::Tree::Alloc>(Beagle::Individual::getTypeAlloc());
    Beagle_StackTraceEndM("const GP::Tree::Alloc::Handle GP::Individual::getTypeAlloc() const");
  }

  /*!
   *  \brief Get the tree data type allocator.
   *  \return An handle to the tree type allocator.
   */
  inline GP::Tree::Alloc::Handle getTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return castHandleT<GP::Tree::Alloc>(Beagle::Individual::getTypeAlloc());
    Beagle_StackTraceEndM("GP::Tree::Alloc::Handle GP::Individual::getTypeAlloc()");
  }

  /*!
   *  \brief Set the tree data type allocator.
   *  \param inTypeAlloc An handle to the new tree type allocator.
   */
  inline void setTypeAlloc(GP::Tree::Alloc::Handle inTypeAlloc)
  {
    Beagle_StackTraceBeginM();
    Beagle::Individual::setTypeAlloc(inTypeAlloc);
    Beagle_StackTraceEndM("void GP::Individual::setTypeAlloc(GP::Tree::Alloc::Handle inTypeAlloc)");
  }

};

}
}

#endif // Beagle_GP_Individual_hpp
