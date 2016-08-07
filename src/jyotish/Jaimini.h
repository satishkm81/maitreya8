/***********************************************************************
 This is the source code of Maitreya, open source platform for Vedic
 and western astrology.

 File       src/jyotish/Jaimini.h
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
#ifndef _JAIMINI_H_
#define _JAIMINI_H_

#include "Expert.h"

class Horoscope;
class Sheet;

/*************************************************//**
*
*  encapsulation of calculations in Jaimini astrology
*
******************************************************/
class JaiminiExpert : public VedicExpert
{
public:

	// the expert is varga dependent
	JaiminiExpert( Horoscope *h, const Varga &varga ) : VedicExpert( h, varga ) {}

	// update all
	void update();

	// update only padas
	int calcPada( const int& );

	// stronger of two rasis
	int getStrongerRasi( const Rasi &rasi1, const Rasi &rasi2 );

	// lord of a rasi (including rahu/ketu for aquarius/scorpio)
	ObjectId getJaiminiLord( const Rasi& );

	// whether the nodes are stronger than saturn/mars for aq/sc
	bool isKetuStrongerThanMars();
	bool isRahuStrongerThanSaturn();

	// number of years for each rasi (0..12)
	int getRasiYears( const Rasi& );

	void write( Sheet*, const bool show_header = true );

	// calculation of wxChara karakas can be run standalone without update()
	void calcCharaKarakas();

	// index is one of ATMA_KARAKA, AMATYA_KARAKA, BHRATRU_KARAKA, MATRU_KARAKA, PITRU_KARAKA, PUTRA_KARAKA, GNATI_KARAKA, DARA_KARAKA
	// return value is the correspoinding planet ISUN..IRAHU
	ObjectId getCharaKaraka( const int &index );

	// returns the karaka fpr the planet ISUN..IRAHU. Return value is ATMA_KARAKA, etc
	int getCharaKarakaProperty( const ObjectId& );

private:

	int calcPlanetAspects( const ObjectId &planet );
	void calcYears();
	void calcNodeStrength();
	int calcSignAspects( const Rasi &rasi );

	int nbplanets[12], rasiYears[12], nbAspectsOnRasi[12], p_chara_karaka[8];
	ObjectId chara_karaka[8];
	int aspSat, aspKet, aspMar, aspRah;
	bool ketuStrongerThanMars, rahuStrongerThanSaturn;
};

#endif

