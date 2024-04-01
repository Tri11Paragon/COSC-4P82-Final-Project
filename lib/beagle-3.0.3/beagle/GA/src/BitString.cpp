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
 *  \file   beagle/GA/src/BitString.cpp
 *  \brief  Source code of class GA::BitString.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.17.2.1 $
 *  $Date: 2007/05/09 01:51:02 $
 */

#include "beagle/GA.hpp"

#include <algorithm>

using namespace Beagle;


/*!
 *  \brief Construct a GA bit string.
 *  \param inSize Size of the bit string.
 *  \param inModel Default values of the bits.
 */
GA::BitString::BitString(unsigned int inSize, bool inModel) :
  std::vector< bool,BEAGLE_STLALLOCATOR<bool> >(inSize, inModel)
{ }


/*!
 *  \brief Decode the value of the binary bit string into a vector of floating-point numbers.
 *  \param inKeys Decoding keys used to tranform the bit string.
 *  \param outVector Vector of floating-point number containing the decoded values.
 */
void GA::BitString::decode(const GA::BitString::DecodingKeyVector& inKeys,
                           std::vector< double,BEAGLE_STLALLOCATOR<double> >& outVector) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  unsigned int lNbBits = 0;
  for(unsigned int i=0; i<inKeys.size(); i++) lNbBits += inKeys[i].mEncoding;

  if(lNbBits != size()) {
    string lMessage = "In GA::BitString::decode(): ";
    lMessage += "The total number of bits specified by the encoding keys (";
    lMessage += uint2str(lNbBits);
    lMessage += ") is different from the number of bits of the genotype string (";
    lMessage += uint2str(size());
    lMessage += ") !";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
#endif // BEAGLE_NDEBUG

  convertBin2Dec(inKeys, (*this), outVector);
  Beagle_StackTraceEndM("void GA::BitString::decode(const std::vector< GA::BitString::DecodingKey,BEAGLE_STLALLOCATOR<GA::BitString::DecodingKey> >& inKeys, std::vector< double,BEAGLE_STLALLOCATOR<double> >& outVector) const");
}


/*!
 *  \brief Decode the value of the gray-coded bit string into a vector of floating-point numbers.
 *  \param inKeys Decoding keys used to tranform the gray-coded bit string.
 *  \param outVector Vector of floating-point number containing the decoded values.
 */
void GA::BitString::decodeGray(const GA::BitString::DecodingKeyVector& inKeys,
                               std::vector< double,BEAGLE_STLALLOCATOR<double> >& outVector) const
{
  Beagle_StackTraceBeginM();
#ifndef BEAGLE_NDEBUG
  unsigned int lNbBits = 0;
  for(unsigned int i=0; i<inKeys.size(); i++) lNbBits += inKeys[i].mEncoding;

  if(lNbBits != size()) {
    string lMessage = "In GA::BitString::decode(): ";
    lMessage += "The total number of bits specified by the encoding keys (";
    lMessage += uint2str(lNbBits);
    lMessage += ") is different from the number of bits of the genotype string (";
    lMessage += uint2str(size());
    lMessage += ") !";
    throw Beagle_RunTimeExceptionM(lMessage);
  }
#endif // BEAGLE_NDEBUG

  std::vector< bool,BEAGLE_STLALLOCATOR<bool> > lBinBitString;
  convertGray2Bin(inKeys, (*this), lBinBitString);
  convertBin2Dec(inKeys, lBinBitString, outVector);
  Beagle_StackTraceEndM("void GA::BitString::decodeGray(const std::vector< GA::BitString::DecodingKey,BEAGLE_STLALLOCATOR<GA::BitString::DecodingKey> >& inKeys, std::vector< double,BEAGLE_STLALLOCATOR<double> >& outVector) const");
}


/*!
 *  \brief Convert a binary bit string to a floating-point vector with keys.
 *  \param inKeys Decoding keys.
 *  \param inBitString Binary bit string to decode.
 *  \param outVector Floating-point number vector resulting from decoding.
 */
