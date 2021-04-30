#ifndef BOOST_SIMULATION_SENSORINFO_HPP
#define BOOST_SIMULATION_SENSORINFO_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

/*******************************************/
/**************** sensor Info **************/
/*******************************************/

struct Sensor_info_t{
	Sensor_info_t(){}
	Sensor_info_t(int i_num, int i_f, int i_A)
	:numSensor(i_num), frequency(i_f), amps(i_A){}
	
		int numSensor;
		int frequency;
		int amps;
};

istream& operator>> (istream& is, Sensor_info_t& msg);
ostream& operator<< (ostream& os, const Sensor_info_t& msg);

#endif //BOOST_SIMULATION_SENSORINFO_HPP
