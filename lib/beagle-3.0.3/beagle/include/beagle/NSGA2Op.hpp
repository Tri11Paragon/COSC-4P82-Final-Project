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
 *  \file   beagle/NSGA2Op.hpp
 *  \brief  Definition of the class NSGA2Op.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.1 $
 *  $Date: 2007/05/09 01:51:15 $
 */


#ifndef Beagle_NSGA2Op_hpp
#define Beagle_NSGA2Op_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Operator.hpp"
#include "beagle/ReplacementStrategyOp.hpp"
#include "beagle/FitnessMultiObj.hpp"


namespace Beagle {

/*!
 *  \class NSGA2Op beagle/NSGA2Op.hpp "beagle/NSGA2Op.hpp"
 *  \brief NSGA2 multiobjective replacement strategy operator class.
 *  \ingroup ECF
 *  \ingroup Op
 *  \ingroup MultiObj
 *
 *  The NSGA2 replacement strategy implement the elitist multiobjective evolutionary algorithm NSGA2
 *  (Non-dominating Sorting Genetic Algorithm) of Deb, Pratap, Agarwal, and Meyarivan.
 *  \par Reference
 *  Kalyanmoy Deb, Amrit Pratab, Sameer Agarwal, and T. Meyarivan, "A Fast and Elitist
 *  Multiobjective Genetic Algorithm: NSGA-II", IEEE transactions on Evolutionary Computations,
 *  vol. 6, no. 2, April 2002, pages 182-197.
 *
 */
class NSGA2Op : public ReplacementStrategyOp {

public:

  //! NSGA2Op allocator type.
  typedef AllocatorT<NSGA2Op,ReplacementStrategyOp::Alloc>
          Alloc;
  //! NSGA2Op handle type.
  typedef PointerT<NSGA2Op,ReplacementStrategyOp::Handle>
          Handle;
  //! NSGA2Op bag type.
  typedef ContainerT<NSGA2Op,ReplacementStrategyOp::Bag>
          Bag;

           NSGA2Op(string inLMRatioName="ec.mulambda.ratio",
                   string inName="NSGA2Op");
  virtual ~NSGA2Op() { }

  virtual void initialize(System& ioSystem);
  virtual void operate(Deme& ioDeme, Context& ioContext);
  virtual void postInit(System& ioSystem);
  virtual void readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

  typedef std::vector< std::vector<unsigned int,BEAGLE_STLALLOCATOR<unsigned int> >,
                       BEAGLE_STLALLOCATOR< std::vector<unsigned int,BEAGLE_STLALLOCATOR<unsigned int> > > >
          Fronts;
  typedef std::vector< std::pair<double,unsigned int>,BEAGLE_STLALLOCATOR< std::pair<double,unsigned int> > >
          Distances;

  void applyAsReplacementStrategy(Deme& ioDeme, Context& ioContext);
  void applyAsStandardOperator(Deme& ioDeme, Context& ioContext);
  void evalCrowdingDistance(Distances& outDistances, const Individual::Bag& inIndividualPool) const;
  void sortFastND(Fronts& outParetoFronts, unsigned int inSortStop,
                  const Individual::Bag& inIndividualPool, Context& ioContext) const;

  UIntArray::Handle mPopSize;      //!< Population size for the evolution.
  Float::Handle     mLMRatio;      //!< (Lambda / Mu) ratio to use for replacement operator.
  string            mLMRatioName;  //!< (Lambda / Mu) ratio parameter name used in register.

public:

  //! CrowdingPair is used internally in the evalCrowdingDistance method.
  typedef std::pair<unsigned int, FitnessMultiObj::Handle> CrowdingPair;

  //! Used internally to sort solutions over their different objectives.
  class IsLessCrowdingPairPredicate {
  public:
    IsLessCrowdingPairPredicate(unsigned int inSortingObj=0) : mSortingObj(inSortingObj) { }
    ~IsLessCrowdingPairPredicate() { }
    inline bool operator()(const CrowdingPair& inLeftPair, const CrowdingPair& inRightPair) const
    {
      Beagle_StackTraceBeginM();
      Beagle_AssertM(mSortingObj < inLeftPair.second->size());
      Beagle_AssertM(mSortingObj < inRightPair.second->size());
      return (*inLeftPair.second)[mSortingObj] < (*inRightPair.second)[mSortingObj];
      Beagle_StackTraceEndM("bool operator()(const CrowdingPair& inLeftPair, const CrowdingPair& inRightPair) const");
    }
  private:
    unsigned int mSortingObj;
  };

};

}

#endif // Beagle_NSGA2Op_hpp


