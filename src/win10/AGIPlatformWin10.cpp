#include "AGIPlatformWin10.h"
#include "core/AGIResourceManager.h"


AGIPlatformWin10::AGIPlatformWin10()
{
}

AGIPlatformWin10::~AGIPlatformWin10()
{
}

AGIPlatformWin10* AGIPlatformWin10::sharedInstance()
{
	static AGIPlatformWin10* kSharedInstance = new AGIPlatformWin10();

	return kSharedInstance;
}

void AGIPlatformWin10::setBundlePath(std::string bundlePath)
{
	AGIResourceManager::sharedBundle()->setBundlePath(bundlePath);
}
