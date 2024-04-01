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
 *  \file   beagle/src/Stats.cpp
 *  \brief  Source code of class Stats.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.12.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#include "beagle/Beagle.hpp"

#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Construct a new stats measure.
 *  \param inId Measure id.
 *  \param inAvg Average of the measure.
 *  \param inStd Standard deviation of the measure.
 *  \param inMax Maximum measure.
 *  \param inMin Minimum measure.
 */
Measure::Measure(Beagle::string inId, double inAvg, double inStd, double inMax, double inMin) :
  mId(inId),
  mAvg(inAvg),
  mStd(inStd),
  mMax(inMax),
  mMin(inMin)
{ }


/*!
 *  \brief Construct new stats.
 *  \param inId Stats id.
 *  \param inGeneration Generation value.
 *  \param inPopSize Population size associated to the statistics.
 *  \param inValid If the stats are valid or not.
 */
Stats::Stats(Beagle::string  inId,
             unsigned int    inGeneration,
             unsigned int    inPopSize,
             bool            inValid) :
  mId(inId),
  mGeneration(inGeneration),
  mPopSize(inPopSize),
  mValid(inValid)
{ }


/*!
 *  \brief Read stats from a XML subtree.
 *  \param inIter XML iterator to read the stats from.
 */
void Stats::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Stats"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Stats> expected!");

  string lValid = inIter->getAttribute("valid").c_str();
  if(lValid.empty() || (lValid == "yes")) {
    mValid = true;
    mId = inIter->getAttribute("id").c_str();

    string lGenerationStr = inIter->getAttribute("generation").c_str();
    if(lGenerationStr.empty() == false) mGeneration = str2uint(lGenerationStr);
    else mGeneration = 0;

    string lPopSizeStr = inIter->getAttribute("popsize").c_str();
    if(lPopSizeStr.empty() == false) mPopSize = str2uint(lPopSizeStr);
    else mPopSize = 0;

    size_type lSize=0;
    for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
      if((lChild->getType()==PACC::XML::eData) && (lChild->getValue()=="Measure")) ++lSize;
    }

    resize(lSize);
    mItemMap.clear();

    size_type lIndexMeasure=0;
    for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
      if(lChild->getType() == PACC::XML::eData) {
        if(lChild->getValue() == "Item") {
          string lKey = lChild->getAttribute("key").c_str();
          if(lKey.empty()) {
            std::ostringstream lOSS;
            lOSS << "expected a key attribute while reading a statistics item!";
            throw Beagle_IOExceptionNodeM(*lChild, lOSS.str().c_str());
          }
          PACC::XML::ConstIterator lChild2 = lChild->getFirstChild();
          if(!lChild2) {
            std::ostringstream lOSS;
            lOSS << "expected an item value while reading a statistics item!";
            throw Beagle_IOExceptionNodeM(*lChild, lOSS.str().c_str());
          }
          if(lChild2->getType() != PACC::XML::eString) {
            std::ostringstream lOSS;
            lOSS << "expected an item value while reading a statistics item!";
            throw Beagle_IOExceptionNodeM(*lChild2, lOSS.str().c_str());
          }
          mItemMap[lKey] = str2dbl(lChild2->getValue().c_str());
        }
        else if(lChild->getValue() == "Measure") {
          (*this)[lIndexMeasure].mId  = lChild->getAttribute("id").c_str();
          (*this)[lIndexMeasure].mAvg = 0.0;
          (*this)[lIndexMeasure].mStd = 0.0;
          (*this)[lIndexMeasure].mMax = 0.0;
          (*this)[lIndexMeasure].mMin = 0.0;
          for(PACC::XML::ConstIterator lChild2=lChild->getFirstChild(); lChild2; ++lChild2) {
            if(lChild2->getType() == PACC::XML::eData) {
              if(lChild2->getValue() == "Avg") {
                PACC::XML::ConstIterator lChild3 = lChild2->getFirstChild();
                if(lChild3->getType() != PACC::XML::eString) continue;
                else (*this)[lIndexMeasure].mAvg = str2dbl(lChild3->getValue().c_str());
              }
              else if(lChild2->getValue() == "Std") {
                PACC::XML::ConstIterator lChild3 = lChild2->getFirstChild();
                if(lChild3->getType() != PACC::XML::eString) continue;
                else (*this)[lIndexMeasure].mStd = str2dbl(lChild3->getValue().c_str());
              }
              else if(lChild2->getValue() == "Max") {
                PACC::XML::ConstIterator lChild3 = lChild2->getFirstChild();
                if(lChild3->getType() != PACC::XML::eString) continue;
                else (*this)[lIndexMeasure].mMax = str2dbl(lChild3->getValue().c_str());
              }
              else if(lChild2->getValue() == "Min") {
                PACC::XML::ConstIterator lChild3 = lChild2->getFirstChild();
                if(lChild3->getType() != PACC::XML::eString) continue;
                else (*this)[lIndexMeasure].mMin = str2dbl(lChild3->getValue().c_str());
              }
            }
          }
          ++lIndexMeasure;
        }
      }
    }
  }
  else if(lValid == "no") mValid = false;
  else throw Beagle_IOExceptionNodeM((*inIter), "bad stats validity value!");
  Beagle_StackTraceEndM("void Stats::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Write a stats into a XML streamer.
 *  \param ioStreamer XML streamer used to write the stats into.
 *  \param inIndent Whether XML output should be indented.
 */
void Stats::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Stats", inIndent);
  if(mValid) {
    if(mId.empty() == false) ioStreamer.insertAttribute("id", mId);
    ioStreamer.insertAttribute("generation", uint2str(mGeneration));
    ioStreamer.insertAttribute("popsize", uint2str(mPopSize));
    for(ItemMap::const_iterator lIter=mItemMap.begin(); lIter!=mItemMap.end(); ++lIter) {
      ioStreamer.openTag("Item", false);
      ioStreamer.insertAttribute("key", lIter->first);
      ioStreamer.insertStringContent(dbl2str(lIter->second).c_str());
      ioStreamer.closeTag();
    }
    for(size_type i=0; i<size(); ++i) {
      ioStreamer.openTag("Measure", inIndent);
      ioStreamer.insertAttribute("id", (*this)[i].mId);
      ioStreamer.openTag("Avg", false);
      ioStreamer.insertStringContent(dbl2str((*this)[i].mAvg).c_str());
      ioStreamer.closeTag();
      ioStreamer.openTag("Std", false);
      ioStreamer.insertStringContent(dbl2str((*this)[i].mStd).c_str());
      ioStreamer.closeTag();
      ioStreamer.openTag("Max", false);
      ioStreamer.insertStringContent(dbl2str((*this)[i].mMax).c_str());
      ioStreamer.closeTag();
      ioStreamer.openTag("Min", false);
      ioStreamer.insertStringContent(dbl2str((*this)[i].mMin).c_str());
      ioStreamer.closeTag();
      ioStreamer.closeTag();
    }
  }
  else ioStreamer.insertAttribute("valid", "no");
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void Stats::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

