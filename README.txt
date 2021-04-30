Forestry Probe Power System simulation was implemented and tested in Linux ubuntu terminal.
The following will be a description of the folder organisation, followed by step compile and run. 

/***************************************/
Folder Organisation

README -> instructions.
FP_Power_System.doc -> Model description, analysis, and simulation example.
makefile

atomics -> contains Cadmium implementation of atomic models.
bin -> contains all executables.
build -> contains build files.
coupled -> contains Cadmium implementation of coupled model.
data_structures -> contains messaging data structures. Refer to data structures if creating your own input data.
input_data -> contains all input files.
simulation_results -> contains log files of the simulations outputs and states. 
test -> contains test scripts for the atomic models 
top_model -> contains Cadmium implementation of the top model
/***************************************/
Instructions

1. Place the folder into DEVS-Models folder of the cadmium simulation environment

2. Open terminal in Forestry_Probe_Power_System_Simulation folder

3. Run the following command: make clean; make all

4. Change terminal directory: cd bin

5. Run main simulation:
	- ./FP_MAIN_SIMULATION ../input_data/weather_input_data.txt ../input_data/SAD_input_test.txt

6. Commands for individual model tests:
	- ./SOLARPANEL_TEST ../input_data/weather_input_data.txt
	- ./SENSOR_TEST ../input_data/Sensor_input_test.txt
	- ./SAD_TEST ../input_data/SAD_input_test.txt
	- ./CONSUMER_TEST ../input_data/SAD_input_test.txt
