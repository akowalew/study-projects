/*
 * asn_structure.cpp
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */

#include "../headers/asn_structure.h"

uint8_t asn_structure::hextou8(char c) const
{
	// funkcja konwertująca znak oznaczający cyfrę z systemu szesnastkowego
	// na wartość dziesiętną
	if(isdigit(c))
		return c - '0' ;
	else // mamy cyfrę systemu szesnastkowego
		return tolower(c) - 'a' + 10 ;
}

uint8_t asn_structure::read_octet(std::istream& iss) const throw(throw_error_e)
{
	// wczytujemy dwa znaki
	char c1, c2 ;
	iss >> c1 >> c2 ;
	if(!iss.good())
		throw(STREAM_FAIL) ;

	// mamy dwa znaki, teraz konwertujemy to na oktet
	uint8_t octet = 0 ;
	// muszą to być znaki cyfr systemu szesnatkowego
	if(!isxdigit(c1) || !isxdigit(c2))
		throw(DATA_FAIL) ;
	octet |= (hextou8(c1) << 4) | (hextou8(c2)) ;

	return octet ;
}

bool asn_structure::try_read_tag_and_check(std::istream& iss, uint8_t tag) throw(throw_error_e)
{
	try
	{
		uint8_t r_tag = 0 ;
		r_tag = read_octet(iss) ; // wczytanie dwóch znaków
		// sprawdzamy, czy to jest tag INTEGER
		if(r_tag != tag)
		{
			// musimy oddać znaki do strumienia
			// dwa znaki
			iss.unget() ;
			iss.unget() ;

			// nie pasuje nam to!
			return false ;
		}

		return true ;
	}
	catch(throw_error_e &err)
	{
		throw(err) ;
	}
}

void asn_structure::try_read_length_contents(	std::istream& iss,
												std::vector<uint8_t> &wektor_zawartosc)
												throw(throw_error_e)
{
	// ok, TAG się zgadza, teraz zczytujemy LENGTH, a potem zczytujemy zawartość do wektora

	try
	{
		uint8_t r_length = read_octet(iss) ; // wczytanie dwóch znaków
		// zakładamy formę krótką, czyli wielkość <= 127 !!!
		if(r_length > 127)
			throw(DATA_FAIL) ;

		// zczytuję zadaną zawartość do pamięci
		uint8_t octet_read ;
		for(uint8_t i = 0 ; i < r_length ; i++)
		{
			try
			{
				octet_read = read_octet(iss) ; // wczytanie dwóch znaków
			}
			catch(throw_error_e &err)
			{
				throw(err) ;
			}

			wektor_zawartosc.push_back(octet_read) ;
		}
	}
	catch(throw_error_e &err)
	{
		throw(err) ;
	}
}

bool asn_structure::try_read_all(	std::istream &iss,
									std::vector<uint8_t> &wektor_zawartosc)
									throw(throw_error_e)
{
	try
	{
		if(!try_read_tag_and_check(iss, tag))
			return false ;	// automatycznie zwrócił dwa znaki do strumienia z powrotem
		//	próba odczytu długości i zawartości
		try_read_length_contents(iss, wektor_zawartosc) ;
	}
	catch(throw_error_e &err) { throw err ; }

	return true ;
}

char asn_structure::u8tohex(uint8_t u8) const
{
	if(u8 >= 10)
		return 'a' + (u8-10) ;
	else
		return '0' + (u8) ;
}

void asn_structure::write_tag_length(std::ostream &oss)
{
	// piszemy pierw TAG
	oss << u8tohex(tag >> 4) ;
	oss << u8tohex(tag & 0x0F) ;

	uint8_t len = get_length() ;
	oss << u8tohex(len >> 4) ;
	oss << u8tohex(len & 0x0F) ;
}
