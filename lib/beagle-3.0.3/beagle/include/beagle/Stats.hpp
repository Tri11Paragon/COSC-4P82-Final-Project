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
 *  \file   beagle/Stats.hpp
 *  \brief  Definition of class Stats.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:17 $
 */

#ifndef Beagle_Stats_hpp
#define Beagle_Stats_hpp

#include <string>

#include "XML.hpp"

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/ContainerAllocatorT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/AssertException.hpp"
#include "beagle/RunTimeException.hpp"

namespace Beagle {


/*!
 *  \struct Measure beagle/Stats.hpp "beagle/Stats.hpp"
 *  \brief Statistical measure of an evolution.
 *  \ingroup ECF
 *  \ingroup FitStats
 */
struct Measure {
  string mId;    //!< Id of the measure.
  double      mAvg;   //!< Average of the statistic measure.
  double      mStd;   //!< Standard deviation of the statistic measure.
  double      mMax;   //!< Maximum of the statistic measure.
  double      mMin;   //!< Minimum of the statistic measure.

  explicit Measure(string inId = "",
                   double inAvg = 0.0,
                   double inStd = 0.0,
                   double inMax = 0.0,
                   double inMin = 0.0);
};


/*!
 *  \class Stats beagle/Stats.hpp "beagle/Stats.hpp"
 *  \brief Generation statistics measure.
 *  \ingroup ECF
 *  \ingroup FitStats
 */
class Stats : public Object,
              public std::vector< Measure,BEAGLE_STLALLOCATOR<Measure> > {

public:

  //! Stats allocator type.
  typedef AllocatorT<Stats,Object::Alloc>
          Alloc;
  //! Stats handle type.
  typedef PointerT<Stats,Object::Handle>
          Handle;
  //! Stats bag type.
  typedef ContainerT<Stats,Object::Bag>
          Bag;

  explicit Stats(string  inId="",
                 unsigned int inGeneration=0,
                 unsigned int inPopSize=0,
                 bool         inValid=false);
  virtual ~Stats() { }

  virtual void read(PACC::XML::ConstIterator inIter);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

  /*!
   *  \brief Add an item to the statistics.
   *  \param inTag Tag of the statistics item.
   *  \param inValue Value of item to add.
   *  \throw RunTimeException If the item is already in the stats.
   */
  inline void addItem(string inTag, double inValue)
  {
    Beagle_StackTraceBeginM();
    ItemMap::const_iterator lIterObj = mItemMap.find(inTag);
    if(lIterObj != mItemMap.end())
      throw Beagle_RunTimeExceptionM("Could not add item to statistics as it already exist!");
    mItemMap[inTag] = inValue;
    Beagle_StackTraceEndM("void Stats::addItem(string inTag, double inValue)");
  }

  /*!
   *  \brief Clear items of the statistics.
   */
  inline void clearItems()
  {
    Beagle_StackTraceBeginM();
    mItemMap.clear();
    Beagle_StackTraceEndM("void Stats::clearItems()");
  }

  /*!
   *  \brief  Delete an item from the statistics.
   *  \param  inTag Tag of the statistics item.
   *  \return Value item deleted.
   *  \throw  RunTimeException If the item is not in the stats.
   */
  inline double deleteItem(string inTag)
  {
    Beagle_StackTraceBeginM();
    ItemMap::iterator lIterObj = mItemMap.find(inTag);
    if(lIterObj == mItemMap.end()) {
      std::ostringstream lOSS;
      lOSS << "Item named \"" << inTag << "\" is not in the statistics!";
      throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
    }
    double lValue = lIterObj->second;
    mItemMap.erase(lIterObj);
    return lValue;
    Beagle_StackTraceEndM("double Stats::deleteItem(string inTag)");
  }

  /*!
   *  \brief  Get value of an item of the statistics.
   *  \param  inTag Item of the statistics item.
   *  \return Value of item.
   *  \throw  RunTimeException if there is no entry associated with the tag.
   */
  inline double& getItem(string inTag)
  {
    Beagle_StackTraceBeginM();
    ItemMap::iterator lIterObj = mItemMap.find(inTag);
    if(lIterObj == mItemMap.end()) {
      std::ostringstream lOSS;
      lOSS << "Item named \"" << inTag << "\" is not in the statistics!";
      throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
    }
    return lIterObj->second;
    Beagle_StackTraceEndM("double& Stats::getItem(string inTag)");
  }

  /*!
   *  \brief  Get value of an item of the statistics.
   *  \param  inTag Item of the statistics item.
   *  \return Value of item.
   *  \throw  RunTimeException if there is no entry associated with the tag.
   */
  inline const double& getItem(string inTag) const
  {
    Beagle_StackTraceBeginM();
    ItemMap::const_iterator lIterObj = mItemMap.find(inTag);
    if(lIterObj == mItemMap.end()) {
      std::ostringstream lOSS;
      lOSS << "Item named \"" << inTag << "\" is not in the statistics!";
      throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
    }
    return lIterObj->second;
    Beagle_StackTraceEndM("const double& Stats::getItem(string inTag) const");
  }

  /*!
   *  \return Id of the stats.
   */
  inline string getId() const
  {
    Beagle_StackTraceBeginM();
    return mId;
    Beagle_StackTraceEndM("string Stats::getId() const");
  }

  /*!
   *  \return Generation of the stats.
   */
  inline unsigned int getGeneration() const
  {
    Beagle_StackTraceBeginM();
    return mGeneration;
    Beagle_StackTraceEndM("unsigned int Stats::getGeneration() const");
  }

  /*!
   *  \return Population size associated to the stats.
   */
  inline unsigned int getPopSize() const
  {
    Beagle_StackTraceBeginM();
    return mPopSize;
    Beagle_StackTraceEndM("unsigned int Stats::getPopSize() const");
  }

  /*!
   *  \brief Is given item is in the statistics.
   *  \param inTag Tag of the statistics item.
   */
  inline bool existItem(string inTag) const
  {
    Beagle_StackTraceBeginM();
    ItemMap::const_iterator lIterObj = mItemMap.find(inTag);
    return (lIterObj != mItemMap.end());
    Beagle_StackTraceEndM("bool Stats::existItem(string inTag) const");
  }

  /*!
   *  \brief Return validity of the stats.
   *  \return True if stats are valid, false if not.
   */
  inline bool isValid() const
  {
    Beagle_StackTraceBeginM();
    return mValid;
    Beagle_StackTraceEndM("bool Stats::isValid() const");
  }

  /*!
   *  \brief Add an item to the statistics.
   *  \param inTag Tag of the statistics item.
   *  \param inValue Value of item to modify.
   */
  inline void modifyItem(string inTag, double inValue)
  {
    Beagle_StackTraceBeginM();
    mItemMap[inTag] = inValue;
    Beagle_StackTraceEndM("void Stats::modifyItem(string inTag, double inValue)");
  }

  /*!
   *  \brief Invalidate the stats.
   */
  inline void setInvalid()
  {
    Beagle_StackTraceBeginM();
    mValid = false;
    Beagle_StackTraceEndM("void Stats::setInvalid()");
  }

  /*!
   *  \brief Validate the stats.
   */
  inline void setValid()
  {
    Beagle_StackTraceBeginM();
    mValid = true;
    Beagle_StackTraceEndM("void Stats::setValid()");
  }

  /*!
   *  \brief Set the generational values of the stats.
   *  \param inId Stats id.
   *  \param inGeneration Generation value.
   *  \param inPopSize Population size.
   *  \param inValid If the stats are valid or not.
   */
  inline void setGenerationValues(string inId,
                                  unsigned int inGeneration,
                                  unsigned int inPopSize,
                                  bool inValid=true)
  {
    Beagle_StackTraceBeginM();
    mId = inId;
    mGeneration = inGeneration;
    mPopSize = inPopSize;
    mValid = inValid;
    Beagle_StackTraceEndM("void Stats::setGenerationValues(string inId, unsigned int inGeneration, unsigned int inPopSize, bool inValid)");
  }

protected:

  typedef std::map< string,double,std::less<string>,
                    BEAGLE_STLALLOCATOR< std::pair<const string,double> > >
          ItemMap;

  ItemMap         mItemMap;     //!< Statistics item evaluated.
  string          mId;          //!< Id of the stats.
  unsigned int    mGeneration;  //!< Generation at which the measure had been taken.
  unsigned int    mPopSize;     //!< Population size.
  bool            mValid;       //!< Flag that indicate whether the stats are valids.

};

}

#endif // Beagle_Stats_hpp
