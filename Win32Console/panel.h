#pragma once

#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <ntddvdeo.h>  //IOCTL_VIDEO_QUERY_SUPPORTED_BRIGHTNESS,

class panel
{
public:
	panel();
	~panel();
};

// Battery capacity, HDD scan, and LCD brightness were implemented via Windows IO Controls
// LCD brightness /IOCTL_DIDEO_SET_DISPLAY_BRIGHTNESS
bool AgingLCDDisplay();
