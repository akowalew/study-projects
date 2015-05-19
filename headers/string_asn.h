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
	string_asn() : value() { tag = 0x18 ; }

	void set_value(std::string val) ;
	std::string get_value() { return value ; }

	uint8_t get_length() { return length ; }
	bool try_read_from_stream( std::istream ss) ;
	void write_to_stream(std::ostream ss) ;

};





#endif /* HEADERS_STRING_ASN_H_ */