void GA::BitString::convertBin2Dec(const GA::BitString::DecodingKeyVector& inKeys,
                                   const std::vector< bool,BEAGLE_STLALLOCATOR<bool> >& inBitString,
                                   std::vector< double,BEAGLE_STLALLOCATOR<double> >& outVector) const
{
  Beagle_StackTraceBeginM();
#ifdef BEAGLE_HAVE_LONG_LONG
  typedef unsigned long long EncodingType;
#else // BEAGLE_HAVE_LONG_LONG
  typedef unsigned long EncodingType;
#endif // BEAGLE_HAVE_LONG_LONG

#ifndef BEAGLE_NDEBUG
  for(unsigned int i=0; i<inKeys.size(); i++) {
    if(inKeys[i].mLowerBound >= inKeys[i].mUpperBound) {
      string lMessage = "In GA::BitString::convertBin2Dec(): ";
      lMessage += "The lower bound value of the ";
      lMessage += uint2ordinal(i+1);
      lMessage += " key (";
      lMessage += dbl2str(inKeys[i].mLowerBound);
      lMessage += ") is bigger or equal to the associated upper bound (";
      lMessage += dbl2str(inKeys[i].mUpperBound);
      lMessage += ") !";
      throw Beagle_RunTimeExceptionM(lMessage);
    }
    if((inKeys[i].mEncoding < 1) || (inKeys[i].mEncoding >= 8*sizeof(EncodingType))) {
      string lMessage = "In GA::BitString::convertBin2Dec(): ";
      lMessage += "The ";
      lMessage += uint2ordinal(i+1);
      lMessage += " encoding value (";
      lMessage += uint2str(inKeys[i].mEncoding);
      lMessage += ") is not in the valid interval (which is [1,";
      lMessage += uint2str((8*sizeof(EncodingType))-1);
      lMessage += "]) !";
      throw Beagle_RunTimeExceptionM(lMessage);
    }
  }
#endif // BEAGLE_NDEBUG

  outVector.resize(inKeys.size());
  std::vector< bool,BEAGLE_STLALLOCATOR<bool> >::const_iterator lBit = inBitString.begin();
  for(unsigned int j=0; j<inKeys.size(); j++) {
    EncodingType lGene=0;
    for(unsigned int k=0; k<inKeys[j].mEncoding; ++k) {
      lGene <<= 1;
      lGene |= ((*lBit++) ? 1 : 0);
    }
    EncodingType lDiv = ~0;
    lDiv <<= inKeys[j].mEncoding;
    lDiv = ~lDiv;
    double lTempVal = double(lGene) / double(lDiv);
    outVector[j] =
      inKeys[j].mLowerBound + (lTempVal * (inKeys[j].mUpperBound - inKeys[j].mLowerBound));
  }
  Beagle_StackTraceEndM("void GA::BitString::convertBin2Dec(const std::vector< GA::BitString::DecodingKey, BEAGLE_STLALLOCATOR<GA::BitString::DecodingKey> >& inKeys, const std::vector< bool,BEAGLE_STLALLOCATOR<bool> >& inBitString, std::vector< double,BEAGLE_STLALLOCATOR<double> >& outVector) const");
}


/*!
 *  \brief Convert a gray-coded bit string to a binary bit string with keys.
 *  \param inKeys Decoding keys.
 *  \param inGrayBitString Gray-coded bit string to decode.
 *  \param outBinBitString Binary bit string resulting from decoding.
 */
void GA::BitString::convertGray2Bin(const GA::BitString::DecodingKeyVector& inKeys,
                                    const std::vector< bool,BEAGLE_STLALLOCATOR<bool> >& inGrayBitString,
                                    std::vector< bool,BEAGLE_STLALLOCATOR<bool> >& outBinBitString) const
{
  Beagle_StackTraceBeginM();
  outBinBitString.resize(inGrayBitString.size());
  unsigned int lPosBS = 0;
  for(unsigned int i=0; i<inKeys.size(); ++i) {
    for(unsigned int j=0; j<inKeys[i].mEncoding; ++j) {
      if(j==0) outBinBitString[lPosBS] = inGrayBitString[lPosBS];
      else outBinBitString[lPosBS] =
        ((bool)inGrayBitString[lPosBS] != (bool)outBinBitString[lPosBS-1]);
      ++lPosBS;
    }
  }
  Beagle_StackTraceEndM("void GA::BitString::convertGray2Bin(const std::vector< GA::BitString::DecodingKey,  BEAGLE_STLALLOCATOR<GA::BitString::DecodingKey> >& inKeys, const std::vector< bool,BEAGLE_STLALLOCATOR<bool> >& inGrayBitString, std::vector< bool,BEAGLE_STLALLOCATOR<bool> >& outBinBitString) const");
}


/*!
 *  \brief Return size (length) of genotype.
 *  \return Size (length) of genotype.
 */
