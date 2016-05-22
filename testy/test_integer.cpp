/*
 * test_integer.cpp
 *
 *  Created on: 30 maj 2015
 *      Author: dicker
 */


#include "../headers/integer_asn.h"

#include <gtest/gtest.h>
#include <climits>
#include <sstream>
#include <string>
using namespace std ;

TEST(IntegerTest, TestKonstruktora)
{
	integer_asn tmp ;
	EXPECT_EQ(0, tmp.is_optional()) ;
	EXPECT_EQ(0, tmp.is_readable()) ;
	// EXPECT_EQ(0, tmp.get_length()) ; tego nie wykonujemy, bo przecież dane nie są gotowe do odzcytu
	EXPECT_EQ(0x02, tmp.get_tag()) ;

	tmp = integer_asn(true) ;
	EXPECT_EQ(true, tmp.is_optional()) ;
}

TEST(IntegerTest, TestModyfikacjiBezposredniej)
{
	integer_asn tmp ;

	tmp.set_value(0x546578FF) ;
	EXPECT_EQ(0x546578FF, tmp.get_value()) ;
	EXPECT_EQ(1, tmp.is_readable()) ;
	EXPECT_EQ(4, tmp.get_length()) ;

	tmp.set_value(0xF0000) ;
	EXPECT_EQ(0x0F0000, tmp.get_value()) ;
	EXPECT_EQ(1, tmp.is_readable()) ;
	EXPECT_EQ(3, tmp.get_length()) ;

	tmp.set_value(INT_MAX) ;
	EXPECT_EQ(INT_MAX, tmp.get_value()) ;
	EXPECT_EQ(4, tmp.get_length()) ;
	tmp.set_value(LONG_MIN) ;
	EXPECT_EQ(sizeof(LONG_MIN), tmp.get_length()) ; // kodowanie u2, czyli na najstarszym bicie '1'
	EXPECT_EQ(LONG_MIN, tmp.get_value()) ;
}

TEST(IntegerTest, TestStrumienia)
{
	integer_asn tmp, tmp2 ;
	std::stringstream strumien ;

	// TEST :
	//	-> zapisujemy do jednego obiektu wartość
	// 	-> wrzucamy do strumienia
	// 	<- odczytujemy ze strumienia do drugiego obietku
	//	== sprawdzamy zgodność wartości
	//		Badamy trzy zasadnicze wartości, maksimum, minimum, zero
	tmp.set_value(LONG_MAX) ;
	tmp.write_to_stream(strumien) ;
	tmp2.try_read_from_stream(strumien) ;
	EXPECT_EQ(tmp.get_value(), tmp2.get_value()) << "Zawartosc strumienia : " << strumien.str() ;

	tmp.set_value(LONG_MIN) ;
	tmp.write_to_stream(strumien) ;
	tmp2.try_read_from_stream(strumien) ;
	EXPECT_EQ(tmp.get_value(), tmp2.get_value()) << "Zawartosc strumienia : " << strumien.str()  ;

	tmp.set_value(0) ;
	tmp.write_to_stream(strumien) ;
	tmp2.try_read_from_stream(strumien) ;
	EXPECT_EQ(tmp.get_value(), tmp2.get_value()) << "Zawartosc strumienia : " << strumien.str()  ;
}

