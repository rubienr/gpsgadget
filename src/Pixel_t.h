#pragma once

#include <Adafruit_NeoPixel.h>
#include <inttypes.h>

// -------------------------------------------------------------------------------------------------

template <uint16_t pin> class Pixel_t : public Adafruit_NeoPixel
{
public:
    Pixel_t();
    ~Pixel_t() = default;

    void setup();

    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setColor(uint32_t c);
};

// -------------------------------------------------------------------------------------------------

template <uint16_t pin> void Pixel_t<pin>::setup()
{
    updateType(NEO_GRB + NEO_KHZ400);
    updateLength(1);
    setPin(pin);

    begin();
    clear();
}

// -------------------------------------------------------------------------------------------------

template <uint16_t pin> Pixel_t<pin>::Pixel_t() : Adafruit_NeoPixel(1, pin, NEO_GRB + NEO_KHZ400) {
    updateType(NEO_GRB + NEO_KHZ400);
    updateLength(1);
    setPin(pin);
}

// -------------------------------------------------------------------------------------------------

template <uint16_t pin> void Pixel_t<pin>::setColor(uint8_t r, uint8_t g, uint8_t b)
{
    Adafruit_NeoPixel::setPixelColor(0, r, g, b);
}

// -------------------------------------------------------------------------------------------------

template <uint16_t pin> void Pixel_t<pin>::setColor(uint32_t c)
{
    Adafruit_NeoPixel::setPixelColor(0, c);
}