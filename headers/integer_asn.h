/**
 * \file integer_asn.h
 * \brief Deklaracja klasy Integer
 */

#ifndef HEADERS_INTEGER_ASN_H_
#define HEADERS_INTEGER_ASN_H_

#include "asn_structure.h"
#include <vector>
typedef long long integer_type ;
/**
 * \brief Klasa reprezentująca liczby całkowite długie
 */
class integer_asn : public asn_structure
{
	integer_type value ; //!< Liczba całkowita długa

public :
	/**
	 * Konstruktor
	 *
	 * Ustawia tag = 0x02 oraz wartość zawartości równą 0
	 * @param opt Czy obiekt ma być opcjonalny ?
	 */
	integer_asn(bool opt = false) : asn_structure(0x02, opt), value(0) {}

	void set_value(integer_type val) ; //!< Zapisuje wartość. @param val Liczba całkowita do zapisania

	/**
	 * \brief Pobiera wartość
	 *
	 * @warning Funkcja nie sprawdza, czy dane można czytać z obiektu.
	 * Samemu trzeba się upewnić
	 * @see is_readable()
	 */
	integer_type get_value() const  { return value ; }
	/**
	 * \brief Pobiera długość
	 *
	 * @warning Funkcja nie sprawdza, czy dane można czytać z obiektu.
	 * Samemu trzeba się upewnić
	 * @see is_readable()
	 */
	uint8_t get_length() const  ;

	/**
	 * \brief Pobiera INTEGER ze strumienia. Oczekiwany tag: 0x02
	 *
	 * @see asn_structure::try_read_from_stream(...)
	 */
	int try_read_from_stream(std::istream& iss)
		throw(throw_error_e) ;
	/**
	 * \brief Zapisuje INTEGER do strumienia. Zapisywany tag: 0x02
	 *
	 * @see asn_structure::write_to_stream(...)
	 */
	void write_to_stream(std::ostream& oss) throw(throw_error_e);

	bool operator==(const integer_asn& comp) const
	{
		return (asn_structure::operator==(comp)) && (value == comp.get_value()) ;
	}

	bool operator==(const asn_structure& comp) const ;

};


#endif /* HEADERS_INTEGER_ASN_H_ */
