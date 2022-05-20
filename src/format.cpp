#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    int hrs = (int)(seconds/3600);
    int mins = ((int)(seconds/60))%60;
    int secs = (int)(seconds%60);

    string hr = std::to_string(hrs);
    string min = std::to_string(mins);
    string sec = std::to_string(secs);
    return hr + ":" + min + ":" + sec; 
    
}

