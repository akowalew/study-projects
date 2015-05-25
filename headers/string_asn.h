/*
 * string_asn.h
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */

#ifndef HEADERS_STRING_ASN_H_
#define HEADERS_STRING_ASN_H_

#include <string>
#include "asn_structure.h"

class string_asn : public asn_structure
{
	std::string value ;

public :
	string_asn(bool opt = false) : asn_structure(opt) { tag = 0x1A ; }

	void set_value(std::string val) ;
	std::string get_value() { return value ; }
	uint8_t get_length() { return length ; }

	int try_read_from_stream( std::istream &iss) throw(throw_error_e) ;
	void write_to_stream(std::ostream &ss) {}

};





#endif /* HEADERS_STRING_ASN_H_ */
