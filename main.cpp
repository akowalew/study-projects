/*
 * main.cpp

 *
 *  Created on: 20 maj 2015
 *      Author: dicker
 */
#include <iostream>
#include <fstream>
#include <cstring>

#include "headers/date_asn.h"
#include "headers/integer_asn.h"
#include "headers/string_asn.h"
#include "headers/asn_sequence.h"
#include "headers/czlowiek_asn.h"

#include <gtest/gtest.h>

using namespace std ;

int main(int argc, char **argv) {

	date_asn i1,
			 i2 = i1 ;
	asn_structure &p1 = i1, &p2 = i2 ;

	i2.dzien.set_value(34) ;
	i2.rok.set_value(544) ;
	i1 = i2 ;

  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}



