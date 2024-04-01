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
 *  \file   beagle/GP/Vivarium.hpp
 *  \brief  Definition of the class GP::Vivarium.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.7.2.2 $
 *  $Date: 2007/09/10 18:24:11 $
 */

/*!
 *  \defgroup GPPop GP Population
 *  \ingroup GPF
 *  \brief Genetic programming population components.
 */

#ifndef Beagle_GP_Vivarium_hpp
#define Beagle_GP_Vivarium_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/Vivarium.hpp"
#include "beagle/VivariumAlloc.hpp"
#include "beagle/VivariumAllocT.hpp"
#include "beagle/GP/Deme.hpp"

namespace Beagle {
namespace GP {

/*!
 *  \class Vivarium beagle/GP/Vivarium.hpp "beagle/GP/Vivarium.hpp"
 *  \brief GP vivarium with default constructor, part of the GP framework.
 *  \ingroup GPF
 *  \ingroup GPPop
 */
class Vivarium : public Beagle::Vivarium {

public:

  //! GP::Vivarium allocator type.
  typedef VivariumAllocT<Vivarium,Beagle::Vivarium::Alloc,GP::Deme::Alloc>
          Alloc;
  //! GP::Vivarium handle type.
  typedef PointerT<Vivarium,Beagle::Vivarium::Handle>
          Handle;
  //! GP::Vivarium bag type.
  typedef ContainerT<Vivarium,Beagle::Vivarium::Bag>
          Bag;

           Vivarium();
  explicit Vivarium(GP::Deme::Alloc::Handle inDemeAlloc);
  explicit Vivarium(GP::Deme::Alloc::Handle inDemeAlloc,
                    Stats::Alloc::Handle inStatsAlloc,
                    HallOfFame::Alloc::Handle inHOFAlloc,
                    unsigned int inN=0);
  explicit Vivarium(GP::Tree::Alloc::Handle inGenotypeAlloc);
  explicit Vivarium(GP::Tree::Alloc::Handle inGenotypeAlloc, Fitness::Alloc::Handle inFitnessAlloc);
  virtual ~Vivarium() { }

  /*!
   *  \brief Access to the nth GP deme handle of the GP vivarium (const version).
   *  \param inN Indice of the GP deme handle to get.
   *  \return GP deme handle at the index n.
   */
  inline const GP::Deme::Handle& operator[](unsigned int inN) const
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,size()-1);
    return castHandleT<GP::Deme>( std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::operator[](inN) );
    Beagle_StackTraceEndM("const GP::Deme::Handle& GP::Vivarium::operator[](unsigned int inN) const");
  }

  /*!
   *  \brief Access to the nth GP deme handle of the GP vivarium.
   *  \param inN Indice of the GP deme handle to get.
   *  \return GP deme handle at the index n.
   */
  inline GP::Deme::Handle& operator[](unsigned int inN)
  {
    Beagle_StackTraceBeginM();
    Beagle_UpperBoundCheckAssertM(inN,size()-1);
    return castHandleT<GP::Deme>( std::vector< Pointer,BEAGLE_STLALLOCATOR<Pointer> >::operator[](inN) );
    Beagle_StackTraceEndM("GP::Deme::Handle& GP::Vivarium::operator[](unsigned int inN)");
  }

  /*!
   *  \brief Get the deme data type allocator.
   *  \return An handle to the deme type allocator.
   */
  inline const GP::Deme::Alloc::Handle getTypeAlloc() const
  {
    Beagle_StackTraceBeginM();
    return castHandleT<GP::Deme::Alloc>(Beagle::Vivarium::getTypeAlloc());
    Beagle_StackTraceEndM("const GP::Deme::Alloc::Handle GP::Vivarium::getTypeAlloc() const");
  }

  /*!
   *  \brief Get the deme data type allocator.
   *  \return An handle to the deme type allocator.
   */
  inline GP::Deme::Alloc::Handle getTypeAlloc()
  {
    Beagle_StackTraceBeginM();
    return castHandleT<GP::Deme::Alloc>(Beagle::Vivarium::getTypeAlloc());
    Beagle_StackTraceEndM("GP::Deme::Alloc::Handle GP::Vivarium::getTypeAlloc()");
  }

  /*!
   *  \brief Set the deme data type allocator.
   *  \param inTypeAlloc An handle to the new deme type allocator.
   */
  inline void setTypeAlloc(GP::Deme::Alloc::Handle inTypeAlloc)
  {
    Beagle_StackTraceBeginM();
    Beagle::Vivarium::setTypeAlloc(inTypeAlloc);
    Beagle_StackTraceEndM("void GP::Vivarium::setTypeAlloc(GP::Deme::Alloc::Handle inTypeAlloc)");
  }

};

}
}

#endif // Beagle_GP_Vivarium_hpp
