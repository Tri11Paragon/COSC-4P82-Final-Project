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
 *  \file   beagle/GP/src/Tree.cpp
 *  \brief  Implementation of the type GP::Tree.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.30.2.2 $
 *  $Date: 2007/09/10 18:24:11 $
 */

#include "beagle/GP.hpp"

#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Construct a GP node.
 *  \param inPrimitive Handle to the primitive refered by the node.
 *  \param inSubTreeSize Sub-tree size, including actual node.
 */
GP::Node::Node(GP::Primitive::Handle inPrimitive, unsigned int inSubTreeSize) :
  mPrimitive(inPrimitive),
  mSubTreeSize(inSubTreeSize)
{ }


/*!
 *  \brief Compare equality of two GP tree's nodes.
 *  \param inRightNode Right node to be compare to.
 *  \return True if nodes are identical, false if not.
 */
bool GP::Node::operator==(const GP::Node& inRightNode) const
{
  Beagle_StackTraceBeginM();
  return (mPrimitive == inRightNode.mPrimitive) && (mSubTreeSize == inRightNode.mSubTreeSize);
  Beagle_StackTraceEndM("bool GP::Node::operator==(const GP::Node& inRightNode) const");
}


/*!
 *  \brief Construct a GP tree of the size given.
 *  \param inSize Size of the tree.
 *  \param inPrimitiveSetIndex Index of the primitive set associated to the current GP tree.
 *  \param inNumberArguments Number of ADF arguments of the GP tree.
 */
GP::Tree::Tree(unsigned int inSize,
               unsigned int inPrimitiveSetIndex,
               unsigned int inNumberArguments) :
#ifdef BEAGLE_HAVE_RTTI
  std::vector< Node,BEAGLE_STLALLOCATOR<Node> >(inSize),
  mPrimitiveSetIndex(inPrimitiveSetIndex),
  mNumberArguments(inNumberArguments),
  mRootType(NULL)
#else // BEAGLE_HAVE_RTTI
  std::vector< Node,BEAGLE_STLALLOCATOR<Node> >(inSize),
  mPrimitiveSetIndex(inPrimitiveSetIndex),
  mNumberArguments(inNumberArguments)
#endif // BEAGLE_HAVE_RTTI
{ }


/*!
 *  \brief Fixes the 'mSubTreeSize' field of the subtree starting at
 *    inNodeIndex.  (Defaults to fixing the entire tree.)
 *  \param inNodeIndex The first node of the subtree to fix.
 *  \return The size of the fixed subtree
 */
unsigned int GP::Tree::fixSubTreeSize(unsigned int inNodeIndex)
{
  Beagle_StackTraceBeginM();
  // Check if this is a terminal
  const unsigned int lNumArgs = 
    (*this)[inNodeIndex].mPrimitive->getNumberArguments();
  if(lNumArgs==0) {
    // This is a terminal
    (*this)[inNodeIndex].mSubTreeSize = 1;
    return 1;
  }
  else {
    // This is a branch
    // Loop through the args, correcting each of those
    unsigned int lSubTreeSize = 1;
    unsigned int lNodeIndex = inNodeIndex+1;
    for(unsigned int i=0; i<lNumArgs; i++) {
      const unsigned int lThisSubTreeSize = fixSubTreeSize(lNodeIndex);
      lSubTreeSize += lThisSubTreeSize;
      lNodeIndex += lThisSubTreeSize;
    }
    (*this)[inNodeIndex].mSubTreeSize = lSubTreeSize;
    return lSubTreeSize;
  }
  Beagle_StackTraceEndM("unsigned int GP::Tree::fixSubTreeSize(unsigned int inNodeIndex)");
}


/*!
 *  \brief Convenient method to obtain this tree's PrimitiveSet.
 *  \param ioContext Context on which to operate.
 *  \return Primitive set for this tree
 */
