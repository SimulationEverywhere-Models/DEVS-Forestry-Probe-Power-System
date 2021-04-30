#ifndef BOOST_SIMULATION_SADINFO_HPP
#define BOOST_SIMULATION_SADINFO_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

/*******************************************/
/**************** sensor Info **************/
/*******************************************/

struct SAD_info_t{
	SAD_info_t(){}
	SAD_info_t(int i_sf, int i_num, int i_f, int i_A)
	:sendF(i_sf), numSensor(i_num), frequency(i_f), amps(i_A){}
	
		int sendF;
		int numSensor;
		int frequency;
		int amps;
};

istream& operator>> (istream& is, SAD_info_t& msg);
ostream& operator<< (ostream& os, const SAD_info_t& msg);

#endif //BOOST_SIMULATION_SADINFO_HPP
