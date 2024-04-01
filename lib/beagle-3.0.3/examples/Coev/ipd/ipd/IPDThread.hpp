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
 *  \file   IPDThread.hpp
 *  \brief  Definition of the type IPDThread.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.1 $
 *  $Date: 2007/05/09 01:51:22 $
 */
 
#ifndef IPDThread_hpp
#define IPDThread_hpp

#include "beagle/Coev.hpp"
#include "Threading.hpp"
#include <vector>


/*!
 *  \class IPDThread IPDThread.hpp "IPDThread.hpp"
 *  \brief Co-evolution thread class for the iterated prisoner's dilemma.
 *  \ingroup IPD
 */
class IPDThread : public PACC::Threading::Thread {

public: 
  IPDThread(Beagle::string inConfigFilename="ipd.conf");
  ~IPDThread();

protected:

  virtual void main();

  Beagle::string mConfigFilename;   //!< Configuration filename

};

#endif // IPDThread_hpp

