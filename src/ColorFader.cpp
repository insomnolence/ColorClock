
#include "ColorFader.h"

ColorFader::ColorFader(Adafruit_NeoPixel &strip, uint16_t ledStart, uint16_t ledEnd)
    : ledStrip(strip), TotalSteps(0), Index(0), StartLed(ledStart), EndLed(ledEnd)
{
    CurrentColor = ledStrip.Color(0, 0, 0);
    NewColor = ledStrip.Color(0, 0, 0);
}

// Update the pattern
void ColorFader::Update()
{
    if (Index < TotalSteps)
    {
        if ((millis() - lastUpdate) > Interval) // time to update
        {
            lastUpdate = millis();

            // Calculate linear interpolation between CurrentColor and NewColor
            // Optimise order of operations to minimize truncation error
            uint8_t red = ((Red(CurrentColor) * (TotalSteps - Index)) + (Red(NewColor) * Index)) / TotalSteps;
            uint8_t green = ((Green(CurrentColor) * (TotalSteps - Index)) + (Green(NewColor) * Index)) / TotalSteps;
            uint8_t blue = ((Blue(CurrentColor) * (TotalSteps - Index)) + (Blue(NewColor) * Index)) / TotalSteps;

            ColorSet(ledStrip.Color(red, green, blue));
            Index++;

            if (Index == TotalSteps)
            {
                CurrentColor = NewColor;
            }
        }
    }
}

// Initialize for a Fade
void ColorFader::Fade(uint32_t newColor, uint16_t steps, uint8_t interval)
{
    Interval = interval;
    TotalSteps = steps;
    NewColor = newColor;
    Index = 0;
}

// Set all pixels to a color (synchronously)
void ColorFader::ColorSet(uint32_t color)
{
    for (int i = StartLed; i <= EndLed; i++)
    {
        ledStrip.setPixelColor(i, color);
    }
    ledStrip.show();
}

// Returns the Red component of a 32-bit color
uint8_t ColorFader::Red(uint32_t color)
{
    return (color >> 16) & 0xFF;
}

// Returns the Green component of a 32-bit color
uint8_t ColorFader::Green(uint32_t color)
{
    return (color >> 8) & 0xFF;
}

// Returns the Blue component of a 32-bit color
uint8_t ColorFader::Blue(uint32_t color)
{
    return color & 0xFF;
}
