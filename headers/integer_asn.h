/*
 * integer_asn.h
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */

#ifndef HEADERS_INTEGER_ASN_H_
#define HEADERS_INTEGER_ASN_H_

#include "asn_structure.h"

class integer_asn : public asn_structure
{
	long value ;

public :
	integer_asn(bool opt = false) : value(0) { tag = 0x02 ; this->is_optional = opt ; }

	void set_value(long val) ;
	long get_value() { return value ; }

	uint8_t get_length() { return length ; }
	bool try_read_from_stream( std::istream ss) { }
	void write_to_stream(std::ostream ss) { }

};


#endif /* HEADERS_INTEGER_ASN_H_ */
