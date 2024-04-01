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
 *  \file   beagle/src/MigrationOp.cpp
 *  \brief  Source code of class MigrationOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.13.2.1 $
 *  $Date: 2007/05/09 01:51:19 $
 */

#include "beagle/Beagle.hpp"

using namespace Beagle;


/*!
 *  \brief Construct a migration operator.
 *  \param inName Name of the migration operator.
 */
MigrationOp::MigrationOp(Beagle::string inName) :
  Operator(inName),
  mMigrationInterval(NULL),
  mNumberMigrants(NULL),
  mPopSize(NULL)
{ }


/*!
 *  \brief Initialize the migration operator.
 *  \param ioSystem Reference to the evolutionary system.
 */
void MigrationOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  if(ioSystem.getRegister().isRegistered("ec.mig.interval")) {
    mMigrationInterval =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.mig.interval"));
  } else {
    mMigrationInterval = new UInt(1);
    string lLongDescrip = "Interval between each migration, in number of generations. ";
    lLongDescrip += "An interval of 0 disables migration.";
    Register::Description lIntervalDescrip(
      "Interval between migrations",
      "UInt",
      "1",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.mig.interval", mMigrationInterval, lIntervalDescrip);
  }

  if(ioSystem.getRegister().isRegistered("ec.mig.size")) {
    mNumberMigrants =
      castHandleT<UInt>(ioSystem.getRegister().getEntry("ec.mig.size"));
  } else {
    mNumberMigrants = new UInt(5);
    Register::Description lSizeDescrip(
      "Size of each migration",
      "UInt",
      "5",
      "Number of individuals migrating between each deme, at a each migration."
    );
    ioSystem.getRegister().addEntry("ec.mig.size", mNumberMigrants, lSizeDescrip);
  }

  if(ioSystem.getRegister().isRegistered("ec.pop.size")) {
    mPopSize = castHandleT<UIntArray>(ioSystem.getRegister().getEntry("ec.pop.size"));
  } else {
    mPopSize = new UIntArray(1,100);
    string lLongDescrip("Number of demes and size of each deme of the population. ");
    lLongDescrip += "The format of an UIntArray is S1,S2,...,Sn, where Si is the ith value. ";
    lLongDescrip += "The size of the UIntArray is the number of demes present in the ";
    lLongDescrip += "vivarium, while each value of the vector is the size of the corresponding ";
    lLongDescrip += "deme.";
    Register::Description lDescription(
      "Vivarium and demes sizes",
      "UIntArray",
      "100",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry("ec.pop.size", mPopSize, lDescription);
  }
  Beagle_StackTraceEndM("void MigrationOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the migration operator on a deme.
 *  \param ioDeme Deme to migrate.
 *  \param ioContext Evolutionary context.
 */
void MigrationOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  if(mMigrationInterval==NULL) 
    throw Beagle_RunTimeExceptionM(string("The migration interval of the migration operator ")+
                                   string("has not been initialized. It is most likely that ")+
                                   string("you need to call the migration operator's ")+
                                   string("initialize() method."));
#endif // BEAGLE_NDEBUG
  unsigned int lMigrationInterval = mMigrationInterval->getWrappedValue();
  if((lMigrationInterval == 0) || (ioContext.getGeneration() == 0)) return;
  if((ioContext.getGeneration() % lMigrationInterval) != 0) return;
  if(mPopSize->size() < 2) {
    Beagle_LogBasicM(
      ioContext.getSystem().getLogger(),
      "migration", "Beagle::MigrationOp",
      string("Warning: Migration can't occur because there are fewer than two demes. Consider")+
      string(" either, removing the migration operator by editing your configuration file, or")+
      string(" adding demes to the vivarium through the register variable 'ec.pop.size'.")
    );
    return;
  }
  unsigned int lNbMigrants = minOf<unsigned int>(mNumberMigrants->getWrappedValue(),ioDeme.size());
  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "migration", "Beagle::MigrationOp",
    string("Migrating ")+uint2str(lNbMigrants)+
    string(" individuals from the ")+uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );
  migrate(ioDeme, ioContext, lNbMigrants);
  Beagle_StackTraceEndM("void MigrationOp::operate(Deme& ioDeme, Context& ioContext)");
}

