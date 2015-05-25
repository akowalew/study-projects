/*
 * asn_sequence.h
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */

#ifndef HEADERS_ASN_SEQUENCE_H_
#define HEADERS_ASN_SEQUENCE_H_

#include <vector>

#include "asn_structure.h"

class asn_sequence : public asn_structure
{
protected :
	std::vector<asn_structure*> wektor_elementow ;

public :

	asn_sequence(bool opt = false) : asn_structure(opt), wektor_elementow(0) { tag = 0x30 ; }

	void add_item(asn_structure* pointer)
	{
		// funkcja dodaje do tablicy wskaźnik na nasz kolejny element w sekwencji
		wektor_elementow.push_back(pointer) ;
	}

	uint8_t get_length() ;

	int try_read_from_stream( std::istream &ss) throw(throw_error_e);

	void write_to_stream(std::ostream &ss) { }
};


#endif /* HEADERS_ASN_SEQUENCE_H_ */
