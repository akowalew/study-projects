/*
 * test_string.cpp
 *
 *  Created on: 30 maj 2015
 *      Author: dicker
 */

#include <gtest/gtest.h>
#include "../headers/string_asn.h"
#include <string>

using namespace std;

TEST(StringTest, TestKonstruktora)
{
	string_asn tmp ;
	EXPECT_EQ(0, tmp.is_optional()) ;
	EXPECT_EQ(0, tmp.is_readable()) ;
	EXPECT_EQ(0x1A, tmp.get_tag()) ;

	tmp = string_asn(true) ;
	EXPECT_EQ(true, tmp.is_optional()) ;
}

TEST(StringTest, TestModyfikacjiBezposredniej)
{
	string_asn tmp ;
	string tekst = string("AlaMaKotaBlaBlaBla") ;

	tmp.set_value(tekst) ;
	EXPECT_EQ(tekst, tmp.get_value()) ;
	EXPECT_EQ(1, tmp.is_readable()) ;
	EXPECT_EQ(tekst.size(), tmp.get_length()) ;

	tekst = "" ;
	tmp.set_value(tekst) ;
	EXPECT_EQ(tekst, tmp.get_value()) ;
	EXPECT_EQ(1, tmp.is_readable()) ;
	EXPECT_EQ(tekst.size(), tmp.get_length()) ;
}

TEST(StringTest, TestStrumienia)
{
	string_asn tmp, tmp2 ;
	std::stringstream strumien ;

	// TEST :
	//	-> zapisujemy do jednego obiektu wartość
	// 	-> wrzucamy do strumienia
	// 	<- odczytujemy ze strumienia do drugiego obietku
	//	== sprawdzamy zgodność wartości
	//		Badamy trzy zasadnicze wartości, maksimum, minimum, zero
	string tekst = string("AlaMaKotaBlaBlaBla") ;

	tmp.set_value(tekst) ;
	tmp.write_to_stream(strumien) ;
	string s = strumien.str() ;
	tmp2.try_read_from_stream(strumien) ;
	EXPECT_EQ(tmp, tmp2) << "Zawartosc strumienia : " << s ;

	tekst = "" ;
	tmp.set_value(tekst) ;
	tmp.write_to_stream(strumien) ;
	tmp2.try_read_from_stream(strumien) ;
	EXPECT_EQ(tmp, tmp2) << "Zawartosc strumienia : " << strumien.str()  ;
}

