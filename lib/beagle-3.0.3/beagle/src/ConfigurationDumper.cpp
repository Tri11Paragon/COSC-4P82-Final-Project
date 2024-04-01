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
 *  \file   beagle/src/ConfigurationDumper.cpp
 *  \brief  Source code of class ConfigurationDumper.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.15.2.1 $
 *  $Date: 2007/05/09 01:51:18 $
 */

#include "beagle/Beagle.hpp"

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <fstream>

using namespace Beagle;


/*!
 *  \brief Construct a configuration file dumper.
 *  \param inSystem Reference to the associated system.
 *  \param inEvolver Reference to the associated evolver.
 *  \param inName File name value.
 */
ConfigurationDumper::ConfigurationDumper(System& inSystem, 
										 Evolver& inEvolver,
										 Beagle::string inName) :
  String(inName),
  mSystem(inSystem),
  mEvolver(inEvolver)
{ }


/*!
 *  \brief Read the file name and then dump the configuration in it.
 *  \param inIter XML iterator to read the file name from.
 */
void ConfigurationDumper::read(PACC::XML::ConstIterator inIter)
{
  Beagle_StackTraceBeginM();
  if(!inIter) return;
  if(inIter->getType() != PACC::XML::eString)
    throw Beagle_IOExceptionNodeM(*inIter, "not a string with a filename");
  getWrappedValue() = inIter->getValue().c_str();
  if(getWrappedValue().empty()) return;

  string lFilenameBak = getWrappedValue() + "~";
  std::remove(lFilenameBak.c_str());
  std::rename(getWrappedValue().c_str(), lFilenameBak.c_str());
  mSystem.getRegister().deleteEntry("ec.conf.dump");
  mSystem.getRegister().deleteEntry("ec.conf.file");
  std::ofstream lOFStream(getWrappedValue().c_str());
  std::cerr << "A typical configuration file named \"";
  std::cerr << getWrappedValue() << "\" is created." << std::endl << std::flush;
  PACC::XML::Streamer lStreamer(lOFStream);
  lStreamer.insertHeader();
  lStreamer.openTag("Beagle");
  lStreamer.insertAttribute("version", BEAGLE_VERSION);
  lStreamer.insertComment("Created by a configuration dump");
  lStreamer.insertComment("Evolver: configuration of the algorithm");
  mEvolver.write(lStreamer);
  lStreamer.insertComment("System: setting of the evolution");
  mSystem.write(lStreamer);
  lStreamer.closeTag();
  lOFStream << std::endl;
  lOFStream.close();
  std::cerr << "Exiting..." << std::endl << std::flush;
  exit(0);
  Beagle_StackTraceEndM("void ConfigurationDumper::read(PACC::XML::ConstIterator inIter)");
}

