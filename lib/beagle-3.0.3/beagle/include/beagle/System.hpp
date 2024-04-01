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
 *  \file   beagle/System.hpp
 *  \brief  Definition of the class System.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.16.2.1 $
 *  $Date: 2007/05/09 01:51:17 $
 */

/*!
 *  \defgroup Sys Evolutionary System
 *  \ingroup ECF
 *  \brief Evolutionary system, part of the generic EC framework.
 *
 *  The internal system is the structure that holds and gives access to the state 
 *  of the genetic engine. These structures are fundamental, because they are used as entry points to 
 *  the data of the evolution.
 *
 *  During the evolutionary processes, a context gives the current state of the evolution. A basic,
 *  general context is implemented in class Beagle::Context. It provides some essential contextual 
 *  informations such as the current deme, individual, genotype and generation. For a specific EC
 *  algorithm, a specific context could be used. For example, a GP specific context is defined, 
 *  Beagle::GP::Context, which contains the call stack with some other GP specific informations. An 
 *  Open BEAGLE context is similar to the execution context of a computer that contains the different
 *  registers, counters and pointers to the actual state of the machine.
 *
 *  Given that the parameters of Open BEAGLE are distributed in the appropriate objects, an agent is 
 *  implemented to take into account these parameter: the register. All the variables that are 
 *  considered as parameters should be registered by giving the reference (object handle) of the 
 *  parameter with the associated namespace and tag. The class Beagle::Register can be seen as a 
 *  centralized database from which any entity could dynamically add, delete, access or modify 
 *  parameters. The register is also responsible of the interpretation of the configuration file and 
 *  set the parameters in consequence. The register also contains reference to every evolution 
 *  operators available.
 *
 *  All the output messages given to the user pass by the logger. It consists of an interface with
 *  the user, that receives all messages, associated with a type, the class name associated to the
 *  message and an output level, and output them in a given device if the used log level allow it.
 *  This is very interesting if a user want, for example, to use Open BEAGLE into a broader system
 *  using a graphical user interface. In such case, the user only need to define his own specialized
 *  logger that will intercept the messages and log them into the desired device, for example and
 *  specific graphical windows. There is  actually one specialized logger, Beagle::LoggerXML, that
 *  log messages in a XML format into a file or the console (the STDOUT). The other very interesting
 *  aspect of the logger is the possibility to choose the log level desired. The messages outputed
 *  are classified into eight categories:
 *  - Nothing (0): Log nothing
 *  - Basic (1): Log essential informations
 *  - Stats (2): Log evolution statistics 
 *  - Info (3): Log general informations (default)
 *  - Detailed (4): Log details on operations
 *  - Trace (5): Log trace of the algorithms 
 *  - Verbose (6): Log details on everything (disabled in optimization mode)
 *  - Debug (7): Debug (enabled only in full debug mode)
 *
 *  The registered parameter \c lg.log.level allows the user to select the desired log
 *  level. For example, if the user choose the log level \e info (3), all messages classified in
 *  categories \e basic (1), \e stats (2), and \e info (3) will be outputed. Log levels
 *  \e basic (1) to \e detailed (4) are appropriate for monitoring evolutions, log levels
 *  \e detailed (4) and \e trace (5) are good to get familiar with the internal functioning of 
 *  Open BEAGLE, while log levels \e trace (5) to \e debug (7) may be useful to debug an user
 *  application, or the framework.
 *
 *  Class Beagle::Randomizer} provides a common pseudo-random numbers generator. The randomizer 
 *  comprises two parameters that are registered (in the register): the internal state and the seed. 
 *  The internal state is an integer that give the actual state of the randomizer. This value change 
 *  at every generation of a random number. This value is useful mainly to restart an evolution from a
 *  milestone. The seed is the value of the first state of the randomizer. By default, the seed is
 *  initiated to the timer value. The seed can be set by the user to reproduce an evolution.
 *
 *  The entry point to these resources is given by an extensible central repository: the system. This 
 *  is simply an entry point that possessed some vital resources of the EC engine: a context 
 *  allocator, a reference to the register, a reference to the logger, and a reference to the 
 *  randomizer. This general system is implemented in the class Beagle::System. The system is 
 *  accessible by any context of a given evolution.
 *
 */


#ifndef Beagle_System_hpp
#define Beagle_System_hpp

#include <set>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Genotype.hpp"
#include "beagle/Individual.hpp"
#include "beagle/HallOfFame.hpp"
#include "beagle/Deme.hpp"
#include "beagle/Randomizer.hpp"
#include "beagle/Register.hpp"
#include "beagle/Logger.hpp"
#include "beagle/Component.hpp"
#include "beagle/ComponentMap.hpp"

namespace Beagle {

// Forward declaration.
class Context;

/*!
 *  \class System beagle/System.hpp "beagle/System.hpp"
 *  \brief Evolutionary system.
 *  \ingroup ECF
 *  \ingroup Sys
 */
class System : public ComponentMap {

public:

