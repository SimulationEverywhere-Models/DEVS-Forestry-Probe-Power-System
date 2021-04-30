/* 
author: Sebastien Cook 101022643 
Date Created: 10/15/2020
Date modified: 10/15/2020

Description: The following is pseudo code for the DEVS atomic model Sensors. Sensors is an atomic model that is part of the consumers coupled model. This is one of the two models that its initial conditions can be modified to change the outcome of the overal simulation. The iitial conditions that can be modified are: number of sensors, frequency(1-i) of samples and power(1-i) and passivePpower(1-i). given its initial conditions, the sensors will output the power used every hour

This model usese the following sensors and specifications:

*/

#ifndef __SENSOR_HPP__
#define __SENSOR_HPP__


#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>

#include "../data_structures/Sensor_info.hpp"

using namespace cadmium;
using namespace std;

//Port definition
struct Sensor_defs{
    struct out : public out_port<int> { };
    struct sensorInfo : public in_port<Sensor_info_t> { };
};

const int maxSensors = 10; //max amount of sensors you can have

template<typename TIME> class Sensor
{
	public:
	// ports
	using input_ports=tuple<typename Sensor_defs::sensorInfo>;
	using output_ports=tuple<typename Sensor_defs::out>;
	
	//state variables
	struct state_type{
		int frequency[maxSensors];
		int amps[maxSensors];
		int Twait[maxSensors];
	};
	state_type state;
	
	//set up
	Sensor(){
		for(int i=0; i < maxSensors; i++){
			state.frequency[i] = 0;
			state.amps[i] = 0;
			state.Twait[i] = 0;
		}
	}
	
	//delta internal
	void internal_transition(){
		for(int i=0; i < maxSensors; i++){
			state.Twait[i] -= 1;
			if(state.Twait[i] == -1){
				state.Twait[i] = state.frequency[i];
			}
		}
	}
	
	//delta external
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
		for(const auto &x : get_messages<typename Sensor_defs::sensorInfo>(mbs)){
              
			int numSensors = x.numSensor;
			if(numSensors >= 0 && numSensors < 10){  //there is a max of 10 sensors 0-9
				state.frequency[numSensors] = x.frequency;
				state.amps[numSensors] = x.amps;
				state.Twait[numSensors] = state.frequency[numSensors];
			}
		}
	}
	
	// confluence transition??
	void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
		internal_transition();
		external_transition(TIME(), std::move(mbs));
	}
	
	//lambs function
	typename make_message_bags<output_ports>::type output() const{
		int ampsOut = 0;
		for(int i=0; i < maxSensors; i++){
			if(state.Twait[i] == 0){
				ampsOut += state.amps[i];
			}
		}
		typename make_message_bags<output_ports>::type bags;
		get_messages<typename Sensor_defs::out>(bags).push_back(ampsOut);
		return bags;
	}
	
	//Time advance
	TIME time_advance() const{
		TIME next_internal; 
		next_internal = TIME("00:00:01:000"); //every hour sends amp "hour" to battery; represented as a second to allow 8760 cycles 
		return next_internal;
	}
	
	friend std::ostringstream& operator<<(std::ostringstream& os, const typename Sensor<TIME>::state_type& i){
		int ampsOut = 0;
		for(int j=0; j< maxSensors; j++){
			if(i.Twait[j] == 0){
				ampsOut += i.amps[j];
			}
		}
		os << "Current use: " << ampsOut;
		return os;
	}
};
#endif // _SENSOR_HPP_
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
