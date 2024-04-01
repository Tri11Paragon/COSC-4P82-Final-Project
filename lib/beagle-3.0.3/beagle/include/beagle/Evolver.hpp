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
 *  \file   beagle/Evolver.hpp
 *  \brief  Definition of the class Evolver.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.1 $
 *  $Date: 2007/05/09 01:51:10 $
 */

#ifndef Beagle_Evolver_hpp
#define Beagle_Evolver_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/System.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Map.hpp"
#include "beagle/OperatorMap.hpp"
#include "beagle/Vivarium.hpp"
#include "beagle/ConfigurationDumper.hpp"
#include "beagle/UInt.hpp"

namespace Beagle {

/*!
 *  \class Evolver beagle/Evolver.hpp "beagle/Evolver.hpp"
 *  \brief Beagle's basic evolver class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class Evolver : public Object {

public:

  //! Evolver allocator type.
  typedef AllocatorT<Evolver,Object::Alloc>
          Alloc;
  //! Evolver handle type.
  typedef PointerT<Evolver,Object::Handle>
          Handle;
  //! Evolver bag type.
  typedef ContainerT<Evolver,Object::Bag>
          Bag;

           Evolver();
  virtual ~Evolver() { }

  void                   addBootStrapOp(string inName);
  void                   addMainLoopOp(string inName);
  void                   addOperator(Operator::Handle inOperator);
  const Operator::Handle getOperator(string inName) const;
  Operator::Handle       getOperator(string inName);
  virtual void           initialize(System::Handle ioSystem, int& ioArgc, char** ioArgv);
  virtual void           initialize(System::Handle ioSystem, string inConfigFilename);
  virtual void           evolve(Vivarium::Handle ioVivarium);
  virtual void           read(PACC::XML::ConstIterator inIter);
  virtual void           readEvolverFile(string inFilename);
  Operator::Handle       removeOperator(string inName);
  virtual void           write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

  /*!
   *  \brief Return a constant reference to the bootstrap operator set.
   *  \return Bootstrap operator set constant reference.
   */
  inline const Operator::Bag& getBootStrapSet() const
  {
    Beagle_StackTraceBeginM();
    return mBootStrapSet;
    Beagle_StackTraceEndM("const Operator::Bag& Evolver::getBootStrapSet() const");
  }

  /*!
   *  \brief Return a reference to the bootstrap operator set.
   *  \return Bootstrap operator set reference.
   */
  inline Operator::Bag& getBootStrapSet()
  {
    Beagle_StackTraceBeginM();
    return mBootStrapSet;
    Beagle_StackTraceEndM("Operator::Bag& Evolver::getBootStrapSet()");
  }

  /*!
   *  \brief Return a constant reference to the main-loop operator set.
   *  \return Main-loop operator set constant reference.
   */
  inline const Operator::Bag& getMainLoopSet() const
  {
    Beagle_StackTraceBeginM();
    return mMainLoopSet;
    Beagle_StackTraceEndM("const Operator::Bag& Evolver::getMainLoopSet() const");
  }

  /*!
   *  \brief Return a reference to the main-loop operator set.
   *  \return Main-loop operator set reference.
   */
  inline Operator::Bag& getMainLoopSet()
  {
    Beagle_StackTraceBeginM();
    return mMainLoopSet;
    Beagle_StackTraceEndM("Operator::Bag& Evolver::getMainLoopSet()");
  }

  /*!
   *  \return Operators map.
   */
  inline OperatorMap& getOperatorMap()
  {
    Beagle_StackTraceBeginM();
    return mOperatorMap;
    Beagle_StackTraceEndM("OperatorMap& Evolver::getOperatorMap()");
  }

  /*!
   *  \return Operators map.
   */
  inline const OperatorMap& getOperatorMap() const
  {
    Beagle_StackTraceBeginM();
    return mOperatorMap;
    Beagle_StackTraceEndM("const OperatorMap& Evolver::getOperatorMap() const");
  }

protected:
  virtual void addBasicOperators();
  virtual void initializeOperators(System& ioSystem);
  virtual void logWelcomeMessages();
  virtual void parseCommandLine(System& ioSystem, int& ioArgc, char** ioArgv);
  virtual void postInitOperators(System& ioSystem);

  System::Handle              mSystemHandle;  //!< Handle to the used system.
  Operator::Bag               mBootStrapSet;  //!< Bootstrap operator set.
  Operator::Bag               mMainLoopSet;   //!< Main-loop operator set.
  OperatorMap                 mOperatorMap;   //!< Operator map.
  ConfigurationDumper::Handle mConfigDumper;  //!< Configuration dumper to file parameter.
  String::Handle              mFileName;      //!< Configuration file name.
  UIntArray::Handle           mPopSize;       //!< Population size for the evolution.

};

}

#endif // Beagle_Evolver_hpp
