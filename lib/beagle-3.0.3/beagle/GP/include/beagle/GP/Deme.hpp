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
 *  \file   beagle/GP/Deme.hpp
 *  \brief  Definition of the class GP::Deme.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.2 $
 *  $Date: 2007/09/10 18:24:11 $
 */

#ifndef Beagle_GP_Deme_hpp
#define Beagle_GP_Deme_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Deme.hpp"
#include "beagle/DemeAlloc.hpp"
#include "beagle/DemeAllocT.hpp"
#include "beagle/GP/Individual.hpp"

namespace Beagle {
namespace GP {


// Forward declaration
class Context;


/*!
 *  \class Deme beagle/GP/Deme.hpp "beagle/GP/Deme.hpp"
 *  \brief A GP deme is a population of GP individual.
 *  \ingroup GPF
 *  \ingroup GPPop
 */
class Deme : public Beagle::Deme {

public:

  //! GP::Deme allocator type.
  typedef DemeAllocT<Deme,Beagle::Deme::Alloc,GP::Individual::Alloc>
          Alloc;
  //! GP::Deme handle type.
  typedef PointerT<Deme,Beagle::Deme::Handle>
          Handle;
  //! GP::Deme bag type.
  typedef ContainerT<Deme,Beagle::Deme::Bag>
          Bag;

           Deme();
  explicit Deme(GP::Individual::Alloc::Handle inIndividualAlloc);
  explicit Deme(GP::Individual::Alloc::Handle inIndividualAlloc,
                Stats::Alloc::Handle inStatsAlloc,
                HallOfFame::Alloc::Handle inHOFAlloc,
                unsigned int inN=0);
  explicit Deme(GP::Tree::Alloc::Handle inGenotypeAlloc);
  explicit Deme(GP::Tree::Alloc::Handle inGenotypeAlloc, Fitness::Alloc::Handle inFitnessAlloc);
  virtual ~Deme() { }

  /*!
   *  \brief Access to the nth GP individual handle of the GP deme (const version).
   *  \param inN Indice of the GP individual handle to get.
   *  \return GP individual handle at the index n.
   */
  inline const GP::Individual::Handle& operator[](unsigned int inN) const
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,size()-1);
    return castHandleT<GP::Individual>(std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::operator[](inN));
    Beagle_StackTraceEndM("const GP::Individual::Handle& GP::Deme::operator[](unsigned int inN) const");
  }

  /*!
   *  \brief Access to the nth GP individual handle of the GP deme.
   *  \param inN Indice of the GP individual handle to get.
   *  \return GP individual handle at the index n.
   */
  inline GP::Individual::Handle& operator[](unsigned int inN)
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,size()-1);
    return castHandleT<GP::Individual>(std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::operator[](inN));
    Beagle_StackTraceEndM("GP::Individual::Handle& GP::Deme::operator[](unsigned int inN)");
  }

  /*!
   *  \brief Get the individual data type allocator.
   *  \return An handle to the individual type allocator.
   */
  inline const GP::Individual::Alloc::Handle getTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return castHandleT<GP::Individual::Alloc>(Beagle::Deme::getTypeAlloc());
    Beagle_StackTraceEndM("const GP::Individual::Alloc::Handle GP::Deme::getTypeAlloc() const");
  }

  /*!
   *  \brief Get the individual data type allocator.
   *  \return An handle to the individual type allocator.
   */
  inline GP::Individual::Alloc::Handle getTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return castHandleT<GP::Individual::Alloc>(Beagle::Deme::getTypeAlloc());
    Beagle_StackTraceEndM("GP::Individual::Alloc::Handle GP::Deme::getTypeAlloc()");
  }

  /*!
   *  \brief Set the individual data type allocator.
   *  \param inTypeAlloc An handle to the new individual type allocator.
   */
  inline void setTypeAlloc(GP::Individual::Alloc::Handle inTypeAlloc)
  {
    Beagle_StackTraceBeginM();
    Beagle::Deme::setTypeAlloc(inTypeAlloc);
    Beagle_StackTraceEndM("");
  }

};

}
}

#endif // Beagle_GP_Deme_hpp
