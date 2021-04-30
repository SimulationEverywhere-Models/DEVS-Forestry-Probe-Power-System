/* 
author: Sebastien Cook 101022643 
Date Created: 10/15/2020
Date modified: 10/15/2020

Description: The following is pseudo code for the DEVS atomic model TransmitterReceiver. TransmitterReceiver is an atomic model that is part of the consumers coupled model. This is one of the two models that its initial conditions can be modified to change the outcome of the overal simulation. The iitial conditions that can be modified are: trasnmition max duration, transmition power, receiver frequency, receiver duration, receiver power. the TransmitterReceiver model will output the power used every hour.

This model usese the following sensors and specifications:

*/

#ifndef _SAD_HPP__
#define _SAD_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>

#include "../data_structures/Sensor_info.hpp"
#include "../data_structures/SAD_info.hpp"

using namespace cadmium;
using namespace std;

//Port definition
struct SAD_defs{
    struct SensorOut : public out_port<Sensor_info_t> {};
    struct out : public out_port<int> {};
    struct info : public in_port<SAD_info_t> {};
};

const int senderAmps = 10; //amp hours of the sender when working 
const int receiverAmps = 5; //amp hours the receiver consumes when active 

template<typename TIME> class SAD
{
	public:
	//ports 
	using input_ports=tuple<typename SAD_defs::info>;
	using output_ports=tuple<typename SAD_defs::out, typename SAD_defs::SensorOut>;
	
	struct state_type{
		bool sending;
		bool receiving;
		bool newData;
		int sFrequency;
		int sendDay;
		int day;
		int hour;
		Sensor_info_t sensor;
	};
	state_type state;
	
	//set up
	SAD(){
		state.sending = false;
		state.receiving = true; //receive first set of instructions
		state.newData = false; 
		state.sFrequency = 1; 
		state.sendDay = 7;
		state.day = 1;
		state.hour = 0;
		state.sensor.numSensor = 0; 
		state.sensor.frequency = 0;
		state.sensor.amps = 0;
	}
	
	//delta internal
	void internal_transition(){
	
		if(!state.receiving && !state.sending)
		{
			state.day++;
		}
		else
		{
			state.hour++;
			state.newData = false;
			if(state.hour == 24)
			{
				state.hour = 0;
				state.day++;
				state.receiving = false; 
				state.sending = false; 
			}
			
		}	
		if(state.day%7 == 0){
			state.receiving = true; //receive once a week on the sunday
		}
		if(state.day == state.sendDay){
			state.sending = true; 
			state.sendDay += state.sFrequency;
			if(state.sendDay < 7) {state.sendDay = 0 + (state.sendDay - 7);}
		}
	}
	
	//delta external
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
		
		if(state.receiving){
			for(const auto &x : get_messages<typename SAD_defs::info>(mbs)){
              			if(x.sendF != 0)							//0 means ignore 
				{
					state.sFrequency = x.sendF;
				}
				state.sensor.numSensor = x.numSensor;
				state.sensor.frequency = x.frequency;
				state.sensor.amps = x.amps;
			}
			state.newData = true;
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
		if(state.receiving)
		{
			ampsOut +=5;
		}
		if(state.sending)
		{
			ampsOut += 10;
		}
		
		
		typename make_message_bags<output_ports>::type bags;
		get_messages<typename SAD_defs::out>(bags).push_back(ampsOut);  
		Sensor_info_t info;
		if(state.newData){
			info.numSensor = state.sensor.numSensor;
			info.frequency = state.sensor.frequency;
			info.amps = state.sensor.amps;
			get_messages<typename SAD_defs::SensorOut>(bags).push_back(info);
		}
		return bags;
	}
	
	//Time advance
	TIME time_advance() const{
		TIME next_internal; 
		if(!state.receiving && !state.sending)
		{
			next_internal = TIME("00:00:24:000");
		}
		else
		{
			next_internal = TIME("00:00:01:000"); //every dayhour while sending or receiving 
		}
		return next_internal;
	}
	
	friend std::ostringstream& operator<<(std::ostringstream& os, const typename SAD<TIME>::state_type& i){
		os << "day: " << i.day << " Receing: " << i.receiving << " new data: " << i.newData << " sending: " << i.sending;
		return os;
	}
	
};
#endif // _SAD_HPP__









































