using System.Device.Gpio;
using System.Diagnostics;

namespace CsTest;

class Program
{
    private static bool kill = false;
    
    public static void Main()
    {
        // Setup termination thread
        Thread termThread = new Thread(() =>
        {
            Console.ReadLine();
            kill = true;
        });
        termThread.Start();

        Chassis.Buzzer.Buzz();
        Chassis.Drivetrain.TestMotors();
        Environment.Exit(0);
        
        while (kill == false)
        {
            /*Console.WriteLine(Chassis.UltraSonicSensor.GetMedianDistanceInMillimeters());
            Thread.Sleep(250);*/
            
            // Chassis.Drivetrain.MoveForward();
        }
    }
}