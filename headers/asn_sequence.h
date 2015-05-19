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
	std::vector<bool> wektor_wystapien ;

public :

	void add_item(asn_structure* pointer) { }
	uint8_t get_length() { }
	bool try_read_from_stream( std::istream ss) { }
	void write_to_stream(std::ostream ss) { }
};


#endif /* HEADERS_ASN_SEQUENCE_H_ */
