#include <math.h> 
#include <assert.h>
#include <iostream>
#include <fstream>
#include <string>

#include "weather_info.hpp"

/***************************************************/
/************* Output stream ************************/
/***************************************************/

ostream& operator<<(ostream& os, const weather_info_t& msg) {
  os << msg.temp << " " << msg.visibility;
  return os;
}

/***************************************************/
/************* Input stream ************************/
/***************************************************/

istream& operator>> (istream& is, weather_info_t& msg) {
  is >> msg.temp;
  is >> msg.visibility;
  return is;
}
