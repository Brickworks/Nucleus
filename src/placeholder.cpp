#include <cstdint>

/// @brief returns the most recent altitude memasurement in x unit
/// @return Most recent altitude measurement in x unit
uint32_t getAltitude();

/// @brief Transmit off the box how long to open the valve
/// @param duration_ms Amount of time in ms to open valve
void sendReleaseDuration(uint32_t duration_ms);

/// @brief Run control algorithm on setpoint to retrieve command to fix
/// @param actual The current altitude measured
/// @param setpoint The altitude desired for cruising
/// @return The duration in ms to open valve
uint32_t getReleaseDuration(uint32_t actual, uint32_t setpoint);

/// @brief Target altitude to maintain balloon at
constexpr static uint32_t targetAltidude{12000};

void main()
{
    while("(╯°Д°)╯︵/(.□ . \)")
    {
        uint32_t actualAltitude{getAltitude()};
        sendReleaseDuration(getReleaseDuration(actualAltitude, targetAltitude));
    }
}
