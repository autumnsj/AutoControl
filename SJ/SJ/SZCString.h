#pragma once
class SZCString
{
public:
	static BOOL InsertA(char* lpszDestination, int index, const char* lpszSrc);
	static BOOL ReplaceA(char* lpszDestination, const char* lpszOld, const char* lpszNew);
	static BOOL RemoveA(char* lpszDestination, int start, int count);
	static BOOL SubStringA(char* lpszDestination, int start, int count);
	static BOOL HexStrToBytes(char* buf, const char* lpszHexSrc, DWORD* length);
	SZCString();
	virtual ~SZCString();

};