  //! System allocator type.
  typedef AllocatorT<System,ComponentMap::Alloc>
          Alloc;
  //! System handle type.
  typedef PointerT<System,ComponentMap::Handle>
          Handle;
  //! System bag type.
  typedef ContainerT<System,ComponentMap::Bag>
          Bag;

           System();
  explicit System(Allocator::Handle  inContextAllocator);
  explicit System(Allocator::Handle  inContextAllocator,
                  Randomizer::Handle inRandomizer,
                  Register::Handle   inRegister,
                  Logger::Handle     inLogger);
  virtual ~System() { }

  virtual void                    addComponent(Component::Handle inComponent);
  virtual const Component::Handle getComponent(const string inName) const;
  virtual Component::Handle       getComponent(const string inName);
  virtual const Component::Handle haveComponent(const string inName) const;
  virtual Component::Handle       haveComponent(const string inName);
  virtual void                    initialize(int& ioArgc, char** ioArgv);
  virtual void                    initialize(string inConfigFileName);
  virtual void                    postInit();
  virtual void                    read(PACC::XML::ConstIterator inIter);
  virtual void                    write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

  /*!
   *  \brief Get a constant reference to the context allocator of the system.
   *  \return Constant reference to the context allocator.
   */
  inline const Allocator& getContextAllocator() const
  {
    Beagle_StackTraceBeginM();
    return *mContextAllocator;
    Beagle_StackTraceEndM("const Allocator& System::getContextAllocator() const");
  }

  /*!
   *  \brief Get reference to the context allocator of the system.
   *  \return Reference to the context allocator.
   */
  inline Allocator& getContextAllocator()
  {
    Beagle_StackTraceBeginM();
    return *mContextAllocator;
    Beagle_StackTraceEndM("Allocator& System::getContextAllocator()");
  }

  /*!
   *  \brief Get a handle to the context allocator of the system.
   *  \return Handle to the context allocator.
   */
  inline Allocator::Handle getContextAllocatorHandle()
  {
    Beagle_StackTraceBeginM();
    return mContextAllocator;
    Beagle_StackTraceEndM("Allocator::Handle System::getContextAllocatorHandle()");
  }


  /*!
   *  \brief Return a constant reference to the logger.
   *  \return Logger constant reference.
   */
  inline const Logger& getLogger() const
  {
    Beagle_StackTraceBeginM();
    return *mLogger;
    Beagle_StackTraceEndM("const Logger& System::getLogger() const");
  }

  /*!
   *  \brief Return a reference to the logger.
   *  \return Logger reference.
   */
  inline Logger& getLogger()
  {
    Beagle_StackTraceBeginM();
    return *mLogger;
    Beagle_StackTraceEndM("Logger& System::getLogger()");
  }

  /*!
   *  \brief Return a handle to the logger.
   *  \return Logger handle.
   */
  inline Logger::Handle getLoggerHandle()
  {
    Beagle_StackTraceBeginM();
    return mLogger;
    Beagle_StackTraceEndM("Logger::Handle System::getLoggerHandle()");
  }

  /*!
   *  \brief Return a constant reference to the randomizer.
   *  \return Evolution randomizer constant reference.
   */
  inline const Randomizer& getRandomizer() const
  {
    Beagle_StackTraceBeginM();
    return *mRandomizer;
    Beagle_StackTraceEndM("const Randomizer& System::getRandomizer() const");
  }

  /*!
   *  \brief Return a reference to the randomizer.
   *  \return Evolution randomizer reference.
   */
  inline Randomizer& getRandomizer()
  {
    Beagle_StackTraceBeginM();
    return *mRandomizer;
    Beagle_StackTraceEndM("Randomizer& System::getRandomizer()");
  }

  /*!
   *  \brief Return a handle to the randomizer.
   *  \return Evolution randomizer handle.
   */
  inline Randomizer::Handle getRandomizerHandle()
  {
    Beagle_StackTraceBeginM();
    return mRandomizer;
    Beagle_StackTraceEndM("Randomizer::Handle System::getRandomizerHandle()");
  }

  /*!
   *  \brief Return a constant reference to the register.
   *  \return Evolution register constant reference.
   */
  inline const Register& getRegister() const
  {
    Beagle_StackTraceBeginM();
    return *mRegister;
    Beagle_StackTraceEndM("const Register& System::getRegister() const");
  }

  /*!
   *  \brief Return a reference to the register.
   *  \return Evolution register reference.
   */
  inline Register& getRegister()
  {
    Beagle_StackTraceBeginM();
    return *mRegister;
    Beagle_StackTraceEndM("Register& System::getRegister()");
  }

  /*!
   *  \brief Return a handle to the register.
   *  \return Evolution register handle.
   */
  inline Register::Handle getRegisterHandle()
  {
    Beagle_StackTraceBeginM();
    return mRegister;
    Beagle_StackTraceEndM("Register::Handle System::getRegisterHandle()");
  }

protected:

  Allocator::Handle   mContextAllocator; //!< Allocator of context.
  Randomizer::Handle  mRandomizer;       //!< Randomizer of the system.
  Register::Handle    mRegister;         //!< Register of the system.
  Logger::Handle      mLogger;           //!< Logger of the system.

};

}

#endif // Beagle_System_hpp
