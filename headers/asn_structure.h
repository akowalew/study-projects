/*
 * asn_structure.h
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */

#ifndef HEADERS_ASN_STRUCTURE_H_
#define HEADERS_ASN_STRUCTURE_H_

#include <istream>
#include <ostream>
#include <cctype>
#include <vector>

typedef unsigned char uint8_t ;

enum throw_error_e
{
	STREAM_FAIL ,
	DATA_FAIL,

};


class asn_structure
{

protected :

	friend class asn_sequence ; // do przedyskutowania!

	bool data_is_optional ; // czy ten obiekt jest opcjonalny ?
	bool data_can_read ;	// czy do danego obiektu zostało coś zapisane?

	uint8_t tag ;
	uint8_t length ;

	uint8_t hextou8(char c) ;

	bool try_read_all(std::istream &iss, std::vector<uint8_t> &wektor_zawartosc) throw(throw_error_e) ;
	uint8_t read_octet(std::istream& iss) throw(throw_error_e) ;

public :

	asn_structure(bool opt = false) : data_is_optional(opt) , data_can_read(false), tag(0), length(0) { }


	bool is_optional() { return data_is_optional ; }
	bool is_readable() { return data_can_read ; }

	uint8_t get_tag() { return tag ; }
	//void set_tag(uint8_t t) { tag = t ; }

	virtual uint8_t get_length() { return length ; }

	// ważne założenie, każdy, jak się okaże, że nie może załadować się, to niech odda znaki, które wczytał
	// funkcja zwraca ilość oktetów, które zostało wyjętych ze strumienia
	virtual int try_read_from_stream( std::istream& ss) throw(throw_error_e) { return -1 ; }
	virtual void write_to_stream(std::ostream& ss) { }

private :

	bool try_read_tag_and_check(std::istream& iss, uint8_t tag) throw(throw_error_e) ;
	void try_read_length_contents(std::istream& iss, std::vector<uint8_t> &wektor_zawartosc) throw(throw_error_e) ;


};


#endif /* HEADERS_ASN_STRUCTURE_H_ */
