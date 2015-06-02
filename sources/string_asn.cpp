/*
 * string_asn.cpp
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */
#include "../headers/string_asn.h"

int string_asn::try_read_from_stream( std::istream &iss) throw(throw_error_e)
{
	data_can_read = false ;
	bool state ;

	std::vector<uint8_t> wektor_zawartosc ;
	try
	{
		state = try_read_all(iss, wektor_zawartosc) ; // czy się zgadza TAG ?
	}
	catch(throw_error_e &ex) { throw ex ; }

	if(!state)
		return -1 ;
	else
	{
		// mamy zawartość, więc teraz odkodujemy to, co odczytaliśmy.
		// po prostu odczytujemy znaki
		value.resize(wektor_zawartosc.size()) ;
		for(unsigned int i = 0 ; i < wektor_zawartosc.size() ; i++)
			value[i] = wektor_zawartosc[i] ;

		data_can_read = true ; // można odczytać daną

		/* INTEGER jest tak prostym typem, że nie trzeba sprawdzać poprawności
		 * na poziomie wczytanych oktetów. Jednym słowem, zgadza się ilość, zgadza się tak
		 * więc taką też liczbę wczytujemy, dodatkowo zwracamy liczbę znaków, które wyjęliśmy
		 * ze strumienia.
		 */

		return 1+1+(get_length()) ; // 1 TAGu, 1 LENGTH, +LENGTH zawartości, bo mamy LENGTH oktetów
		// każdy po dwa znaki
	}
}

void string_asn::set_value(std::string val)
{
	value = val ;
	data_can_read = true ;
}


void string_asn::write_to_stream(std::ostream & oss) throw(throw_error_e)
{
	if(!is_readable())
		throw DATA_FAIL ;
	write_tag_length(oss) ;
	uint8_t u ;

	for(std::string::iterator it = value.begin() ;
			it != value.end() ; it++)
	{
		u = static_cast<uint8_t>(*it) ;
		oss << u8tohex(u >> 4) ;
		oss << u8tohex(u & 0x0F) ;
	}
}

bool string_asn::operator==(const asn_structure& comp) const
{
	if(typeid(comp) != typeid(*this))
		return false ;
	else
	{
		const string_asn *tmp = dynamic_cast<const string_asn*>(&comp) ;
		return ((tmp != nullptr) && (*this == *tmp)) ;
	}
}
