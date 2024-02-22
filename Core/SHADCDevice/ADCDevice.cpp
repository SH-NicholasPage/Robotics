#include "ADCDevice.hpp"

ADCDevice::ADCDevice()
{
    address = 0;
    gpioInitialise(); //Initialize pigpio.
}

int ADCDevice::detectI2C(int addr)
{
    _fd = i2cOpen(1, addr, 0);
    if (_fd < 0)
    {		
        printf("Error address : 0x%x \n",addr);
        return 0 ;
    } 
    else
    {	
        if(i2cWriteDevice(_fd, nullptr, 0) < 0)
        {
            printf("Not found device in address 0x%x \n",addr);
            return 0;
        }
        else
        {
            printf("Found device in address 0x%x \n",addr);
            return 1;
        }
    }
}

int ADCDevice::analogRead(int chn)
{
    std::cerr << "Implemented in subclass!" << std::endl;
    return 0;
}


PCF8591::PCF8591(int addr)
{
    address = addr;
    cmd = 0x40;		//The default command for PCF8591 is 0x40.
    gpioInitialise();
    detectI2C(address);
    std::cout << "PCF8591 setup successful!" << std::endl;
}
int PCF8591::analogRead(int chn)
{
    i2cWriteByte(_fd, cmd+chn);
    char data;
    i2cReadDevice(_fd, &data, 1);
    i2cWriteByte(_fd, cmd+chn);
    i2cReadDevice(_fd, &data, 1);
    return data;
}
int PCF8591::analogWrite(int value)
{
    return i2cWriteByteData(_fd, cmd, value);
}

ADS7830::ADS7830(int addr)
{
    address = addr;
    cmd = 0x84;
    gpioInitialise();
    detectI2C(address);
    std::cout << "ADS7830 setup successful!" << std::endl;
}

int ADS7830::analogRead(int chn)
{
    i2cWriteByte(_fd, cmd|(((chn<<2 | chn>>1)&0x07)<<4));
    char data;
    i2cReadDevice(_fd, &data, 1);
    return data;
}
