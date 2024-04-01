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
 *  \file   beagle/Individual.hpp
 *  \brief  Definition of the class Individual.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12.2.1 $
 *  $Date: 2007/05/09 01:51:14 $
 */

#ifndef Beagle_Individual_hpp
#define Beagle_Individual_hpp

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
#include "beagle/Genotype.hpp"
#include "beagle/IndividualAlloc.hpp"
#include "beagle/Fitness.hpp"

namespace Beagle {

// Forward declaration
class IndividualBag;
class System;

/*!
 *  \class Individual beagle/Individual.hpp "beagle/Individual.hpp"
 *  \brief An individual inherits from a container of genotypes and has a fitness member.
 *  \ingroup ECF
 *  \ingroup Pop
 */
class Individual : public Genotype::Bag {

public:

  //! Individual allocator type.
  typedef IndividualAlloc
          Alloc;
  //! Individual handle type.
  typedef PointerT<Individual,Genotype::Bag::Handle>
          Handle;
  //! Individual bag type.
  typedef IndividualBag
          Bag;

  explicit Individual(Genotype::Alloc::Handle inGenotypeAlloc=NULL);
  explicit Individual(Genotype::Alloc::Handle inGenotypeAlloc,
                      Fitness::Alloc::Handle inFitnessAlloc,
                      unsigned int inN=0);
           Individual(const Individual& inRightIndividual);
  virtual ~Individual() { }

  Individual& operator=(const Individual& inRightIndividual);

  virtual void         copyData(const Container& inOrigContainer);
  virtual double       getDistanceGenotype(const Individual& inRightIndividual) const;
  virtual double       getDistancePhenotype(const Individual& inRightIndividual) const;
  virtual unsigned int getSize() const;
  virtual bool         isEqual(const Object& inRightObj) const;
  virtual bool         isIdentical(const Individual& inRightIndividual) const;
  virtual bool         isLess(const Object& inRightObj) const;
  virtual bool         readFromFile(string inFileName, System& ioSystem);
  virtual void         readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext);
  virtual void         write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

  /*!
   *  \brief Get the fitness value of the individual.
   *  \return Fitness value.
   */
  inline const Fitness::Handle getFitness() const
  {
    Beagle_StackTraceBeginM();
    return mFitness;
    Beagle_StackTraceEndM("const Fitness::Handle Individual::getFitness() const");
  }

  /*!
   *  \brief Get the fitness value of the individual.
   *  \return Fitness value.
   */
  inline Fitness::Handle getFitness()
  {
    Beagle_StackTraceBeginM();
    return mFitness;
    Beagle_StackTraceEndM("Fitness::Handle Individual::getFitness()");
  }

  /*!
   *  \brief Get the fitness allocator of the individual.
   *  \return Fitness allocator.
   */
  inline const Fitness::Alloc::Handle getFitnessAlloc() const
  {
    Beagle_StackTraceBeginM();
    return mFitnessAlloc;
    Beagle_StackTraceEndM("const Fitness::Alloc::Handle Individual::getFitnessAlloc() const");
  }

  /*!
   *  \brief Get the fitness allocator of the individual.
   *  \return Fitness allocator.
   */
  inline Fitness::Alloc::Handle getFitnessAlloc()
  {
    Beagle_StackTraceBeginM();
    return mFitnessAlloc;
    Beagle_StackTraceEndM("Fitness::Alloc::Handle Individual::getFitnessAlloc()");
  }

  /*!
   *  \brief Set the fitness value of the individual.
   *  \param inFitness Set the fitness value.
   */
  inline void setFitness(Fitness::Handle inFitness)
  {
    Beagle_StackTraceBeginM();
    mFitness = inFitness;
    Beagle_StackTraceEndM("void Individual::setFitness(Fitness::Handle inFitness)");
  }

  /*!
   *  \brief Set the fitness allocator of the individual.
   *  \param inFitnessAlloc Fitness allocator.
   */
  inline void setFitnessAlloc(Fitness::Alloc::Handle inFitnessAlloc)
  {
    Beagle_StackTraceBeginM();
    mFitnessAlloc = inFitnessAlloc;
    Beagle_StackTraceEndM("void Individual::setFitnessAlloc(Fitness::Alloc::Handle inFitnessAlloc)");
  }

protected:

  Fitness::Alloc::Handle mFitnessAlloc;  //!< Fitness allocator.
  Fitness::Handle        mFitness;       //!< Fitness value of the current individual.

public:

  virtual void read(PACC::XML::ConstIterator inIter);   // Undefined for Individual!

};

}

#endif // Beagle_Individual_hpp
