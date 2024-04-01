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
 *  \file   beagle/HallOfFame.hpp
 *  \brief  Definition of the class HallOfFame.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.2 $
 *  $Date: 2007/09/10 18:24:11 $
 */

#ifndef Beagle_HallOfFame_hpp
#define Beagle_HallOfFame_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Individual.hpp"
#include "beagle/IndividualAlloc.hpp"
#include "beagle/Logger.hpp"

namespace Beagle {

// Forward declaration.
class Deme;


/*!
 *  \class HallOfFame beagle/HallOfFame.hpp "beagle/HallOfFame.hpp"
 *  \brief A hall-of-fame is a population of the best-of-run individuals.
 *  \warning The individuals in the hall-of-fame are not used at all in the evolution process.
 *  \ingroup ECF
 *  \ingroup Pop
 */
class HallOfFame : public Object {

public:

  /*!
   *  \struct Member beagle/HallOfFame.hpp "beagle/HallOfFame.hpp"
   *  \brief A member of the hall-of-fame, that is a best-of-run individual.
   *  \ingroup ECF
   *  \ingroup Pop
   */
  struct Member {
    Individual::Handle mIndividual;    //!< Individual member of the hall-of-fame.
    unsigned int       mGeneration;    //!< Generation of introduction in the hall-of-fame.
    unsigned int       mDemeIndex;     //!< Deme index to which the member belong.

    Member(Individual::Handle inIndividual=NULL,
           unsigned int inGeneration=0,
           unsigned int inDemeIndex=0);
    bool operator==(const HallOfFame::Member& inRightMember) const;
    bool operator<(const HallOfFame::Member& inRightMember) const;
    bool operator>(const HallOfFame::Member& inRightMember) const; 
  };

  //! HallOfFame allocator type.
  typedef AllocatorT<HallOfFame,Object::Alloc>
          Alloc;
  //! HallOfFame handle type.
  typedef PointerT<HallOfFame,Object::Handle>
          Handle;
  //! HallOfFame bag type.
  typedef ContainerT<HallOfFame,Object::Bag>
          Bag;

  explicit HallOfFame(Individual::Alloc::Handle inIndivAlloc=NULL);
  virtual ~HallOfFame() { }

          void log(Logger::LogLevel inLogLevel, Context& ioContext) const;
  virtual void readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
          void resize(unsigned int inNewSize);
          void resize(unsigned int inNewSize, const HallOfFame::Member& inModel);
          void sort();
  virtual bool updateWithDeme(unsigned int inSizeHOF, const Deme& inDeme, Context& ioContext);
  virtual bool updateWithIndividual(unsigned int inSizeHOF, const Individual& inIndividual,
                                    Context& ioContext);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

  virtual void read(PACC::XML::ConstIterator inIter);           // Undefined for HallOfFame!

  /*!
   *  \brief  Access to a constant reference to the Nth member of the hall-of-fame.
   *  \param  inN Indice of the member to get.
   *  \return Constant reference to the member at the indice N.
   */
  inline const HallOfFame::Member& operator[](unsigned int inN) const
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,mMembers.size()-1);
    return mMembers[inN];
    Beagle_StackTraceEndM("const HallOfFame::Member& HallOfFame::operator[](unsigned int inN) const");
  }

  /*!
   *  \brief  Access to a reference to the Nth member of the hall-of-fame.
   *  \param  inN Indice of the member to get.
   *  \return Reference to the member at the indice N.
   */
  inline HallOfFame::Member& operator[](unsigned int inN)
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,mMembers.size()-1);
    return mMembers[inN];
    Beagle_StackTraceEndM("HallOfFame::Member& HallOfFame::operator[](unsigned int inN)");
  }

  /*!
   *  \brief  Get the allocator of the individuals contained in the hall-of-fame.
   *  \return An handle to the individuals allocator.
   */
  inline const Individual::Alloc::Handle getIndivAlloc() const
  {
    Beagle_StackTraceBeginM();
    return mIndivAllocator;
    Beagle_StackTraceEndM("const Individual::Alloc::Handle HallOfFame::getIndivAlloc() const");
  }

  /*!
   *  \brief  Get the allocator of the individuals contained in the hall-of-fame.
   *  \return An handle to the individuals allocator.
   */
  inline Individual::Alloc::Handle getIndivAlloc()
  {
    Beagle_StackTraceBeginM();
    return mIndivAllocator;
    Beagle_StackTraceEndM("Individual::Alloc::Handle HallOfFame::getIndivAlloc()");
  }

  /*!
   *  \brief  Set the allocator of the individuals contained in the hall-of-fame.
   *  \param inIndivAlloc Handle to the individuals allocator.
   */
  inline void setIndivAlloc(Individual::Alloc::Handle inIndivAlloc)
  {
    Beagle_StackTraceBeginM();
    mIndivAllocator = inIndivAlloc;
    Beagle_StackTraceEndM("void HallOfFame::setIndivAlloc(Individual::Alloc::Handle inIndivAlloc)");
  }

  /*!
   *  \return Number of members in the hall-of-fame.
   */
  inline unsigned int size() const
  {
    Beagle_StackTraceBeginM();
    return mMembers.size();
    Beagle_StackTraceEndM("unsigned int HallOfFame::size() const");
  }

protected:

  Individual::Alloc::Handle  mIndivAllocator;  //!< Individuals allocator.
  std::vector< HallOfFame::Member,BEAGLE_STLALLOCATOR<HallOfFame::Member> > 
                             mMembers;         //!< Vector of members of the hall-of-fame.

};

}


#endif // Beagle_HallOfFame_hpp