GP::PrimitiveSet& GP::Tree::getPrimitiveSet(GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  Beagle_AssertM(mPrimitiveSetIndex < ioContext.getSystem().getPrimitiveSuperSet().size());
  return *(ioContext.getSystem().getPrimitiveSuperSet()[mPrimitiveSetIndex]);
  Beagle_StackTraceEndM("GP::PrimitiveSet& GP::Tree::getPrimitiveSet(GP::Context& ioContext) const");
}


#ifdef BEAGLE_HAVE_RTTI

/*!
 *  \brief Get desired type as tree root.
 *  \param ioContext Evolutionary context.
 *  \return Tree root desired type.
 */
const std::type_info* GP::Tree::getRootType(GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  if(mRootType == NULL)
    return ioContext.getSystem().getPrimitiveSuperSet()[mPrimitiveSetIndex]->getRootType();
  return mRootType;
  Beagle_StackTraceEndM("const std::type_info* GP::Tree::getRootType(GP::Context& ioContext) const");
}

#endif // BEAGLE_HAVE_RTTI


/*!
 *  \brief Return number of nodes of GP tree.
 *  \return Number of nodes of GP tree.
 */
unsigned int GP::Tree::getSize() const
{
  Beagle_StackTraceBeginM();
  return size();
  Beagle_StackTraceEndM("unsigned int GP::Tree::getSize() const");
}


/*!
 *  \brief Get the maximum depth of a sub-tree.
 *  \param inNodeIndex Index of the node root to the sub-tree.
 *  \return Depth to the given sub-tree.
 *  \throw Beagle::AssertException If the node index given is to out-of-bound.
 */
unsigned int GP::Tree::getTreeDepth(unsigned int inNodeIndex) const
{
  Beagle_StackTraceBeginM();
  // Check if there aren't any nodes in the tree
  if(size()==0) return 0;

  Beagle_UpperBoundCheckAssertM(inNodeIndex,size()-1);
  unsigned int lDepth = 1;
  unsigned int lChildNodeIndex = inNodeIndex + 1;
  for(unsigned int i=0; i<(*this)[inNodeIndex].mPrimitive->getNumberArguments(); i++) {
    unsigned int lChildDepth = getTreeDepth(lChildNodeIndex);
    lDepth = maxOf<unsigned int>(lDepth, lChildDepth+1);
    lChildNodeIndex += (*this)[lChildNodeIndex].mSubTreeSize;
  }
  return lDepth;
  Beagle_StackTraceEndM("unsigned int GP::Tree::getTreeDepth(unsigned int inNodeIndex) const");
}


/*!
 *  \brief Interpret the GP tree.
 *  \param outResult Datum containing the result of the interpretation.
 *  \param ioContext GP evolutionary context.
 *  \throw Beagle::ObjectException When tree is empty or not in contextual individual.
 *  \throw Beagle::AssertException When the contextual individual is a NULL pointer.
 *  \throw Beagle::GP::MaxNodesExecutionException If number of nodes execution is more than allowed.
 *  \throw Beagle::GP::MaxTimeExecutionException If elapsed execution time is more than allowed.
 */
