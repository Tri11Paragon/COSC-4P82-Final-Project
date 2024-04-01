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
 *  \file   beagle/Exception.hpp
 *  \brief  Definition of class Exception.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.9.2.1 $
 *  $Date: 2007/05/09 01:51:11 $
 */

/*!
 *  \defgroup Except Exceptions
 *  \ingroup OOF
 *  \brief Open BEAGLE custom exceptions, part of the Object Oriented Foundations.
 *
 *  Open BEAGLE integrates the C++ exceptions mechanism. Keeping it OO, an hierarchy of exceptions are
 *  defined. At the top there is the abstract exception superclass, Beagle::Exception, from which 
 *  every other Open BEAGLE exception classes inherit. This class inherit from the standard
 *  \c std::exception class, which allow catch of Open BEAGLE exceptions in a simple
 *  \c catch(std::exception&) expression.
 *
 */

 
#ifndef Beagle_Exception_hpp
#define Beagle_Exception_hpp

#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"


/*!
 *  \def   Beagle_StackTraceBeginM
 *  \brief Used to declare the start of a section of code that will
 *    appear in an exception's stack trace
 *  \ingroup Except
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_StackTraceBeginM() \
  try {
#else // BEAGLE_NDEBUG
#define Beagle_StackTraceBeginM()
#endif // BEAGLE_NDEBUG


/*!
 *  \def   Beagle_StackTraceEndM
 *  \brief Used to declare the end of a section of code that will
 *    appear in an exception's stack trace
 *  \param NAME Function/method name that is traced.
 *  \ingroup Except
 */
#ifndef BEAGLE_NDEBUG
#define Beagle_StackTraceEndM(NAME) \
  } \
  catch (Beagle::Exception& inException) { \
    inException.pushStackTrace(NAME,__FILE__,__LINE__); \
    throw; \
  }
#else // BEAGLE_NDEBUG
#define Beagle_StackTraceEndM(NAME)
#endif // BEAGLE_NDEBUG


namespace Beagle {

// Forward declarations
template <class T, class BaseType> class AbstractAllocT;
template <class T, class BaseType> class PointerT;
template <class T, class BaseType> class ContainerT;

/*!
 *  \class Exception beagle/Exception.hpp "beagle/Exception.hpp"
 *  \brief Basic Beagle exception class.
 *  \ingroup OOF
 *  \ingroup Except
 */
class Exception : public Object, public std::exception {

public:

  //! Exception allocator type.
  typedef AbstractAllocT<Exception,Object::Alloc>
          Alloc;
  //! Exception handle type.
  typedef PointerT<Exception,Object::Handle>
          Handle;
  //! Exception bag type.
  typedef ContainerT<Exception,Object::Bag>
          Bag;

  explicit Exception(string inMessage="");

  virtual ~Exception() throw() { }

  virtual void explain(std::ostream& ioES=std::cerr) throw();
          void terminate(std::ostream& ioES=std::cerr) throw();
  virtual const char* what() const throw();

  /*!
   *  \brief  Getting the actual name of a Beagle exception.
   *  \return Exception name.
   */
  virtual const char* getExceptionName() const throw() =0;

  /*!
   *  \brief  Return error message of the actual exception.
   *  \return Error message.
   */
  inline string getMessage() const throw()
  {
    return mMessage;
  }

  /*!
   *  \brief  Add an item to the exception's stack trace.
   *  \param inName Name of the method/function traced.
   *  \param inFilename Source filename where stack tracing is done.
   *  \param inLineNumber Line number in source where stack tracing is done.
   */
  inline void pushStackTrace(const char* inName, const char* inFilename, unsigned int inLineNumber)
  {
    mStackTrace.push_back(StackTraceItem(inName,inFilename,inLineNumber));
  }

  /*!
   *  \brief Set error message of actual exception.
   *  \param inMessage Value of error message.
   */
  inline void setMessage(string inMessage)
  {
    mMessage = inMessage;
  }

protected:

  /*!
   *  \brief Internal struct to store stack trace elements.
   */
  struct StackTraceItem {
    string  mName;        //!< Name of the method/function traced.
    string  mFilename;    //!< Source filename where stack tracing is done.
    unsigned int mLineNumber;  //!< Line number in source where stack tracing is done.

    /*!
     *  \brief Construct a stack trace item.
     *  \param inName Name of the method/function traced.
     *  \param inFilename Source filename where stack tracing is done.
     *  \param inLineNumber Line number in source where stack tracing is done.
     */
    explicit StackTraceItem(const char* inName="", 
                            const char* inFilename="", 
                            unsigned int inLineNumber=0) :
      mName(inName), mFilename(inFilename), mLineNumber(inLineNumber) 
    {}
  };

  std::vector< StackTraceItem,BEAGLE_STLALLOCATOR<StackTraceItem> >
         mStackTrace;  //!< Stack trace of the calls
  string mMessage;     //!< Exception message describing the abnormal situation.

};

}

#endif // Beagle_Exception_hpp
