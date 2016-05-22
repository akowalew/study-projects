/*
 * test_czlowiek.cpp
 *
 *  Created on: 2 cze 2015
 *      Author: dicker
 */

#include <gtest/gtest.h>
#include "../headers/czlowiek_asn.h"
#include <sstream>

using namespace std;

TEST(Test_czlowiek_asn, TestStrumienia)
{
	czlowiek_asn czlA, czlB ;
	stringstream strumien ;

	// pierw czlowiek powinien być nie do odczytu, zgodnie z założeniami
	EXPECT_EQ(0, czlA.is_readable()) ;

	// wypełniam człowieka wartościami
	string s1 = "Adam", s2 = "Kowalewski" ;
	long long l1 = 20, l2 = 28, l3 = 8, l4 = 1995 ;

	czlA.imie.set_value(s1) ;
	czlA.nazwisko.set_value(s2) ;
	// NIE ZAPISAŁEM WIEKU!
	czlA.data_urodzenia.set_value(l2, l3, l4) ;


	// zapisuję do strumienia
	czlA.write_to_stream(strumien) ;
	// odczytuję ze strumienia
	czlB.try_read_from_stream(strumien) ;

	// sprawdzam, czy wszystko się zgadza, co wczytałem
	EXPECT_EQ(1, czlB.imie == czlA.imie) ;
	EXPECT_EQ(czlB.imie.get_value(),s1) ;

	EXPECT_EQ(1, czlB.nazwisko == czlA.nazwisko) ;
	EXPECT_EQ(czlB.nazwisko.get_value(), s2) ;

	/*
	EXPECT_EQ(1, czlB.wiek == czlA.wiek) ;
	EXPECT_EQ(czlB.wiek.get_value(), l1) ; */

	// potwierdzam, iż wiek jest opcjonalny i nie wczytał się (bo go nie było)
	EXPECT_EQ(1, czlA.wiek.is_optional()) ;
	EXPECT_EQ(0, czlA.wiek.is_readable()) ;
	EXPECT_EQ(0, czlB.wiek.is_readable()) ;

	EXPECT_EQ(1, czlB.data_urodzenia == czlA.data_urodzenia) ;
	EXPECT_EQ(czlB.data_urodzenia.dzien.get_value(), l2) ;
	EXPECT_EQ(czlB.data_urodzenia.miesiac.get_value(), l3) ;
	EXPECT_EQ(czlB.data_urodzenia.rok.get_value(), l4) ;

	EXPECT_EQ(1, czlA == czlB) ;

	// dokonuję lekkich modyfikacji i powtarzam procedurę.
	strumien.str("") ;
	strumien.clear() ;
	czlB.data_urodzenia.set_value(43, 43, 43) ;
	czlB.imie.set_value("Brzeczyszczykiewicz") ;
	czlB.write_to_stream(strumien) ;

	czlA.try_read_from_stream(strumien) ;
	EXPECT_EQ(1, czlA.is_readable()) ;
	EXPECT_EQ(czlB, czlA) ;
}


