/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/src/Individual.cpp
 *  \brief  Source code of class LinGP::Individual.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.1 $
 *  $Date: 2005/10/06 15:43:44 $
 */

#include "beagle/GP.hpp"
#include "beagle/ObjectException.hpp" 
#include "beagle/LinGP/Individual.hpp"
#include "beagle/LinGP/Program.hpp"
#include "beagle/LinGP/Context.hpp"
#include "beagle/FitnessSimple.hpp"
#include "beagle/LinGP/Program.hpp"

using namespace Beagle;


/*!
 *  \brief Contruct a LinGP individual.
 *  \par Note:
 *    Use LinGP::Program allocator to allocate the genotypes.
 */
LinGP::Individual::Individual() :
  Beagle::Individual(new LinGP::Program::Alloc)
{ }


/*!
 *  \brief Contruct a linear GP individual from a program allocator.
 *  \param inGenotypeAlloc Allocator of linear GP programs.
 */
LinGP::Individual::Individual(LinGP::Program::Alloc::Handle inGenotypeAlloc) :
  Beagle::Individual(inGenotypeAlloc)
{ }


/*!
 *  \brief Contruct a linear GP individual from a genotype allocator and a fitness allocator.
 *  \param inGenotypeAlloc Allocator of linear GP programs.
 *  \param inFitnessAlloc Fitness allocator.
 *  \param inN Number of genotypes of the GP individual.
 */
LinGP::Individual::Individual(LinGP::Program::Alloc::Handle inGenotypeAlloc,
                              Fitness::Alloc::Handle inFitnessAlloc,
                              size_type inN) :
  Beagle::Individual(inGenotypeAlloc, inFitnessAlloc, inN)
{ }


/*!
 *  \brief Interpret the linear GP individual.
 *  \param ioContext Linear GP evolutionary context.
 *  \throw Beagle::ObjectException When individual or program are empty.
 *  \throw Beagle::AssertException When context is not correctly set.
 */
void LinGP::Individual::run(LinGP::Context& ioContext)
{
  Beagle_AssertM(&ioContext.getIndividual() == this);
  if(empty()) throw Beagle_ObjectExceptionM("Could not interpret, individual got no programs!");
  if((*this)[0]->empty())
    throw Beagle_ObjectExceptionM("Could not interpret, first program is empty!");
  Program::Handle lOldProgramHandle = ioContext.getGenotypeHandle();
  unsigned int lOldProgramIndex = ioContext.getGenotypeIndex();
  ioContext.setGenotypeIndex(0);
  ioContext.setGenotypeHandle((*this)[0]);
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "individual", "Beagle::LinGP::Individual",
    string("Running the ")+uint2ordinal(ioContext.getIndividualIndex()+1)+
    string(" individual")
  );
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "individual", "Beagle::LinGP::Individual",
    string("The individual is: ")+serialize()
  );
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "individual", "Beagle::LinGP::Individual",
    string("Executing the first Program root node \"")+
    (*(*this)[0])[0]->getName()+"\""
  );
  (*(*this)[0])[0]->execute(ioContext);
  ioContext.setGenotypeIndex(lOldProgramIndex);
  ioContext.setGenotypeHandle(lOldProgramHandle);
}

