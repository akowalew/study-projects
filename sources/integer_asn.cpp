/*
 * integer_asn.cpp
 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */

#include "../headers/integer_asn.h"

#include <stack>

void integer_asn::set_value(integer_type val)
{
	value = val ;
	data_can_read = true ;
}

int integer_asn::try_read_from_stream(std::istream& iss) throw(throw_error_e)
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
		value = 0 ;
		unsigned int sz = wektor_zawartosc.size() - 1 ;
		for(unsigned int i = 0 ; /*i < wektor_zawartosc.size()*/ ; i++)
		{
			value |= wektor_zawartosc[i] ;
			if(i == sz)
				break ;
			value <<= 8 ;
		}

		data_can_read = true ; // można odczytać daną

		/* INTEGER jest tak prostym typem, że nie trzeba sprawdzać poprawności
		 * na poziomie wczytanych oktetów. Jednym słowem, zgadza się ilość, zgadza się tak
		 * więc taką też liczbę wczytujemy, dodatkowo zwracamy liczbę znaków, które wyjęliśmy
		 * ze strumienia.
		 */

		return 1+1+(wektor_zawartosc.size()) ; // 1 TAGu, 1 LENGTH, +LENGTH zawartości, bo mamy LENGTH oktetów
		// każdy po dwa znaki
	}
}

void integer_asn::write_to_stream(std::ostream& oss) throw(throw_error_e)
{
	if(!is_readable())
		throw DATA_FAIL ;
	write_tag_length(oss) ;

	// zapisujemy wartość
	uint8_t len = get_length() ;
	uint8_t octet ;

	for(int i = len-1 ; i >= 0 ; i--)
	{
		octet = (value >> (8 * i)) & 0xFF ;
		oss << u8tohex(octet >> 4) ;
		oss << u8tohex(octet & 0x0F) ;
	}
}

uint8_t integer_asn::get_length() const
{
	if(value < 0) // na samym początku jest jedynka, więc trzeba zapisać całą liczbę
		return sizeof(value) ;
	integer_type val = value ;
	uint8_t ret = 0 ;
	do
	{
		val >>= 8 ;
		ret++ ;
	}while(val) ;

	return ret ;
}

bool integer_asn::operator==(const asn_structure& comp) const
{
	if(typeid(comp) != typeid(*this))
		return false ;
	else
	{
		const integer_asn *tmp = dynamic_cast<const integer_asn*>(&comp) ;
		return ((tmp != nullptr) && (*this == *tmp)) ;
	}
}
