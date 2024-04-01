/*
 *  Portable Agile C++ Classes (PACC)
 *  Copyright (C) 2004 by Marc Parizeau
 *  http://manitou.gel.ulaval.ca/~parizeau/PACC
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
 * \file PACC/Util/Date.cpp
 * \brief Class methods for the portable time and date.
 * \author Marc Parizeau, Laboratoire de vision et syst&egrave;mes num&eacute;riques, Universit&eacute; Laval
 * $Revision: 1.6.2.1 $
 * $Date: 2007/09/10 18:24:10 $
 */

#include "Util/Date.hpp"

using namespace std;
using namespace PACC;

//! Construct with specified date \c inYear, \c inMonth, and \c inDay.
Date::Date(unsigned int inYear, unsigned int inMonth, unsigned int inDay) : mTime(0)
{
	setDate(inYear, inMonth, inDay);
	setTime(0, 0, 0);
}

//! Return formatted date and time using the std::strftime string format \c inFormat.
std::string Date::get(const string& inFormat) const
{
#ifdef WIN32
	struct tm& lTime = *::localtime(&mTime);
#else
    struct tm lTime;
	::localtime_r(&mTime, &lTime);
#endif
	int lBufSize = 128;
	char* lBuffer = new char[lBufSize];
	while(::strftime(lBuffer, lBufSize, inFormat.c_str(), &lTime) == 0) {
		// buffer needs to be enlarged
		delete[] lBuffer;
		lBuffer = new char[lBufSize*=2];
	}
	string lDate(lBuffer);
	delete[] lBuffer;
	return lDate;
}

//! Return day of month for this date (1-31).
unsigned int Date::getDayOfMonth(void) const
{
#ifdef WIN32
	struct tm& lTime = *::localtime(&mTime);
#else
    struct tm lTime;
	::localtime_r(&mTime, &lTime);
#endif
	return lTime.tm_mday;
}

//! Return day of week for this date (sunday=0-6).
unsigned int Date::getDayOfWeek(void) const
{
#ifdef WIN32
	struct tm& lTime = *::localtime(&mTime);
#else
    struct tm lTime;
	::localtime_r(&mTime, &lTime);
#endif
	return lTime.tm_wday;
}

//! Return day of year for this date (1-365).
unsigned int Date::getDayOfYear(void) const
{
#ifdef WIN32
	struct tm& lTime = *::localtime(&mTime);
#else
    struct tm lTime;
	::localtime_r(&mTime, &lTime);
#endif
	return lTime.tm_yday+1;
}

//! Return hour of day for this date (0-23).
unsigned int Date::getHourOfDay(void) const
{
#ifdef WIN32
	struct tm& lTime = *::localtime(&mTime);
#else
    struct tm lTime;
	::localtime_r(&mTime, &lTime);
#endif
	return lTime.tm_hour;
}

//! Return minutes of hour for this date (0-59).
unsigned int Date::getMinutesOfHour(void) const
{
#ifdef WIN32
	struct tm& lTime = *::localtime(&mTime);
#else
    struct tm lTime;
	::localtime_r(&mTime, &lTime);
#endif
	return lTime.tm_min;
}

//! Return seconds of minutes for this date (0-59).
double Date::getSecondsOfMinute(void) const
{
#ifdef WIN32
	struct tm& lTime = *::localtime(&mTime);
#else
    struct tm lTime;
	::localtime_r(&mTime, &lTime);
#endif
	return lTime.tm_sec;
}

//! Return month of year for this date (1-12).
unsigned int Date::getMonthOfYear(void) const
{
#ifdef WIN32
	struct tm& lTime = *::localtime(&mTime);
#else
    struct tm lTime;
	::localtime_r(&mTime, &lTime);
#endif
	return lTime.tm_mon+1;
}

//! Return this date expressed in seconds since January 1st, 1970.
unsigned int Date::getTimeInSeconds(void) const
{
	return mTime;
}

//! Return year of this date.
unsigned int Date::getYear(void) const
{
#ifdef WIN32
	struct tm& lTime = *::localtime(&mTime);
#else
    struct tm lTime;
	::localtime_r(&mTime, &lTime);
#endif
	return lTime.tm_year+1900;
}

//! Set date to year \c inyear, month \c inMonth, and day \c inDay (time is unchanged).
void Date::setDate(unsigned int inYear, unsigned int inMonth, unsigned int inDay)
{
#ifdef WIN32
	struct tm& lTime = *::localtime(&mTime);
#else
    struct tm lTime;
	::localtime_r(&mTime, &lTime);
#endif
	lTime.tm_year = inYear-1900;
	lTime.tm_mon = inMonth-1;
	lTime.tm_mday = inDay;
	mTime = ::mktime(&lTime);
}

//! Set time to \c inHour hour, \c inMinutes minutes, and \c inSeconds seconds (date is unchanged).
void Date::setTime(unsigned int inHour, unsigned int inMinutes, unsigned int inSeconds)
{
#ifdef WIN32
	struct tm& lTime = *::localtime(&mTime);
#else
    struct tm lTime;
	::localtime_r(&mTime, &lTime);
#endif
	lTime.tm_hour = inHour;
	lTime.tm_min = inMinutes;
	lTime.tm_sec = inSeconds;
	mTime = ::mktime(&lTime);
}
