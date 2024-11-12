using System.Device.Gpio;

namespace CsTest;

public enum Pin : int
{
    Buzzer = 17,
    UltraSonicEcho = 22,
    UltraSonicTrigger = 27
}

public static class Chassis
{
    public static GpioController Controller { get; }
    
    public static Buzzer Buzzer { get; }
    public static UltraSonicSensor UltraSonicSensor { get; }
    public static Drivetrain Drivetrain { get; }
    
    static Chassis()
    {
        Controller = new GpioController();
        Buzzer = new Buzzer(Controller, (int)Pin.Buzzer);
        UltraSonicSensor = new UltraSonicSensor(Controller, 
            echoPin: (int)Pin.UltraSonicEcho, 
            triggerPin: (int)Pin.UltraSonicTrigger);
        Drivetrain = Drivetrain.getDriveTrain(); // Singleton
        
        AppDomain.CurrentDomain.ProcessExit += D_Chassis;
    }
    
    // Destructor
    private static void D_Chassis(object? sender, EventArgs e) 
    {
        Controller.Dispose();
    }
}