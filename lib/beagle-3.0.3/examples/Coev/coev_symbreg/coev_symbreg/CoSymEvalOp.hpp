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
 *  \file   CoSymEvalOp.hpp
 *  \brief  Declaration of the class CoSymEvalOp.
 *  \author Jiachuan Wang <jiacwang@ecs.umass.edu>
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.14.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

/*!
 *  \defgroup CoSym Co-evolutionary Symbolic Regression Example
 *  \brief Co-evolutionary Symbolic Regression (coev_symbreg):
 *    Mixed real-valued GA - GP co-evolution example with Open BEAGLE
 *
 *  \par Objective
 *  Two populations competitive co-evolution for symbolic regression.
 *  First population is made of standard GP mathematical expressions.
 *  Second population is made of individual representing 20 \f$(x_i,y_i)\f$
 *  samples in the domain \f$[-1,1]\f$ of the equation \f$x^4+x^3+x^2+x\f$.
 *  The global objective is to a find symbolic expressions that
 *  "generalizes" well over the domain sampled.
 *
 *  \par Terminal set of GP expressions
 *  - X (the independent variable)
 *  - Ephemeral constants randomly generated in \f$[-1,1]\f$
 *
 *  \par Function set of GP expressions
 *  - +
 *  - -
 *  - *
 *  - /     (protected division)
 *
 *  \par Representation of training sets
 *  Vector of 20 real-valued numbers in \f$[-1,1]\f$.
 *
 *  \par Fitness
 *  Root Mean Square (RMS) error of the GP expressions on the 20 samples
 *  of the training set. GP expression minimizes the RMS error, while
 *  training set maximizes it. Individuals of one population are
 *  evaluated against last generation other population best performing
 *  individual.
 *
 *  \par Reference
 *  Liviu Panait and Sean Luke, Methods for Evolving Robust Programs,
 *  Proceedings of Genetic and Evolutionary Computation -- GECCO-2003,
 *  LNCS, Vol. 2724, pp. 1740-1751, Springer-Verlag, 12-16 July 2003.
 *
 */

#ifndef CoSymEvalOp_hpp
#define CoSymEvalOp_hpp

#include "beagle/Coev.hpp"
#include <vector>

/*!
 *  \class CoSymEvalOp CoSymEvalOp.hpp "CoSymEvalOp.hpp"
 *  \brief Abstract co-evolution evaluation class operator for the co-evolutionary
 *    symbolic regression.
 *  \ingroup CoSym
 */
class CoSymEvalOp : public Beagle::Coev::GPEvaluationOp {

public:

  //! CoSymEvalOp allocator type.
  typedef Beagle::AllocatorT<CoSymEvalOp,Beagle::Coev::GPEvaluationOp::Alloc>
          Alloc;
  //!< CoSymEvalOp handle type.
  typedef Beagle::PointerT<CoSymEvalOp,Beagle::Coev::GPEvaluationOp::Handle>
          Handle;
  //!< CoSymEvalOp bag type.
  typedef Beagle::ContainerT<CoSymEvalOp,Beagle::Coev::GPEvaluationOp::Bag>
          Bag;

  explicit CoSymEvalOp(Beagle::string inName="CoSymEvalOp");

  virtual void evaluateSets(Beagle::Coev::GPEvaluationOp::EvalSetVector& ioSets);
  virtual void makeSets(Beagle::Individual::Bag& ioIndivBag, Beagle::Context::Handle ioContext) =0;

protected:

  double evaluateRMS(Beagle::GA::FloatVector& inTrainSet,
                     Beagle::GP::Individual& inExpression,
                     Beagle::GP::Context& ioContext) const;

};

#endif // CoSymEvalOp_hpp
