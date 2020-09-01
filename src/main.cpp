#include <Arduino.h>

struct Resources
{
    void setup() {}

    void process() {}
} r;

void setup() { r.setup(); }

void loop() { r.process(); }
