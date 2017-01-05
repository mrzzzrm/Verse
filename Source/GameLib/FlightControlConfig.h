#pragma once

struct FlightControlComponent
{
    float maxSpeed = 0.0f;
    float acceleration = 0.0f;
};

struct FlightControlConfig
{
    FlightControlComponent forward;
    FlightControlComponent backward;
    FlightControlComponent horizontal;
    FlightControlComponent vertical;
    FlightControlComponent angular;
};