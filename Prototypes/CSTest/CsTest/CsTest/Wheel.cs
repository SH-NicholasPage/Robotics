using System.Device.Pwm;

namespace CsTest;

public class Wheel : IDisposable
{
    private readonly PwmChannel _pwmChannel;

    public Wheel(int pwmPin, int frequency = 1000)
    {
        _pwmChannel = PwmChannel.Create(0, pwmPin, frequency);
        _pwmChannel.Start();
    }

    public void SetMotorSpeed(double dutyCycle)
    {
        if (dutyCycle < 0 || dutyCycle > 1)
        {
            throw new ArgumentOutOfRangeException(nameof(dutyCycle), "Duty cycle must be between 0 and 1!");
        }

        _pwmChannel.DutyCycle = dutyCycle;
    }

    public void StopMotor() => _pwmChannel.Stop();

    public void Dispose()
    {
        _pwmChannel.Dispose();
    }
}