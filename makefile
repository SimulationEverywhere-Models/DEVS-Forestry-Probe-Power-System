CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ../../cadmium/include
INCLUDEDESTIMES=-I ../../DESTimes/include

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

#TARGET TO COMPILE ALL THE TESTS TOGETHER 
weather.o: data_structures/weather_info.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/weather_info.cpp -o build/weather.o
	
sensor.o: data_structures/Sensor_info.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/Sensor_info.cpp -o build/sensor.o
	
SAD.o: data_structures/SAD_info.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/SAD_info.cpp -o build/SAD.o
	
	
#TARGET TO COMPILE ALL THE TESTS TOGETHER
SolarPanel_test.o: test/SolarPanel_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/SolarPanel_test.cpp -o build/SolarPanel_test.o
	
Sensor_test.o: test/Sensors_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/Sensors_test.cpp -o build/Sensor_test.o
	
SAD_test.o: test/SAD_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/SAD_test.cpp -o build/SAD_test.o
	
Battery_test.o: test/Battery_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/Battery_test.cpp -o build/Battery_test.o
	
Consumer_test.o: test/PowerConsumer_test.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/PowerConsumer_test.cpp -o build/Consumer_test.o

tests: SolarPanel_test.o weather.o Sensor_test.o sensor.o SAD_test.o SAD.o Battery_test.o Consumer_test.o
	$(CC) -g -o bin/SOLARPANEL_TEST build/SolarPanel_test.o build/weather.o
	$(CC) -g -o bin/SENSOR_TEST build/Sensor_test.o build/sensor.o
	$(CC) -g -o bin/SAD_TEST build/SAD_test.o build/SAD.o build/sensor.o
	$(CC) -g -o bin/BATTERY_TEST build/Battery_test.o
	$(CC) -g -o bin/CONSUMER_TEST build/Consumer_test.o build/SAD.o build/sensor.o
	
#TARGET TO COMPILE ONLY SIMULATOR
main_top.o: top_model/main.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/main.cpp -o build/main_top.o
simulator: main_top.o weather.o SAD.o sensor.o
	$(CC) -g -o bin/FP_MAIN_SIMULATION build/main_top.o build/weather.o build/SAD.o build/sensor.o

#TARGET TO COMPILE EVERYTHING
all: simulator tests

#CLEAN COMMANDS
clean:
	rm -f bin/* build/*

