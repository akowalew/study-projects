/*
 * test_date.cpp
 *
 *  Created on: 30 maj 2015
 *      Author: dicker
 */

#include <gtest/gtest.h>
#include "../headers/date_asn.h"
#include <sstream>
using namespace std;

TEST(DateTest, TestKonstruktora)
{
	date_asn tmp, tmp2(true, true, true, true) ;

	EXPECT_EQ(0, tmp.is_optional()) ;
	EXPECT_EQ(0, tmp.is_readable()) ;
	// EXPECT_EQ(0, tmp.get_length()) ; tego nie wykonujemy, bo przecież dane nie są gotowe do odzcytu
	EXPECT_EQ(0x30, tmp.get_tag()) ;

	EXPECT_EQ(true, tmp2.is_optional()) ;
}

TEST(DateTest, TestModyfikacjiBezposredniej)
{
	// wszystkie pola składowe są opcjonalne
	date_asn tmp = date_asn(true, true, true, false), tmp1 = date_asn(true, false, true, false) ;

	// na początku data powinna być nie do odczytu, bo nic nie wpisaliśmy do niej
	EXPECT_EQ(0, tmp.is_readable()) ;

	// wpisujemy dzień i miesiąc, roku nie wpisujemy
	integer_type d = 28, m = 8 ;
	tmp.dzien.set_value(d) ;
	tmp.miesiac.set_value(m) ;

	// wszystkiego nie wpisaliśmy, ale mamy rok opcjonalny, więc data powinna być do odczytania
	// sprawdzamy poprawny stan odczytywalności pól
	EXPECT_EQ(1, tmp.is_readable()) ;
	EXPECT_EQ(1, tmp.dzien.is_readable()) ;
	EXPECT_EQ(1, tmp.miesiac.is_readable()) ;
	EXPECT_EQ(0, tmp.rok.is_readable()) ;

	// pola składowe są normalnymi obiektami ASN_STRUCTURE, więc nie ma potrzeby jeszcze sprawdzania
	// wszystkich drobnostek, bo to robię w innych testach jednostkowych.


	// inny przykład, zadeklarujmy pewne pola jako obowiązkowe i nie wypełnijmy ich
	// dzień i rok opcjonalny, miesiąc obowiązkowy

	tmp1.rok.set_value(33) ;
	// po wypełnieniu tylko roku, stan daty powinien być nie do odczytania w całości
	// miesiąc do wypełnienia jest obowiązkowy
	EXPECT_EQ(0, tmp1.is_readable()) ;
	EXPECT_EQ(0, tmp1.dzien.is_readable()) ;
	EXPECT_EQ(0, tmp1.miesiac.is_readable()) ;
	EXPECT_EQ(1, tmp1.rok.is_readable()) ;
}

TEST(DateTest, TestStrumienia)
{
	// dzień, miesiąc, rok - pola opcjonalne
	date_asn tmp1(true, true, true), tmp2(true, true, true) ;
	stringstream strumien ;

	tmp1.set_value(28, 8, 1995) ;
	EXPECT_EQ(1, tmp1.is_readable()) ;
	tmp1.write_to_stream(strumien) ;
	tmp2.try_read_from_stream(strumien) ;
	EXPECT_EQ(tmp1, tmp2 ) ; // daty powinny się zrównać
}
