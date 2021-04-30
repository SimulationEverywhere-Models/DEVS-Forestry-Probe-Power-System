#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "Sensor_info.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator<<(ostream& os, const Sensor_info_t& msg) {
  os << msg.numSensor << " " << msg.frequency << " " << msg.amps;
  return os;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator>> (istream& is, Sensor_info_t& msg) {
  is >> msg.numSensor;
  is >> msg.frequency;
  is >> msg.amps;
  return is;
}
