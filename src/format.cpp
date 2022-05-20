#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hours = (int)(seconds/3600);
    int minutes = ((int)(seconds/60))%60;
    int second = (int)(seconds%60);

    string hr = std::to_string(hours);
    string min = std::to_string(minutes);
    string sec = std::to_string(seconds);
    return hr+":"+min+":"+sec; 
    
}

