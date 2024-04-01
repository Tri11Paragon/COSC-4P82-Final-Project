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
 *  \file   beagle/VivariumAlloc.hpp
 *  \brief  Definition of the class VivariumAlloc.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

#ifndef Beagle_VivariumAlloc_hpp
#define Beagle_VivariumAlloc_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Genotype.hpp"
#include "beagle/Individual.hpp"
#include "beagle/Deme.hpp"
#include "beagle/DemeAlloc.hpp"
#include "beagle/Stats.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/ContainerAllocatorT.hpp"
#include "beagle/HallOfFame.hpp"


namespace Beagle {

/*!
 *  \brief Typedef of the type from which class Beagle::VivariumAlloc inherits.
 *  \warning This is a workaround for MS Visual C++ .NET, which seems to have difficulty
 *    to fall on its feet with the type Vivarium::Bag::Alloc.
 *  \related Beagle::VivariumAlloc
 *  \ingroup Allocs
 */
typedef Deme::Bag::Alloc 
        VivariumAllocBaseType;

/*!
 *  \class VivariumAlloc beagle/VivariumAlloc.hpp "beagle/VivariumAlloc.hpp"
 *  \brief Vivarium specific allocator type.
 *  \ingroup ECF
 *  \ingroup Pop
 *  \ingroup Allocs
 */
class VivariumAlloc : public VivariumAllocBaseType {

public:

  //! VivariumAlloc allocator type.
  typedef AllocatorT<VivariumAlloc,VivariumAllocBaseType::Alloc>
          Alloc;
  //! VivariumAlloc handle type.
  typedef PointerT<VivariumAlloc,VivariumAllocBaseType::Handle>
          Handle;
  //! VivariumAlloc bag type.
  typedef ContainerT<VivariumAlloc,VivariumAllocBaseType::Bag>
          Bag;

  explicit VivariumAlloc(Deme::Alloc::Handle inDemeAlloc=NULL);
  explicit VivariumAlloc(Deme::Alloc::Handle inDemeAlloc,
                         Stats::Alloc::Handle inStatsAlloc,
                         HallOfFame::Alloc::Handle inHOFAlloc);
  virtual ~VivariumAlloc() { }

  virtual Object*    allocate() const;
  virtual Object*    clone(const Object& inOrigObj) const;
  virtual Container* cloneData(const Container& inOrigContainer) const;
  virtual void       copy(Object& outCopyObj, const Object& inOrigObj) const;
  virtual void       copyData(Container& outCopyContainer, const Container& inOrigContainer) const;

  /*!
   *  \brief Get the hall-of-fame allocator of the vivarium.
   *  \return Hall-of-fame allocator.
   */
  inline const HallOfFame::Alloc::Handle getHallOfFameAlloc() const
  {
    Beagle_StackTraceBeginM();
    return mHOFAlloc;
    Beagle_StackTraceEndM("const HallOfFame::Alloc::Handle VivariumAlloc::getHallOfFameAlloc() const");
  }

  /*!
   *  \brief Get the hall-of-fame allocator of the vivarium.
   *  \return Hall-of-fame allocator.
   */
  inline HallOfFame::Alloc::Handle getHallOfFameAlloc()
  {
    Beagle_StackTraceBeginM();
    return mHOFAlloc;
    Beagle_StackTraceEndM("HallOfFame::Alloc::Handle VivariumAlloc::getHallOfFameAlloc()");
  }

  /*!
   *  \brief Set the hall-of-fame allocator of the vivarium.
   *  \param inHOFAlloc Hall-of-fame allocator.
   */
  inline void setHallOfFameAlloc(HallOfFame::Alloc::Handle inHOFAlloc)
  {
    Beagle_StackTraceBeginM();
    mHOFAlloc = inHOFAlloc;
    Beagle_StackTraceEndM("void VivariumAlloc::setHallOfFameAlloc(HallOfFame::Alloc::Handle inHOFAlloc)");
  }

  /*!
   *  \brief Get the statistics allocator of the vivarium.
   *  \return Statistics allocator.
   */
  inline const Stats::Alloc::Handle getStatsAlloc() const
  {
    Beagle_StackTraceBeginM();
    return mStatsAlloc;
    Beagle_StackTraceEndM("const Stats::Alloc::Handle VivariumAlloc::getStatsAlloc() const");
  }

  /*!
   *  \brief Get the statistics allocator of the vivarium.
   *  \return Statistics allocator.
   */
  inline Stats::Alloc::Handle getStatsAlloc()
  {
    Beagle_StackTraceBeginM();
    return mStatsAlloc;
    Beagle_StackTraceEndM("Stats::Alloc::Handle VivariumAlloc::getStatsAlloc()");
  }

  /*!
   *  \brief Set the statistics allocator of the vivarium.
   *  \param inStatsAlloc Statistics allocator.
   */
  inline void setStatsAlloc(Stats::Alloc::Handle inStatsAlloc)
  {
    Beagle_StackTraceBeginM();
    mStatsAlloc = inStatsAlloc;
    Beagle_StackTraceEndM("void VivariumAlloc::setStatsAlloc(Stats::Alloc::Handle inStatsAlloc)");
  }

  /*!
   *  \brief Get the deme allocator.
   *  \return Constant handle to the deme allocator.
   */
  inline const Deme::Alloc::Handle getContainerTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return castHandleT<const Deme::Alloc>(mContainerTypeAlloc);
    Beagle_StackTraceEndM("const Deme::Alloc::Handle VivariumAlloc::getContainerTypeAlloc() const");
  }

  /*!
   *  \brief Get the deme allocator.
   *  \return Handle to the deme allocator.
   */
  inline Deme::Alloc::Handle getContainerTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return castHandleT<Deme::Alloc>(mContainerTypeAlloc);
    Beagle_StackTraceEndM("Deme::Alloc::Handle VivariumAlloc::getContainerTypeAlloc()");
  }

  /*!
   *  \brief Set the deme allocator.
   *  \param inDemeAlloc The new deme allocator.
   */
  inline void setContainerTypeAlloc(Deme::Alloc::Handle inDemeAlloc)
  {
    Beagle_StackTraceBeginM();
    mContainerTypeAlloc = inDemeAlloc;
    Beagle_StackTraceEndM("void VivariumAlloc::setContainerTypeAlloc(Deme::Alloc::Handle inDemeAlloc)");
  }

protected:

  HallOfFame::Alloc::Handle mHOFAlloc;    //!< Hall-of-fame allocator.
  Stats::Alloc::Handle      mStatsAlloc;  //!< Statistics allocator.

};

}

#endif // Beagle_VivariumAlloc_hpp
