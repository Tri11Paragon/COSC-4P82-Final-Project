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
 *  \file   beagle/AssertException.hpp
 *  \brief  Definition of class AssertException.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.8.2.2 $
 *  $Date: 2007/09/10 18:24:11 $
 */

#ifndef Beagle_AssertException_hpp
#define Beagle_AssertException_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/TargetedException.hpp"


/*!
 *  \def   Beagle_AssertM
 *  \brief Assert that the condition is true. If not, throw a Beagle::AssertException.
 *  \param COND Condition to test, to assert.
 *  \throw Beagle::AssertException The condition is false.
 *  \ingroup Except
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_AssertM(COND) \
  if( !(COND) ) throw Beagle::AssertException("Assertion failed.",__FILE__,__LINE__)
#else // BEAGLE_NDEBUG
#define Beagle_AssertM(COND)
#endif // BEAGLE_NDEBUG


/*!
 *  \def   Beagle_NonNullPointerAssertM
 *  \brief Assert that the pointer is non null. If not, throw a Beagle::AssertException.
 *  \param PTR Pointer to test.
 *  \throw Beagle::AssertException If the pointer is NULL.
 *  \ingroup Except
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_NonNullPointerAssertM(PTR) \
  if( !(PTR) ) throw Beagle::AssertException("Pointer asserted is NULL.",__FILE__,__LINE__)
#else  // BEAGLE_NDEBUG
#define Beagle_NonNullPointerAssertM(PTR)
#endif // BEAGLE_NDEBUG


/*!
 *  \def   Beagle_BoundCheckAssertM
 *  \brief Assert that the bound of a value.
 *  \param VALUE Value to test.
 *  \param MIN   Minimal value accepted.
 *  \param MAX   Maximal value accepted.
 *  \throw Beagle::AssertException If the value is out of bound.
 *  \ingroup Except
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_BoundCheckAssertM(VALUE,MIN,MAX) \
  if( (VALUE) < (MIN) ) { \
    std::ostringstream lOSS; \
    lOSS << "Out of bound assertion: Value (" << VALUE << ") is less than the minimum accepted (" << MIN << ")."; \
    throw Beagle::AssertException( \
      lOSS.str().c_str(), \
     __FILE__,__LINE__); \
  } \
  if( (VALUE) > (MAX) ) { \
    std::ostringstream lOSS; \
    lOSS << "Out of bound assertion: Value (" << VALUE << ") is more than the maximum accepted (" << MAX << ")."; \
    throw Beagle::AssertException( \
      lOSS.str().c_str(), \
     __FILE__,__LINE__); \
  }
#else  // BEAGLE_NDEBUG
#define Beagle_BoundCheckAssertM(VALUE,MIN,MAX)
#endif // BEAGLE_NDEBUG


/*!
 *  \def   Beagle_UpperBoundCheckAssertM
 *  \brief Assert the upper bound of an index value.
 *  \param INDEX Index value to test.
 *  \param UPBOUND Upper bound.
 *  \throw Beagle::AssertException If the value is out of bound.
 *  \ingroup Except
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_UpperBoundCheckAssertM(INDEX,UPBOUND) \
  if( (INDEX) > (UPBOUND) ) { \
    std::ostringstream lOSS; \
    lOSS << "Out of bound assertion: Index (" << INDEX << ") is more than the upper bound accepted (" << UPBOUND << ")."; \
    throw Beagle::AssertException( \
      lOSS.str(), \
     __FILE__,__LINE__); \
  }
#else  // BEAGLE_NDEBUG
#define Beagle_UpperBoundCheckAssertM(INDEX,UPBOUND)
#endif // BEAGLE_NDEBUG


namespace Beagle {

// Forward declarations.
template <class T, class BaseType> class AllocatorT;
template <class T, class BaseType> class PointerT;
template <class T, class BaseType> class ContainerT;

/*!
 *  \class AssertException beagle/AssertException.hpp "beagle/AssertException.hpp"
 *  \brief Beagle exception indicating an badly asserted condition.
 *  \ingroup OOF
 *  \ingroup Except
 */
class AssertException : public TargetedException {

public:

  //! AssertException allocator type.
  typedef AllocatorT<AssertException,TargetedException::Alloc>
          Alloc;
  //! AssertException handle type.
  typedef PointerT<AssertException,TargetedException::Handle>
          Handle;
  //! AssertException bag type.
  typedef ContainerT<AssertException,TargetedException::Bag>
          Bag;

  explicit AssertException(string  inMessage="",
                           string  inFileName="",
                           unsigned int inLineNumber=0);
  virtual ~AssertException() throw() { }

  virtual const char* getExceptionName() const throw();

};

}

#endif // Beagle_AssertException_hpp