void GP::Tree::interpret(GP::Datum& outResult, GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(empty()) throw Beagle_ObjectExceptionM("Could not interpret, tree is empty!");
  GP::Individual::Handle lIndiv = ioContext.getIndividualHandle();
  if(lIndiv == NULL) 
    throw Beagle_RunTimeExceptionM(string("GP::Tree::interpret(): The handle to the current ")+
      string("individual is NULL. This handle is obtained from the Context. The most likely ")+
      string("cause of this error is that the Context has not been set correctly. Consider ")+
      string("Context::setIndividualHandle()."));

  // Check that this tree is part of the individual specified in the Context  
  unsigned int lTreeIndex = 0;
  for(; lTreeIndex < lIndiv->size(); lTreeIndex++) {
    if(this == (*lIndiv)[lTreeIndex].getPointer()) break;
  }
  if(lTreeIndex == lIndiv->size())
    throw Beagle_ObjectExceptionM("Interpreted tree is not in the actual individual of the context!");

  Tree::Handle lOldTreeHandle = ioContext.getGenotypeHandle();
  unsigned int lOldTreeIndex  = ioContext.getGenotypeIndex();
  ioContext.setGenotypeIndex(lTreeIndex);
  ioContext.setGenotypeHandle(Handle(this));
  Beagle_LogVerboseM(
    ioContext.getSystem().getLogger(),
    "gptree", "Beagle::GP::Tree",
    string("Interpreting the ")+uint2ordinal(lTreeIndex+1)+
    string(" tree of the ")+uint2ordinal(ioContext.getIndividualIndex()+1)+
    string(" individual")
  );
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "gptree", "Beagle::GP::Tree",
    string("The tree is: ")+ioContext.getGenotype().serialize()
  );
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "gptree", "Beagle::GP::Tree",
    string("Executing the tree root node \"")+
    (*this)[0].mPrimitive->getName()+"\""
  );
  ioContext.setNodesExecutionCount(0);
  ioContext.incrementNodesExecuted();
  ioContext.getExecutionTimer().reset();  
  ioContext.pushCallStack(0);
  (*this)[0].mPrimitive->execute(outResult, ioContext);
  ioContext.popCallStack();
  ioContext.checkExecutionTime();
  Beagle_LogDebugM(
    ioContext.getSystem().getLogger(),
    "gptree", "Beagle::GP::Tree",
    string("Result of executing the ")+uint2ordinal(ioContext.getGenotypeIndex()+1)+string(" tree: ")+outResult.serialize()
  );
  ioContext.setGenotypeIndex(lOldTreeIndex);
  ioContext.setGenotypeHandle(lOldTreeHandle);
  Beagle_StackTraceEndM("void GP::Tree::interpret(GP::Datum& outResult, GP::Context& ioContext)");
}


/*!
 *  \brief Compare the equality of two GP trees.
 *  \param inRightObj Right tree to be compare to tha actual one.
 *  \return True if the trees are identical, false if not.
 */
bool GP::Tree::isEqual(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const GP::Tree& lRightTree = castObjectT<const GP::Tree&>(inRightObj);
  if(size() != lRightTree.size()) return false;
  return std::equal(begin(), end(), lRightTree.begin());
  Beagle_StackTraceEndM("bool GP::Tree::isEqual(const Object& inRightObj) const");
}


/*!
 *  \brief Read a GP tree from an XML subtree.
 *  \param inIter XML iterator to use to read the tree from.
 *  \param ioContext Context to use to read the tree.
 */
