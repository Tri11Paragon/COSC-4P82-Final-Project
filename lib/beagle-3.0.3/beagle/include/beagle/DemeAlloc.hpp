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
 *  \file   beagle/DemeAlloc.hpp
 *  \brief  Definition of the class DemeAlloc.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10.2.1 $
 *  $Date: 2007/05/09 01:51:10 $
 */

#ifndef Beagle_DemeAlloc_hpp
#define Beagle_DemeAlloc_hpp

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
#include "beagle/ContainerAllocatorT.hpp"
#include "beagle/Stats.hpp"
#include "beagle/IndividualAlloc.hpp"
#include "beagle/Individual.hpp"
#include "beagle/IndividualBag.hpp"
#include "beagle/Genotype.hpp"
#include "beagle/HallOfFame.hpp"

namespace Beagle {

/*!
 *  \brief Typedef of the type from which class Beagle::DemeAlloc inherits.
 *  \warning This is a workaround for MS Visual C++ .NET, which seems to have difficulty
 *    to fall on its feet with the type Individual::Bag::Alloc.
 *  \related Beagle::DemeAlloc
 *  \ingroup Allocs
 */
typedef Individual::Bag::Alloc
        DemeAllocBaseType;

/*!
 *  \class DemeAlloc beagle/DemeAlloc.hpp "beagle/DemeAlloc.hpp"
 *  \brief Deme specific allocator type.
 *  \ingroup ECF
 *  \ingroup Pop
 *  \ingroup Allocs
 */
class DemeAlloc : public DemeAllocBaseType {

public:

  //! DemeAlloc allocator type.
  typedef AllocatorT<DemeAlloc,DemeAllocBaseType::Alloc>
          Alloc;
  //! DemeAlloc handle type.
  typedef PointerT<DemeAlloc,DemeAllocBaseType::Handle>
          Handle;
  //! DemeAlloc bag type.
  typedef ContainerT<DemeAlloc,DemeAllocBaseType::Bag>
          Bag;

  explicit DemeAlloc(Individual::Alloc::Handle inIndividualAlloc=NULL);
  explicit DemeAlloc(Individual::Alloc::Handle inIndividualAlloc,
                     Stats::Alloc::Handle inStatsAlloc,
                     HallOfFame::Alloc::Handle inHOFAlloc);
  virtual ~DemeAlloc() { }

  virtual Object*    allocate() const;
  virtual Object*    clone(const Object& inOrigObj) const;
  virtual Container* cloneData(const Container& inOrigContainer) const;
  virtual void       copy(Object& outCopyObj, const Object& inOrigObj) const;
  virtual void       copyData(Container& outCopyContainer, const Container& inOrigContainer) const;

  /*!
   *  \brief Get the hall-of-fame allocator of the deme.
   *  \return Hall-of-fame allocator.
   */
  inline const HallOfFame::Alloc::Handle getHallOfFameAlloc() const
  {
    Beagle_StackTraceBeginM();
    return mHOFAlloc;
    Beagle_StackTraceEndM("const HallOfFame::Alloc::Handle DemeAlloc::getHallOfFameAlloc() const");
  }

  /*!
   *  \brief Get the hall-of-fame allocator of the deme.
   *  \return Hall-of-fame allocator.
   */
  inline HallOfFame::Alloc::Handle getHallOfFameAlloc()
  {
    Beagle_StackTraceBeginM();
    return mHOFAlloc;
    Beagle_StackTraceEndM("HallOfFame::Alloc::Handle DemeAlloc::getHallOfFameAlloc()");
  }

  /*!
   *  \brief Set the hall-of-fame allocator of the deme.
   *  \param inHOFAlloc Hall-of-fame allocator.
   */
  inline void setHallOfFameAlloc(HallOfFame::Alloc::Handle inHOFAlloc)
  {
    Beagle_StackTraceBeginM();
    mHOFAlloc = inHOFAlloc;
    Beagle_StackTraceEndM("void DemeAlloc::setHallOfFameAlloc(HallOfFame::Alloc::Handle inHOFAlloc)");
  }

  /*!
   *  \brief Get the statistics allocator of the deme.
   *  \return Statistics allocator.
   */
  inline const Stats::Alloc::Handle getStatsAlloc() const
  {
    Beagle_StackTraceBeginM();
    return mStatsAlloc;
    Beagle_StackTraceEndM("const Stats::Alloc::Handle DemeAlloc::getStatsAlloc() const");
  }

  /*!
   *  \brief Get the statistics allocator of the deme.
   *  \return Statistics allocator.
   */
  inline Stats::Alloc::Handle getStatsAlloc()
  {
    Beagle_StackTraceBeginM();
    return mStatsAlloc;
    Beagle_StackTraceEndM("Stats::Alloc::Handle DemeAlloc::getStatsAlloc()");
  }

  /*!
   *  \brief Set the statistics allocator of the deme.
   *  \param inStatsAlloc Statistics allocator.
   */
  inline void setStatsAlloc(Stats::Alloc::Handle inStatsAlloc)
  {
    Beagle_StackTraceBeginM();
    mStatsAlloc = inStatsAlloc;
    Beagle_StackTraceEndM("void DemeAlloc::setStatsAlloc(Stats::Alloc::Handle inStatsAlloc)");
  }

  /*!
   *  \brief Get the individual allocator.
   *  \return Constant handle to the individual allocator.
   */
  inline const Individual::Alloc::Handle getContainerTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return castHandleT<const Individual::Alloc>(mContainerTypeAlloc);
    Beagle_StackTraceEndM("const Individual::Alloc::Handle DemeAlloc::getContainerTypeAlloc() const");
  }

  /*!
   *  \brief Get the individual allocator.
   *  \return Handle to the individual allocator.
   */
  inline Individual::Alloc::Handle getContainerTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return castHandleT<Individual::Alloc>(mContainerTypeAlloc);
    Beagle_StackTraceEndM("Individual::Alloc::Handle DemeAlloc::getContainerTypeAlloc()");
  }

  /*!
   *  \brief Set the Individual allocator.
   *  \param inIndividualAlloc The new Individual allocator.
   */
  inline void setContainerTypeAlloc(Individual::Alloc::Handle inIndividualAlloc)
  {
    Beagle_StackTraceBeginM();
    mContainerTypeAlloc = inIndividualAlloc;
    Beagle_StackTraceEndM("void DemeAlloc::setContainerTypeAlloc(Individual::Alloc::Handle inIndividualAlloc)");
  }

protected:
  HallOfFame::Alloc::Handle mHOFAlloc;    //!< Hall-of-fame allocator.
  Stats::Alloc::Handle      mStatsAlloc;  //!< Statistics allocator.

};

}

#endif // Beagle_DemeAlloc_hpp
