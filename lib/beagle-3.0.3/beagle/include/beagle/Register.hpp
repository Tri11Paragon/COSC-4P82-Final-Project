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
 *  \file   beagle/Register.hpp
 *  \brief  Definition of the class Register.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.10.2.1 $
 *  $Date: 2007/05/09 01:51:16 $
 */

#ifndef Beagle_Register_hpp
#define Beagle_Register_hpp

#include <map>
#include <string>

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Map.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/String.hpp"
#include "beagle/Operator.hpp"
#include "beagle/Component.hpp"


namespace Beagle {

// Forward declaration
class Context;
class System;


/*!
 *  \class Register beagle/Register.hpp "beagle/Register.hpp"
 *  \brief Register implementation, where all the parameters of the framework are centralized.
 *  \ingroup ECF
 *  \ingroup Sys
 */
class Register : public Component {

public:

  /*!
   *  \struct Description beagle/Register.hpp "beagle/Register.hpp"
   *  \brief Parameter extensive description.
   */
  struct Description {
    string mBrief;         //!< Brief description of the parameter.
    string mType;          //!< Type of the parameter.
    string mDefaultValue;  //!< Default value of the parameter.
    string mDescription;   //!< Long description of the parameter.

             Description();
    explicit Description(string inBrief, string inType, string inDefaultValue, string inDescription);
  };

  //! Register allocator type.
  typedef AllocatorT<Register,Component::Alloc>
          Alloc;
  //! Register handle type.
  typedef PointerT<Register,Component::Handle>
          Handle;
  //! Register bag type.
  typedef ContainerT<Register,Component::Bag>
          Bag;

           Register();
  virtual ~Register() { }

  void               addEntry(string inTag, Object::Handle inEntry,
                              const Description& inDescription);
  Object::Handle     deleteEntry(string inTag);
  const Description& getDescription(string inTag) const;
  virtual void       initialize(System& ioSystem);
  bool               isRegistered(string inTag) const;
  Object::Handle     modifyEntry(string inTag, Object::Handle inNewEntry);
  Description        modifyDescription(string inTag, const Description& inNewDescription);
  virtual void       parseCommandLine(System& ioSystem, int& ioArgc, char** ioArgv);
  virtual void       read(PACC::XML::ConstIterator inIter);
  virtual void       readParametersFile(string inFileName, System& ioSystem);
  virtual void       readWithSystem(PACC::XML::ConstIterator inIter, System& ioSystem);
  virtual void       write(PACC::XML::Streamer& ioStreamer, bool inIndent=true) const;

  /*!
   *  \brief Get an entry of the register.
   *  \param inTag Tag of the parameter.
   *  \return Smart pointer to the entry. Return a NULL pointer if there is no entry
   *     associated with the tag.
   */
  inline Object::Handle operator[](string inTag)
  {
    Beagle_StackTraceBeginM();
    Map::iterator lIterObj = mParameters.find(inTag);
    if(lIterObj == mParameters.end()) return Object::Handle(NULL);
    else return lIterObj->second;
    Beagle_StackTraceEndM("Object::Handle Register::operator[](string inTag)");
  }

  /*!
   *  \brief Get an entry of the register.
   *  \param inTag Tag of the parameter.
   *  \return Constant smart pointer to the entry. Return a NULL pointer if there is no entry
   *     associated with the tag.
   */
  inline const Object::Handle operator[](string inTag) const
  {
    Beagle_StackTraceBeginM();
    Map::const_iterator lIterObj = mParameters.find(inTag);
    if(lIterObj == mParameters.end()) return Object::Handle(NULL);
    else return lIterObj->second;
    Beagle_StackTraceEndM("const Object::Handle Register::operator[](string inTag) const");
  }

  /*!
   *  \brief Get an entry of the register.
   *  \param inTag Tag of the parameter.
   *  \return Smart pointer to the entry. Return a NULL pointer if there is no entry
   *     associated with the tag.
   */
  inline Object::Handle getEntry(string inTag)
  {
    Beagle_StackTraceBeginM();
    Map::iterator lIterObj = mParameters.find(inTag);
    if(lIterObj == mParameters.end()) return Object::Handle(NULL);
    else return lIterObj->second;
    Beagle_StackTraceEndM("Object::Handle Register::getEntry(string inTag)");
  }

  /*!
   *  \brief Get an entry of the register.
   *  \param inTag Tag of the parameter.
   *  \return Constant smart pointer to the entry. Return a NULL pointer if there is no entry
   *     associated with the tag.
   */
  inline const Object::Handle getEntry(string inTag) const
  {
    Beagle_StackTraceBeginM();
    Map::const_iterator lIterObj = mParameters.find(inTag);
    if(lIterObj == mParameters.end()) return Object::Handle(NULL);
    else return lIterObj->second;
    Beagle_StackTraceEndM("const Object::Handle Register::getEntry(string inTag) const");
  }

protected:

  typedef std::map< string,Description,std::less<string>,
                    BEAGLE_STLALLOCATOR< std::pair<const string,Description > > >
          DescriptionMap;
  typedef std::vector< Operator::Handle,BEAGLE_STLALLOCATOR<Operator::Handle> >
          OperatorVector;

  void  addDescription(string inTag, const Description& inDescription);
  char* eraseArg(int inN, int& ioArgc, char** ioArgv) const;
  void  interpretArgs(System& ioSystem, int& ioArgc, char** ioArgv);
  void  showHelp(char** ioArgv, std::ostream& ioOs=std::cerr) const;
  void  showUsage(char** ioArgv, std::ostream& ioOs=std::cerr) const;

  Map              mParameters;     //!< Param. of the register.
  DescriptionMap   mDescriptions;   //!< Parameters descriptions.
  OperatorVector   mOperatorUsed;   //!< Operator used.
  String::Handle   mFileName;       //!< Configuration file name.

};

}

#endif // Beagle_Register_hpp