void GP::Tree::readWithContext(PACC::XML::ConstIterator inIter, Beagle::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  GP::Context& lGPContext = castObjectT<GP::Context&>(ioContext);

  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Genotype"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Genotype> expected!");

  string lType = inIter->getAttribute("type").c_str();
  if(lType.empty())
    throw Beagle_IOExceptionNodeM(*inIter, "GP tree type of the genotype is not present!");
  if(lType != "gptree")
    throw Beagle_IOExceptionNodeM(*inIter, string("type of genotype mismatch, expected ")
      +string("\"gptree\" but read \"")+lType+string("\" instead!"));

  // Get primitive set index
  string lPrimitiveSetIndexText = inIter->getAttribute("primitiveSetIndex").c_str();
  if(lPrimitiveSetIndexText.empty()) {
    // No primitive set index was specified.  This must be an old
    // tree.  Assume index is equal to tree's index
    unsigned int lGenotypeIndex = ioContext.getGenotypeIndex();
    if(lGenotypeIndex >= lGPContext.getSystem().getPrimitiveSuperSet().size()) {
      throw Beagle_RunTimeExceptionM(string("In GP::Tree::readWithContext(): The ")+
        string("'primitiveSetIndex' attribute was missing from an individual's genotype. ")+
        string("It would normally be assumed that such a tree was to be mapped to the ")+
        string("primitive set of the same index as the genotype. In this case that would ")+
        string("result in an incorrect mapping because there are not enough primitive sets ")+
        string("in the System. Perhaps this individual was not intended to be read with the ")+
        string("current set of primitive sets?"));
    }
    setPrimitiveSetIndex(lGenotypeIndex);
  }
  else {
    // primitiveSetIndex is a valid attribute.
    unsigned int lPrimitiveSetIndex = str2uint(lPrimitiveSetIndexText);
    if(lPrimitiveSetIndex >= lGPContext.getSystem().getPrimitiveSuperSet().size()) {
      string lMessage = string("In GP::Tree::readWithContext(): The 'primitiveSetIndex' ")+
        string("attribute contained the value '") + lPrimitiveSetIndexText + 
        string("' which was read as the number '") + uint2str(lPrimitiveSetIndex) + 
        string("'. This value is incorrect as there are not enough primitive sets in the ")+
        string("System. Perhaps this individual was not intended to be read with the current ")+
        string("set of primitive sets?");
      throw Beagle_RunTimeExceptionM(lMessage);
    }
    setPrimitiveSetIndex(lPrimitiveSetIndex);
  }

  // Get numberArguments attribute
  string lNumberArgumentsText = inIter->getAttribute("numberArguments").c_str();
  if(lNumberArgumentsText.empty()) {
    // numberArguments attribute wasn't defined.  This must be an old
    // tree.  Assume the number of arguments is zero.
    setNumberArguments(0);
  }
  else {
    // numberArguments is a valid attribute.
    setNumberArguments(str2uint(lNumberArgumentsText));
  }

  // Get size attribute and reserve size accordingly
  string lSizeText = inIter->getAttribute("size").c_str();
  if(lSizeText.empty()==false) reserve(str2uint(lSizeText));

  PACC::XML::ConstIterator lChild = inIter->getFirstChild();
  if((!lChild) || (lChild->getType()!=PACC::XML::eData))
    throw Beagle_IOExceptionNodeM(*lChild, "expected a XML tag for the GP tree!");

  clear();
  readSubTree(lChild, lGPContext);
  Beagle_StackTraceEndM("void GP::Tree::readWithContext(PACC::XML::ConstIterator inIter, Beagle::Context& ioContext)");
}


/*!
 *  \brief Read a GP subtree from a XML subtree.
 *  \param inIter XML iterator to read GP tree from.
 *  \param ioContext Context to use to map the node to the appropriate primitive.
 *  \return Read subtree size.
 *  \throw Beagle::InternalException When the the tree format is incorrect.
 */
