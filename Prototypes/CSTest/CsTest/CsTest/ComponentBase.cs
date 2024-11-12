using System.Device.Gpio;

namespace CsTest;

public abstract class ComponentBase
{
    protected GpioController Controller { get; init; }

    protected ComponentBase(GpioController controller)
    {
        Controller = controller;
    }
}