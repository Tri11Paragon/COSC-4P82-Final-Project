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
 *  \file   beagle/GP/MutationEphemeralOpT.hpp
 *  \brief  Definition and implementation of template GP::MutationEphemeralOpT.
 *  \author Christian Gagne
 *  $Revision: 1.4.2.1 $
 *  $Date: 2007/05/09 01:51:05 $
 */

#ifndef Beagle_GP_MutationEphemeralOpT_hpp
#define Beagle_GP_MutationEphemeralOpT_hpp

#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/MutationOp.hpp"
#include "beagle/UInt.hpp"
#include "beagle/Float.hpp"
#include "beagle/Bool.hpp"
#include "beagle/GP/Context.hpp"
#include "beagle/GP/System.hpp"
#include "beagle/GP/Individual.hpp"
#include "beagle/GP/Primitive.hpp"
#include "beagle/GP/EphemeralT.hpp"

namespace Beagle {
namespace GP {


/*!
 *  \class MutationEphemeralOpT beagle/GP/MutationEphemeralOpT.hpp 
 *    "beagle/GP/MutationEphemeralOpT.hpp"
 *  \brief GP ephemeral mutation operator class, change value of ephemeral constants.
 *  \param T Type of the constants.
 *  \ingroup GPF
 *  \ingroup GPOp
 */
template <class T>
class MutationEphemeralOpT : public Beagle::MutationOp {

public:

  //! GP::MutationEphemeralOpT allocator type.
  typedef AllocatorT<Beagle::GP::MutationEphemeralOpT<T>,Beagle::MutationOp::Alloc>
          Alloc;
  //! GP::MutationEphemeralOpT handle type.
  typedef PointerT<Beagle::GP::MutationEphemeralOpT<T>,Beagle::MutationOp::Handle>
          Handle;
  //! GP::MutationEphemeralOpT bag type.
  typedef ContainerT<Beagle::GP::MutationEphemeralOpT<T>,Beagle::MutationOp::Bag>
          Bag;

  explicit MutationEphemeralOpT(string inMutationPbName="gp.muteph.indpb",
                                string inEphemeralNameParamName="gp.muteph.primit",
                                string inName="GP-MutationEphemeralOpT");
  virtual ~MutationEphemeralOpT() { }

  virtual void initialize(Beagle::System& ioSystem);
  virtual bool mutate(Beagle::Individual& ioIndividual, Beagle::Context& ioContext);
  virtual void readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap);
  virtual void writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

protected:

  String::Handle mEphemeralName;           //!< Name of the ephemeral primitive mutated.
  string         mEphemeralNameParamName;  //!< Name of the ephemeral primitive name parameter.

};

}
}


/*!
 *  \brief Construct a GP ephemeral mutation operator.
 *  \param inMutationPbName Mutation probability parameter name used in register.
 *  \param inEphemeralNameParamName Parameter name used to state the name of the ephemeral primitive
 *    that are mutated.
 *  \param inName Name of the operator.
 */
template <class T>
Beagle::GP::MutationEphemeralOpT<T>::MutationEphemeralOpT(string inMutationPbName,
                                                          string inEphemeralNameParamName,
                                                          string inName) :
  Beagle::MutationOp(inMutationPbName, inName),
  mEphemeralNameParamName(inEphemeralNameParamName)
{ }


/*!
 *  \brief Initialize the GP ephemeral mutation operator.
 *  \param ioSystem System of the evolution.
 */
template <class T>
void Beagle::GP::MutationEphemeralOpT<T>::initialize(Beagle::System& ioSystem)
{
  Beagle_StackTraceBeginM();
  Beagle::MutationOp::initialize(ioSystem);

  if(ioSystem.getRegister().isRegistered(mMutationPbName)) {
    ioSystem.getRegister().deleteEntry(mMutationPbName);
  }

  if(ioSystem.getRegister().isRegistered(mMutationPbName)) {
    mMutationProba = castHandleT<Float>(ioSystem.getRegister()[mMutationPbName]);
  } else {
    mMutationProba = new Float(float(0.05));
    string lLongDescrip = "Ephemeral mutation probability for an individual. ";
    lLongDescrip += "A ephemeral mutation change the value of a specific primitive.";
    Register::Description lProbaDescription(
      "Ephemeral mutation probability",
      "Float",
      "0.05",
      lLongDescrip
    );
    ioSystem.getRegister().addEntry(mMutationPbName, mMutationProba, lProbaDescription);
  }

  if(ioSystem.getRegister().isRegistered(mEphemeralNameParamName)) {
    mEphemeralName = castHandleT<String>(ioSystem.getRegister()[mEphemeralNameParamName]);
  } else {
    mEphemeralName = new String("E");
    Register::Description lDescription(
      "Ephemeral primitive name",
      "String",
      "E",
      string("Name of the ephemeral primitive for which the values are modified ")+
      string("by the ephemeral mutation operator.")
    );
    ioSystem.getRegister().addEntry(mEphemeralNameParamName, mEphemeralName, lDescription);
  }
  Beagle_StackTraceEndM("void Beagle::GP::MutationEphemeralOpT<T>::initialize(Beagle::System& ioSystem)");
}


/*!
 *  \brief Mutating ephemerals values of a GP individual.
 *  \param ioIndividual GP individual to mutate ephemerals value.
 *  \param ioContext Context of the evolution.
 *  \return True if the individual is effectively mutated, false if not.
 */
