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
 *  \file   beagle/FitnessMultiObj.hpp
 *  \brief  Definition of the class FitnessMultiObj.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.1 $
 *  $Date: 2007/05/09 01:51:11 $
 */

/*!
 *  \defgroup MultiObj Multiobjective Evolutionary Algorithms
 *  \ingroup ECF
 *  \brief Multiobjective fitness and related operators, part of the generic EC framework.
 *
 *  The first element of Open BEAGLE's multiobjective EA (MOEA) facilities is the  
 *  multiobjective fitness measure, called FitnessMultiObj. It is made of a STL vector of floats,
 *  where each values of the vector is associated to one optimization objective. The basic class 
 *  FitnessMultiObj defines the method isDominated, which is used for Pareto ranking in the MOEA
 *  methods. By default, the domination is evaluated on the basis that all objectives must be
 *  maximized. The minimization equivalent is defined in class FitnessMultiObjMin. For mixed
 *  maximization/minization problems, a specific fitness classes inheriting from FitnessMultiObj 
 *  must be defined, with the method isDominated properly overdefined. A statistics computation
 *  operator associated to class FitnessMultiObj is defined by class StatsCalcFitnessMultiObjOp.
 *
 *  MOEA method NSGA2 is defined by operator NSGA2Op. This operator can only be used as a
 *  replacement strategy in a breeder tree. In addition to the domination evaluating
 *  method defined in FitnessMultiObj::isDominated, the operator uses method
 *  FitnessMultiObj::getScalingFactors to normalize distances between solution in the objective
 *  space. This method returns by default a vector filled with 1.0, which mean that the value
 *  of the objective are already on the same scale, or normalized. For problems with non-normalized
 *  objective values, the method getScalingFactors must be properly overdefined.
 *
 *  In the context of MOEA, the hall-of-fame is not meaningful, as there is not strict relation
 *  order between the solutions found out of the Pareto ranking. The hall-of-fame sizes
 *  must then be set to 0. For a specific problem where the use of hall-of-fames make some sense,
 *  the method Fitness::isLess must be overdefined accordingly. In class FitnessMultiObj, relation
 *  ordering is based on the first objective value.
 *
 */
 
#ifndef Beagle_FitnessMultiObj_hpp
#define Beagle_FitnessMultiObj_hpp

#include <vector>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Stats.hpp"
#include "beagle/Fitness.hpp"

namespace Beagle {

/*!
 *  \class FitnessMultiObj beagle/FitnessMultiObj.hpp "beagle/FitnessMultiObj.hpp"
 *  \brief Multiobjective fitness measure class.
 *  \ingroup ECF
 *  \ingroup FitStats
 *  \ingroup MultiObj
 */
class FitnessMultiObj : public Fitness,
                        public std::vector< float,BEAGLE_STLALLOCATOR<float> > {

public:

  //! FitnessMultiObj allocator type.
  typedef AllocatorT<FitnessMultiObj,Fitness::Alloc>
          Alloc;
  //! FitnessMultiObj handle type.
  typedef PointerT<FitnessMultiObj,Fitness::Handle>
          Handle;
  //! FitnessMultiObj bag type.
  typedef ContainerT<FitnessMultiObj,Fitness::Bag>
          Bag;

  explicit FitnessMultiObj(unsigned int inSize=0, float inValue=0.0);
  explicit FitnessMultiObj(const std::vector< float,BEAGLE_STLALLOCATOR<float> >& inFitness);
  virtual ~FitnessMultiObj() { }

  virtual std::vector< float,BEAGLE_STLALLOCATOR<float> > getScalingFactors() const;
  virtual bool isDominated(const FitnessMultiObj& inRightFitness) const;
  virtual bool isEqual(const Object& inRightObj) const;
  virtual bool isLess(const Object& inRightObj) const;
  virtual void read(PACC::XML::ConstIterator inIter);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

};

}

#endif // Beagle_FitnessMultiObj_hpp
