//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_coupled.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Data structures
#include "../data_structures/weather_info.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs

#include "../atomics/SolarPanel.hpp"


//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
struct weatherIn : public cadmium::in_port<weather_info_t>{};

/***** Define output ports for coupled model *****/
struct ampOut: public cadmium::out_port<int>{};

/***input reader atomic model***/
template<typename T>
class InputReader_weather_info_t : public iestream_input<weather_info_t,T> {
	public:
		InputReader_weather_info_t () = default;
		InputReader_weather_info_t (const char* file_path) : iestream_input<weather_info_t,T>(file_path) {}
};


int main(){

	//input reader
	const char * i_input_data_weather = "../input_data/weather_input_data.txt";
	shared_ptr<dynamic::modeling::model> input_reader_weather = dynamic::translate::make_dynamic_atomic_model<InputReader_weather_info_t, TIME, const char* >("input_reader_weather", std::move(i_input_data_weather)); 

	//solar panel instantiation 
	shared_ptr<dynamic::modeling::model> solarPanel1 = dynamic::translate::make_dynamic_atomic_model<SolarPanel,TIME>("solarPanel1");
	
	//top model
	dynamic::modeling::Ports iports_TOP = {};
	dynamic::modeling::Ports oports_TOP = {typeid(ampOut)};
	dynamic::modeling::Models submodels_TOP = {input_reader_weather, solarPanel1};
	//external inputs
	dynamic::modeling::EICs eics_TOP = {};
	//external outputs
	dynamic::modeling::EOCs eocs_TOP = {
	dynamic::translate::make_EOC<SolarPanel_defs::out,ampOut>("solarPanel1")
	};
	//internal links
	dynamic::modeling::ICs ics_TOP = {
	dynamic::translate::make_IC<iestream_input_defs<weather_info_t>::out,SolarPanel_defs::weather>("input_reader_weather","solarPanel1")
	};
	
	shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
	
	TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);
	
	//loggers
	static ofstream out_messages("../simulation_results/solarPanel_sim_outputs.txt");
	struct oss_sink_messages{
        	static ostream& sink(){          
            	return out_messages;
        	}
    	};
    
    static ofstream out_state("../simulation_results/solarPanel_sim_state.txt");
    struct oss_sink_state{
        static ostream& sink(){          
            return out_state;
        }
    };
    
    using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;

    /************** Runner call ************************/ 
    dynamic::engine::runner<NDTime, logger_top> r(TOP,{0});
    r.run_until(NDTime("02:25:59:000"));
    return 0;
}

















