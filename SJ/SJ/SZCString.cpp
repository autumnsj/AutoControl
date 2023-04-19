#include "pch.h"
#include "SZCString.h"

BOOL SZCString::RemoveA(char* lpszDestination, int start, int count)
{


	try
	{
		lpszDestination += start;
		while (lpszDestination[count])
		{
			lpszDestination[0] = lpszDestination[count];
			lpszDestination++;
		}
		lpszDestination[0] = 0;

	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;

}

BOOL SZCString::InsertA(char* lpszDestination, int index, const char* lpszSrc)
{

	try
	{
		char* temp = new char[strlen(lpszDestination) + strlen(lpszSrc) + 1];
		strcpy(temp, lpszDestination + index);
		lpszDestination[index] = 0;
		strcat(lpszDestination, lpszSrc);
		strcat(lpszDestination, temp);
		delete[]temp;
	}
	catch (...)
	{
		return FALSE;
	}

	return TRUE;

}
BOOL SZCString::ReplaceA(char* lpszDestination, const char* lpszOld, const char* lpszNew)
{
	try
	{
		do
		{
			if (!memcmp(lpszDestination, lpszOld, strlen(lpszOld)))
			{
				RemoveA(lpszDestination, 0, strlen(lpszOld));
				InsertA(lpszDestination, 0, lpszNew);
				lpszDestination += strlen(lpszOld);
			}
			lpszDestination++;
		} while (lpszDestination[0]);
	}
	catch (...)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL SZCString::SubStringA(char* lpszDestination, int start, int count)
{
	RemoveA(lpszDestination, 0, start);
	lpszDestination[count] = 0;
	return TRUE;
}
BOOL SZCString::HexStrToBytes(char* buf, const char* lpszHexSrc, DWORD* length)
{
	BOOL ret = TRUE;
	char* temp = new char[strlen(lpszHexSrc) + 1];
	strcpy(temp, lpszHexSrc);
	ReplaceA(temp, " ", "");
	if (length)
	{
		*length = strlen(temp) / 2;
	}
	try
	{
		int i = 0;
		while (temp[i * 2])
		{
			char sItem[3];
			memcpy(sItem, temp + i * 2, 2);
			sItem[2] = 0;
			buf[i] = (char)strtol(sItem, NULL, 16);
			i++;
		}
	}
	catch (...)
	{
		ret = FALSE;
	}
	delete[]temp;
	return ret;
}