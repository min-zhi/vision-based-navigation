#include "stdafx.h"
#include "StellaB2Function.h"
#include <math.h>
#include <string.h>
#include <windows.h>
#include <assert.h>

static const unsigned char STX = 0x02;
static const unsigned char ETX = 0x03;

inline int bound (int v, int lo, int hi)
{
	if (v < lo) return lo;
	if (v > hi) return hi;
	return v;
}

CStellaB2Function::CStellaB2Function (CSerialPort *serial)
: _serial(serial)
{
	_serial->Flush ();
}

CStellaB2Function::~CStellaB2Function ()
{
}

bool CStellaB2Function::WriteCommand (CString strCommand)
{
	unsigned char* strBuf = new unsigned char[strCommand.GetLength()];
	int i = 0;
	_serial->Write ((char *)&STX, 1);
	Sleep (1);
	for (i=0; i<strCommand.GetLength(); ++i) {
		strBuf[i] = strCommand[i];
		_serial->Write ((char *)& strBuf[i], 1);
		Sleep (1);
	}
	strBuf[i] = NULL;
	_serial->Write ((char *)&ETX, 1);
	Sleep (1);
	return true;
}

bool CStellaB2Function::Velocity (int leftMotor, int rightMotor)
{
	leftMotor = bound (leftMotor, -999, 999);
	rightMotor = bound (rightMotor, -999, 999);

	unsigned char leftSign = (leftMotor >= 0) ? 'F' : 'B';
	unsigned char rightSign = (leftMotor >= 0) ? 'F' : 'B';

	leftMotor = abs(leftMotor);
	rightMotor = abs(rightMotor);

	unsigned char cmd_left[] = { STX, 
		'C', 'V', 'L', 
		leftSign, 
		(leftMotor/100)%10 + '0', 
		(leftMotor/10)%10 + '0', 
		(leftMotor/1)%10 + '0', 
		ETX };
	WritePacket (cmd_left, sizeof(cmd_left));

	unsigned char cmd_right[] = { STX, 
		'C', 'V', 'R', 
		rightSign, 
		(rightMotor/100)%10 + '0', 
		(rightMotor/10)%10 + '0', 
		(rightMotor/1)%10 + '0', 
		ETX };
	WritePacket (cmd_right, sizeof(cmd_right));

	return true;
}

bool CStellaB2Function::RunDistant (int dist, int time)
{
	dist = bound (dist, 0, 999);
	time = bound (time, 0, 999);
	//dist = abs(dist);
	//time = abs(time);

	unsigned char cmd_rundist[] = { STX, 
		'C', 'P', 'A', 'R','0','0','0','D', 
		'F',
		(dist/100)%10 + '0', 
		(dist/10)%10 + '0', 
		(dist/1)%10 + '0', 
		'T',
		(time/100)%10 + '0', 
		(time/10)%10 + '0', 
		(time/1)%10 + '0',
		ETX };
	WritePacket (cmd_rundist, sizeof(cmd_rundist));
	return true;
}

bool CStellaB2Function::Stop (int mode)
{
	assert (mode == 1 || mode == 2 || mode == 3);

	unsigned char cmd[] = { STX, 'C', 'S', 'T', 'O', 'P', mode + '0', ETX };
	WritePacket (cmd, sizeof(cmd));
	return true;
}

bool CStellaB2Function::Reset ()
{
	unsigned char cmd[] = { STX, 'C', 'R', 'E', 'S', 'E', 'T', 'A', ETX };
	WritePacket (cmd, sizeof(cmd));
	return true;
}

bool CStellaB2Function::Init ()
{
	unsigned char cmd[] = { STX, 'C', 'I', 'N', 'I', 'T', ETX };
	WritePacket (cmd, sizeof(cmd));
	return true;
}
bool CStellaB2Function::Forward ()   //go forward
{
	unsigned char cmd[] = { STX, 'C', 'P', 'A', 'R','0','0','0','D', 'F', '0', '3','5','T','0','0','5', ETX };  //Run both 2 wheels forward:    STXCVAF100ETX
	WritePacket (cmd, sizeof(cmd));
	return true;
}
bool CStellaB2Function::Backward ()   //go backward
{
	//unsigned char cmd[] = { STX, 'C', 'V', 'B', '0','5','0','B', '0','5','0','P', ETX };  //Run both 2 wheels backward:    STXCVAB100ETX
	unsigned char cmd[] = { STX, 'C', 'P', 'A', 'R','0','0','0','D', 'B', '2', '0','0','T','0','1','8', ETX };  
	WritePacket (cmd, sizeof(cmd));
	return true;
}
bool CStellaB2Function::Right ()    //turn right
{
	unsigned char cmd[] = { STX, 'C', 'P', 'A', 'R','0','9','0','D', 'F', '0', '0','0','T','0','0','4', ETX };  //Run left wheel forward :    STXCVAB100ETX
	//unsigned char cmd[] = { STX, 'C', 'P', 'A', 'R','0','9','0','D', 'F', '0', '6','0','T','0','1','0', ETX };  //Run left wheel forward :    STXCVAB100ETX
	WritePacket (cmd, sizeof(cmd));
	return true;
}

