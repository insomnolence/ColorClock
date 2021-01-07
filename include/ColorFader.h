#include <Adafruit_NeoPixel.h>

class ColorFader 
{
public:
    // Member Variables:
    Adafruit_NeoPixel ledStrip; 

    uint16_t TotalSteps;             // total number of steps in the pattern
    uint16_t Index;                  // current step within the pattern

    uint16_t StartLed; // Starting pin for range
    uint16_t EndLed;   // Ending pin for range

    unsigned long Interval;   // milliseconds between updates
    unsigned long lastUpdate; // last update of position

    uint32_t CurrentColor, NewColor; // What colors are in use

    // Constructor - calls base-class constructor to initialize strip
    ColorFader(Adafruit_NeoPixel &strip, uint16_t ledStart, uint16_t ledEnd);

    // Update the pattern
    void Update();

    // Initialize for a Fade
    void Fade(uint32_t newColor, uint16_t steps, uint8_t interval);

    // Set all pixels to a color (synchronously)
    void ColorSet(uint32_t color);

    // Returns the Red component of a 32-bit color
    uint8_t Red(uint32_t color);

    // Returns the Green component of a 32-bit color
    uint8_t Green(uint32_t color);

    // Returns the Blue component of a 32-bit color
    uint8_t Blue(uint32_t color);
};