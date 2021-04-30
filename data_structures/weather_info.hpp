#ifndef BOOST_SIMULATION_WEATHER_HPP
#define BOOST_SIMULATION_WEATHER_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

/*******************************************/
/**************** weather_info_t **************/
/*******************************************/

struct weather_info_t{
	weather_info_t(){}
	weather_info_t(int i_t, int i_v)
	:temp(i_t), visibility(i_v){}
	
		int temp;
		int visibility;
};

istream& operator>> (istream& is, weather_info_t& msg);
ostream& operator<< (ostream& os, const weather_info_t& msg);

#endif //BOOST_SIMULATION_WEATHER_HPP