unsigned int GP::Tree::readSubTree(PACC::XML::ConstIterator inIter, GP::Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if(inIter->getType() != PACC::XML::eData)
    throw Beagle_IOExceptionNodeM(*inIter, "tag expected!");

  GP::PrimitiveSuperSet& lPrimitiveSuperSet = 
    ioContext.getSystem().getPrimitiveSuperSet();
  if(lPrimitiveSuperSet.empty()) {
    throw Beagle_RunTimeExceptionM(string("There are no primitive sets in the primitive ")+
      string("super set. Consider Beagle::GP::PrimitiveSuperSet::insert() and Beagle::")+
      string("GP::PrimitiveSet. For more help, see the examples provided with Beagle."));
  }

  unsigned int lPrimitiveSetIndex = getPrimitiveSetIndex();
  if(lPrimitiveSetIndex >= lPrimitiveSuperSet.size()) {
    throw Beagle_RunTimeExceptionM(string("While attempting to read a tree, the ")+
      string("primitiveSetIndex of the tree was discovered to be larger than the number of ")+
      string("PrimitiveSets in the PrimitiveSuperSet. This tree can only be read when there ")+
      string("are sufficient PrimitiveSets in the super set."));
  }
  GP::PrimitiveSet::Handle lPrimitiveSet = lPrimitiveSuperSet[lPrimitiveSetIndex];

  GP::Primitive::Handle lPrimitive = lPrimitiveSet->getPrimitiveByName(inIter->getValue().c_str());
  if(lPrimitive == NULL) {
    std::ostringstream lOSS;
    lOSS << "no primitive named \"" <<  inIter->getValue();
    lOSS << "\" found in the primitive set";
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  unsigned int lNodeIdx = size();

  unsigned int lNbArgs = 0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() == PACC::XML::eData) ++lNbArgs;
  }
  GP::Primitive::Handle lNewReference = lPrimitive->giveReference(lNbArgs, ioContext);
  if(lNewReference==NULL) { 
    std::ostringstream lOSS;
    lOSS << "While reading a subtree, a primitive (named \"";
    lOSS << inIter->getValue() << "\") gave an unexpected NULL reference.";
    throw Beagle_RunTimeExceptionM(lOSS.str().c_str());
  }
  push_back(Node(lNewReference, 0));
  (*this)[lNodeIdx].mPrimitive->readWithContext(inIter, ioContext);

  unsigned int lSubTreeSize = 1;
  unsigned int lNbrGPChild = 0;
  for(PACC::XML::ConstIterator lChild=inIter->getFirstChild(); lChild; ++lChild) {
    if(lChild->getType() == PACC::XML::eData) {
      lSubTreeSize += readSubTree(lChild, ioContext);
      ++lNbrGPChild;
    }
  }
  if(lNbrGPChild != (*this)[lNodeIdx].mPrimitive->getNumberArguments()) {
    std::ostringstream lOSS;
    lOSS << "number of arguments stated by the GP primitives (";
    lOSS << (*this)[lNodeIdx].mPrimitive->getNumberArguments();
    lOSS << ") mismatch the number of arguments read for the GP node (";
    lOSS << lNbrGPChild << ")!";
    throw Beagle_IOExceptionNodeM(*inIter, lOSS.str().c_str());
  }
  (*this)[lNodeIdx].mSubTreeSize = lSubTreeSize;
  return lSubTreeSize;
  Beagle_StackTraceEndM("unsigned int GP::Tree::readSubTree(PACC::XML::ConstIterator inIter, GP::Context& ioContext)");
}


/*!
 *  \brief Set the context to the node given.
 *  \param inNodeIndex Index of the node to use to set the context.
 *  \param ioContext Evolutionary context to set to the given node.
 *  \throw Beagle::AssertException If the tree is in a bad state.
 */
void GP::Tree::setContextToNode(unsigned int inNodeIndex, GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  if(inNodeIndex >= size()) {
    throw Beagle_RunTimeExceptionM(string("In GP::Tree::setContextToNode(): Failed to set ")+
      string("the context to the ")+uint2ordinal(inNodeIndex+1)+
      string(" node because there are only ")+uint2str(size())+
      string(" nodes in this tree."));
  }
  ioContext.emptyCallStack();
  unsigned int lIndex = 0;
  ioContext.pushCallStack(lIndex);
  while(lIndex < inNodeIndex) {
    unsigned int lChildIndex = lIndex + 1;
    unsigned int lArgument=0;
    for(; lArgument<(*this)[lIndex].mPrimitive->getNumberArguments(); lArgument++) {
      if((lChildIndex+(*this)[lChildIndex].mSubTreeSize) > inNodeIndex) break;
      lChildIndex += (*this)[lChildIndex].mSubTreeSize;
    }
    Beagle_AssertM(lArgument < (*this)[lIndex].mPrimitive->getNumberArguments());
    Beagle_AssertM(lChildIndex < size());
    lIndex = lChildIndex;
    ioContext.pushCallStack(lIndex);
  }
  Beagle_AssertM(lIndex == inNodeIndex);
  Beagle_StackTraceEndM("void GP::Tree::setContextToNode(unsigned int inNodeIndex, GP::Context& ioContext) const");
}


