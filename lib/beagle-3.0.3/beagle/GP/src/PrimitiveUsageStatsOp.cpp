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

#include "beagle/GP.hpp"

#include <iostream>
#include <map>
#include <string>

using namespace Beagle;


// This restrict the scope of the class to the actual file only.
namespace {

typedef std::map< Beagle::string,unsigned int,std::less<Beagle::string>,
                  BEAGLE_STLALLOCATOR<std::pair<const Beagle::string,unsigned int> > >
        CountMap;

/*!
 *  \brief Internal class used to log primitive usage.
 */
class PrimitUsageCount : public Beagle::Object,
                   public CountMap
{
public:
  explicit PrimitUsageCount(unsigned int inDemeID=0, unsigned int inGeneration=0);
  virtual ~PrimitUsageCount() { }
  void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;
  unsigned int mDemeID;
  unsigned int mGeneration;
};


/*!
 *  \brief Construct usage count class.
 */
PrimitUsageCount::PrimitUsageCount(unsigned int inDemeID,
                       unsigned int inGeneration) :
  mDemeID(inDemeID),
  mGeneration(inGeneration)
{ }


/*!
 *  \brief Write usage count to the streamer.
 *  \param ioStreamer XML streamer to write usage count to.
 *  \param inIndent Whether XML output should be indented.
 */
void PrimitUsageCount::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("PrimitiveUsageStats", inIndent);
  ioStreamer.insertAttribute("deme", int2str(mDemeID));
  ioStreamer.insertAttribute("generation", int2str(mGeneration));
  for(const_iterator lIter=begin(); lIter!=end(); ++lIter) {
    ioStreamer.openTag("Primitive", inIndent);
    ioStreamer.insertAttribute("name", lIter->first);
    ioStreamer.insertAttribute("count", int2str(lIter->second));
    ioStreamer.closeTag();
  }
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void PrimitUsageCount::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

}


/*!
 *  \brief Construct primitive usage display operator.
 *  \param inName Name of the primitive.
 */
GP::PrimitiveUsageStatsOp::PrimitiveUsageStatsOp(Beagle::string inName) :
  Operator(inName)
{ }


/*!
 *  \brief Compute primitive usage statistics from deme.
 *  \param ioDeme Deme to compute primitive usage on.
 *  \param ioContext Evolutionary context.
 */
void GP::PrimitiveUsageStatsOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  // Compute usage statistics
  PrimitUsageCount lCounter(ioContext.getDemeIndex(), ioContext.getGeneration());
  for(unsigned int i=0; i<ioDeme.size(); ++i) {
    GP::Individual::Handle lIndi = castHandleT<GP::Individual>(ioDeme[i]);
    // Loop through trees
    for(unsigned int j=0; j<lIndi->size(); ++j) {
      GP::Tree::Handle lTree = castHandleT<GP::Tree>((*lIndi)[j]);
      // Loop through nodes of the tree
      for(unsigned int k=0; k<lTree->size(); ++k) {
        GP::Node& lNode = (*lTree)[k];
        string lPrimitName = lNode.mPrimitive->getName();
        CountMap::iterator lIter = lCounter.find(lPrimitName);
        if(lIter == lCounter.end()) lCounter[lPrimitName] = 1;
        else ++(lIter->second);
      }
    }
  }

  // Display usage statistics
  Beagle_LogObjectM(
    ioContext.getSystem().getLogger(),
    Logger::eStats,
    "stats", "Beagle::GP::PrimitiveUsageStatsOp",
    lCounter
  );
  Beagle_StackTraceEndM("void GP::PrimitiveUsageStatsOp::operate(Beagle::Deme& ioDeme, Beagle::Context& ioContext)");
}



