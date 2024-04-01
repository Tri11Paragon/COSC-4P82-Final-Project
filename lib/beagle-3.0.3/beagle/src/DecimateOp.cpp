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
 *  \file   beagle/src/DecimateOp.cpp
 *  \brief  Source code of class DecimateOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

#include "beagle/Beagle.hpp"

#include <cmath>
#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Construct a plain new decimation operator.
 *  \param inDecimationRatioName Decimation ratio parameter name.
 *  \param inName Name of the decimation operator.
 */
DecimateOp::DecimateOp(Beagle::string inDecimationRatioName, Beagle::string inName) :
  Operator(inName),
  mDecimationRatioName(inDecimationRatioName)
{ }


/*!
 *  \brief Initialize the decimation operator.
 *  \param ioSystem Reference to the system to use for decimation.
 */
void DecimateOp::initialize(System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Operator::initialize(ioSystem);

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

  if(ioSystem.getRegister().isRegistered(mDecimationRatioName)) {
    mDecimationRatio = castHandleT<Float>(ioSystem.getRegister().getEntry(mDecimationRatioName));
  } else {
    mDecimationRatio = new Float((float)0.125);
    Register::Description lDescription(
      "Decimation ratio",
      "Float",
      "0.125",
      string("Decimation ratio, which mean how much smaller will be the population ") +
      string("resulting from a decimation operation compared to the actual population size. ")+
      string("If the ratio value is -1.0, the deme is resized to the size given in parameter ")+
      string("'ec.pop.size'.")
    );
    ioSystem.getRegister().addEntry(mDecimationRatioName, mDecimationRatio, lDescription);
  }
  Beagle_StackTraceEndM("void DecimateOp::initialize(System& ioSystem)");
}


/*!
 *  \brief Apply the decimation operation on the deme.
 *  \param ioDeme Current deme of individuals to decimate.
 *  \param ioContext Context of the evolution.
 */
void DecimateOp::operate(Deme& ioDeme, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_ValidateParameterM((mDecimationRatio->getWrappedValue()<=1.0),
                            mDecimationRatioName,
                            "The decimation ratio must be lesser or equal to 1.0.");

  Beagle_LogTraceM(
    ioContext.getSystem().getLogger(),
    "decimation", "Beagle::DecimateOp",
    string("Applying decimation operation on the ")+
    uint2ordinal(ioContext.getDemeIndex()+1)+" deme"
  );

  unsigned int lLambda = 0;
  if(mDecimationRatio->getWrappedValue() == -1.0) {
    Beagle_AssertM(ioContext.getDemeIndex() < mPopSize->size());
    lLambda = (*mPopSize)[ioContext.getDemeIndex()];
  }
  else {
    lLambda = (unsigned int)std::ceil(mDecimationRatio->getWrappedValue()*float(ioDeme.size()));
    Beagle_AssertM(ioContext.getDemeIndex() < mPopSize->size());
    int lDiffSize = (*mPopSize)[ioContext.getDemeIndex()] - lLambda;
    if((lDiffSize >= -1) && (lDiffSize <= 1)) lLambda = (*mPopSize)[ioContext.getDemeIndex()];
  }
  Beagle_AssertM(lLambda <= ioDeme.size());

  std::make_heap(ioDeme.begin(), ioDeme.end(), IsLessPointerPredicate());
  Individual::Bag lSurvivors;
  for(unsigned int i=0; i<lLambda; ++i) {
    lSurvivors.push_back(ioDeme.front());
    std::pop_heap(ioDeme.begin(), (ioDeme.end()-i), IsLessPointerPredicate());
  }
  ioDeme.resize(0);
  ioDeme.insert(ioDeme.begin(), lSurvivors.begin(), lSurvivors.end());
  Beagle_StackTraceEndM("void DecimateOp::operate(Deme& ioDeme, Context& ioContext)");
}

