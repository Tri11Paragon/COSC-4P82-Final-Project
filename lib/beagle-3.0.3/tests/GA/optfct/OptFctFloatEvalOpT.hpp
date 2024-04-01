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
 *  \file   OptFctFloatEvalOp.hpp
 *  \brief  Definition of the type OptFctFloatEvalOp.
 *  \author Christian Gagne
 *  $Revision: 1.3.2.1 $
 *  $Date: 2007/05/11 19:13:09 $
 */

#ifndef OptFctFloatEvalOp_hpp
#define OptFctFloatEvalOp_hpp

#include "beagle/GA.hpp"


/*!
 *  \class OptFctFloatEvalOpT OptFctFloatEvalOpT.hpp "OptFctFloatEvalOpT.hpp"
 *  \brief Evaluation class operator for the optimisation with float vector GA.
 */
template <class T>
class OptFctFloatEvalOpT : public Beagle::EvaluationOp {

public:

  //! OptFctFloatEvalOpT allocator type.
  typedef Beagle::AllocatorT< OptFctFloatEvalOpT<T>,Beagle::EvaluationOp::Alloc >
          Alloc;
  //!< OptFctFloatEvalOpT handle type.
  typedef Beagle::PointerT< OptFctFloatEvalOpT<T>,Beagle::EvaluationOp::Handle >
          Handle;
  //!< OptFctFloatEvalOpT bag type.
  typedef Beagle::ContainerT< OptFctFloatEvalOpT<T>,Beagle::EvaluationOp::Bag >
          Bag;

  explicit OptFctFloatEvalOpT(std::string inName="OptFctFloatEvalOpT");
  virtual ~OptFctFloatEvalOpT() { }

  virtual Beagle::Fitness::Handle evaluate(Beagle::Individual& inIndividual,
                                           Beagle::Context& ioContext);

};


/*!
 *  \brief Construct an evaluation operator for the optimization with float vector GA.
 *  \param inName Operator name.
 */
template <class T>
OptFctFloatEvalOpT<T>::OptFctFloatEvalOpT(std::string inName) :
  Beagle::EvaluationOp(inName)
{ }


/*!
 *  \brief Evaluate the fitness of an individual for real-valued optimization function.
 *  \param inIndividual Individual to use for fitness evaluation.
 *  \param ioContext Evolutionary context.
 */
template <class T>
Beagle::Fitness::Handle OptFctFloatEvalOpT<T>::evaluate(Beagle::Individual& inIndividual,
                                                        Beagle::Context& ioContext)
{
  Beagle_AssertM(inIndividual.size() == 1);
  Beagle::GA::FloatVector::Handle lFloatVector =
    Beagle::castHandleT<Beagle::GA::FloatVector>(inIndividual[0]);
  T lFunctor;
  const double lF = lFunctor(*lFloatVector);
  return new Beagle::FitnessSimpleMin(lF);
}


#endif // OptFctFloatEvalOp_hpp
