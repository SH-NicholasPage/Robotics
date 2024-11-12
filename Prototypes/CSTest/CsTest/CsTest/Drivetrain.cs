using System.Device.I2c;

namespace CsTest;

public class Drivetrain
{
    private readonly Wheel frontLeftWheel;
    private readonly Wheel frontRightWheel;
    private readonly Wheel backLeftWheel;
    private readonly Wheel backRightWheel;
    
    #region Singleton implementation

    private Drivetrain()
    {
        frontLeftWheel = new Wheel(13);
        frontRightWheel = new Wheel(19);
        backLeftWheel = new Wheel(12);
        backRightWheel = new Wheel(20);
        // 21 is also a valid PCM pin
    }

    // Eager loading to avoid thread-unsafety
    private static Drivetrain instance = new Drivetrain();

    public static Drivetrain getDriveTrain()
    {
        return instance;
    }
    #endregion

    public void TestWheels()
    {
        for (int i = 0; i < 32; i++)
        {
            Wheel testWheel = new Wheel(i);
            testWheel.SetMotorSpeed(0.75);
        }
    }
}