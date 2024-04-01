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
 *  \file   TrainSetEvalOp.hpp
 *  \brief  Declaration of the class TrainSetEvalOp.
 *  \author Jiachuan Wang <jiacwang@ecs.umass.edu>
 *  \author Christian Gagne <cgagne@gmail.com>
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#ifndef TrainSetEvalOp_hpp
#define TrainSetEvalOp_hpp

#include "beagle/Coev.hpp"
#include "CoSymEvalOp.hpp"
#include <vector>


class TrainSetEvalOp : public CoSymEvalOp {

public:

  //! TrainSetEvalOp allocator type.
  typedef Beagle::AllocatorT<TrainSetEvalOp,CoSymEvalOp::Alloc>
          Alloc;
  //!< TrainSetEvalOp handle type.
  typedef Beagle::PointerT<TrainSetEvalOp,CoSymEvalOp::Handle>
          Handle;
  //!< TrainSetEvalOp bag type.
  typedef Beagle::ContainerT<TrainSetEvalOp,CoSymEvalOp::Bag>
          Bag;

  explicit TrainSetEvalOp();

  virtual void makeSets(Beagle::Individual::Bag& ioIndivBag,
                        Beagle::Context::Handle ioContext);

protected:

  Beagle::Individual::Handle mLastGenBestIndividual;   //!< Copy of last gen. best individual.

};

#endif // TrainSetEvalOp_hpp
