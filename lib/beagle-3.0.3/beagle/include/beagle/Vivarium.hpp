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
 *  \file   beagle/Vivarium.hpp
 *  \brief  Definition of the class Vivarium.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.11.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

/*!
 *  \defgroup Pop Population
 *  \ingroup ECF
 *  \brief Components of the population, part of the generic EC framework.
 *
 *  The population is structured into four layers: a vivarium, the demes, the individuals and the 
 *  genotypes. The first layer, the vivarium, comprises the whole population of a specific evolution, 
 *  that is, an aggregate of one or more demes. A deme is a population that evolves 
 *  in an independent environment. Generally, at each generation, there are some individuals that 
 *  migrate between the demes that compose a vivarium. A deme is implemented in class Beagle::Deme. 
 *  The class provides and implements an interface of standard methods necessary to evolve its own
 *  population. The class Beagle::Vivarium describes a bag of demes, which is itself an bag of 
 *  individuals. Each deme also has a hall-of-fame where the best-of-run individuals are conserved, 
 *  and its statistics.
 *
 *  The next underlying layer, the individual, is defined in class Beagle::Individual. It is an bag 
 *  of genotypes, the parts that compose a genome. The genotypes are tightly related to the 
 *  representation of the individuals for a given EC algorithm. The genotype interface is declared in 
 *  the abstract class Beagle::Genotype.
 *
 */

 
#ifndef Beagle_Vivarium_hpp
#define Beagle_Vivarium_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/ContainerAllocatorT.hpp"
#include "beagle/Deme.hpp"
#include "beagle/DemeAlloc.hpp"
#include "beagle/Stats.hpp"
#include "beagle/VivariumAlloc.hpp"

namespace Beagle {

/*!
 *  \class Vivarium beagle/Vivarium.hpp "beagle/Vivarium.hpp"
 *  \brief A vivarium is a set of demes.
 *  \ingroup ECF
 *  \ingroup Pop
 */
class Vivarium : public Deme::Bag {

public:

  //! Vivarium allocator type.
  typedef VivariumAlloc
          Alloc;
  //! Vivarium handle type.
  typedef PointerT<Vivarium,Deme::Bag::Handle>
          Handle;
  //! Vivarium bag type.
  typedef ContainerT<Vivarium,Deme::Bag::Bag>
          Bag;

  explicit Vivarium(Deme::Alloc::Handle inDemeAlloc=NULL);
  explicit Vivarium(Deme::Alloc::Handle inDemeAlloc,
                    Stats::Alloc::Handle inStatsAlloc,
                    HallOfFame::Alloc::Handle inHOFAlloc,
                    size_type inN=0);
  explicit Vivarium(Genotype::Alloc::Handle inGenotypeAlloc);
  explicit Vivarium(Genotype::Alloc::Handle inGenotypeAlloc, Fitness::Alloc::Handle inFitnessAlloc);
           Vivarium(const Vivarium& inOrigVivarium);
  virtual ~Vivarium() { }

  Vivarium& operator=(const Vivarium& inOrigVivarium);

