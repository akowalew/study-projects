/**
 * \file string_asn.h
 * \brief Deklaracja klasy reprezentującej napisy
 */

#ifndef HEADERS_STRING_ASN_H_
#define HEADERS_STRING_ASN_H_

#include <string>
#include "asn_structure.h"
/**
 * \brief Klasa reprezentująca napis
 *
 *
 */
class string_asn : public asn_structure
{
	std::string value ;	//!< Napis, którego

public :
	/**
	 * Konstruktor, przypisuje napisowi tag = 0x1A
	 *
	 * @param opt Czy napis, jako obiekt, ma być opcjonalny?
	 */
	string_asn(bool opt = false) : asn_structure(0x1A, opt) { }

	/**
	 * \brief Zapisuje do obiektu napis
	 * @param val napis, który przepiszemy
	 */
	void set_value(std::string val) ;
	/**
	 * \brief Pobiera napis od obiektu.
	 *
	 * @warning Funkcja nie sprawdza, czy dane można czytać z obiektu.
	 * Samemu trzeba się upewnić.
	 * @see is_readable()
	 */
	std::string get_value() const { return value ; }
	/**
	 * \brief Pobiera długość naszego napisu, w oktetach.
	 *
	 * @warning Funkcja nie sprawdza, czy dane można czytać z obiektu.
	 * Samemu trzeba się upewnić.
	 * @see is_readable()
	 */
	uint8_t get_length() const { return value.size() ; }

	/**
	 * \brief Pobiera obiekt STRING ze strumienia. Oczekiwany tag: 0x1A
	 *
	 * @see asn_structure::try_read_from_stream(...)
	 */
	int try_read_from_stream( std::istream &iss) throw(throw_error_e) ;
	/**
	 * \brief Zapisuje obiekt STRING do strumienia. Zapisywany tag: 0x1A
	 *
	 * @see asn_structure::write_to_stream(...)
	 */
	void write_to_stream(std::ostream &ss) throw(throw_error_e);

	/**
	 * \brief Czy dwa napisy są sobie równe?
	 *
	 * @param comp string do porównania
	 */
	bool operator==(const string_asn& comp) const
	{
		return asn_structure::operator==(comp) && (value == comp.get_value()) ;
	}

	/**
	 * \brief Czy dwa napisy są sobie równe?
	 *
	 * @param comp string do porównania
	 */
	bool operator==(const asn_structure& comp) const ;
};

#endif /* HEADERS_STRING_ASN_H_ */
