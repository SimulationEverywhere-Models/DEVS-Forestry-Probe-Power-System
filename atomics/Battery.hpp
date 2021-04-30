/* 
author: Sebastien Cook 101022643 
Date Created: 10/15/2020
Date modified: 10/15/2020

Description: The following is pseudo code for the DEVS atomic model Battery. The battery is the model thats effected by both the solarPanel and powerCOnsumers models. Its also the model connected to the top models output since its power level is of interest. The solarPanel model inputs power given to the battery to charge it. The powerConsumer inputs the power used from the battery. The Battery then outputs its power level every hour. 

This model usese the following battery specifications:
https://www.amazon.ca/Renogy-100Ah-Lithium-Phosphate-Battery/dp/B07SZ6B266/ref=sr_1_2_sspa?dchild=1&keywords=rv+battery&qid=1602968136&sr=8-2-spons&psc=1&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUExNTBPUDhDSEI2TVQ3JmVuY3J5cHRlZElkPUEwODU0MTI2SDlLUlZKRldRV1lXJmVuY3J5cHRlZEFkSWQ9QTA3MTE3MDkxTTc3VVMyVFBVOE9UJndpZGdldE5hbWU9c3BfYXRmJmFjdGlvbj1jbGlja1JlZGlyZWN0JmRvTm90TG9nQ2xpY2s9dHJ1ZQ==
*/

#ifndef _BATTERY_HPP__
#define _BATTERY_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>

using namespace cadmium;
using namespace std;

//Port definition
struct Battery_defs{
    struct out : public out_port<int> { };
    struct ampsIn : public in_port<int> { };
    struct ampsOut : public in_port<int> { };
};


template<typename TIME> class Battery
{
	public:
	//ports
	using input_ports=tuple<typename Battery_defs::ampsIn, typename Battery_defs::ampsOut>;
	using output_ports=tuple<typename Battery_defs::out>;
	
	struct state_type{
		int charge;  //precentage battery life left
		TIME next_internal;
	};
	state_type state;
	
	//setup
	Battery(){
		state.charge = 100; 
		state.next_internal = std::numeric_limits<TIME>::infinity();
	}
	
	//delta internal
	void internal_transition(){
	 //do nothing since this is a passive model
	}
	
	//delta external
	void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
	
		
		vector<int> used = get_messages<typename Battery_defs::ampsOut>(mbs);
		vector<int>  given = get_messages<typename Battery_defs::ampsIn>(mbs);
		
		state.charge += (given[0]-used[0]); 
		if(state.charge > 100)
		{
			state.charge = 100; 
		}
		if(state.charge < 0)
		{
			state.charge = 0;
		}
	}
	
	// confluence transition??
	void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs){
		internal_transition();
		external_transition(TIME(), std::move(mbs));
	}
	
	//lambda function 
	typename make_message_bags<output_ports>::type output() const{
		typename make_message_bags<output_ports>::type bags;
		get_messages<typename Battery_defs::out>(bags).push_back(state.charge);
		return bags;
	}
	
	//Time advance
	TIME time_advance() const{
		TIME next_internal;
		next_internal = std::numeric_limits<TIME>::infinity();
		return next_internal;
	}
	
	friend std::ostringstream& operator<<(std::ostringstream& os, const typename Battery<TIME>::state_type& i){
		os << "Charge: " << i.charge;
		return os;
	}
};
#endif // _BATTERY_HPP_
































