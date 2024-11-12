using System.Device.Gpio;

namespace CsTest;

public sealed class Buzzer : ComponentBase
{
    private readonly int pin;

    public Buzzer(GpioController controller, int pin)
        : base(controller)
    {
        this.pin = pin;
    }

    public void Buzz(int duration = 200)
    {
        Controller.OpenPin(pin, PinMode.Output);
        Controller.Write(pin, PinValue.High);
        Thread.Sleep(duration);
        Controller.Write(pin, PinValue.Low);
        Controller.ClosePin(pin);
    }
}