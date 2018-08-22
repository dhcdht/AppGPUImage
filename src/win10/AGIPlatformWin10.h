#pragma once

#include <iostream>
#include <string>

class AGIPlatformWin10
{
public:
	AGIPlatformWin10();
	virtual ~AGIPlatformWin10();

	static AGIPlatformWin10* sharedInstance();

public:
	void setBundlePath(std::string bundlePath);
};