bool CStellaB2Function::Left ()    //turn Left
{	unsigned char cmd[] = { STX, 'C', 'P', 'A', 'L','0','9','0','D', 'F', '0', '0','0','T','0','0','4', ETX };  //Run left wheel forward :    STXCVAB100ETX
	//unsigned char cmd[] = {STX, 'C', 'P', 'A', 'L','0','9','0','D', 'F', '0', '6','0','T','0','1','0', ETX };  //Run right wheel forward :    STXCVAB100ETX
	WritePacket (cmd, sizeof(cmd));
	return true;
}


bool CStellaB2Function::Rotate360 ()    //turn around CPAR 3 6 0 DF 3 1 4 T 0 1 0
{
	unsigned char cmd[] = { STX, 'C', 'P', 'A', 'R','3','6','0','D', 'F', '3', '1','4','T','0','1','0', ETX };  
	WritePacket (cmd, sizeof(cmd));
	return true;
}

bool CStellaB2Function::Program2 ()    
{
	unsigned char cmd[] = { STX, 'C', 'P', 'A', 'L','0','4','5','D', 'F', '1', '0','0','T','0','1','2', ETX };  //Turn right 90 degree in 2s
	WritePacket (cmd, sizeof(cmd));
	return true;
}
bool CStellaB2Function::GetState (char *state)
{
	unsigned char cmd[] = { STX, 'G', 'S', 'T', 'A', 'T', 'E', ETX };
	WritePacket (cmd, sizeof(cmd));

	unsigned char res[3] = { 0, };
	int n_res = ReadPacket (res, sizeof(res));
	if (n_res == sizeof(res) && res[0] == STX && res[2] == ETX) {
		*state = res[1] - '0';
		return true;
	}
	else {
		return false;
	}
}

bool CStellaB2Function::GetPosition (unsigned char leftMotor[9], unsigned char rightMotor[9])
{
	unsigned char cmd[] = { STX, 'G', 'P', 'O', 'S', 'I', 'T', 'I', 'O', 'N', 'A', ETX };
	WritePacket (cmd, sizeof(cmd));
	unsigned char res[20] = { 0, };
	int n_res = ReadPacket(res, sizeof(res));
	if (n_res == sizeof(res)) {
		for (int i=0;i<=6;i++)
		{	
			leftMotor[i] = res[1+i];
			rightMotor[i] = res[10+i];
		}
	
		if (res[1] == 'B') leftMotor[0] ='-' ; else leftMotor[0] ='+';
		if (res[10] == 'B') rightMotor[0] = '-';else rightMotor[0] = '+';
		return true;
	}
	else {
		return false;
	}
}
bool CStellaB2Function::GetSpeed (unsigned char leftMotor[6], unsigned char rightMotor[6])
{
	unsigned char cmd[] = { STX, 'G', 'V', 'E', 'L', 'O', 'C', 'I', 'T', 'Y', 'A', ETX };
	WritePacket (cmd, sizeof(cmd));

	unsigned char res[14] = { 0, };
	int n_res = ReadPacket(res, sizeof(res));
	if (n_res == sizeof(res)) {
		for (int i=0;i<=3;i++)
		{	
			leftMotor[i] = res[1+i];
			rightMotor[i] = res[7+i];
		}

		if (res[1] == 'B') leftMotor[0] ='-' ; else leftMotor[0] ='+';
		if (res[7] == 'B') rightMotor[0] = '-';else rightMotor[0] = '+';
		return true;
	}
	else {
		return false;
	}
}
const char *CStellaB2Function::GetStateString (char state)
{
	switch (state) {
	case 0: return "Normal state";
	case 1: return "Right wheel overload condition";
	case 2: return "Left wheel overload condition";
	case 3: return "Overload condition both wheels";
	case 4: return "Over voltage conditions (applied voltage of 15V or higher)";
	case 5: return "Low voltage conditions (applied voltage of 10V or less)";
	case 6: return "Target position is reached";
	case 7: return "The rotation of the wheel being";
	default: return "Unknown status";
	}
}

const char *CStellaB2Function::GetErrorString (char errorCode)
{
	switch (errorCode) {
	case  8: return "Motor command limits the speed command value beyond the location you entered.";
	case  9: return "Character in the command string if it gets more or less";
	case 10: return "Command string is not valid.";
	case 11: return "Command string in the direction of the data is not correct.";
	case 12: return "Input data is out of bounds.";
	default: return "Error code is unknown.";
	}
}

int CStellaB2Function::WritePacket (unsigned char *command, int length)
{
	for (int i=0; i<length; ++i) {
		_serial->Write ((char *)&command[i], 1);
		 Sleep (1/1000);
	}
	return length;
}

int CStellaB2Function::ReadPacket (unsigned char *response, int length, unsigned long timeout)
{
	int n_recv = 0;

	timeout += GetTickCount ();

	while (0 < length) {
		int n = _serial->Read ((char *)response, length);
		if (0 < n) {
			if (n_recv == 0 && response[0] != STX) {
				n = FindAndMove (response, n, STX);			
			}
			response += n;
			length -= n;
			n_recv += n;

			if (0 < n_recv && response[-1] == ETX) {
				break;
			}
		}
		else if (n == 0) {
			if (timeout < GetTickCount()) {
				break;
			}
		}
		else {
			return -1;
		}
	};
	return n_recv;
}

int CStellaB2Function::FindAndMove (unsigned char *response, int n, unsigned char command)
{
	for (int i=0; i<n; ++i) {
		if (response[i] == command) {
			memmove (&response[0], &response[i], n-i);
			return n-i;
		}
	}
	//there is no response within the command.
	// Received data is ignored by all.
	return 0;
}
