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
 *  \file   beagle/Operator.hpp
 *  \brief  Definition of the class Operator.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10.2.1 $
 *  $Date: 2007/05/09 01:51:16 $
 */

/*!
 *  \defgroup Op Operators and Evolvers
 *  \ingroup ECF
 *  \brief Generic evolutionary operators and evolvers, part of the generic EC framework.
 *
 *  The operator is a central concept of Open BEAGLE as an EC framework. In Open BEAGLE, the 
 *  main-loop of operations executed on populations is dynamically defined. The operators are 
 *  specified at run-time and the user is free to define them for his evolution. This gave the 
 *  opportunity to easily and rapidly experiment numerous variants of EC. The operator and evolver 
 *  model is based on the \e Strategy design pattern, which is applied to the
 *  evolutionary algorithms. The operator interface is declared in the abstract class
 *  Beagle::Operator. 
 *  \code
 *  class Operator : public Object {
 *  public:
 *    Operator() { }
 *    virtual void initialize(System& ioSystem) { }
 *    virtual void operate(Deme& ioDeme,Context& ioContext) =0;
 *  };
 *  \endcode
 *
 *  Before the characteristic method is applied to demes, method Beagle::Operator::initialize is 
 *  invocated. In this method, the operator usually registers it's own parameters, probabilities or
 *  anything else, used by the characteristic operation. The characteristic operation is defined in 
 *  the virtual method Beagle::Operator::operate. There is a bunch of predefined operators in Open 
 *  BEAGLE. To name a few of them, the tournament selection operator (Beagle::TournamentSelectionOp), 
 *  the GP tree crossover operator  (Beagle::GP::CrossoverOp) and and the statistics
 *  computation operator (Beagle::StatsCalculateOp).
 *
 *  The user that define his own operators must be aware that the system is not fully
 *  set-up when the Beagle::Operator::initialize method is called. For example, the random number 
 *  generator must not be used, as the seed can be modified thereafter on the command-line or a
 *  configuration file. The rule is that the Beagle::Operator::initialize method must be used only to 
 *  add elements to the evolution system.
 *
 *  The operators of a specific evolution are inserted into the evolver that supervises the evolution 
 *  process. This object, implemented in class Beagle::Evolver, comprises two major attributes: the
 *  bootstrap operator set and the main-loop operator set. The bootstrap operators set contains an 
 *  ordered list of operators to apply on each deme, for the initial generation. The main loop 
 *  operators set is an ordered list of operators to apply iteratively, at each generation, on each 
 *  deme. The user could launch an evolution, by calling the method Beagle::Evolver::evolve with the 
 *  vivarium to evolve as argument.
 *  \code
 *  class Evolver : public Object {
 *  public:
 *    virtual void initialize(System::Handle ioSystem,int& ioArgc,char** ioArgv);
 *    virtual void evolve(Vivarium::Handle ioVivarium);
 *  protected:
 *    Operator::Bag mBootStrapSet;
 *    Operator::Bag mMainLoopSet;
 *  };
 *  \endcode
 *  For common EC algorithms, the user usually needs not to create custom sequences of operators. In 
 *  fact, some classes inheriting of Beagle::Evolver can be used to create evolvers with predefined 
 *  operator sets. If a special EC algorithm is needed, a custom building method can be invocated and 
 *  the evolver should be configured properly.
 *
 */

 
#ifndef Beagle_Operator_hpp
#define Beagle_Operator_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/NamedObject.hpp"

namespace Beagle {

// Forward declaration.
class Deme;
class Context;
class System;
class OperatorMap;


/*!
 *  \class Operator beagle/Operator.hpp "beagle/Operator.hpp"
 *  \brief Evolutionary operator abstract class.
 *  \ingroup ECF
 *  \ingroup Op
 */
class Operator : public NamedObject {

public:

  //! Operator allocator type.
  typedef AbstractAllocT<Operator,NamedObject::Alloc>
          Alloc;
  //! Operator handle type.
  typedef PointerT<Operator,NamedObject::Handle>
          Handle;
  //! Operator bag type.
  typedef ContainerT<Operator,NamedObject::Bag>
          Bag;

  explicit Operator(string inName="UnnamedOperator");
  virtual ~Operator() { }

  /*!
   *  \brief Apply the operation on a deme in the given context.
   *  \param ioDeme Reference to the deme on which the operation takes place.
   *  \param ioContext Evolutionary context of the operation.
   */
  virtual void operate(Deme& ioDeme, Context& ioContext) = 0;

  virtual PointerT<Operator,NamedObject::Handle> giveReference();
  virtual void readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap);
  virtual void write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

  /*!
   *  \brief Initialize the operator.
   *  \param ioSystem Reference to the evolutionary system.
   */
  virtual void initialize(System& ioSystem) { }

  /*!
   *  \return Whether the operator has been initialized or not.
   */
  inline bool isInitialized() const
  {
    Beagle_StackTraceBeginM();
    return mInitialized;
    Beagle_StackTraceEndM("bool Operator::isInitialized() const");
  }

  /*!
   *  \return Whether the operator has been post-initialized or not.
   */
  inline bool isPostInitialized() const
  {
    Beagle_StackTraceBeginM();
    return mPostInitialized;
    Beagle_StackTraceEndM("bool Operator::isPostInitialized() const");
  }

  /*!
   *  \brief Post-initialization operator hook.
   *  \param ioSystem Reference to the evolutionary system.
   */
  virtual void postInit(System& ioSystem) { }

  /*!
   *  \brief Set the operator initialization state.
   *  \param inInitialized Initialization state
   */
  inline void setInitialized(bool inInitialized=true)
  {
    Beagle_StackTraceBeginM();
    mInitialized = inInitialized;
    Beagle_StackTraceEndM("void Operator::setInitialized(bool inInitialized)");
  }

  /*!
   *  \brief Set the operator initialization state.
   *  \param inPostInitialized Initialization state
   */
  inline void setPostInitialized(bool inPostInitialized=true)
  {
    Beagle_StackTraceBeginM();
    mPostInitialized = inPostInitialized;
    Beagle_StackTraceEndM("void Operator::setPostInitialized(bool inPostInitialized)");
  }

private:

  bool  mInitialized;      //!< Operator initialized.
  bool  mPostInitialized;  //!< Operator post-initialized.

public:

  virtual void read(PACC::XML::ConstIterator inIter);     // Undefined read method!

};

}

#endif // Beagle_Operator_hpp
