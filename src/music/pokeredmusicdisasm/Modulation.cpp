#include <sstream>
#include "Modulation.h"
using namespace std;

odulation::Modulation()
{
	delay = 0;
	depth = 0;
	rate = 0;
}

odulation::Modulation(unsigned char* byte) // Parse Immidiately
{
	Parse(byte);
}

odulation::Modulation(unsigned char delay, unsigned char depth, unsigned char rate, bool) // Set value
{
	SetDelay(delay);
	SetDepth(depth);
	SetRate(rate);
}

// Direct Getter/Setter Functions
unsigned char Modulation::GetDelay()
{
	return delay;
}

void Modulation::SetDelay(unsigned char value)
{
	delay = value;
}

unsigned char Modulation::GetDepth()
{
	return depth;
}

void Modulation::SetDepth(unsigned char value)
{
	depth = value;
}

unsigned char Modulation::GetRate()
{
	return rate;
}

void Modulation::SetRate(unsigned char value)
{
	rate = value;
}

bool Modulation::IsValid(unsigned char* byte)
{
	if(byte[0] == 0xEA)
	{
		error = false;	// Unblock assembling
		return true;
	}
	else
	{
		error = true;	// Block assembling
		return false;
	}
}

string Modulation::GenAsm()
{
	string tmpRet = AbstractData::GenAsm();
	if(tmpRet != "") return tmpRet;

	stringstream tmpAsmOut;
	tmpAsmOut << "mus_mod " << (short)delay << ", " << (short)depth << ", " << (short)rate;
	return tmpAsmOut.str();
}

bool Modulation::Parse(unsigned char* byte)
{
	if(!AbstractData::Parse(byte)) return false;
	
	delay = byte[1];
	
	depth = byte[2] & 0xF0;
	depth >>= 4;

	rate = byte[2] & 0x0F;
	return true;
}

unsigned int Modulation::Arguments()
{
	// 2 1-byte arguments = 2
	return 2;
}