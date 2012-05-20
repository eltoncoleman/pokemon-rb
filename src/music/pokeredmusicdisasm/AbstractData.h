#ifndef ABSTRACTDATA_H
#define ABSTRACTDATA_H

#include <string>

// All information types inherit from here
class AbstractData
{
public:
    AbstractData();

    virtual std::string GenAsm(); // Generate Assembly Output
    virtual bool Parse(unsigned char* byte); // Parse Given Data
	virtual bool GetError(); // Get Error (No Write, Error is read only)

	virtual bool IsValid(unsigned char* byte); // Check for byte validity
    virtual unsigned int Arguments(); // Number of arguments taken

protected:
    bool error; // Whether there's an error in parsing or not
};

#endif