unsigned int GA::BitString::getSize() const
{
  Beagle_StackTraceBeginM();
  return size();
  Beagle_StackTraceEndM("unsigned int GA::BitString::getSize() const");
}


/*!
 *  \brief  Test if two bit strings are equals.
 *  \param  inRightObj Second bit string used for the comparison.
 *  \return True if bit strings are equal, false if not.
 *  \par Note:
 *    Returns true if the range [ 0, min(end(),inRightObj.end()) )
 *    are identical when compared element-by-element, and otherwise returns false.
 */
bool GA::BitString::isEqual(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const GA::BitString& lRightBS = castObjectT<const GA::BitString&>(inRightObj);
  unsigned int lSizeCompared = minOf<unsigned int>(size(), lRightBS.size());
  return std::equal(begin(), begin()+lSizeCompared, lRightBS.begin());
  Beagle_StackTraceEndM("bool GA::BitString::isEqual(const Object& inRightObj) const");
}


/*!
 *  \brief  Test the ordering of two bit strings.
 *  \param  inRightObj Second bit string used for the ordering test.
 *  \return True if the actual bit string is before the second, false if not.
 *  \par Note:
 *    Returns true if the actual bit string is lexicographically less than the right object
 *    in the range [ 0, min(end(),inRightObj.end()) ) when compared element-by-element,
 *    and otherwise returns false.
 */
bool GA::BitString::isLess(const Object& inRightObj) const
{
  Beagle_StackTraceBeginM();
  const GA::BitString& lRightBS = castObjectT<const GA::BitString&>(inRightObj);
  unsigned int lSizeCompared = minOf<unsigned int>(size(),lRightBS.size());
  return std::lexicographical_compare(begin(), begin()+lSizeCompared,
                                      lRightBS.begin(), lRightBS.begin()+lSizeCompared);
  Beagle_StackTraceEndM("bool GA::BitString::isLess(const Object& inRightObj) const");
}


/*!
 *  \brief Read a bit string from a XML subtree.
 *  \param inIter XML iterator from which the bit string is read.
 *  \param ioContext Evolutionary context to use to read the bit string.
 *  \throw IOException If a reading error occured (generally bad file format)!
 */
void GA::BitString::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)
{
  Beagle_StackTraceBeginM();
  if((inIter->getType()!=PACC::XML::eData) || (inIter->getValue()!="Genotype"))
    throw Beagle_IOExceptionNodeM(*inIter, "tag <Genotype> expected!");

  string lType = inIter->getAttribute("type").c_str();
  if(lType.empty())
    throw Beagle_IOExceptionNodeM(*inIter, "bit string type of the genotype is not present!");
  if(lType != "bitstring")
    throw Beagle_IOExceptionNodeM(*inIter, "type of genotype mismatch, expected \"bitstring\"!");

  PACC::XML::ConstIterator lChild = inIter->getFirstChild();
  if((!lChild) || (lChild->getType()!=PACC::XML::eString))
    throw Beagle_IOExceptionNodeM(*lChild, "expected content for the bit string!");

  clear();
  std::istringstream lISS(lChild->getValue());
  while(lISS.eof() == false) {
    char lBuf = '\0';
    lISS >> lBuf;
    if(lBuf=='\0') break;
    if((lBuf != '0') && (lBuf != '1'))
      throw Beagle_IOExceptionNodeM(*lChild, "bit string value is not 0 or 1!");
    push_back((lBuf == '1') ? true : false);
  }
  Beagle_StackTraceEndM("void GA::BitString::readWithContext(PACC::XML::ConstIterator inIter, Context& ioContext)");
}


/*!
 *  \brief Write a bit string into a XML streamer.
 *  \param ioStreamer XML streamer into which the bit string is written.
 *  \param inIndent Whether XML output should be indented.
 */
void GA::BitString::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const
{
  Beagle_StackTraceBeginM();
  ioStreamer.openTag("Genotype", false);
  ioStreamer.insertAttribute("type", "bitstring");
  ioStreamer.insertAttribute("size", uint2str(size()));
  std::ostringstream lOSS;
  for(size_type i=0; i<size(); i++) lOSS << ((*this)[i] ? '1' : '0');
  ioStreamer.insertStringContent(lOSS.str());
  ioStreamer.closeTag();
  Beagle_StackTraceEndM("void GA::BitString::write(PACC::XML::Streamer& ioStreamer, bool inIndent) const");
}