/*!
 *  \brief Validate the topology of a sub-tree the a given node.
 *  \param inNodeIndex Index in the tree of the root node of the sub-tree to validate.
 *  \param ioContext Evolutionary context.
 *  \throw Beagle::AssertException When the evolutionary context is badly configured.
 */
bool GP::Tree::validateSubTree(unsigned int inNodeIndex, GP::Context& ioContext) const
{
  Beagle_StackTraceBeginM();
  Beagle_AssertM(inNodeIndex < size());
  Beagle_AssertM(inNodeIndex == ioContext.getCallStackTop());
  if((*this)[inNodeIndex].mPrimitive->validate(ioContext) == false) {
    Beagle_LogVerboseM(
      ioContext.getSystem().getLogger(),
      "tree", "Beagle::GP::Tree",
      string("Subtree validation failed because the primitive (\"")+
      (*this)[inNodeIndex].mPrimitive->getName()+string("\", ")+
      uint2ordinal(inNodeIndex+1)+string(" node) failed validation.")
    );
    return false;
  }
  unsigned int lChildNodeIndex = inNodeIndex + 1;
  for(unsigned int i=0; i<(*this)[inNodeIndex].mPrimitive->getNumberArguments(); i++) {
    ioContext.pushCallStack(lChildNodeIndex);
    bool lChildValidation = validateSubTree(lChildNodeIndex, ioContext);
    ioContext.popCallStack();
    if(lChildValidation == false) return false;
    lChildNodeIndex += (*this)[lChildNodeIndex].mSubTreeSize;
  }
  return true;
  Beagle_StackTraceEndM("bool GP::Tree::validateSubTree(unsigned int inNodeIndex, GP::Context& ioContext) const");
}


/*!
 *  \brief Write a GP tree into a XML streamer.
 *  \param ioStreamer XML streamer to write the tree into.
 *  \param inIndent Whether XML output should be indented.
 */
void GP::Tree::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Genotype", inIndent);
  ioStreamer.insertAttribute("type", "gptree");
  ioStreamer.insertAttribute("size", uint2str(size()));
  ioStreamer.insertAttribute("depth", uint2str(getTreeDepth()));
  ioStreamer.insertAttribute("primitSetId", uint2str(getPrimitiveSetIndex()));
  ioStreamer.insertAttribute("nbArgs", uint2str(getNumberArguments()));
  if(size() > 0) {
    unsigned int lSizeSubTree = writeSubTree(ioStreamer, 0, inIndent);
    Beagle_AssertM(lSizeSubTree == size());
  }
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void GP::Tree::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}


/*!
 *  \brief Write a GP sub-tree into a XML streamer.
 *  \param ioStreamer XML streamer to write the tree into.
 *  \param inN Index of the actual subtree root node in the vector.
 *  \param inIndent Whether XML output should be indented.
 *  \throw Beagle::AssertException When sub-tree size doesn't match.
 */
unsigned int GP::Tree::writeSubTree(PACC::XML::Streamer& ioStreamer,
                                    unsigned int inN,
                                    bool inIndent) const
{
  Beagle_StackTraceBeginM();
  Beagle_AssertM(inN < size());
  unsigned int lNumberArguments = (*this)[inN].mPrimitive->getNumberArguments();
  ioStreamer.openTag((*this)[inN].mPrimitive->getName().c_str(), inIndent);
  (*this)[inN].mPrimitive->writeContent(ioStreamer, inIndent);
  unsigned int lSubTreeSize = 1;
  for(unsigned int i=0; i<lNumberArguments; ++i) {
    lSubTreeSize += writeSubTree(ioStreamer, (lSubTreeSize+inN), inIndent);
  }
  Beagle_AssertM(lSubTreeSize == (*this)[inN].mSubTreeSize);
  ioStreamer.closeTag();
  return lSubTreeSize;
  Beagle_StackTraceEndM("unsigned int GP::Tree::writeSubTree(PACC::XML::Streamer& ioStreamer, unsigned int inN, bool inIndent) const");
}


