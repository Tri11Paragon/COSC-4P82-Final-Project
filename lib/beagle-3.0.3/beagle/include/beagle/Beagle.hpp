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
 *  \file   beagle/Beagle.hpp
 *  \brief  File to include to import all the basic/generic Beagle classes.
 *  \author Christian Gagne
 *  \author Marc Parizeau
 *  $Revision: 1.27.2.1 $
 *  $Date: 2007/05/09 01:51:08 $
 */

/*!
 *  \namespace Beagle
 *  \brief     Namespace of all classes and global functions of the Open BEAGLE framework.
 *  \ingroup OOF
 *  \ingroup ECF
 */

/*!
 *  \mainpage Open BEAGLE Reference Manual
 *
 *  You can use the actual manual in different ways. If you are looking for documentation to get
 *  information on specific element of the Open BEAGLE framework, you can check in the
 *  <A HREF="annotated.html">compound list</A> for a specific class, or the
 *  <A HREF="functions.html">compound member index</A> for a specific method. If you intent to use the 
 *  manual in order to get familiar with the framework, start by browsing the
 *  <A HREF="modules.html">modules</A>, especially the examples modules. The different components are
 *  there logically regrouped to facilitate assimilation of the internal framework organization.
 */

/*!
 *  \defgroup OOF Object Oriented Foundations
 *  \brief Classes and functions related to the Object Oriented (OO) foundations, the basis of
 *    the Open BEAGLE architecture.
 */

/*!
 *  \defgroup ECF Generic EC Framework
 *  \brief Generic component of the framework architecture for evolutionary computation.
 *
 *  The generic EC framework is an extension of OO foundations. It offers a solid basis for
 *  implementing evolutionary algorithms. It is composed of a generic structure for populations,
 *  an evolution system and a set of operators packed in an evolver.
 */


#ifndef Beagle_Beagle_hpp
#define Beagle_Beagle_hpp

// PACC classes

#include "Math.hpp"
#include "Util.hpp"
#include "XML.hpp"

// Object Oriented Foundation

#include "beagle/config.hpp"
#include "beagle/macros.hpp"
#include "beagle/Object.hpp"
#include "beagle/NamedObject.hpp"
#include "beagle/Pointer.hpp"
#include "beagle/PointerT.hpp"
#include "beagle/castObjectT.hpp"
#include "beagle/Allocator.hpp"
#include "beagle/AllocatorT.hpp"
#include "beagle/AbstractAllocT.hpp"
#include "beagle/NamedObject.hpp"
#include "beagle/Exception.hpp"
#include "beagle/TargetedException.hpp"
#include "beagle/ValidationException.hpp"
#include "beagle/AssertException.hpp"
#include "beagle/BadCastException.hpp"
#include "beagle/InternalException.hpp"
#include "beagle/ObjectException.hpp"
#include "beagle/IOException.hpp"
#include "beagle/RunTimeException.hpp"
#include "beagle/IsEqualPointerPredicate.hpp"
#include "beagle/IsLessPointerPredicate.hpp"
#include "beagle/IsMorePointerPredicate.hpp"
#include "beagle/IsEqualMapPairPredicate.hpp"
#include "beagle/IsLessMapPairPredicate.hpp"
#include "beagle/ContainerAllocator.hpp"
#include "beagle/Container.hpp"
#include "beagle/ContainerT.hpp"
#include "beagle/ContainerAllocatorT.hpp"
#include "beagle/AbstractContainerAllocT.hpp"
#include "beagle/Map.hpp"
#include "beagle/RouletteT.hpp"
#include "beagle/WrapperT.hpp"
#include "beagle/ArrayT.hpp"
#include "beagle/Bool.hpp"
#include "beagle/Char.hpp"
#include "beagle/Short.hpp"
#include "beagle/Int.hpp"
#include "beagle/Long.hpp"
#include "beagle/UChar.hpp"
#include "beagle/UShort.hpp"
#include "beagle/UInt.hpp"
#include "beagle/ULong.hpp"
#include "beagle/Float.hpp"
#include "beagle/Double.hpp"
#include "beagle/String.hpp"
#include "beagle/Matrix.hpp"
#include "beagle/Vector.hpp"

