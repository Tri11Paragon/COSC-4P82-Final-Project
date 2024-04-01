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
 *  \file   beagle/Deme.hpp
 *  \brief  Definition of the class Deme.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12.2.1 $
 *  $Date: 2007/05/09 01:51:10 $
 */

#ifndef Beagle_Deme_hpp
#define Beagle_Deme_hpp

#include "XML.hpp"

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
#include "beagle/Genotype.hpp"
#include "beagle/Individual.hpp"
#include "beagle/IndividualAlloc.hpp"
#include "beagle/Stats.hpp"
#include "beagle/HallOfFame.hpp"
#include "beagle/DemeAlloc.hpp"
#include "beagle/Logger.hpp"

namespace Beagle {

/*!
 *  \class Deme beagle/Deme.hpp "beagle/Deme.hpp"
 *  \brief A deme is a population of individuals, a "hall of fame", and some statistics.
 *  \ingroup ECF
 *  \ingroup Pop
 */
class Deme : public Individual::Bag {

public:

  //! Deme allocator type.
  typedef DemeAlloc
          Alloc;
  //! Deme handle type.
  typedef PointerT<Deme,Individual::Bag::Handle>
          Handle;
  //! Deme mixed bag type.
  typedef ContainerT<Deme,Individual::Bag::Bag>
          Bag;

  explicit Deme(Individual::Alloc::Handle inIndAlloc=NULL);
  explicit Deme(Individual::Alloc::Handle inIndAlloc,
                Stats::Alloc::Handle inStatsAlloc,
                HallOfFame::Alloc::Handle inHOFAlloc,
                size_type inN=0);  
  explicit Deme(Genotype::Alloc::Handle inGenotypeAlloc);
  explicit Deme(Genotype::Alloc::Handle inGenotypeAlloc, Fitness::Alloc::Handle inFitnessAlloc);
           Deme(const Deme& inOriginalDeme);
  virtual ~Deme() { }

  Deme& operator=(const Deme& inOriginalDeme);

