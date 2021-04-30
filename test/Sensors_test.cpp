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
#include "../data_structures/Sensor_info.hpp"


//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/Sensors.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
struct infoIn : public cadmium::in_port<Sensor_info_t>{};

/***** Define output ports for coupled model *****/
struct sensorUsed : public out_port<int>{};

/***input reader atomic model***/
template<typename T>
class InputReader_Sensor_info_t : public iestream_input<Sensor_info_t,T> {
	public:
		InputReader_Sensor_info_t () = default;
		InputReader_Sensor_info_t (const char* file_path) : iestream_input<Sensor_info_t, T>(file_path) {}
};

int main(){

	//input reader
	const char * i_input_data_Sensor = "../input_data/Sensor_input_test.txt";
	shared_ptr<dynamic::modeling::model> input_reader_Sensor = dynamic::translate::make_dynamic_atomic_model<InputReader_Sensor_info_t, TIME, const char* >("input_reader_Sensor", std::move(i_input_data_Sensor)); 

	//solar panel instantiation 
	shared_ptr<dynamic::modeling::model> sensor1 = dynamic::translate::make_dynamic_atomic_model<Sensor,TIME>("sensor1");
	
	//top model
	dynamic::modeling::Ports iports_TOP = {};
	dynamic::modeling::Ports oports_TOP = {typeid(sensorUsed)};
	dynamic::modeling::Models submodels_TOP = {input_reader_Sensor,sensor1};
	//external inputs
	dynamic::modeling::EICs eics_TOP = {};
	//external outputs
	dynamic::modeling::EOCs eocs_TOP = {
	dynamic::translate::make_EOC<Sensor_defs::out,sensorUsed>("sensor1")
	};
	//internal links
	dynamic::modeling::ICs ics_TOP = {
	dynamic::translate::make_IC<iestream_input_defs<Sensor_info_t>::out, Sensor_defs::sensorInfo>("input_reader_Sensor","sensor1")
	};
	
	shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
	
	TOP = make_shared<dynamic::modeling::coupled<TIME>>("TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP);
	
	//loggers
	static ofstream out_messages("../simulation_results/FP_SENSOR_SIMULATION_outputs.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    
    static ofstream out_state("../simulation_results/FP_SENSOR_SIMULATION_state.txt");
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

















