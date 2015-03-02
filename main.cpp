#include <cstdlib>
#include <windows.h>
#include <iostream>
#include <math.h>
//#include <time.h>

// 24 volt is calibrated to 203 on the Velleman board analog sensor 1, in [0..255] steps. So we do 24 / 203 to get the VOLT_STEPPING
#define VOLT_STEPPING		0.1182266

typedef void(CALLBACK* t_func)(int);
typedef void(CALLBACK* t_func0)();
typedef int(CALLBACK* t_func1)();
typedef void(CALLBACK* t_func2)(int *, int *);
typedef void(CALLBACK* t_func3)(int, int);
typedef int(CALLBACK* t_func4)(int);
typedef bool(CALLBACK* t_func5)(int);

// we get these functions from K8055D.DLL to communicate with the K8055 board

t_func4 OpenDevice;
t_func0 CloseDevice;
t_func0 Version_;
t_func4 ReadAnalogChannel;
t_func2 ReadAllAnalog;
t_func3 OutputAnalogChannel;
t_func3 OutputAllAnalog;
t_func ClearAnalogChannel;
t_func0 ClearAllAnalog;
t_func SetAnalogChannel;
t_func0 SetAllAnalog;
t_func WriteAllDigital;
t_func ClearDigitalChannel;
t_func0 ClearAllDigital;
t_func SetDigitalChannel;
t_func0 SetAllDigital;
t_func5 ReadDigitalChannel;
t_func1 ReadAllDigital;

HINSTANCE hDLL;
int foundDLL = 0;
int init();

using namespace std;

// a function that does a reliable rounding of a number. Rounds x (double) to prec (int) digits after the comma.

double round(double x, int prec)
{
	double power = 1.0;
	int i;

	if (prec > 0)
		for (i = 0; i < prec; i++)
			power *= 10.0;
	else if (prec < 0)
		for (i = 0; i < prec; i++)
			power /= 10.0;

	if (x > 0)
		x = floor(x * power + 0.5) / power;
	else if (x < 0)
		x = ceil(x * power - 0.5) / power;

	if (x == -0)
		x = 0;

	return x;
}

int main(int argc, char *argv[])
{
	int CardAddress;
	int dataOut1, dataOut2;
	int dataIn1, dataIn2;
	int h;

	h = init();
	if (!h)
	{
		//	cout << "DLL found" << endl;
		foundDLL = 1;
	}
	else
	{
		cout << "DLL not found" << endl;
	}
	if (foundDLL)
	{
		int CardAddr = (0); // jumpers SK5 and SK6 determine the card address. SK5 and SK6 = on -> Card Address = 0
		h = OpenDevice(CardAddr);
		if (h == 0)
		{
			//	cout << "K8055 Card connected" << endl;
		}
		else
		{
			cout << "K8055 Card not found" << endl;
		}
	}
	if (foundDLL)
	{
		// show current timestamp
		// time_t curr=time(0);
		// cout << "current time is: " << ctime(&curr) <<endl;
		// Read both analog inputs. Note that we only need the first one.
		ReadAllAnalog(&dataIn1, &dataIn2);
		double voltage = dataIn1 * VOLT_STEPPING;
		double voltage_rounded = round(voltage, 1);
		cout << "Battery reading (Volt) = " << voltage_rounded << endl << endl;
		// Close the device after reading	
		CloseDevice();
		FreeLibrary(hDLL);
		//		system("pause"); // debug
	}

}

int init()
{
	hDLL = LoadLibrary("k8055d");
	if (hDLL != NULL)
	{
		OpenDevice = (t_func4)GetProcAddress(hDLL, "OpenDevice");
		if (!OpenDevice)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		CloseDevice = (t_func0)GetProcAddress(hDLL, "CloseDevice");
		if (!CloseDevice)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		ReadAnalogChannel = (t_func4)GetProcAddress(hDLL, "ReadAnalogChannel");
		if (!ReadAnalogChannel)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		ReadAllAnalog = (t_func2)GetProcAddress(hDLL, "ReadAllAnalog");
		if (!ReadAllAnalog)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		OutputAnalogChannel = (t_func3)GetProcAddress(hDLL, "OutputAnalogChannel");
		if (!OutputAnalogChannel)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		OutputAllAnalog = (t_func3)GetProcAddress(hDLL, "OutputAllAnalog");
		if (!OutputAllAnalog)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		ClearAnalogChannel = (t_func)GetProcAddress(hDLL, "ClearAnalogChannel");
		if (!ClearAnalogChannel)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		ClearAllAnalog = (t_func0)GetProcAddress(hDLL, "ClearAllAnalog");
		if (!ClearAllAnalog)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		SetAnalogChannel = (t_func)GetProcAddress(hDLL, "SetAnalogChannel");
		if (!SetAnalogChannel)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		SetAllAnalog = (t_func0)GetProcAddress(hDLL, "SetAllAnalog");
		if (!SetAllAnalog)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		WriteAllDigital = (t_func)GetProcAddress(hDLL, "WriteAllDigital");
		if (!WriteAllDigital)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		ClearDigitalChannel = (t_func)GetProcAddress(hDLL, "ClearDigitalChannel");
		if (!ClearDigitalChannel)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		ClearAllDigital = (t_func0)GetProcAddress(hDLL, "ClearAllDigital");
		if (!ClearAllDigital)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		SetDigitalChannel = (t_func)GetProcAddress(hDLL, "SetDigitalChannel");
		if (!SetDigitalChannel)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		SetAllDigital = (t_func0)GetProcAddress(hDLL, "SetAllDigital");
		if (!SetAllDigital)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		ReadDigitalChannel = (t_func5)GetProcAddress(hDLL, "ReadDigitalChannel");
		if (!ReadDigitalChannel)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		ReadAllDigital = (t_func1)GetProcAddress(hDLL, "ReadAllDigital");
		if (!ReadAllDigital)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		Version_ = (t_func0)GetProcAddress(hDLL, "Version");
		if (!Version_)
		{						// handle the error
			FreeLibrary(hDLL);
			return -2;
		}
		return 0;				// ok
	}
	return -1;					// error load DLL
}
