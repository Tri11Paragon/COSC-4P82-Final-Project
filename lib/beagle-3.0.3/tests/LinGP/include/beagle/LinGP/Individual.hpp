/*
 *  Open BEAGLE
 *  Copyright (C) 2001-2004 by Christian Gagne and Marc Parizeau
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
 *  Universite Laval, Quebec, Canada, G1V 4J7
 *  http://vision.gel.ulaval.ca
 *
 */

/*!
 *  \file   beagle/LinGP/Individual.hpp
 *  \brief  Definition of the class LinGP::Individual.
 *  \author Christian Gagne <cgagne@gel.ulaval.ca>
 *  \author Alain-Daniel Bourdage <adb@btc.bm>
 *  $Revision: 1.1.2.1 $
 *  $Date: 2007/09/10 18:24:21 $
 */

#ifndef Beagle_LinGP_Individual_hpp
#define Beagle_LinGP_Individual_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Individual.hpp"
#include "beagle/IndividualAllocT.hpp"
#include "beagle/LinGP/Program.hpp"

namespace Beagle {
namespace LinGP {

/*!
 *  \class Individual beagle/LinGP/Individual.hpp "beagle/LinGP/Individual.hpp"
 *  \brief An individual inherits from a uniform bag of genotypes and has a fitness member.
 *  \ingroup LinGPF
 *  \ingroup LinGPPop
 */
class Individual : public Beagle::Individual {

public:

  //! LinGP::Individual allocator type.
  typedef IndividualAllocT<LinGP::Individual,Beagle::Individual::Alloc,LinGP::Program::Alloc>
          Alloc;
  //! LinGP::Individual handle type.
  typedef PointerT<LinGP::Individual,Beagle::Individual::Handle>
          Handle;
  //! LinGP::Individual bag type.
  typedef ContainerT<LinGP::Individual,Beagle::Individual::Bag>
          Bag;

           Individual();
  explicit Individual(LinGP::Program::Alloc::Handle inGenotypeAlloc);
  explicit Individual(LinGP::Program::Alloc::Handle inGenotypeAlloc,
                      Fitness::Alloc::Handle inFitnessAlloc,
                      size_type inN=0);
  virtual ~Individual() { }

  void run(LinGP::Context& ioContext);

  /*!
   *  \brief Acces to a constant handle the nth Program of the LinGP::Individual.
   *  \param inN Indice of the LinGP::Program to get
   *  \return Constant pointer (LinGP::Program::Handle) at the indice N.
   */
  inline const LinGP::Program::Handle& operator[](unsigned int inN) const
  {
    Beagle_UpperBoundCheckAssertM(inN,size()-1);
    return castHandleT<LinGP::Program>(std::vector<Pointer>::operator[](inN));
  }

  /*!
   *  \brief Acces to an handle the nth Program of the LinGP::Individual.
   *  \param inN Indice of the LinGP::Program to get.
   *  \return Pointer (LinGP::Program::Handle) at the indice N.
   */
  inline LinGP::Program::Handle& operator[](unsigned int inN)
  {
    Beagle_UpperBoundCheckAssertM(inN,size()-1);
    return castHandleT<LinGP::Program>(std::vector<Pointer>::operator[](inN));
  }

  /*!
   *  \brief Get the program data type allocator.
   *  \return An handle to the program type allocator.
   */
  inline const LinGP::Program::Alloc::Handle getTypeAlloc() const
  {
    return castHandleT<LinGP::Program::Alloc>(Beagle::Individual::getTypeAlloc());
  }

  /*!
   *  \brief Get the program data type allocator.
   *  \return An handle to the program type allocator.
   */
  inline LinGP::Program::Alloc::Handle getTypeAlloc()
  {
    return castHandleT<LinGP::Program::Alloc>(Beagle::Individual::getTypeAlloc());
  }

  /*!
   *  \brief Set the program data type allocator.
   *  \param inTypeAlloc An handle to the new program type allocator.
   */
  inline void setTypeAlloc(LinGP::Program::Alloc::Handle inTypeAlloc)
  {
    Beagle::Individual::setTypeAlloc(inTypeAlloc);
  }

};

}
}

#endif // Beagle_LinGP_Individual_hpp
