#include <iostream>
#include <cstdint>
#include <string>
#include <stdlib.h> //rand
#include <time.h>

using namespace std;

/// @brief returns the most recent altitude memasurement in x unit
/// @return Most recent altitude measurement in x unit
uint32_t getAltitude()
{
    uint32_t altitude = rand() % 10000;
    cout << "Altitude: " << altitude << endl;
    return altitude;
}

void sendReleaseDuration(uint32_t duration_ms) 
{
    cout << "Sending command to release for " << duration_ms << " seconds!" << endl;
}

/// @brief Run control algorithm on setpoint to retrieve command to fix
/// @param actual The current altitude measured
/// @param setpoint The altitude desired for cruising
/// @return The duration in ms to open valve
uint32_t getReleaseDuration(uint32_t actual, uint32_t setpoint)
{
    return actual - setpoint;
}

int main()
{
    uint32_t current_time = time(NULL);
    uint32_t future_time = current_time + 3;
        while (current_time < future_time)
    {
        current_time = time(NULL);
        sendReleaseDuration(getReleaseDuration(getAltitude(), 5000));
        }
    return 0;
}