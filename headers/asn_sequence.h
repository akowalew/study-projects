/*
 * asn_sequence.h
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */
/**
 * \file asn_sequence.h
 * \brief Deklaracja klasy sekwencji
 */
#ifndef HEADERS_ASN_SEQUENCE_H_
#define HEADERS_ASN_SEQUENCE_H_

#include <vector>
#include <iostream>
#include <typeinfo>
#include "asn_structure.h"

/**
 * \brief Sekwencja, zbiór publicznych obiektów różnego typu
 */
class asn_sequence : public asn_structure
{
protected :
	/**
	 * \brief Wektor/Lista, do którego ładujemy wskaźniki do obiektów składowych
	 *
	 */
	std::vector<asn_structure*> wektor_elementow ;
protected :

	/**
	 * \brief Dodaj element do sekwencji
	 *
	 * Dodajemy tutaj wskaźniki na obiekty składowe tworzonej przez nas sekwencji.
	 * Trzeba pamiętać o kolejności, bo jest ona istotna.
	 *
	 * @param pointer wskaźnik na obiekt składowy
	 */
	void add_item(asn_structure* pointer)
	{ wektor_elementow.push_back(pointer) ; }
public :
	/** \brief Konstruktor Sekwencji
	 *
	 * 	Lista wskaźników jest pusta, tag sekwencji to 0x30
	 * 	@param opt Czy ta sekwencja jako obiekt ma być opcjonalna?
	 */
	asn_sequence(bool opt = false) : asn_structure(0x30, opt) { wektor_elementow.clear() ; }

	/**
	 * \brief Pobierz łączną długość sekwencji
	 *
	 * @return Pobiera długość każdego z elementów, który można odczytać, i sumuje wszystkie
	 * @warning Funkcja nie sprawdza poprawności. Tzn. jeśli sekwencja jest zaznaczona jako
	 * nie do odczytania, to funkcja może zwrócić błędne dane.
	 * @see is_readable()
	 */
	uint8_t get_length() const ;

	/**
	 * \brief Spróbuj załadować całą sekwencję ze strumienia
	 *
	 * Podczas ładowania sekwencji kładziony jest nacisk na to, czy dana składowa
	 * sekwencji może być opcjonalna.
	 *
	 * @see asn_structure::try_read_from_stream(...)
	 */
	int try_read_from_stream( std::istream &ss)
		throw(throw_error_e);
	/**
	 * \brief Zapisuje całą sekwencję do pliku.
	 */
	void write_to_stream(std::ostream &ss) throw(throw_error_e) ;

	/**
	 * \brief Czy sekwencja jest w całości możliwa do odczytu ?
	 *
	 * Są dwie możliwości, kiedy sekwencja jest niemożliwa do odczytu:
	 * 1) Którykolwiek z obowiązkowych elementów nie jest zapisany
	 * 2) Mimo, iż wszystkie obiekty mogą być opcjonalne, nie mamy żadnego zapisanego,
	 * więc w sumie nie mamy niczego do odczytu.
	 *
	 * @see asn_structure::is_readable()
	 */
	bool is_readable() const ;

	/**
	 * \brief Czy dwie sekwencje są równe sobie ?
	 *
	 * Dwie sekwencje są sobie równe, kiedy mają takie same dłgości wektorów wskaźników
	 * oraz kiedy wszystkie obiekty po kolei są sobie równe.
	 * @param comp Referencja do sekwencji do porównania.
	 * @see asn_structure::operator==
	 */
	bool operator==(const asn_sequence& comp) const ;

	/**
	 * \brief Czy dwie sekwencje są równe sobie ?
	 * @param comp Referencja do struktury do porównania
	 * @see asn_sequence::operator==(const asn_sequence& comp)
	 */
	bool operator==(const asn_structure& comp) const ;

	/**
	 * \brief Konstruktor kopiujący.
	 *
	 * Tak właściwie to on tylko czyści wektor elementów oraz kopiuje informacje bazy asn_structure
	 * @param obj sekwencja, z której kopiujemy.
	 */
	asn_sequence(const asn_sequence& obj) : asn_structure(obj)
		{ wektor_elementow.clear() ; }

	/**
	 * \brief Operator przypisania.
	 *
	 * Robi to samo, co kontruktor kopiujący.
	 * @see asn_sequence(const asn_sequence& obj)
	 */
	asn_sequence& operator=(const asn_sequence& obj) ;
};


#endif /* HEADERS_ASN_SEQUENCE_H_ */
