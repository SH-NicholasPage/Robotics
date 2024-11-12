using System.Device.Gpio;
using System.Diagnostics;

namespace CsTest;

public class UltraSonicSensor : ComponentBase
{
    private readonly int echoPin;
    private readonly int triggerPin;
    
    public UltraSonicSensor(GpioController controller, int echoPin, int triggerPin)
        : base(controller)
    {
        this.echoPin = echoPin;
        this.triggerPin = triggerPin;
    }

    private void OpenPins()
    {
        Controller.OpenPin(triggerPin, PinMode.Output);
        Controller.OpenPin(echoPin, PinMode.Input);
    }

    private void ClosePins()
    {
        Controller.ClosePin(triggerPin);
        Controller.ClosePin(echoPin);
    }
    
    /// <summary>
    /// Faster, but less accurate.
    /// </summary>
    /// <returns></returns>
    public double GetDistanceInMillimeters()
    {
        OpenPins();
        // Send a 10 microsecond pulse to the trigger pin
        Controller.Write((int)Pin.UltraSonicTrigger, PinValue.Low);
        Thread.Sleep(2); // Wait for 2ms to ensure the signal is low
        Controller.Write((int)Pin.UltraSonicTrigger, PinValue.High);
        Thread.Sleep(TimeSpan.FromMilliseconds(0.01)); // 10 microsecond pulse
        Controller.Write((int)Pin.UltraSonicTrigger, PinValue.Low);

        // Wait for the echo pin to go high (start of pulse)
        while (Controller.Read((int)Pin.UltraSonicEcho) == PinValue.Low) { }

        Stopwatch stopwatch = Stopwatch.StartNew();

        // Wait for the echo pin to go low (end of pulse)
        while (Controller.Read((int)Pin.UltraSonicEcho) == PinValue.High) { }

        stopwatch.Stop();
        double pulseDuration = stopwatch.Elapsed.TotalMilliseconds * 1000; // Convert to microseconds

        ClosePins();
        
        // Calculate distance in meters
        double distanceInMeters = (pulseDuration * 343) / 2 / 1_000_000;
        return distanceInMeters * 1000;
    }
    
    /// <summary>
    /// Slower, but more accurate
    /// </summary>
    /// <param name="count"></param>
    /// <returns></returns>
    public double GetMedianDistanceInMillimeters(int count = 5)
    {
        // Take three distance measurements
        double[] distances = new double[count];
        for (int i = 0; i < count; i++)
        {
            distances[i] = GetDistanceInMillimeters();
            Thread.Sleep(5);
        }

        // Sort the array to find the median
        Array.Sort(distances);

        // Return the median value
        return distances[count / 2];
    }
}