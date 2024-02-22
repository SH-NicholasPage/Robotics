#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <pigpio.h>
#include <ADCDevice.hpp>

using namespace std;

#define MOTOR_FRONT_LEFT 27
#define MOTOR_FRONT_RIGHT 22

sig_atomic_t stopFlag = 0;

void signalHandler(int signum)
{
    stopFlag = 1;
    cout << "Signal " << signum << " received." << endl;
}

ADCDevice *adc;

void whichMotorWorksLol();
void testStraight(int);

int main()
{
    adc = new ADCDevice();
    cout << "Testing motors..." << endl;

    if(adc->detectI2C(0x48))// Detect the pcf8591.
    {
        delete adc;                // Free previously pointed memory
        adc = new PCF8591();       // If detected, create an instance of PCF8591.
    }
    else if(adc->detectI2C(0x4b))// Detect the ads7830
    {
        delete adc;               // Free previously pointed memory
        adc = new ADS7830();      // If detected, create an instance of ADS7830.
    }
    else
    {
        cerr << "No correct I2C address found!" << endl;
        return -1;
    }

    //Can't catch SIGKILL sadge
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    if(gpioInitialise() < 0)
    {
        cerr << "Pigpio initialization failed." << endl;
        return 1;
    }

    whichMotorWorksLol();

    gpioTerminate();

    cout << "Gracefully stopping." << endl;

    return 0;
}

void whichMotorWorksLol()
{
    int knownPorts[3] = {17, 27, 22};
    int knownPorts_length = sizeof(knownPorts)/sizeof(knownPorts[0]);

    for(int i = 0; i < 33; i++)
    {
        bool skipPort = false;

        for(int j = 0; j < knownPorts_length; j++)
        {
            if(knownPorts[j] == i)
            {
                skipPort = true;
                cout << "Skipping port " << i << "." << endl;
                break;
            }
        }

        if(skipPort == false)
        {
            cout << "Testing port " << i << "..." << endl;
            gpioWrite(i, PI_HIGH);
            this_thread::sleep_for(chrono::milliseconds(800));
            gpioWrite(i, PI_LOW);
            this_thread::sleep_for(chrono::seconds(4));
        }
    }
}

void testStraight(int distance)
{
    gpioWrite(MOTOR_FRONT_LEFT, PI_HIGH);
    this_thread::sleep_for(chrono::seconds(1));
    gpioWrite(MOTOR_FRONT_LEFT, PI_LOW);
}