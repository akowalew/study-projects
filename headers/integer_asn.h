/*
 * integer_asn.h
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */

#ifndef HEADERS_INTEGER_ASN_H_
#define HEADERS_INTEGER_ASN_H_

#include "asn_structure.h"
#include <vector>

class integer_asn : public asn_structure
{
	long value ;

public :
	integer_asn(bool opt = false) : asn_structure(opt), value(0) { tag = 0x02 ; }

	void set_value(long val) ;

	long get_value()
	{
		return value ;
	}

	uint8_t get_length()
	{
		return length ;
	}



	bool try_read_tag_and_check(std::istream& iss, uint8_t tag)
		throw(throw_error_e) ;

	void try_read_length_contents(std::istream& iss, std::vector<uint8_t> &wektor_zawartosc)
		throw(throw_error_e) ;

	int try_read_from_stream(std::istream& iss)
		throw(throw_error_e) ;

	void write_to_stream(std::ostream& oss) ;

};


#endif /* HEADERS_INTEGER_ASN_H_ */
