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
 *  \file   beagle/GP/src/Vivarium.cpp
 *  \brief  Source code of class GP::Vivarium.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.1 $
 *  $Date: 2007/05/09 01:51:07 $
 */

#include "beagle/GP.hpp"

using namespace Beagle;


/*!
 *  \brief Contruct a vivarium instanciated for standard GP.
 */
GP::Vivarium::Vivarium() :
  Beagle::Vivarium(new GP::Deme::Alloc(new GP::Individual::Alloc(new GP::Tree::Alloc)))
{ }


/*!
 *  \brief Contruct a vivarium from a deme allocator.
 *  \param inDemeAlloc Allocator of GP demes.
 */
GP::Vivarium::Vivarium(GP::Deme::Alloc::Handle inDemeAlloc) :
  Beagle::Vivarium(inDemeAlloc)
{ }


/*!
 *  \brief Contruct a vivarium from a deme allocator and a statistics allocator.
 *  \param inDemeAlloc Allocator of GP demes.
 *  \param inStatsAlloc Statistics allocator.
 *  \param inHOFAlloc Hall-of-fame allocator.
 *  \param inN Number of demes in the vivarium.
 */
GP::Vivarium::Vivarium(GP::Deme::Alloc::Handle inDemeAlloc,
                       Stats::Alloc::Handle inStatsAlloc,
                       HallOfFame::Alloc::Handle inHOFAlloc,
                       unsigned int inN) :
  Beagle::Vivarium(inDemeAlloc, inStatsAlloc, inHOFAlloc, inN)
{ }


/*!
 *  \brief Construct a vivarium from a genotype allocator.
 *  \param inGenotypeAlloc Genotype allocator.
 */
GP::Vivarium::Vivarium(GP::Tree::Alloc::Handle inGenotypeAlloc) :
  Beagle::Vivarium(new GP::Deme::Alloc(new GP::Individual::Alloc(inGenotypeAlloc)))
{ }


/*!
 *  \brief Construct a vivarium with a genotype allocator and a fitness allocator.
 *  \param inGenotypeAlloc Genotype allocator.
 *  \param inFitnessAlloc Fitness allocator.
 */
GP::Vivarium::Vivarium(GP::Tree::Alloc::Handle inGenotypeAlloc,
                       Fitness::Alloc::Handle inFitnessAlloc) :
  Beagle::Vivarium(new GP::Deme::Alloc(new GP::Individual::Alloc(inGenotypeAlloc, inFitnessAlloc)))
{ }

