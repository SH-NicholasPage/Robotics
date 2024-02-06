#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <pigpio.h>

using namespace std;

#define TRIGGER_PIN 27
#define ECHO_PIN 22

#define MAX_DISTANCE 220
const int TIMEOUT = (MAX_DISTANCE * 60); //Maximum measure distance * 60

sig_atomic_t stopFlag = 0;

void signalHandler(int signum)
{
    stopFlag = 1;
    cout << "Signal " << signum << " received." << endl;
}

long pulseIn(int, int);
float getSonar();

int main()
{
    cout << "Ultrasonic sensor test is starting..." << endl;

    //Can't catch SIGKILL sadge
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    if(gpioInitialise() < 0)
    {
        cerr << "Pigpio initialization failed." << endl;
        return 1;
    }

    float distance;
    gpioSetMode(TRIGGER_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT);

    while(stopFlag == 0)//Shut up stupid idiot you are LITERALLY a global signal fool ofc you aren't updated in the loop body why would you be
    {
        distance = getSonar();
        cout << "The distance is " <<  distance <<  "cm" << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }

    gpioTerminate();

    cout << "Gracefully stopping." << endl;//Yeah, right OMEGALUL

    return 0;
}

float getSonar()
{
    float distances[3];
    int midIndex = 1;

    for(int i = 0; i < 3; i++)
    {
        gpioWrite(TRIGGER_PIN, PI_HIGH);
        this_thread::sleep_for(chrono::microseconds(10));
        gpioWrite(TRIGGER_PIN, PI_LOW);
        long pingTime = pulseIn(ECHO_PIN, PI_HIGH);
        distances[i] = static_cast<float>(pingTime) * 343.0f / 2.0f / 10000.0f; //Calc distance with speed of sound (343 m/s)

        for(int j = 0; j < i; j++)
        {
            if(distances[j] > distances[i])
            {
                float temp = distances[j];
                distances[j] = distances[i];
                distances[i] = temp;
            }
        }
    }

    return distances[midIndex];
}

long pulseIn(int pinIn, int level)
{
    struct timeval tn{}, t0{}, t1{};
    long micros;
    gettimeofday(&t0, nullptr);

    while(gpioRead(pinIn) != level)
    {
        gettimeofday(&tn, nullptr);
        if(tn.tv_sec > t0.tv_sec)
        {
            micros = 1000000L;
        }
        else
        {
            micros = 0L; //Me too no L's lmaooo
        }
        micros += tn.tv_usec - t0.tv_usec;
        if(micros > TIMEOUT) return 0;
    }

    gettimeofday(&t1, nullptr);

    while(gpioRead(pinIn) == level)
    {
        gettimeofday(&tn, nullptr);
        if(tn.tv_sec > t0.tv_sec)
        {
            micros = 1000000L;
        }
        else
        {
            micros = 0L;
        }
        micros += tn.tv_usec - t0.tv_usec;
        if(micros > TIMEOUT) return 0;
    }

    if(tn.tv_sec > t1.tv_sec)
    {
        micros = 1000000L;
    }
    else
    {
        micros = 0L;
    }

    micros += tn.tv_usec - t1.tv_usec;
    return micros;
}