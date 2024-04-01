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
 *  \file   beagle/GP/src/Deme.cpp
 *  \brief  Source code of class GP::Deme.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.1 $
 *  $Date: 2007/05/09 01:51:06 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a deme.
 *  \par Note:
 *    Use a default GP::Individual allocator to initialize the population.
 */
GP::Deme::Deme() :
  Beagle::Deme(new GP::Individual::Alloc(new GP::Tree::Alloc))
{ }


/*!
 *  \brief Construct a deme with a GP individual allocator.
 *  \param inIndividualAlloc Handle to the allocator of individuals.
 */
GP::Deme::Deme(GP::Individual::Alloc::Handle inIndividualAlloc) :
  Beagle::Deme(inIndividualAlloc)
{ }


/*!
 *  \brief Construct a deme with a GP individual allocator and a statistics deme.
 *  \param inIndividualAlloc Handle to the allocator of individuals.
 *  \param inStatsAlloc Statistics allocator.
 *  \param inHOFAlloc Hall-of-fame allocator.
 *  \param inN Size of deme.
 */
GP::Deme::Deme(GP::Individual::Alloc::Handle inIndividualAlloc,
               Stats::Alloc::Handle inStatsAlloc,
               HallOfFame::Alloc::Handle inHOFAlloc,
               unsigned int inN) :
  Beagle::Deme(inIndividualAlloc, inStatsAlloc, inHOFAlloc, inN)
{ }


/*!
 *  \brief Construct a deme with a GP tree allocator.
 *  \param inGenotypeAlloc Genotype allocator.
 */
GP::Deme::Deme(GP::Tree::Alloc::Handle inGenotypeAlloc) :
  Beagle::Deme(new GP::Individual::Alloc(inGenotypeAlloc))
{ }


/*!
 *  \brief Construct a deme with a GP tree allocator and a fitness allocator.
 *  \param inGenotypeAlloc Genotype allocator.
 *  \param inFitnessAlloc Fitness allocator.
 */
GP::Deme::Deme(GP::Tree::Alloc::Handle inGenotypeAlloc, Fitness::Alloc::Handle inFitnessAlloc) :
  Beagle::Deme(new GP::Individual::Alloc(inGenotypeAlloc, inFitnessAlloc))
{ }

