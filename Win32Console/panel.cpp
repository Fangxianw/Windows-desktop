#include "panel.h"

panel::panel()
{
}

panel::~panel()
{
}

bool AgingLCDDisplay()
{
	// open LCD Device
	HANDLE hDevice = CreateFile(L"\\\\.\\LCD",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	DWORD dwSupportBriNum = 0;
	BYTE bri_array[256];
	memset(bri_array, 0, sizeof(bri_array));

	if (INVALID_HANDLE_VALUE == hDevice)
	{
		printf("Failed to open panel device!\n");
		return false;
	}

	// query supported brightness
	if (0 == DeviceIoControl(hDevice,
		IOCTL_VIDEO_QUERY_SUPPORTED_BRIGHTNESS,
		NULL,
		0,
		bri_array,
		sizeof(bri_array),
		&dwSupportBriNum,
		NULL))
	{
		printf("Failed to query panel supported brightness!\n");
		CloseHandle(hDevice);
		return false;
	}

	//query current brightness
	DISPLAY_BRIGHTNESS db;
	db.ucDCBrightness = DISPLAYPOLICY_AC;
	DWORD dwOut = 0;
	if (0 == DeviceIoControl(hDevice,
		IOCTL_VIDEO_QUERY_DISPLAY_BRIGHTNESS,
		NULL,
		0,
		&db,
		sizeof(DISPLAY_BRIGHTNESS),
		&dwOut,
		NULL))
	{
		printf("Failed to get panel brightness!\n");
		CloseHandle(hDevice);
		return false;
	}

	DISPLAY_BRIGHTNESS new_db;
	//brightness from min->max						
	for (DWORD i = 0; i < dwSupportBriNum; i++)
	{
		new_db.ucACBrightness = bri_array[i];
		new_db.ucDCBrightness = bri_array[i];
		new_db.ucDisplayPolicy = DISPLAYPOLICY_BOTH;
		if (0 == DeviceIoControl(hDevice,
			IOCTL_VIDEO_SET_DISPLAY_BRIGHTNESS,
			&new_db,
			sizeof(DISPLAY_BRIGHTNESS),
			NULL,
			0,
			&dwOut,
			NULL))
		{
			printf("Failed to set panel brightness to %d! \n", i);
			CloseHandle(hDevice);
			return false;
		}
		printf("Set panel brightness to %d/%d \n", i, dwSupportBriNum);

		Sleep((int)(10 * 1000 / dwSupportBriNum));
	}

	//restore brightness
	if (0 == DeviceIoControl(hDevice,
		IOCTL_VIDEO_SET_DISPLAY_BRIGHTNESS,
		&db,
		sizeof(DISPLAY_BRIGHTNESS),
		NULL,
		0,
		&dwOut,
		NULL))
	{
		printf("Failed to set restore brightness ! \n");
		CloseHandle(hDevice);
		return false;
	}

	return true;
}

/*
int main()
{
if (!AgingLCDDisplay())
{
printf("LCD BrightLess error \n");
}
printf("end.\n");
return 0;
}*/