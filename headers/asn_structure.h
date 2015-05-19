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

typedef unsigned char uint8_t ;

class asn_structure
{

protected :
	bool is_optional ; // czy ten obiekt jest opcjonalny ?
	bool can_read ;	// czy do danego obiektu zostało coś zapisane?

	uint8_t tag ;
	uint8_t length ;

public :

	asn_structure(bool opt = false) : is_optional(opt) , can_read(false), tag(0), length(0) { }


	uint8_t get_tag() { return tag ; }
	void set_tag(uint8_t t) { tag = t ; }

	virtual uint8_t get_length() { return length ; }
	virtual bool try_read_from_stream( std::istream ss) { return false ; }
	virtual void write_to_stream(std::ostream ss) { }

};


#endif /* HEADERS_ASN_STRUCTURE_H_ */
