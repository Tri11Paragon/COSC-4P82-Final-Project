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
 *  \file   beagle/IfThenElseOp.hpp
 *  \brief  Definition of the class IfThenElseOp.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.6.2.1 $
 *  $Date: 2007/05/09 01:51:14 $
 */

#ifndef Beagle_IfThenElseOp_hpp
#define Beagle_IfThenElseOp_hpp

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/Operator.hpp"


namespace Beagle {

/*!
 *  \class IfThenElseOp beagle/IfThenElseOp.hpp "beagle/IfThenElseOp.hpp"
 *  \brief If-then-else operator class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class IfThenElseOp : public Operator {

public:

  //! IfThenElseOp allocator type
  typedef AllocatorT<IfThenElseOp,Operator::Alloc>
          Alloc;
  //! IfThenElseOp handle type.
  typedef PointerT<IfThenElseOp,Operator::Handle>
          Handle;
  //! IfThenElseOp bag type.
  typedef ContainerT<IfThenElseOp,Operator::Bag>
          Bag;

  explicit IfThenElseOp(string inConditionTag="",
                        string inConditionValue="",
                        string inName="IfThenElseOp");
  virtual ~IfThenElseOp() { }

  virtual Operator::Handle giveReference();  
  virtual void             initialize(System& ioSystem);
          void             insertPositiveOp(string inName, OperatorMap& inOpMap);
          void             insertNegativeOp(string inName, OperatorMap& inOpMap);
  virtual void             operate(Deme& ioDeme, Context& ioContext);
  virtual void             postInit(System& ioSystem);
  virtual void             readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap);
  virtual void             write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

  /*!
   *  \return Parameter tag of condition to test in if-then-else operator.
   */
  inline string getConditionTag() const
  {
    Beagle_StackTraceBeginM();
    return mConditionTag;
    Beagle_StackTraceEndM("string IfThenElseOp::getConditionTag() const");
  }

  /*!
   *  \return Condition value tested for positive condition in if-then-else operator.
   */
  inline string getConditionValue() const
  {
    Beagle_StackTraceBeginM();
    return mConditionValue;
    Beagle_StackTraceEndM("string IfThenElseOp::getConditionValue() const");
  }

  /*!
   *  \brief Return a constant reference to the positive operator set.
   *  \return Positive operator set constant reference.
   */
  inline const Operator::Bag& getPositiveSet() const
  {
    Beagle_StackTraceBeginM();
    return mPositiveOpSet;
    Beagle_StackTraceEndM("const Operator::Bag& IfThenElseOp::getPositiveSet() const");
  }

  /*!
   *  \brief Return a reference to the positive operator set.
   *  \return Positive operator set reference.
   */
  inline Operator::Bag& getPositiveSet()
  {
    Beagle_StackTraceBeginM();
    return mPositiveOpSet;
    Beagle_StackTraceEndM("Operator::Bag& IfThenElseOp::getPositiveSet()");
  }

  /*!
   *  \brief Return a constant reference to the negative operator set.
   *  \return Negative operator set constant reference.
   */
  inline const Operator::Bag& getNegativeSet() const
  {
    Beagle_StackTraceBeginM();
    return mNegativeOpSet;
    Beagle_StackTraceEndM("const Operator::Bag& IfThenElseOp::getNegativeSet() const");
  }

  /*!
   *  \brief Return a reference to the negative operator set.
   *  \return Negative operator set reference.
   */
  inline Operator::Bag& getNegativeSet()
  {
    Beagle_StackTraceBeginM();
    return mNegativeOpSet;
    Beagle_StackTraceEndM("Operator::Bag& IfThenElseOp::getNegativeSet()");
  }

  /*!
   *  \brief Set parameter tag of condition to use.
   */
  inline void setConditionTag(string inTag)
  {
    Beagle_StackTraceBeginM();
    mConditionTag = inTag;
    Beagle_StackTraceEndM("void IfThenElseOp::setConditionTag(string inTag)");
  }

  /*!
   *  \brief Set parameter value of condition to use.
   */
  inline void setConditionValue(string inValue)
  {
    Beagle_StackTraceBeginM();
    mConditionValue = inValue;
    Beagle_StackTraceEndM("void IfThenElseOp::setConditionValue(string inValue)");
  }

private:

  Operator::Bag  mPositiveOpSet;   //!< Operator set to execute when the condition is true.
  Operator::Bag  mNegativeOpSet;   //!< Operator set to execute when the condition is false.
  string    mConditionTag;    //!< Parameter tag to test.
  string    mConditionValue;  //!< Parameter value for a positive condition.

};

}

#endif // Beagle_IfThenElseOp_hpp
