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
 *  \file   IPDEvalOp.hpp
 *  \brief  Definition of the type IPDEvalOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.14.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

/*!
 *  \defgroup IPD Iterated Prisoner's Dilemma Co-evolution GA Example
 *  \brief Iterated Prisoner's Dilemma (ipd): Co-evolution GA example with Open BEAGLE.
 *
 *  \par Objective
 *  Find solutions to the iterated prisoner's dilemma that get the highest reward for
 *  its actions.
 *
 *  \par Representation
 *  Bit strings made of 3 bits. First bit is the action to take at the first turn
 *  (0=defect, 1=cooperate), the second bit is the action to make knowing that 
 *  the companion has defected at the last turn, and the third bit is the action to
 *  do knowing that the companion has cooperate the last turn.
 *
 *  \par Fitness
 *  For each turn, the individual receives points for its action. Each evaluation is done
 *  on a total of 8 turns (iterations). The points given for each turn are the following:
 *  - 0 points if the player cooperate and the companion defects.
 *  - 1 points if the player defects and the companion defects.
 *  - 3 points if the player defects and the companion cooperate.
 *  - 5 points if the player cooperate and the companion cooperate.
 *
 */
 
#ifndef IPDEvalOp_hpp
#define IPDEvalOp_hpp

#include "beagle/Coev.hpp"
#include <vector>



/*!
 *  \class IPDEvalOp IPDEvalOp.hpp "IPDEvalOp.hpp"
 *  \brief Co-evolution evaluation class operator for the iterated prisoner's dilemma.
 *  \ingroup IPD
 */
class IPDEvalOp : public Beagle::Coev::EvaluationOp {

public:

  //! IPDEvalOp allocator type.
  typedef Beagle::AllocatorT<IPDEvalOp,Beagle::Coev::EvaluationOp::Alloc>
          Alloc;
  //!< IPDEvalOp handle type.
  typedef Beagle::PointerT<IPDEvalOp,Beagle::Coev::EvaluationOp::Handle>
          Handle;
  //!< IPDEvalOp bag type.
  typedef Beagle::ContainerT<IPDEvalOp,Beagle::Coev::EvaluationOp::Bag>
          Bag;

  explicit IPDEvalOp();

  virtual void evaluateSets(Beagle::Coev::EvaluationOp::EvalSetVector& ioSets);
  virtual void makeSets(Beagle::Individual::Bag& ioIndivBag,
                        Beagle::Context::Handle ioContext);

protected:

  void playIPD(const Beagle::Individual& inInd1, const Beagle::Individual& inInd2,
               unsigned int& outScoreInd1, unsigned int& outScoreInd2) const;

};

#endif // IPDEvalOp_hpp