template <class T>
bool Beagle::GP::MutationEphemeralOpT<T>::mutate(Beagle::Individual& ioIndividual, 
                                                 Beagle::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  Beagle_LogDetailedM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::GP::MutationEphemeralOpT",
    string("Mutating ")+uint2ordinal(ioContext.getGenotypeIndex()+1)+
    string(" individual with GP::MutationEphemeralOpT")
  );

  GP::Individual& lIndividual = castObjectT<GP::Individual&>(ioIndividual);
  GP::Context& lContext = castObjectT<GP::Context&>(ioContext);

  // Get index of potential primitives with parameters that can be selected for mutation.
  std::vector< std::pair<unsigned int,unsigned int>,
               BEAGLE_STLALLOCATOR< std::pair<unsigned int,unsigned int> > > lPotentialParam;
  for(unsigned int i=0; i<lIndividual.size(); ++i) {
    GP::Tree& lTree = *lIndividual[i];
    for(unsigned int j=0; j<lTree.size(); ++j) {
      if(lTree[j].mPrimitive->getName() == *mEphemeralName) {
        lPotentialParam.push_back(std::make_pair(i,j));
      }
    }
  }

  // Return if there is not potential parameters.
  if(lPotentialParam.empty()) return false;

  // Mutating a primitive
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::GP::MutationEphemeralOpT",
    string("Individual before GP parameters mutation: ")+
    ioIndividual.serialize()
  );

  // Store original context values
  unsigned int lOldGenotypeIndex = lContext.getGenotypeIndex();
  GP::Tree::Handle lOldGenotypeHandle = lContext.getGenotypeHandle();

  // Get reference to primitive to mutate and other objects.
  const unsigned int lSelectedParam =
    lContext.getSystem().getRandomizer().rollInteger(0,lPotentialParam.size()-1);
  GP::Tree::Handle lSelectedTree = lIndividual[lPotentialParam[lSelectedParam].first];
  lContext.setGenotypeIndex(lPotentialParam[lSelectedParam].first);
  lContext.setGenotypeHandle(lSelectedTree);
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::GP::MutationEphemeralOpT",
    string("Mutating the parameter of the ")+
    uint2ordinal(lPotentialParam[lSelectedParam].second+1)+
    string(" node in the ")+uint2ordinal(lPotentialParam[lSelectedParam].first+1)+
    string(" tree")
  ); 

  // Mutate parameter value.
  GP::Primitive::Handle lSelectedPrimit =
    (*lSelectedTree)[lPotentialParam[lSelectedParam].second].mPrimitive;
  typename GP::EphemeralT<T>::Handle lSelectedEphemeral =
    castHandleT<typename GP::EphemeralT<T> >(lSelectedPrimit);
  GP::Primitive::Handle lGeneratedPrimit =
    lSelectedEphemeral->generate(mEphemeralName->getWrappedValue(), lContext);
  (*lSelectedTree)[lPotentialParam[lSelectedParam].second].mPrimitive = lGeneratedPrimit;

  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::GP::MutationEphemeralOpT",
    string("Changing the ephemeral from ")+lSelectedPrimit->serialize()+
    string(" to ")+lGeneratedPrimit->serialize()
  );

  // Restore original context values
  lContext.setGenotypeIndex(lOldGenotypeIndex);
  lContext.setGenotypeHandle(lOldGenotypeHandle);

  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "mutation", "Beagle::GP::MutationEphemeralOpT",
    string("Individual after GP parameters mutation: ")+
    ioIndividual.serialize()
  );

  return true;
  Beagle_StackTraceEndM("bool GP::MutationEphemeralOpT<T>::mutate(Individual& ioIndividual, Context& ioContext)");
}


/*!
 *  \brief Read a mutation operator from XML subtree.
 *  \param inIter XML iterator to use to read crossover operator.
 *  \param inOpMap Operator map to use to read crossover operator.
 */
template <class T>
void Beagle::GP::MutationEphemeralOpT<T>::readWithMap(PACC::XML::ConstIterator inIter,
                                                      Beagle::OperatorMap& inOpMap)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!=getName().c_str())) {
    std::ostringstream lOSS;
    lOSS << "tag <" << getName() << "> expected!" << std::flush;
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  string lMutationPbReadName = inIter->getAttribute("mutationpb").c_str();
  if(lMutationPbReadName.empty() == false) mMutationPbName = lMutationPbReadName;
  string lEphemeralNameParamName = inIter->getAttribute("primitname").c_str();
  if(lEphemeralNameParamName.empty() == false) mEphemeralNameParamName = lEphemeralNameParamName;
  Beagle_StackTraceEndM("void GP::MutationEphemeralOpT<T>::readWithMap(PACC::XML::ConstIterator inIter, OperatorMap& inOpMap)");
}


/*!
 *  \brief Write mutation operator into XML streamer.
 *  \param ioStreamer XML streamer to write mutation operator into.
 *  \param inIndent Whether XML output should be indented.
 */
template <class T>
void Beagle::GP::MutationEphemeralOpT<T>::writeContent(PACC::XML::Streamer& ioStreamer,
                                                       bool inIndent) const
{
  Beagle_StackTraceBeginM();
  Beagle::MutationOp::writeContent(ioStreamer, inIndent);
  ioStreamer.insertAttribute("primitname", mEphemeralNameParamName);
  Beagle_StackTraceEndM("void GP::MutationEphemeralOpT<T>::writeContent(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}


#endif // Beagle_GP_MutationEphemeralOpT_hpp
