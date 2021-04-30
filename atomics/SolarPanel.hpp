/* 
author: Sebastien Cook 101022643 
Date Created: 10/15/2020
Date modified: 10/15/2020

Description: The following is pseudo code for the DEVS atomic model SolarPanel. This model has an ibout of temperature and visibility of every hour in a year. The model andvances 1h every iteration, and calculates the power output of a solar panel based on the temperature and visibility.

This model usese the solar panel specifications of the following panel:
https://www.amazon.ca/NewPowa-Solar-Charge-Controller-Regulator/dp/B01DOIIKJ2
*/

#ifndef __SOLAR_HPP__
#define __SOLAR_HPP__


#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>

#include "../data_structures/weather_info.hpp"

using namespace cadmium;
using namespace std;

//Port definition
struct SolarPanel_defs{
    struct out : public out_port<int> { };
    struct weather : public in_port<weather_info_t> { };
};

const int ampHour = 8.33;	//typical 100W solar panel charges typical 12v battery at 8.33 amps per hour in perfect conditions
const int TC = 0.55; //temp coefficient -> power outputed reduces by 0.55% when above 25 c
const int maxView = 16.1; //the visibility at which efficency is at 100%

template<typename TIME> class SolarPanel
{
	public:
	// ports
	using input_ports=tuple<typename SolarPanel_defs::weather>;
	using output_ports=tuple<typename SolarPanel_defs::out>;
	
	//state variables
	struct state_type{
		int ef;
		bool day;
		int temp;
		int visibility;
		int time;
	};
	state_type state;
	
	//setup
	SolarPanel(){
		state.ef = 0;	       //its just setup so its not charging just yet
		state.day = false;    //starts at false because the weather trace starts at time 0
		state.temp = 25;            //start it at its ideal temp, will be changed by first input
		state.visibility = 16.1; //max visibility of trace
		state.time = 0;      //tracks time of day since internal time only recodrs time elapsed
	}
	
	//delta internal
	void internal_transition(){
		float f = 1;
		int efficiency = 100;
		state.time += 1; 
		if(state.time == 24){
			state.time = 0;
		}
		if(state.time >= 8 && state.time <= 16){ //sunrise at 8 and sunset at 5 on average
			state.day = true;
		}
		else{
			state.day = false; 
		}
		
		if(state.day){
			
			f = ((float)state.visibility/16);
			efficiency = (int)(f *100);
			if(state.temp <= 25){
			state.ef = efficiency; 
			} //ignore since ideal temp is below 25 degrees
			else if(state.temp > 25 && state.temp <= 85){
				state.ef = efficiency - ((state.temp-25)*0.0055);
			}
			else{
				state.ef = 0; //too hot, solar panel stops producing
			}
			if(state.ef < 0){state.ef = 0;} //no negative precentage
		}
		else{
			state.ef = 0;   //panel doesnt produce power at night
		}
	}
	
	//delta external
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
		
		for(const auto &x : get_messages<typename SolarPanel_defs::weather>(mbs)){
              
			state.temp = x.temp;
			state.visibility = x.visibility;
		}
	}
	
	// confluence transition??
	void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
		internal_transition();
		external_transition(TIME(), std::move(mbs));
	}
	
	//lambda function
	typename make_message_bags<output_ports>::type output() const{
		int out = 8;
		if(state.ef <= 0){out = 0;}
		else if(state.ef >= 86 && state.ef <= 99){ out = 7;}
		else if(state.ef >= 73 && state.ef < 86) { out = 6;}
		else if(state.ef >= 61 && state.ef < 73) { out = 5;}
		else if(state.ef >= 48 && state.ef < 61) { out = 4;}
		else if(state.ef >= 36 && state.ef < 48) { out = 3;}
		else if(state.ef >= 24 && state.ef < 36) { out = 2;}
		else if(state.ef > 0 && state.ef < 24) { out = 1;}
		typename make_message_bags<output_ports>::type bags;
		get_messages<typename SolarPanel_defs::out>(bags).push_back(out);  //output the current amp hour based on the weather of the last hour
		return bags;
	}
	
	//Time advance
	TIME time_advance() const{
		TIME next_internal;
		next_internal = TIME("00:00:01:000"); //every hour sends amp hour to battery
		return next_internal;
	}
	
	friend std::ostringstream& operator<<(std::ostringstream& os, const typename SolarPanel<TIME>::state_type& i){
		os << "temp: " << i.temp << " visibility: " << i.visibility << " efficieny: " << i.ef << " time: " << i.time << " day? " << i.day;
		return os;
	}
};
#endif // _SOLAR_HPP_

