// Generic EC framework

#include "beagle/Stats.hpp"
#include "beagle/Fitness.hpp"
#include "beagle/FitnessSimple.hpp"
#include "beagle/FitnessSimpleMin.hpp"
#include "beagle/FitnessMultiObj.hpp"
#include "beagle/FitnessMultiObjMin.hpp"
#include "beagle/Genotype.hpp"
#include "beagle/Individual.hpp"
#include "beagle/IndividualAlloc.hpp"
#include "beagle/IndividualAllocT.hpp"
#include "beagle/IndividualBag.hpp"
#include "beagle/Deme.hpp"
#include "beagle/DemeAlloc.hpp"
#include "beagle/DemeAllocT.hpp"
#include "beagle/Vivarium.hpp"
#include "beagle/VivariumAlloc.hpp"
#include "beagle/VivariumAllocT.hpp"
#include "beagle/HallOfFame.hpp"
#include "beagle/ParetoFrontHOF.hpp"
#include "beagle/ParetoFrontCalculateOp.hpp"
#include "beagle/Register.hpp"
#include "beagle/ConfigurationDumper.hpp"
#include "beagle/RegisterReadOp.hpp"
#include "beagle/Logger.hpp"
#include "beagle/LoggerXML.hpp"
#include "beagle/Randomizer.hpp"
#include "beagle/Context.hpp"
#include "beagle/Component.hpp"
#include "beagle/ComponentMap.hpp"
#include "beagle/System.hpp"
#include "beagle/Operator.hpp"
#include "beagle/OperatorMap.hpp"
#include "beagle/Evolver.hpp"
#include "beagle/BreederOp.hpp"
#include "beagle/BreederNode.hpp"
#include "beagle/ReplacementStrategyOp.hpp"
#include "beagle/GenerationalOp.hpp"
#include "beagle/SteadyStateOp.hpp"
#include "beagle/MuCommaLambdaOp.hpp"
#include "beagle/MuPlusLambdaOp.hpp"
#include "beagle/NSGA2Op.hpp"
#include "beagle/NPGA2Op.hpp"
#include "beagle/IfThenElseOp.hpp"
#include "beagle/MilestoneReadOp.hpp"
#include "beagle/MilestoneWriteOp.hpp"
#include "beagle/TerminationOp.hpp"
#include "beagle/TermMaxGenOp.hpp"
#include "beagle/TermMaxEvalsOp.hpp"
#include "beagle/TermMaxFitnessOp.hpp"
#include "beagle/TermMinFitnessOp.hpp"
#include "beagle/StatsCalculateOp.hpp"
#include "beagle/StatsCalcFitnessMultiObjOp.hpp"
#include "beagle/StatsCalcFitnessSimpleOp.hpp"
#include "beagle/MigrationOp.hpp"
#include "beagle/MigrationRandomRingOp.hpp"
#include "beagle/HierarchicalFairCompetitionOp.hpp"
#include "beagle/EvaluationOp.hpp"
#include "beagle/EvaluationMultipleOp.hpp"
#include "beagle/InitializationOp.hpp"
#include "beagle/SelectionOp.hpp"
#include "beagle/SelectParsimonyTournOp.hpp"
#include "beagle/SelectRandomOp.hpp"
#include "beagle/SelectRouletteOp.hpp"
#include "beagle/SelectTournamentOp.hpp"
#include "beagle/CrossoverOp.hpp"
#include "beagle/MutationOp.hpp"
#include "beagle/SteadyStateOp.hpp"
#include "beagle/DecimateOp.hpp"
#include "beagle/OversizeOp.hpp"
#include "beagle/RandomShuffleDemeOp.hpp"
#include "beagle/Randomizer.hpp"
#include "beagle/RouletteT.hpp"
#include "beagle/InvalidateFitnessOp.hpp"
#include "beagle/IndividualSizeFrequencyStatsOp.hpp"

#endif // Beagle_Beagle_hpp

