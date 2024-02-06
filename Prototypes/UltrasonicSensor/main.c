#include <stdio.h>
#include <pigpio.h>
#include <sys/time.h>
#include <signal.h>

#define TRIGGER_PIN 27
#define ECHO_PIN 22

#define MAX_DISTANCE 220
const int TIMEOUT = (MAX_DISTANCE * 60); //Maximum measure distance * 60

volatile sig_atomic_t stopFlag = 0;

void signalHandler(int signum)
{
    stopFlag = 1;
    printf("Signal %d received.", signum);
}

long pulseIn(int, int);
float getSonar();

int main()
{
    printf("Ultrasonic sensor test is starting...\n");

    //Can't catch SIGKILL sadge
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);

    if(gpioInitialise() < 0)
    {
        fprintf(stderr, "Pigpio initialization failed.\n");
        return 1;
    }

    float distance = 0;
    gpioSetMode(TRIGGER_PIN, PI_OUTPUT);
    gpioSetMode(ECHO_PIN, PI_INPUT);

    while(stopFlag == 0)
    {
        distance = getSonar();
        printf("The distance is %.2f cm\n", distance);
        fflush(stdout); // Flush printf output
        time_sleep(1);
    }

    gpioTerminate();

    printf("Gracefully stopping.");//Yeah, right OMEGALUL

    return 0;
}

float getSonar()
{
    float distances[3];
    int midIndex = 1;

    //Yes, this could be in the for loop, but I am prioritizing fast code over good code
    gpioWrite(TRIGGER_PIN, PI_HIGH);
    time_sleep(0.00001);
    gpioWrite(TRIGGER_PIN, PI_LOW);
    long pingTime = pulseIn(ECHO_PIN, PI_HIGH);
    distances[0] = (float) pingTime * 343.0f / 2.0f / 10000.0f; //Calc distance with speed of sound (343 m/s)

    for(int i = 1; i < 3; i++)
    {
        gpioWrite(TRIGGER_PIN, PI_HIGH);
        time_sleep(0.00001);
        gpioWrite(TRIGGER_PIN, PI_LOW);
        pingTime = pulseIn(ECHO_PIN, PI_HIGH);
        distances[i] = (float) pingTime * 343.0f / 2.0f / 10000.0f; //Calc distance with speed of sound (343 m/s)

        if(distances[i] < distances[i - 1])
        {
            float temp = distances[i];
            distances[i] = distances[i - 1];
            distances[i - 1] = temp;
        }
    }

    return distances[midIndex];
}

long pulseIn(int pinIn, int level)
{
    struct timeval tn, t0, t1;
    long micros = 0;
    gettimeofday(&t0, NULL);

    while(gpioRead(pinIn) != level)
    {
        gettimeofday(&tn, NULL);
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

    gettimeofday(&t1, NULL);

    while(gpioRead(pinIn) == level)
    {
        gettimeofday(&tn, NULL);
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