  virtual void copyData(const Container& inOrigContainer);
  virtual void readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
  virtual void readMigrationBuffer(PACC::XML::ConstIterator inIter, Context& ioContext);
  virtual void readPopulation(PACC::XML::ConstIterator inIter, Context& ioContext);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
  virtual void writeMigrationBuffer(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
  virtual void writePopulation(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
 
  /*!
   *  \brief Get the current deme Hall-of-Fame.
   *  \return Constant reference to the Hall-of-Fame.
   */
  inline const HallOfFame& getHallOfFame() const
  {
    Beagle_StackTraceBeginM();
    return *mHallOfFame;
    Beagle_StackTraceEndM("const HallOfFame& Deme::getHallOfFame() const");
  }

  /*!
   *  \brief Get the current deme Hall-of-Fame.
   *  \return Constant reference to the Hall-of-Fame.
   */
  inline HallOfFame& getHallOfFame()
  {
    Beagle_StackTraceBeginM();
    return *mHallOfFame;
    Beagle_StackTraceEndM("HallOfFame& Deme::getHallOfFame()");
  }

  /*!
   *  \brief Get the hall-of-fame allocator.
   *  \return Constant handle to the hall-of-fame allocator.
   */
  inline const HallOfFame::Alloc::Handle getHallOfFameAlloc() const
  {
    Beagle_StackTraceBeginM();
    return mHOFAlloc;
    Beagle_StackTraceEndM("const HallOfFame::Alloc::Handle Deme::getHallOfFameAlloc() const");
  }

  /*!
   *  \brief Get the hall-of-fame allocator.
   *  \return Handle to the hall-of-fame allocator.
   */
  inline HallOfFame::Alloc::Handle getHallOfFameAlloc()
  {
    Beagle_StackTraceBeginM();
    return mHOFAlloc;
    Beagle_StackTraceEndM("HallOfFame::Alloc::Handle Deme::getHallOfFameAlloc()");
  }

  /*!
   *  \brief Get the current deme migration buffer.
   *  \return Constant reference to the migration buffer.
   */
  inline const Individual::Bag& getMigrationBuffer() const
  {
    Beagle_StackTraceBeginM();
    return *mMigrationBuffer;
    Beagle_StackTraceEndM("const Individual::Bag& Deme::getMigrationBuffer() const");
  }

  /*!
   *  \brief Get the current deme migration buffer.
   *  \return Constant reference to the migration buffer.
   */
  inline Individual::Bag& getMigrationBuffer()
  {
    Beagle_StackTraceBeginM();
    return *mMigrationBuffer;
    Beagle_StackTraceEndM("Individual::Bag& Deme::getMigrationBuffer()");
  }

  /*!
   *  \brief Get the deme evolution statistics.
   *  \return Constant handle to the deme evolution statistics.
   */
  inline const Stats::Handle getStats() const
  {
    Beagle_StackTraceBeginM();
    return mStats;
    Beagle_StackTraceEndM("const Stats::Handle Deme::getStats() const");
  }

  /*!
   *  \brief Get the deme evolution statistics.
   *  \return Handle to the deme evolution statistics.
   */
  inline Stats::Handle getStats()
  {
    Beagle_StackTraceBeginM();
    return mStats;
    Beagle_StackTraceEndM("Stats::Handle Deme::getStats()");
  }

  /*!
   *  \brief Get the statistics allocator.
   *  \return Handle to the evolution statistics allocator.
   */
  inline const Stats::Alloc::Handle getStatsAlloc() const
  {
    Beagle_StackTraceBeginM();
    return mStatsAlloc;
    Beagle_StackTraceEndM("const Stats::Alloc::Handle Deme::getStatsAlloc() const");
  }

  /*!
   *  \brief Get the statistics allocator.
   *  \return Handle to the evolution statistics allocator.
   */
  inline Stats::Alloc::Handle getStatsAlloc()
  {
    Beagle_StackTraceBeginM();
    return mStatsAlloc;
    Beagle_StackTraceEndM("Stats::Alloc::Handle Deme::getStatsAlloc()");
  }

  /*!
   *  \brief Set the hall-of-fame allocator.
   *  \param inHOFAlloc Hall-of-fame allocator to use.
   */
  inline void setHallOfFameAlloc(HallOfFame::Alloc::Handle inHOFAlloc)
  {
    Beagle_StackTraceBeginM();
    mHOFAlloc = inHOFAlloc;
    Beagle_StackTraceEndM("void Deme::setHallOfFameAlloc(HallOfFame::Alloc::Handle inHOFAlloc)");
  }

  /*!
   *  \brief Set the hall-of-fame.
   *  \param inHallOfFame Hall-of-fame to use.
   */
  inline void setHallOfFame(HallOfFame::Handle inHallOfFame)
  {
    Beagle_StackTraceBeginM();
    mHallOfFame = inHallOfFame;
    Beagle_StackTraceEndM("void Deme::setHallOfFame(HallOfFame::Handle inHallOfFame)");
  }

  /*!
   *  \brief Set the migration buffer.
   *  \param inMigrationBuffer Migration buffer to use.
   */
  inline void setMigrationBuffer(Individual::Bag::Handle inMigrationBuffer)
  {
    Beagle_StackTraceBeginM();
    mMigrationBuffer = inMigrationBuffer;
    Beagle_StackTraceEndM("void Deme::setMigrationBuffer(Individual::Bag::Handle inMigrationBuffer)");
  }

  /*!
   *  \brief Set the statistics allocator.
   *  \param inStatsAlloc Statistics allocator.
   */
  inline void setStatsAlloc(Stats::Alloc::Handle inStatsAlloc)
  {
    Beagle_StackTraceBeginM();
    mStatsAlloc = inStatsAlloc;
    Beagle_StackTraceEndM("void Deme::setStatsAlloc(Stats::Alloc::Handle inStatsAlloc)");
  }

  /*!
   *  \brief Set the statistics.
   *  \param inStats Statistics.
   */
  inline void setStats(Stats::Handle inStats)
  {
    Beagle_StackTraceBeginM();
    mStats = inStats;
    Beagle_StackTraceEndM("void Deme::setStats(Stats::Handle inStats)");
  }

protected:
  HallOfFame::Alloc::Handle  mHOFAlloc;         //!< Hall-of-fame allocator.
  HallOfFame::Handle         mHallOfFame;       //!< Deme hall-of-fame.
  Individual::Bag::Handle    mMigrationBuffer;  //!< Migration buffer.
  Stats::Alloc::Handle       mStatsAlloc;       //!< Statistics allocator.
  Stats::Handle              mStats;            //!< Deme statistics.

public:
  virtual void read(PACC::XML::ConstIterator inIter);  // Undefined for Deme!

};

}

#endif // Beagle_Deme_hpp
