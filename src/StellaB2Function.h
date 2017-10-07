#pragma once

#include "SerialPort.h"

class CStellaB2Function {
public:
	CStellaB2Function (CSerialPort *serial);
	~CStellaB2Function ();

	bool Velocity (int leftMotor, int rightMotor);
	bool Stop (int mode = 1); // mode: 1 - Motor Free, 2 - Emergency stop, 3- deceleration stop
	bool Reset ();
	bool Init ();
	bool Forward ();
	bool Backward ();
	bool Right ();
	bool Left ();
	bool RunDistant (int dist, int time);

	bool Rotate360 () ;
	bool Program2 ();
	bool GetState (char *state);
	bool GetPosition(unsigned char leftMotor[9], unsigned char rightMotor[9]);
	bool GetSpeed (unsigned char leftMotor[6], unsigned char rightMotor[6]);
	bool WriteCommand (CString strCommand);
	const char *GetStateString (char state);
	const char *GetErrorString (char errorCode);

private:
	int WritePacket (unsigned char *command, int length);
	int ReadPacket (unsigned char *response, int length, unsigned long timeout = 100);
	int FindAndMove (unsigned char *response, int n, unsigned char command);

private:
	CSerialPort *_serial;
};
