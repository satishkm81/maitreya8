/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Eclipse.h
 Release    8.0
 Author     Martin Pettau
 Copyright  2003-2016 by the author

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

  http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
************************************************************************/

#ifndef _ECLIPSE_H_
#define _ECLIPSE_H_

#include <list>

class Writer;
class EclipseData;
class Sheet;
class Table;

using namespace std;

/*************************************************//**
*
* \brief Encapsulates calculation of Eclipses
*
******************************************************/
class EclipseExpert
{
public:
	EclipseExpert();
	~EclipseExpert();

	void update( const double &startjd, const double &endjd, const int &planet, const int &type, const bool &vedic );

	void write( Sheet*, const bool &useLocaltime );

private:
	list<EclipseData*> data;
};

#endif


