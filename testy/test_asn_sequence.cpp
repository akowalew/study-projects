/*
 * test_asn_sequence.cpp
 *
 *  Created on: 30 maj 2015
 *      Author: dicker
 */

#include <gtest/gtest.h>
#include <sstream>
#include "../headers/asn_sequence.h"
#include "../headers/integer_asn.h"
#include "../headers/string_asn.h"
#include "../headers/date_asn.h"
using namespace std ;

#include <iostream>

TEST(SequenceTest, TestKonstruktora)
{
	asn_sequence tmp, tmp2(true) ;
	EXPECT_EQ(0, tmp.is_optional()) ;
	EXPECT_EQ(0, tmp.is_readable()) ;
	EXPECT_EQ(0x30, tmp.get_tag()) ;

	EXPECT_EQ(true, tmp2.is_optional()) ;
}

class sekwencjaTestowa : public asn_sequence
{
public :

	integer_asn objA ;
	string_asn 	objB ;

	sekwencjaTestowa(bool optA = false, bool optB = false, bool opt = false )
		: asn_sequence(opt), objA(optA), objB(optB)
	{
		save_pointers() ;
	}

	sekwencjaTestowa(const sekwencjaTestowa& x) : asn_sequence(x), objA(x.objA), objB(x.objB)
	{
		save_pointers() ;
	}

private :
	void save_pointers()
	{
		add_item(&objA) ;
		add_item(&objB) ;
	}
};

TEST(SequenceTest, TestStrumieniaPrzypadek1)
{
	sekwencjaTestowa sek1(true, true, true), sek2(true, true, true) ;
	stringstream ss1, ss2 ;

	// sprawdzam poprawność operatora porównania. Na początku oba obiekty są te same(p. wyżej)
	EXPECT_EQ(1, sek1 == sek2) ;

	// 1.a. Przypadek: Wszystkie dane są opcjonalne. Ładujemy wszystko, zapisujemy, odczytujemy.
	sek1.objA.set_value(123) ;
	sek1.objB.set_value("blabla") ;
	sek1.write_to_stream(ss1) ;
	sek2.try_read_from_stream(ss1) ;
	EXPECT_EQ(1, sek1 == sek2) ;

	// 1.b Przypadek: Skoro Wszystkie dane są załadowane. Ładujemy do strumienia, psujemy strumień, wczytujemy
	// oczekujemy wyjątku.
	ss1.clear() ;
	ss1.str("") ;

	sek1.write_to_stream(ss1) ;
	sek2 = sekwencjaTestowa(true, true, true) ;
	string tests = ss1.str().substr(0, ss1.str().size() - 2) ;
	// wywalam dwa znaki ze strumienia od końca, czyli będzie problemz  ilością znaków

	ss1.clear() ;
	ss1.str("") ;
	ss1 << tests ;

	EXPECT_THROW( sek2.try_read_from_stream(ss1), throw_error_e) << ss1.str() ;
}

TEST(SequenceTest, TestStrumieniaPrzypadek2)
{
	sekwencjaTestowa sek1(true, true, true), sek2(true, true, true) ;
	stringstream ss1, ss2 ;
	// 2.a Skoro wsystko może być opcjonalne, to jednej z danych nie zapisuję
	sek1 = sekwencjaTestowa(true, true, true) ;
	sek2 = sekwencjaTestowa(true, true, true) ;

	sek1.objB.set_value("sdada") ;
	EXPECT_EQ(1, sek1.is_readable()) ;
	EXPECT_EQ(1, sek1.objA.is_optional()) ;

	ss1.clear() ;
	ss1.str("") ;

	sek1.write_to_stream(ss1) ;
	EXPECT_NE(sek2.try_read_from_stream(ss1), -1) ;	// poprawność świadczy o tym, że dostaliśmy coś większego od zera
	// a dokładniej : różne od -1

	EXPECT_EQ(1, sek2.is_readable()) ;
	EXPECT_EQ(0, sek2.objA.is_readable()) ; // zgodnie z załżeniem, objA powinien się nei wczytać(bo go nie było)
	EXPECT_EQ(1, sek2.objB.is_readable()) ;
}

TEST(SequenceTest, TestStrumieniaPrzypadek3)
{
	sekwencjaTestowa sek1(true, false, true), sek2(false, true, true) ;
	stringstream ss1, ss2 ;

	// 3. Przypadek: pole w klasie nie jest opcjonalne, jest obowiązkowe, ponadto nie zapisane,
	// a my chcemy na siłę tak niekompletną klasę zapisać

	EXPECT_EQ(0, sek1.is_readable()) ;
	EXPECT_THROW(sek1.write_to_stream(ss1), throw_error_e) ;
	sek1.objB.set_value("dadas") ;
	sek1.write_to_stream(ss1) ;

	// teraz mamy sytuację następującą : sek2 pierwsze pole ma obowiązkowe, a do sek1 wpisaliśmy tylko pole drugie
	// wczytywanie powinno się nie powieść.
	EXPECT_EQ(-1, sek2.try_read_from_stream(ss1)) ;
	EXPECT_EQ(0, sek2.is_readable()) ;

}

TEST(SequenceTest, TestStrumieniaPrzypadek4)
{
	sekwencjaTestowa sek1(true, false, true), sek2(false, true, true) ;
	stringstream ss1, ss2 ;

	// 4. Oczekujemy konkretnego typu sekwencji, a dostajemy inny typ.
	// Uogólnienie przypadku 3.b

	date_asn data ;
	data.set_value(11, 22, 33) ;
	data.write_to_stream(ss1) ;
	EXPECT_EQ(-1, sek1.try_read_from_stream(ss1)) ;

}
