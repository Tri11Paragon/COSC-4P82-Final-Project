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
 *  \file   beagle/src/System.cpp
 *  \brief  Source code of class System.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  \author Matthew Walker
 *  $Revision: 1.19.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */

#include "beagle/Beagle.hpp"

using namespace std;
using namespace Beagle;


/*!
 *  \brief Construct a new system with the default context allocator, randomizer, register,
 *    and logger.
 */
System::System() :
  mContextAllocator(new Context::Alloc),
  mRandomizer(new Randomizer),
  mRegister(new Register),
  mLogger(new LoggerXML)
{
  Beagle_StackTraceBeginM();
  addComponent(mRandomizer);
  addComponent(mRegister);
  addComponent(mLogger);
  Beagle_StackTraceEndM("System::System()");
}


/*!
 *  \brief Construct a new system with a given context allocator and default randomizer,
 *    register, and logger.
 *  \param inContextAllocator Handle to the context allocator used.
 */
System::System(Allocator::Handle inContextAllocator) :
  mContextAllocator(castHandleT<Context::Alloc>(inContextAllocator)),
  mRandomizer(new Randomizer),
  mRegister(new Register),
  mLogger(new LoggerXML)
{
  Beagle_StackTraceBeginM();
  addComponent(mRandomizer);
  addComponent(mRegister);
  addComponent(mLogger);
  Beagle_StackTraceEndM("System::System(Allocator::Handle inContextAllocator)");
}


/*!
 *  \brief Construct a new system with a given context allocator, randomizer and register.
 *  \param inContextAllocator Handle to the context allocator used.
 *  \param inRandomizer Handle to the randomizer used.
 *  \param inRegister Handle to the register used.
 *  \param inLogger Handle to the logger used.
 */
System::System(Allocator::Handle  inContextAllocator,
               Randomizer::Handle inRandomizer,
               Register::Handle   inRegister,
               Logger::Handle     inLogger) :
  mContextAllocator(castHandleT<Context::Alloc>(inContextAllocator)),
  mRandomizer(inRandomizer),
  mRegister(inRegister),
  mLogger(inLogger)
{
  Beagle_StackTraceBeginM();
  addComponent(mRandomizer);
  addComponent(mRegister);
  addComponent(mLogger);
  Beagle_StackTraceEndM("System::System(Allocator::Handle inContextAllocator, Randomizer::Handle inRandomizer, Register::Handle inRegister, Logger::Handle inLogger)");
}


/*!
 *  \brief Add a new component to the system.
 *  \param inComponent The component to add.
 */
void System::addComponent(Component::Handle inComponent)
{
  Beagle_StackTraceBeginM();
  // Check component is valid and hasn't already been added.
  Beagle_NonNullPointerAssertM(inComponent);
  if(find(inComponent->getName())!=end())
    throw Beagle_ObjectExceptionM(string("A component named \"")+inComponent->getName()+
                                  string("\" is already in the system."));

  // Add component.
  Beagle_LogDetailedM(
    getLogger(),
    "system", "Beagle::System",
    string("Adding component named \"")+inComponent->getName()+string("\" into the system")
  );
  (*this)[inComponent->getName()] = inComponent;
  Beagle_StackTraceEndM("void System::addComponent(Component::Handle inComponent)");
}


/*!
 *  \brief Get a component by name.
 *  \param inName The name of the component to get.
 *  \return A constant component.
 */
const Beagle::Component::Handle System::getComponent(const Beagle::string inName) const
{
  Beagle_StackTraceBeginM();
  const ComponentMap::const_iterator lItr = find(inName);
  if (lItr == end()) {
    throw Beagle_RunTimeExceptionM(string("Could not retrieve component named \"")+inName+
      string("\". Either the component is not installed, or the name you have provided is incorrect."));
  }
  return castHandleT<Component>(lItr->second);
  Beagle_StackTraceEndM("const Beagle::Component::Handle System::getComponent(const string inName) const");
}


/*!
 *  \brief Get a component by name.
 *  \param inName The name of the component to get.
 *  \return A component.
 */
Beagle::Component::Handle System::getComponent(const Beagle::string inName)
{
  Beagle_StackTraceBeginM();
  ComponentMap::iterator lItr = find(inName);
  if (lItr == end()) {
    throw Beagle_RunTimeExceptionM(string("Could not retrieve component named \"")+inName+
    string("\".  Either the component is not installed, or the name you have provided is incorrect."));
  }
  return castHandleT<Component>(lItr->second);
  Beagle_StackTraceEndM("Beagle::Component::Handle System::getComponent(const string inName)");
}


/*!
 *  \brief Check if the component named inName exists in the System.
 *  \param inName The name of the component.
 *  \return If the component exists then it returns the component,
 *  otherwise it returns a NULL Handle.
 */
const Beagle::Component::Handle System::haveComponent(const Beagle::string inName) const
{
  Beagle_StackTraceBeginM();
  const ComponentMap::const_iterator lItr = find(inName);
  if (lItr == end()) return Component::Handle(NULL);
  return castHandleT<Component>(lItr->second);
  Beagle_StackTraceEndM("const Beagle::Component::Handle System::haveComponent(const string inName) const");
}


