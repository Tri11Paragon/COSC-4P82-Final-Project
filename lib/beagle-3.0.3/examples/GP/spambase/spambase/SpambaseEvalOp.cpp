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
 *  \file   SpambaseEvalOp.cpp
 *  \brief  Implementation of the class SpambaseEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.1 $
 *  $Date: 2007/05/09 01:51:24 $
 */

#include "beagle/GP.hpp"
#include "SpambaseEvalOp.hpp"

#include <cmath>
#include <fstream>
#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Construct a new spambase evaluation operator.
 *  \param inFilename Default filename used.
 */
SpambaseEvalOp::SpambaseEvalOp(Beagle::string inFilename) :
  GP::EvaluationOp("SpambaseEvalOp"),
  mFilename(NULL),
  mFilenameDefault(inFilename)
{ }


/*!
 *  \brief Initialize the spambase evaluation operator.
 *  \param ioSystem System of the evolution.
 */
void SpambaseEvalOp::initialize(Beagle::System& ioSystem)
{
  Beagle::GP::EvaluationOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered("spambase.filename")) {
    mFilename = castHandleT<String>(ioSystem.getRegister()["spambase.filename"]);
  } else {
    mFilename = new String(mFilenameDefault);
    Register::Description lDescription(
      "Filename of spambase",
      "String",
      mFilenameDefault,
      "Name of the file containing the spambase."
    );
    ioSystem.getRegister().addEntry("spambase.filename", mFilename, lDescription);
  }
}


/*!
 *  \brief Evaluate the individual fitness for the spambase problem.
 *  \param inIndividual Individual to evaluate.
 *  \param ioContext Evolutionary context.
 *  \return Handle to the fitness measure,
 */
Fitness::Handle SpambaseEvalOp::evaluate(GP::Individual& inIndividual, GP::Context& ioContext)
{
  unsigned int lGoodOutput = 0;
  std::random_shuffle(mShuffledTable.begin(), mShuffledTable.end(),
                      ioContext.getSystem().getRandomizer());
  for(unsigned int i=0; i<Spambase_TestSize; i++) {
    unsigned int lIndex = mShuffledTable[i];
    for(unsigned int j=0; j<mInputs[lIndex].size(); j++) {
      Beagle::string lName = "IN";
      lName += uint2str(j);
      setValue(lName, mInputs[lIndex][j], ioContext);
    }
    Bool lResult;
    inIndividual.run(lResult, ioContext);
    if(lResult.getWrappedValue() == mOutputs[lIndex].getWrappedValue()) lGoodOutput++;
  }
  double lFitness = double(lGoodOutput) / Spambase_TestSize;
  return new FitnessSimple(lFitness);
}


/*!
 *  \brief Post-initialization hook the spambase evaluation operator.
 *  \param ioSystem System of the evolution.
 */
void SpambaseEvalOp::postInit(Beagle::System& ioSystem)
{
   GP::EvaluationOp::postInit(ioSystem);

  if((mFilename!=NULL) && (mFilename->getWrappedValue().empty()==false)) {
    Beagle_LogBasicM(
      ioSystem.getLogger(),
      "evaluation", "SpambaseEvalOp",
      Beagle::string("Reading spambase data from file \"")+mFilename->getWrappedValue()+"\""
    );
    readData(mFilename->getWrappedValue(), Spambase_DataSize);
  }
}


/*!
 *  \brief Read data of the spambase problem.
 *  \param inFilename Name of the file in which the spambase problem data are.
 *  \param inSizeData Number of entry in the data base.
 *  \throw InternalException When the file format is not valid.
 */
void SpambaseEvalOp::readData(Beagle::string inFilename, unsigned int inSizeData)
{
  mInputs.resize(inSizeData);
  mOutputs.resize(inSizeData);
  std::ifstream lDataIS(inFilename.c_str());
  if(!lDataIS) {
    Beagle::string lMessage = "Could not open spambase data file \"";
    lMessage += inFilename;
    lMessage += "\".";
    throw Beagle_InternalExceptionM(lMessage);
  }
  for(unsigned int i=0; i<inSizeData; i++) {
    Beagle::string lStringData;
    if(!lDataIS) {
      Beagle::string lMessage = "Could not open spambase data file \"";
      lMessage += inFilename;
      lMessage += "\".";
      throw Beagle_InternalExceptionM(lMessage);
    }
    lDataIS >> lStringData;
    unsigned int lStringPos = 0;
    mInputs[i].resize(57);
    for(unsigned int j=0; j<mInputs[i].size(); j++) {
      unsigned int lFoundPos = lStringData.find(',', lStringPos);
      if(lFoundPos >= lStringData.size()) {
        Beagle::string lMessage = "Bad format of spambase data file \"";
        lMessage += inFilename;
        lMessage += "\".";
        throw Beagle_InternalExceptionM(lMessage);
      }
      Beagle::string lBuffer(lStringData, lStringPos, lFoundPos-lStringPos);
      mInputs[i][j] = str2dbl(lBuffer);
      lStringPos = lFoundPos + 1;
    }
    Beagle::string lBuffer2(lStringData, lStringPos, 1);
    mOutputs[i] = (bool)str2uint(lBuffer2);
  }
  lDataIS.close();
  mShuffledTable.clear();
  for(unsigned int i=0; i<inSizeData; i++) mShuffledTable.push_back(i);
}

