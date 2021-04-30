//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Data structures
#include "../data_structures/SAD_info.hpp"
#include "../data_structures/Sensor_info.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/SenderReceiver.hpp"
#include "../atomics/Sensors.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

//PowerConsumer Port definition
struct Consumer_defs{
	struct Consumer_In : public in_port<SAD_info_t> {};	
	struct Consumer_out : public out_port<int> {};
};

/****** SenderReceiver Generator atomic model instantiation ******/
shared_ptr<dynamic::modeling::model> SAD1 = dynamic::translate::make_dynamic_atomic_model<SAD,TIME>("SAD1");

/****** sensor atomic model instantiations ****************/
shared_ptr<dynamic::modeling::model> Sensor1 = dynamic::translate::make_dynamic_atomic_model<Sensor,TIME>("Sensor1");

/******** Consumer Model **********/
dynamic::modeling::Ports iports_consumer = {typeid(Consumer_defs::Consumer_In)};
dynamic::modeling::Ports oports_consumer = {typeid(Consumer_defs::Consumer_out)};
dynamic::modeling::Models submodels_consumer = {SAD1,Sensor1};
//external input link
dynamic::modeling::EICs eics_consumers = {
dynamic::translate::make_EIC<Consumer_defs::Consumer_In,SAD_defs::info>("SAD1")
};
//external input link
dynamic::modeling::EOCs eocs_consumers = {

dynamic::translate::make_EOC<SAD_defs::out,Consumer_defs::Consumer_out>("SAD1"),

dynamic::translate::make_EOC<Sensor_defs::out,Consumer_defs::Consumer_out>("Sensor1")
};
//internal link
dynamic::modeling::ICs ics_consumers = {
dynamic::translate::make_IC<SAD_defs::SensorOut,Sensor_defs::sensorInfo>("SAD1","Sensor1" )
};






