  virtual void copyData(const Container& inOrigContainer);
  virtual void readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
  virtual void readPopulation(PACC::XML::ConstIterator inIter, Context& ioContext);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
  virtual void writePopulation(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

  /*!
   *  \brief Get the current deme Hall-of-Fame.
   *  \return Constant reference to the Hall-of-Fame.
   */
  inline const HallOfFame& getHallOfFame() const
  {
    Beagle_StackTraceBeginM();
    return *mHallOfFame;
    Beagle_StackTraceEndM("const HallOfFame& Vivarium::getHallOfFame() const");
  }

  /*!
   *  \brief Get the current deme Hall-of-Fame.
   *  \return Constant reference to the Hall-of-Fame.
   */
  inline HallOfFame& getHallOfFame()
  {
    Beagle_StackTraceBeginM();
    return *mHallOfFame;
    Beagle_StackTraceEndM("HallOfFame& Vivarium::getHallOfFame()");
  }

  /*!
   *  \brief Get the hall-of-fame allocator.
   *  \return Constant handle to the hall-of-fame allocator.
   */
  inline const HallOfFame::Alloc::Handle getHallOfFameAlloc() const
  {
    Beagle_StackTraceBeginM();
    return mHOFAlloc;
    Beagle_StackTraceEndM("const HallOfFame::Alloc::Handle Vivarium::getHallOfFameAlloc() const");
  }

  /*!
   *  \brief Get the hall-of-fame allocator.
   *  \return Handle to the hall-of-fame allocator.
   */
  inline HallOfFame::Alloc::Handle getHallOfFameAlloc()
  {
    Beagle_StackTraceBeginM();
    return mHOFAlloc;
    Beagle_StackTraceEndM("HallOfFame::Alloc::Handle Vivarium::getHallOfFameAlloc()");
  }

  /*!
   *  \brief Get the vivarium evolution statistics.
   *  \return Constant handle to the vivarium evolution statistics.
   */
  inline const Stats::Handle getStats() const
  {
    Beagle_StackTraceBeginM();
    return mStats;
    Beagle_StackTraceEndM("const Stats::Handle Vivarium::getStats() const");
  }

  /*!
   *  \brief Get the vivarium evolution statistics.
   *  \return Handle to the vivarium evolution statistics.
   */
  inline Stats::Handle getStats()
  {
    Beagle_StackTraceBeginM();
    return mStats;
    Beagle_StackTraceEndM("Stats::Handle Vivarium::getStats()");
  }

  /*!
   *  \brief Get the statistics allocator.
   *  \return Handle to the evolution statistics allocator.
   */
  inline const Stats::Alloc::Handle getStatsAlloc() const
  {
    Beagle_StackTraceBeginM();
    return mStatsAlloc;
    Beagle_StackTraceEndM("const Stats::Alloc::Handle Vivarium::getStatsAlloc() const");
  }

  /*!
   *  \brief Get the statistics allocator.
   *  \return Handle to the evolution statistics allocator.
   */
  inline Stats::Alloc::Handle getStatsAlloc()
  {
    Beagle_StackTraceBeginM();
    return mStatsAlloc;
    Beagle_StackTraceEndM("Stats::Alloc::Handle Vivarium::getStatsAlloc()");
  }

  /*!
   *  \brief Set the hall-of-fame allocator.
   *  \param inHOFAlloc Hall-of-fame allocator to use.
   */
  inline void setHallOfFameAlloc(HallOfFame::Alloc::Handle inHOFAlloc)
  {
    Beagle_StackTraceBeginM();
    mHOFAlloc = inHOFAlloc;
    Beagle_StackTraceEndM("void Vivarium::setHallOfFameAlloc(HallOfFame::Alloc::Handle inHOFAlloc)");
  }

  /*!
   *  \brief Set the hall-of-fame.
   *  \param inHallOfFame Hall-of-fame to use.
   */
  inline void setHallOfFame(HallOfFame::Handle inHallOfFame)
  {
    Beagle_StackTraceBeginM();
    mHallOfFame = inHallOfFame;
    Beagle_StackTraceEndM("void Vivarium::setHallOfFame(HallOfFame::Handle inHallOfFame)");
  }

  /*!
   *  \brief Set the statistics allocator.
   *  \param inStatsAlloc Statistics allocator.
   */
  inline void setStatsAlloc(Stats::Alloc::Handle inStatsAlloc)
  {
    Beagle_StackTraceBeginM();
    mStatsAlloc = inStatsAlloc;
    Beagle_StackTraceEndM("void Vivarium::setStatsAlloc(Stats::Alloc::Handle inStatsAlloc)");
  }

  /*!
   *  \brief Set the statistics.
   *  \param inStats Statistics.
   */
  inline void setStats(Stats::Handle inStats)
  {
    Beagle_StackTraceBeginM();
    mStats = inStats;
    Beagle_StackTraceEndM("void Vivarium::setStats(Stats::Handle inStats)");
  }

protected:

  HallOfFame::Alloc::Handle  mHOFAlloc;     //!< Hall-of-fame allocator.
  HallOfFame::Handle         mHallOfFame;   //!< Vivarium's hall-of-fame.
  Stats::Alloc::Handle       mStatsAlloc;   //!< Statistics allocator.
  Stats::Handle              mStats;        //!< Vivarium statistics.

public:

  virtual void read(PACC::XML::ConstIterator inIter);    // Undefined for Vivarium!

};

}

#endif // Beagle_Vivarium_hpp

