#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <wiringPi.h>

using namespace std;

#define TRIGGER_PIN 2  // GPIO 27 is pin 2 in wiringPi
#define ECHO_PIN 3     // GPIO 22 is pin 3 in wiringPi

#define MAX_DISTANCE 220
const int TIMEOUT = (MAX_DISTANCE * 60); // Maximum measure distance * 60

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

    if (wiringPiSetup() == -1)
    {
        cerr << "WiringPi initialization failed." << endl;
        return 1;
    }

    // Can't catch SIGKILL
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    float distance;
    pinMode(TRIGGER_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    while (stopFlag == 0)
    {
        distance = getSonar();
        cout << "The distance is " << distance << "cm" << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }

    cout << "Gracefully stopping." << endl;

    return 0;
}

float getSonar()
{
    float distances[3];
    int midIndex = 1;

    for (int i = 0; i < 3; i++)
    {
        digitalWrite(TRIGGER_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIGGER_PIN, LOW);
        long pingTime = pulseIn(ECHO_PIN, HIGH);
        distances[i] = static_cast<float>(pingTime) * 343.0f / 2.0f / 10000.0f; // Calc distance with speed of sound (343 m/s)

        for (int j = 0; j < i; j++)
        {
            if (distances[j] > distances[i])
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

    while (digitalRead(pinIn) != level)
    {
        gettimeofday(&tn, nullptr);
        if (tn.tv_sec > t0.tv_sec)
        {
            micros = 1000000L;
        }
        else
        {
            micros = 0L; // Me too no L's lmaooo
        }
        micros += tn.tv_usec - t0.tv_usec;
        if (micros > TIMEOUT)
            return 0;
    }

    gettimeofday(&t1, nullptr);

    while (digitalRead(pinIn) == level)
    {
        gettimeofday(&tn, nullptr);
        if (tn.tv_sec > t0.tv_sec)
        {
            micros = 1000000L;
        }
        else
        {
            micros = 0L;
        }
        micros += tn.tv_usec - t0.tv_usec;
        if (micros > TIMEOUT)
            return 0;
    }

    if (tn.tv_sec > t1.tv_sec)
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
