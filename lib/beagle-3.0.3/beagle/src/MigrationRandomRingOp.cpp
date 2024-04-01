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
 *  \file   beagle/src/MigrationRandomRingOp.cpp
 *  \brief  Source code of class MigrationRandomRingOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a random ring migration operator,
 *  \param inName Name of the random ring migration operator,
 */
MigrationRandomRingOp::MigrationRandomRingOp(Beagle::string inName) :
  MigrationOp(inName)
{ }


/*!
 *  \brief Migrate randomly choosen individuals of a deme, following a ring topology.
 *  \param ioDeme Deme to migrate the individuals.
 *  \param ioContext Evolutionary context.
 *  \param inNumberMigrants Number of migrants.
 */
void MigrationRandomRingOp::migrate(Deme& ioDeme, Context& ioContext, unsigned int inNumberMigrants)
{
  Beagle_StackTraceBeginM();
  const unsigned int lInIndex = (ioContext.getDemeIndex()==0) ? (ioContext.getVivarium().size()-1) :
                                                                (ioContext.getDemeIndex()-1);
  Individual::Bag& lInBuffer  = ioContext.getVivarium()[lInIndex]->getMigrationBuffer();
  const unsigned int lInMaxIndex = minOf<unsigned int>(lInBuffer.size(), inNumberMigrants);
  Individual::Bag& lOutBuffer = ioDeme.getMigrationBuffer();
  lOutBuffer.resize(0);

  // For the buffer part that is already initialized.
  for(unsigned int i=0; i<lInMaxIndex; i++) {
    const unsigned int lMigInd =
      ioContext.getSystem().getRandomizer().rollInteger(0,ioDeme.size()-1);
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "migration", "Beagle::MigrationRandomRingOp",
      string("Migrating (swap) the ")+uint2ordinal(lMigInd+1)+
      " individual"
    );
    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "migration", "Beagle::MigrationRandomRingOp",
      string("The individual is: ")+ioDeme[lMigInd]->serialize()
    );

    lOutBuffer.push_back(ioDeme[lMigInd]);
    ioDeme[lMigInd] = castHandleT<Individual>(lInBuffer.back());
    lInBuffer.pop_back();
  }

  // If the buffer size if less than the number of migrants, fill by cloning individuals.
  for(unsigned int j=lInMaxIndex; j<inNumberMigrants; j++) {
    const unsigned int lChoosenIndiv =
      ioContext.getSystem().getRandomizer().rollInteger(0, ioDeme.size()-1);
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "migration", "Beagle::MigrationRandomRingOp",
      string("Migrating (copy) the ")+uint2ordinal(lChoosenIndiv+1)+
      " individual"
    );
    Beagle_LogDebugM(
      ioContext.getSystem().getLogger(),
      "migration", "Beagle::MigrationRandomRingOp",
      string("The individual is: ")+ioDeme[lChoosenIndiv]->serialize()
    );

    lOutBuffer.push_back(castHandleT<Individual>(
      ioDeme.getTypeAlloc()->cloneData(*ioDeme[lChoosenIndiv])));
  }
  Beagle_StackTraceEndM("void MigrationRandomRingOp::migrate(Deme& ioDeme, Context& ioContext, unsigned int inNumberMigrants)");
}


