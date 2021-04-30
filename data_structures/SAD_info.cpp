#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "SAD_info.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator<<(ostream& os, const SAD_info_t& msg) {
  os << msg.sendF << " " << msg.numSensor << " " << msg.frequency << " " << msg.amps;
  return os;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator>> (istream& is, SAD_info_t& msg) {
  is >> msg.sendF;
  is >> msg.numSensor;
  is >> msg.frequency;
  is >> msg.amps;
  return is;
}