/*!
 *  \brief Check if the component named inName exists in the System.
 *  \param inName The name of the component.
 *  \return If the component exists then it returns a constant
 *  component, otherwise it returns a NULL Handle.
 */
Beagle::Component::Handle System::haveComponent(const Beagle::string inName)
{
  Beagle_StackTraceBeginM();
  const ComponentMap::const_iterator lItr = find(inName);
  if (lItr == end()) return Component::Handle(NULL);
  return castHandleT<Component>(lItr->second);
  Beagle_StackTraceEndM("Beagle::Component::Handle System::haveComponent(const string inName)");
}


/*!
 *  \brief Initialize the system.
 *  \param ioArgc Number of elements on the command-line.
 *  \param ioArgv Element on the command-line.
 */
void System::initialize(int& ioArgc, char** ioArgv)
{
  Beagle_StackTraceBeginM();
  // Initializing the system.
  Beagle_LogDetailedM(
    getLogger(),
    "system", "Beagle::System",
    "Initializing system"
  );

  // Initialize the components.
  for(ComponentMap::iterator lItr=begin(); lItr!=end(); ++lItr) {
    Component::Handle lComponent = castHandleT<Component>(lItr->second);
    Beagle_LogDetailedM(
      getLogger(),
      "system", "Beagle::System",
      string("Initializing component named \"")+lComponent->getName()+string("\"")
    );
    lComponent->initialize(*this);
  }

  // Parsing the command-line.
  Beagle_LogTraceM(
    (*mLogger),
    "system", "Beagle::System",
    "Parsing the command-line"
  );
  mRegister->parseCommandLine(*this, ioArgc, ioArgv);
  Beagle_StackTraceEndM("void System::initialize(int& ioArgc, char** ioArgv)");
}


/*!
 *  \brief Initialize the system.
 *  \param inConfigFileName Name of the filename containing the system configuration.
 */
void System::initialize(Beagle::string inConfigFileName)
{
  Beagle_StackTraceBeginM();
  // Initializing the system.
  Beagle_LogDetailedM(
    getLogger(),
    "system", "Beagle::System",
    "Initializing system"
  );

  // Initialize the components.
  for(iterator lItr=begin(); lItr!=end(); ++lItr) {
    Component::Handle lComponent = castHandleT<Component>(lItr->second);
    Beagle_LogDetailedM(
      getLogger(),
      "system", "Beagle::System",
      string("Initializing component named \"")+lComponent->getName()+string("\"")
    );
    lComponent->initialize(*this);
  }

  // Read configuration file.
  if(inConfigFileName.empty()==false) {
    mRegister->readParametersFile(inConfigFileName, *this);
  }
  Beagle_StackTraceEndM("void System::initialize(string inConfigFileName)");
}


/*!
 *  \brief Post-initialization hook of the system.
 */
void System::postInit()
{
  Beagle_StackTraceBeginM();
  Beagle_LogDetailedM(
    getLogger(),
    "system", "Beagle::System",
    "Post-initializing system"
  );

  // Post-initialize the components.
  for(ComponentMap::iterator lItr=begin(); lItr!=end(); ++lItr) {
    Component::Handle lComponent = castHandleT<Component>(lItr->second);
    Beagle_LogDetailedM(
      getLogger(),
      "system", "Beagle::System",
      string("Post-initializing component named \"")+lComponent->getName()+string("\"")
    );
    lComponent->postInit(*this);
  }
  Beagle_StackTraceEndM("void System::postInit()");
}


/*!
 *  \brief Read evolutionary system from XML node.
 *  \param inIter XML iterator used to read system.
 */
void System::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="System"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <System> expected!");

  // First reading pass, read randomizer, register and logger.
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() == PACC::XML::eData) {
      if(lChild->getValue() == "Randomizer")    mRandomizer->readWithSystem(lChild, *this);
      else if(lChild->getValue() == "Register") mRegister->readWithSystem(lChild, *this);
      else if(lChild->getValue() == "Logger")   mLogger->readWithSystem(lChild, *this);
    }
  }

  // Second reading pass, read other components.
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() == PACC::XML::eData) {
      if((lChild->getValue() != "Randomizer") && 
         (lChild->getValue() != "Register") &&
         (lChild->getValue() != "Logger")) {
        if(find(lChild->getValue().c_str())==end())
          throw Beagle_ObjectExceptionM(string("The component named \"")+
                                        string(lChild->getValue().c_str())+
                                        string("\" is not present in the system."));
        (*this)[lChild->getValue().c_str()]->readWithSystem(lChild, *this);
      }
    }
  }
  Beagle_StackTraceEndM("void System::read(PACC::XML::ConstIterator inIter)");
}


/*!
 *  \brief Write system into a XML streamer.
 *  \param ioStreamer XML streamer to write system into.
 *  \param inIndent Whether XML output should be indented.
 */
void System::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("System", inIndent);
  for(ComponentMap::const_iterator lItr=begin(); lItr!=end(); ++lItr) {
    const Component::Handle lComponent = castHandleT<const Component>(lItr->second);;
    lComponent->write(ioStreamer, inIndent);
  }
  ioStreamer.closeTag(); // System
  Beagle_StackTraceEndM("void System::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}

