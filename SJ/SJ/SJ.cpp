// SJ.cpp : 定义 DLL 的导出函数。
//

#include "pch.h"
#include "framework.h"
#include "SJ.h"
#include <boost/regex.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/find_iterator.hpp>
#include "SZCString.h"
#include "xasm.h"



////////////////////////////////非导出块START//////////////////////////////////////////

void GetAbsolutePoints(int nX_i, int nY_i, POINT* point)
{
	HWND hwnd = GetDesktopWindow();
	RECT rect;
	GetWindowRect(hwnd, &rect);
	point->x = 65536.0 / rect.right * nX_i;
	point->y = 65536.0 / rect.bottom * nY_i;
}
wchar_t* AnsiToUnicode(const char* str)
{
	//DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, str, -1, NULL, 0);  
	DWORD dwNum = strlen(str) * 2;
	wchar_t* pwText;
	pwText = new wchar_t[dwNum];
	MultiByteToWideChar(CP_ACP, 0, str, -1, pwText, dwNum);
	return pwText;
}
void Alert(LPCSTR msg)
{
	MessageBoxA(NULL, msg, "SZ模块", 0);
}
BOOL  CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam)
{

	PWNDINFO wnd = (PWNDINFO)lParam;
	DWORD dwPid = 0;
	GetWindowThreadProcessId(hWnd, &dwPid); // 获得找到窗口所属的进程    
	if (dwPid == wnd->dwPid) // 判断是否是目标进程的窗口    
	{

		wnd->hWnd = hWnd;
		CHAR sCaption[WINDOW_TEXT_LENGTH];
		CHAR sClass[WINDOW_TEXT_LENGTH];
		SendMessageA(hWnd, WM_GETTEXT, WINDOW_TEXT_LENGTH, (LPARAM)sCaption);
		GetClassNameA(hWnd, sClass, WINDOW_TEXT_LENGTH);
		TRACE(L"hwnd=%x,class=%s,caption=%s\n", hWnd, AnsiToUnicode(sClass), AnsiToUnicode(sCaption));
		int ret = 0;
		if (strcmp("", wnd->sCaption))
		{
			ret = strcmp(sCaption, wnd->sCaption);
		}
		if (strcmp("", wnd->sClass))
		{
			if (!ret)
			{
				ret = strcmp(sClass, wnd->sClass);
			}
		}
		if (!ret)
		{
			wnd->success = TRUE;
			return FALSE;
		}
	}
	EnumChildWindows(hWnd, EnumWindowCallBack, lParam);
	if (wnd->success)
	{
		return FALSE;
	}
	return TRUE;
}
BOOL SZMemcmp(const char* lpData, const char* lpFeatureBytes, DWORD dwSize)
{

	int i = 0;
	while (i < dwSize)
	{
		if (lpFeatureBytes[i] != 0 && lpData[i] != lpFeatureBytes[i])
		{
			return FALSE;
		}
		i++;
	}
	return TRUE;

}
char* GetBmpData(LPCSTR lpszName, BITMAP& bm)
{

	char* lpBufbmp;
	HANDLE hp;
	HDC hdc, hdcmem;
	HBITMAP hBm;
	BITMAPINFO   b;
	//加载图片
	hBm = (HBITMAP)LoadImageA(NULL,/*"c:\\1.bmp"*/lpszName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	TRACE(L"加载图片句柄:%x\n", hBm);
	GetObject(hBm, sizeof(BITMAP), (LPVOID)&bm); //获得句柄
	hdc = ::GetWindowDC(NULL);
	hdcmem = CreateCompatibleDC(hdc);
	SelectObject(hdcmem, hBm);
	BitBlt(hdcmem, 0, 0, bm.bmWidth, bm.bmHeight, hdcmem, 0, 0, SRCCOPY);

	b.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	b.bmiHeader.biWidth = bm.bmWidth;
	b.bmiHeader.biHeight = bm.bmHeight;
	b.bmiHeader.biPlanes = 1;
	b.bmiHeader.biBitCount = 24;
	b.bmiHeader.biCompression = BI_RGB;
	b.bmiHeader.biSizeImage = 0;
	b.bmiHeader.biXPelsPerMeter = 0;
	b.bmiHeader.biYPelsPerMeter = 0;
	b.bmiHeader.biClrUsed = 0;
	b.bmiHeader.biClrImportant = 0;
	b.bmiColors[0].rgbBlue = 8;
	b.bmiColors[0].rgbGreen = 8;
	b.bmiColors[0].rgbRed = 8;
	b.bmiColors[0].rgbReserved = 0;

	hp = GetProcessHeap();
	lpBufbmp = (char*)HeapAlloc(hp, HEAP_ZERO_MEMORY, bm.bmHeight * bm.bmWidth * 4);
	GetDIBits(hdc, hBm, 0, bm.bmHeight, lpBufbmp, &b, DIB_RGB_COLORS);

	DeleteObject(&hBm);
	DeleteDC(hdc);
	DeleteDC(hdcmem);
	return lpBufbmp;
}
//从数据char *lpBufscr中查找*lpBufbmp;
int FindBmpBuf(int x1, int y1, int x2, int y2, int& fx, int& fy, DWORD dwX, DWORD dwY, BITMAP& bm, char* lpBufbmp, BITMAP& bb, char* lpBufscr)
{
	POINT pt, pttmp;
	fx = -1;
	fy = -1;

	pt = FindFirstPoint(x1, y1, x2, y2, dwX, dwY, bm, lpBufbmp, bb, lpBufscr);
	if (Compare(pt, bm, lpBufbmp, bb, lpBufscr, dwX, dwY))    //找到的第一个点正是位图的第一个点，此时的坐标正是位图所在的坐标
	{
		fx = pt.x; fy = pt.y; return 1;
	}
	else//出现第一个点找到后却并不与位图吻合
	{
		pttmp.y = pt.y + 1;
		while (pt = FindFirstPoint(pt.x + 1, pt.y, x2, pt.y + 1, dwX, dwY, bm, lpBufbmp, bb, lpBufscr), pt.x >= 0)
		{
			if (Compare(pt, bm, lpBufbmp, bb, lpBufscr, dwX, dwY))
			{
				fx = pt.x;
				fy = pt.y;
				return 1;
			}
		}
		while (pt = FindFirstPoint(x1, pttmp.y, x2, y2, dwX, dwY, bm, lpBufbmp, bb, lpBufscr), pt.x >= 0)
		{
			if (Compare(pt, bm, lpBufbmp, bb, lpBufscr, dwX, dwY))
			{
				fx = pt.x;
				fy = pt.y;
				return 1;
			}
			else
			{
				pttmp.y = pt.y + 1;
				while (pt = FindFirstPoint(pt.x + 1, pt.y, x2, pt.y + 1, dwX, dwY, bm, lpBufbmp, bb, lpBufscr), pt.x >= 0)
					if (Compare(pt, bm, lpBufbmp, bb, lpBufscr, dwX, dwY))
					{
						fx = pt.x; fy = pt.y;
						return 1;
					}
			}
		}
	}
	return 0;
}
char* GetScreenData(int dx, int dy, BITMAP& bb, DWORD& dwX, DWORD& dwY)
{

	char* lpBufscr;
	HBITMAP   hBitmap, hOld;         //图片句柄
	HDC   hDC, hcDC;                 //设备DC
	BITMAPINFO   b;                 //图片数据信息
	HANDLE   hp;                    //句柄

	dwX = GetSystemMetrics(SM_CXSCREEN);          //屏幕的宽度
	dwY = GetSystemMetrics(SM_CYSCREEN);          //屏幕的高度


	//dwX=min(dwX,(unsigned)dx);      //取给定宽度与获得宽度的最小那个
	//dwY=min(dwY,(unsigned)dy);      //取给定高度与获得高度的最小那个
	HWND hwndScr = GetDesktopWindow();
	LockWindowUpdate(hwndScr);
	hDC = ::GetDCEx(hwndScr, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE);
	hcDC = CreateCompatibleDC(hDC);
	hBitmap = CreateCompatibleBitmap(hDC, dwX, dwY);   //在DC中创建一个兼容DC图片缓冲区
	hOld = (HBITMAP)SelectObject(hcDC, hBitmap);      //将hcDc选择到设备中去
	BitBlt(hcDC, 0, 0, dwX, dwY, hDC, 0, 0, SRCCOPY | 0x40000000);       //将hDc中的像素直接复制到hcDc中
	/*设置屏幕图片的信息*/
	bb.bmWidth = dwX;       //宽度
	bb.bmHeight = dwY;      //高度
	bb.bmPlanes = 1;         //绘图面板数，一般为1
	bb.bmWidthBytes = bb.bmWidth * 3;   //宽度的字节数，一行占多少个字节
	bb.bmBitsPixel = 3;              //像素位数
	bb.bmType = 0;                   //图片类型

	/*设置图片信息*/
	b.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);       //图片头大小
	b.bmiHeader.biWidth = dwX;                                 //图片宽度
	b.bmiHeader.biHeight = dwY;                                //图片长度
	b.bmiHeader.biPlanes = 1;                            //绘图面板数
	b.bmiHeader.biBitCount = 24;                            //图像的位数，24真彩色
	b.bmiHeader.biCompression = BI_RGB;                  //指定的压缩算法(格式)，BI_RGB不压缩
	b.bmiHeader.biSizeImage = 0;                        //图片实际大小，宽*高*byte，指定了BI_RGB就为0

	b.bmiHeader.biXPelsPerMeter = 0;                    //每米包含的像素数
	b.bmiHeader.biYPelsPerMeter = 0;

	b.bmiHeader.biClrUsed = 0;                         //使用的颜色数，0表示使用全部颜色
	b.bmiHeader.biClrImportant = 0;                    //重要的颜色数,0表示全部重要

	/*颜色表**/
	b.bmiColors[0].rgbBlue = 8;                               //蓝色值
	b.bmiColors[0].rgbGreen = 8;                              //绿色值
	b.bmiColors[0].rgbRed = 8;                                //红色值
	b.bmiColors[0].rgbReserved = 0;                           //保留
	//在堆上分配空间
	hp = GetProcessHeap();
	lpBufscr = (char*)HeapAlloc(hp, HEAP_ZERO_MEMORY, bb.bmHeight * bb.bmWidth * 4);

	GetDIBits(hcDC, hBitmap, 0, dwY, lpBufscr, &b, DIB_RGB_COLORS);                    //获取指定兼容位图的位,在hcDc上

	//释放申请的DC
	ReleaseDC(NULL, hDC);
	DeleteDC(hcDC);
	DeleteObject(hBitmap);
	DeleteObject(hOld);
	LockWindowUpdate(NULL);
	//	HeapFree(hp,0,lpBufscr);
	return lpBufscr;

}
/**在屏幕中找与图片第一个点相同颜色的坐标**/
POINT FindFirstPoint(int x1, int y1, int x2, int y2, DWORD dwX, DWORD dwY, BITMAP& bm, char* lpBufbmp, BITMAP& bb, char* lpBufscr)
{

	int color = GetBmpPixel(0, 0, dwX, dwY, bm, lpBufbmp);
	POINT pt;
	pt.x = -1;
	pt.y = -1;
	int fx, fy, xtmp = x1;;
	fx = fy = -1;
	//查找
	for (; y1 < y2; y1++)           //行
		for (x1 = xtmp; x1 < x2; x1++) //列
		{
			if (GetScrPixel(x1, y1, dwX, dwY, bb, lpBufscr) == color)
			{
				pt.x = x1;
				pt.y = y1;
				return pt;
			}
		}
	return pt;
}
/**获得图片指定位置的颜色值**/
int GetBmpPixel(int x, int y, DWORD dwX, DWORD dwY, BITMAP& bm, char* lpBufbmp)
{
	if (x<0 || x>(int)dwX || x<0 || x>(int)dwY)return-1;
	BYTE   bBlue = lpBufbmp[bm.bmWidthBytes * (bm.bmHeight - y - 1) + bm.bmBitsPixel * x / 8 + 0];
	BYTE   bGreen = lpBufbmp[bm.bmWidthBytes * (bm.bmHeight - y - 1) + bm.bmBitsPixel * x / 8 + 1];
	BYTE   bRed = lpBufbmp[bm.bmWidthBytes * (bm.bmHeight - y - 1) + bm.bmBitsPixel * x / 8 + 2];

	COLORREF   color = RGB(bBlue, bGreen, bRed);
	return int(color);
}
/**获得屏幕指定位置的颜色值**/
int GetScrPixel(int x, int y, DWORD dwX, DWORD dwY, BITMAP& bb, char* lpBufscr)
{

	if (x<0 || x>(int)dwX || y<0 || x>(int)dwY)return-1;
	BYTE   bBlue = lpBufscr[bb.bmWidthBytes * (dwY - y - 1) + bb.bmBitsPixel * x + 0];
	BYTE   bGreen = lpBufscr[bb.bmWidthBytes * (dwY - y - 1) + bb.bmBitsPixel * x + 1];
	BYTE   bRed = lpBufscr[bb.bmWidthBytes * (dwY - y - 1) + bb.bmBitsPixel * x + 2];

	COLORREF   color = RGB(bBlue, bGreen, bRed);
	return int(color);
}
//比较屏幕pt点位图矩形大小的所有颜色和位置是否与位图相同
int Compare(POINT pt, BITMAP& bm, char* lpBufbmp, BITMAP& bb, char* lpBufscr, DWORD dwX, DWORD dwY)
{
	int x, y;
	if ((unsigned)bm.bmHeight > dwY - pt.y)return 0;   //图片超出了屏幕

	for (y = 0; y < bm.bmHeight - 1; y++)    //图片行
	{
		for (x = 0; x < bm.bmWidth - 1; x++)   //图片的列
		{
			if (GetBmpPixel(x, y, dwX, dwY, bm, lpBufbmp) == GetScrPixel(pt.x + x, pt.y + y, dwX, dwY, bb, lpBufscr))   //比较图片中的点，与相应屏幕点的颜色
			{


			}
			else
			{
				return 0;
			}
		}
	}
	return 1;
}
//比较屏幕pt点位图矩形大小的所有颜色和位置是否与位图条件匹配
//位图backcolor颜色点忽略，颜色偏差errorcolor，允许不匹配点的个数errorcount
//比较成功返回1，否则返回0
int CompareEx(POINT pt, int backcolor, int errorcolor, int errorcount, BITMAP& bm, BITMAP& bb, DWORD dwX, DWORD dwY, char* lpBufbmp, char* lpBufscr)
{
	int x, y, colorB, colorS, count = 0;
	if ((unsigned)bm.bmHeight > dwY - pt.y)return 0;
	for (y = 0; y < bm.bmHeight - 1; y++)
		for (x = 0; x < bm.bmWidth - 1; x++)
		{
			colorB = GetBmpPixel(x, y, dwX, dwY, bm, lpBufbmp);
			colorS = GetScrPixel(pt.x + x, pt.y + y, dwX, dwY, bb, lpBufscr);
			if (colorB == colorS || colorB == backcolor);
			else
			{
				if (abs(GetRValue((COLORREF)colorB) - GetRValue((COLORREF)colorS) > GetRValue((COLORREF)errorcolor))
					|| abs(GetGValue((COLORREF)colorB) - GetGValue((COLORREF)colorS) > GetGValue((COLORREF)errorcolor))
					|| abs(GetBValue((COLORREF)colorB) - GetBValue((COLORREF)colorS) > GetBValue((COLORREF)errorcolor))) {
					count++;
				}
			}
		}
	if (count > errorcount)
		return 0;
	else
		return 1;
}


///////////////////////////////非导出块END///////////////////////////////////////////
extern "C" HWND SJ_API SZFindWindow(LPCSTR lpClassName, LPCSTR lpWindowName)
{

	HWND Hwnd = ::FindWindowA(lpClassName, lpWindowName);
	if (GetWindowLong(Hwnd, GWL_STYLE) & WS_VISIBLE)
	{
		return Hwnd;
	}
	return (HWND)0;
}


extern "C" HWND SJ_API SZFindWindowByProcess(LPCSTR lpProcessName, LPCSTR lpClassName, LPCSTR lpWindowName)
{
	wchar_t* lpTempProcessName = AnsiToUnicode(lpProcessName);
	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(pe32);
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		return (HWND)0;
	}

	BOOL bMore = ::Process32First(hProcessSnap, &pe32);
	while (bMore)
	{
		if (!lstrcmp(pe32.szExeFile, lpTempProcessName))
		{
			break;
		}
		bMore = ::Process32Next(hProcessSnap, &pe32);
	}
	::CloseHandle(hProcessSnap);

	if (lstrcmp(pe32.szExeFile, lpTempProcessName))
	{
		delete[]lpTempProcessName;
		return 0;
	}
	WNDINFO wndinfo = { 0 };
	wndinfo.dwPid = pe32.th32ProcessID;
	if (lpWindowName != NULL)
	{
		strcpy(wndinfo.sCaption, lpWindowName);
	}
	if (lpClassName != NULL)
	{
		strcpy(wndinfo.sClass, lpClassName);
	}
	delete[]lpTempProcessName;
	EnumWindows(EnumWindowCallBack, (LPARAM)&wndinfo);
	return wndinfo.hWnd;
}
extern "C" int SJ_API SZGetWindowRect(HWND hwnd, RECT* rect)
{
	BOOL ret = GetWindowRect(hwnd, rect);
	return ret;
}

extern "C" int SJ_API SZMoveTo(int x, int y)
{
	//SetCursorPos(x, y);
	POINT point;
	GetAbsolutePoints(x, y, &point);
	INPUT input;
	input.type = INPUT_MOUSE;
	input.mi.time = GetTickCount();    // 若赋值为0 ， 则由系统分配时间
	input.mi.dx = (LONG)point.x;
	input.mi.dy = (LONG)point.y;
	input.mi.mouseData = 0;      // 若未使用滑轮，则赋值0·
	input.mi.dwFlags = (MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE);
	// 使用绝对坐标值，若使用相对坐标值，则只需要删除MOUSEEVENTF_ABSOLUTE 即可

	input.mi.dwExtraInfo = 0;
	SendInput(1, &input, sizeof(INPUT));
	return TRUE;
}
extern "C" int SJ_API SZLeftClick()
{
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	return 0;
}
extern "C" int SJ_API SZRightClick()
{
	mouse_event(MOUSEEVENTF_RIGHTDOWN | MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	return 0;
}
extern "C" int SJ_API SZLeftDown()
{
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	return 0;
}
extern "C" int SJ_API SZLeftUp()
{
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
	return 0;
}
extern "C" int SJ_API SZRightDown()
{
	mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
	return 0;
}
extern "C" int SJ_API SZRightUp()
{
	mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
	return 0;
}
extern "C" int SJ_API SZMoveWindow(HWND hwnd, int x, int y)
{
	RECT rect;
	if (!GetWindowRect(hwnd, &rect))
	{
		return 0;
	}
	return MoveWindow(hwnd, x, y, rect.right - rect.left, rect.bottom - rect.top, TRUE);
}

extern "C" int SJ_API SZSetWindowState(HWND hwnd, int flag)
{
	switch (flag)
	{
	case 1://激活指定窗口
		SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetCapture(hwnd);
		ReleaseCapture();
		break;
	case 8://置顶指定窗口
		SetWindowPos(hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		SetCapture(hwnd);
		ReleaseCapture();
		SetFocus(hwnd);
		break;
	case 9://取消置顶指定窗口
		SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		break;
	case 15://使指定的窗口获取输入焦点
		//CWnd(hwnd).GetFocus();
		SetFocus(hwnd);
		break;
	}
	return 1;
	//return SetWindowLong(hwnd,GWL_EXSTYLE,nStyle);
}
extern "C" int SJ_API SZKeyPress(int vk)
{
	keybd_event(vk, 0, 0, GetMessageExtraInfo());
	Sleep(5);
	keybd_event(vk, 0, KEYEVENTF_KEYUP, GetMessageExtraInfo());
	Sleep(5);
	return 1;
}
extern "C" int SJ_API SZKeyDown(int vk)
{
	keybd_event(vk, 0, 0, GetMessageExtraInfo());
	Sleep(5);
	return 1;
}
extern "C" int SJ_API SZKeyUp(int vk)
{
	keybd_event(vk, 0, KEYEVENTF_KEYUP, GetMessageExtraInfo());
	Sleep(5);
	return 1;
}
#ifndef CAPTUREBLT
#define CAPTUREBLT 0x40000000
#endif
extern "C" int SJ_API SZCapture(int x1, int y1, int x2, int y2, LPCSTR file)
{
	RECT rect;
	rect.left = x1;
	rect.top = y1;
	rect.right = x2;
	rect.bottom = y2;
	LPRECT lpRect = &rect;
	HDC      hdc, hdcMem;
	HBITMAP  hemfCopy;
	HWND     hwndScr;
	int      dstcx, dstcy;
	if (lpRect)
	{
		dstcx = lpRect->right - lpRect->left;
		dstcy = lpRect->bottom - lpRect->top;
	}
	else
	{
		dstcx = GetSystemMetrics(SM_CXSCREEN);
		dstcy = GetSystemMetrics(SM_CYSCREEN);
	}
	if (LockWindowUpdate(hwndScr = GetDesktopWindow()))
	{
		hdc = GetDCEx(hwndScr, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE);
		hdcMem = CreateCompatibleDC(hdc);
		if (NULL == hdcMem)
		{
			ReleaseDC(hwndScr, hdc);
			LockWindowUpdate(NULL);
			return NULL;
		}
		hemfCopy = CreateCompatibleBitmap(hdc, abs(dstcx), abs(dstcy));
		if (NULL == hemfCopy)
		{
			DeleteDC(hdcMem);
			ReleaseDC(hwndScr, hdc);
			LockWindowUpdate(NULL);
			return NULL;
		}
		SelectObject(hdcMem, hemfCopy);
		if (lpRect)
			StretchBlt(hdcMem, 0, 0, abs(dstcx), abs(dstcy),
				hdc, lpRect->left, lpRect->top, dstcx, dstcy, SRCCOPY | CAPTUREBLT);
		else
			BitBlt(hdcMem, 0, 0, dstcx, dstcy,
				hdc, 0, 0, SRCCOPY | CAPTUREBLT);
		DeleteDC(hdcMem);
		ReleaseDC(hwndScr, hdc);
		LockWindowUpdate(NULL);
		//  return hemfCopy;
	  //	return TRUE;

		HDC     hDC;
		//当前分辨率下每象素所占字节数            
		int     iBits;
		//位图中每象素所占字节数            
		WORD     wBitCount;
		//定义调色板大小，     位图中像素字节大小     ，位图文件大小     ，     写入文件字节数                
		DWORD     dwPaletteSize = 0, dwBmBitsSize = 0, dwDIBSize = 0, dwWritten = 0;
		//位图属性结构                
		BITMAP     Bitmap;
		//位图文件头结构            
		BITMAPFILEHEADER     bmfHdr;
		//位图信息头结构                
		BITMAPINFOHEADER     bi;
		//指向位图信息头结构                    
		LPBITMAPINFOHEADER     lpbi;
		//定义文件，分配内存句柄，调色板句柄                
		HANDLE     fh, hDib, hPal, hOldPal = NULL;

		//计算位图文件每个像素所占字节数                
		hDC = CreateDC(L"DISPLAY", NULL, NULL, NULL);
		iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
		DeleteDC(hDC);
		if (iBits <= 1)
			wBitCount = 1;
		else  if (iBits <= 4)
			wBitCount = 4;
		else if (iBits <= 8)
			wBitCount = 8;
		else
			wBitCount = 24;

		GetObject(hemfCopy, sizeof(Bitmap), (LPSTR)&Bitmap);
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biWidth = Bitmap.bmWidth;
		bi.biHeight = Bitmap.bmHeight;
		bi.biPlanes = 1;
		bi.biBitCount = wBitCount;
		bi.biCompression = BI_RGB;
		bi.biSizeImage = 0;
		bi.biXPelsPerMeter = 0;
		bi.biYPelsPerMeter = 0;
		bi.biClrImportant = 0;
		bi.biClrUsed = 0;

		dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

		//为位图内容分配内存                
		hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
		lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
		*lpbi = bi;

		//     处理调色板                    
		hPal = GetStockObject(DEFAULT_PALETTE);
		if (hPal)
		{
			hDC = ::GetDC(NULL);
			hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
			RealizePalette(hDC);
		}

		//     获取该调色板下新的像素值                
		GetDIBits(hDC, hemfCopy, 0, (UINT)Bitmap.bmHeight,
			(LPSTR)lpbi + sizeof(BITMAPINFOHEADER) + dwPaletteSize,
			(BITMAPINFO*)lpbi, DIB_RGB_COLORS);

		//恢复调色板                    
		if (hOldPal)
		{
			::SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
			RealizePalette(hDC);
			::ReleaseDC(NULL, hDC);
		}

		//创建位图文件                    
		fh = CreateFileA(file, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

		if (fh == INVALID_HANDLE_VALUE)         return     FALSE;

		//     设置位图文件头                
		bmfHdr.bfType = 0x4D42;     //     "BM"                
		dwDIBSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;
		bmfHdr.bfSize = dwDIBSize;
		bmfHdr.bfReserved1 = 0;
		bmfHdr.bfReserved2 = 0;
		bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;
		//     写入位图文件头                
		WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);
		//     写入位图文件其余内容                
		WriteFile(fh, (LPSTR)lpbi, dwDIBSize, &dwWritten, NULL);
		//清除                    
		GlobalUnlock(hDib);
		GlobalFree(hDib);
		CloseHandle(fh);

		return     TRUE;
	}
	return NULL;
}
extern "C" int SJ_API SZSendString(HWND hwnd, LPCSTR str)
{
	int i = 0;
	while (str[i])
	{
		SendMessageA(hwnd, WM_IME_CHAR, str[i], 1);
		i++;
	}
	return 1;
}
extern "C" int SJ_API SZGetModuleBaseAddr(HWND hwnd, LPCSTR module_name)
{
	DWORD nPid;
	GetWindowThreadProcessId(hwnd, &nPid);
	MODULEENTRY32 modul;
	HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nPid);
	modul.dwSize = sizeof(MODULEENTRY32);
	WCHAR* tempModule_name = AnsiToUnicode(module_name);
	BOOL result = Module32First(snap, &modul);
	TRACE(L"%s\n", modul.szModule);
	while (wcscmp(modul.szModule, tempModule_name))
	{
		TRACE(L"%s\n", modul.szModule);
		if (!Module32Next(snap, &modul))
		{
			result = FALSE;
			break;
		}
	}
	delete[] tempModule_name;
	CloseHandle(snap);
	if (result)
	{
		return (int)modul.modBaseAddr;
	}
	return NULL;

}
///提权
extern "C" BOOL SJ_API SZPrivilgeEscalation()
{
	HANDLE hToken;
	LUID sedebugnameValue;
	TOKEN_PRIVILEGES tkp;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken))
	{
		OutputDebugString(L"提权失败1");
		return FALSE;
	}

	if (!LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
	{
		CloseHandle(hToken);
		TRACE(L"提权失败2");
		return FALSE;
	}
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Luid = sedebugnameValue;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL))
	{
		TRACE(L"提权失败3");
		CloseHandle(hToken);
		return FALSE;
	}
	else
		return TRUE;
}
extern "C" int SJ_API SZAsmToBytes(DWORD addr, LPCSTR lpAsmCode, char* lpByteBuff, int* pByteBuffSize)
{
	char tempcode[1024];
	strcpy(tempcode, lpAsmCode);
	TRACE(L"开辟的地址:%08X", addr);
	TRACE(L"机器码如下:");
	char error[256] = { 0 };
	int j = 0;
	int m = 0;
	XASM m_asm;
	char* cmd = strtok(tempcode, "|");
	while (cmd != NULL)
	{
		t_asmmodel t_asm;
		j = m_asm.Assemble(cmd, (DWORD)addr + m, &t_asm, 0, 4, error);
		if (j <= 0)
		{
			char errortemp[4];
			strcpy(errortemp, cmd);
			errortemp[3] = 0;
			if (!_stricmp(errortemp, "db "))
			{
				lpByteBuff[m] = (char)strtol(cmd + 3, NULL, 16);
				TRACE(L"%02X", lpByteBuff[m]);
				m++;
			}
			else if (!_stricmp(errortemp, "ret"))
			{
				SHORT retn = (SHORT)strtol(cmd + 4, NULL, 16);
				if (retn)
				{
					lpByteBuff[m] = 0xc2;
					TRACE(L"%02X", lpByteBuff[m]);
					m++;
					*((SHORT*)(lpByteBuff + m)) = retn;
					TRACE(L"%02X", lpByteBuff[m]);
					m++;
					TRACE(L"%02X", lpByteBuff[m]);
					m++;
				}
				else
				{
					lpByteBuff[m] = 0xc3;
					TRACE(L"%02X", lpByteBuff[m]);
					m++;
				}
			}
			else
			{
				strcat(error, "\r\n错误代码:");
				strcat(error, cmd);
				Alert(error);
				return 0;
			}
		}
		for (int k = 0; k < j; k++)
		{
			lpByteBuff[m] = (BYTE)t_asm.code[k];
			TRACE(L"%02X", lpByteBuff[m]);
			m++;
		}
		cmd = strtok(NULL, "|");
	}
	*pByteBuffSize = m;
	TRACE(L"\r\n");
	return 1;
}
#define allocSize 1024
#define MainTreadMsgId 12345
extern "C" int SJ_API  SZAsmCall(HWND hwnd,
	LPCSTR asmCode,
	int mode,
	DWORD* ret,//返回值
	PMainTreadHookInfo lpMainTreadHookInfo //如果是主线程代码申请内存空间地址，
)
{

	TRACE(L"返回值地址ret:%X\r\n 代码地址:%X", lpMainTreadHookInfo->dwRetAddr, lpMainTreadHookInfo->dwCodeAddr);

	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	if (!SZPrivilgeEscalation())
	{
		TRACE(L"提权失败");
		return 0;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (!hProcess) {
		TRACE(L"OpenProcess失败");
		return 0;
	}
	LPVOID CodeAddr = NULL;

	if (mode == 0)
	{
		CodeAddr = VirtualAllocEx(hProcess, NULL, allocSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	}
	else if (mode == 1)
	{
		if (lpMainTreadHookInfo->dwHhookAddr == 0)
		{
			lpMainTreadHookInfo->dwCodeAddr = (DWORD)VirtualAllocEx(hProcess, NULL, allocSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
		}
		CodeAddr = (LPVOID)lpMainTreadHookInfo->dwCodeAddr;
	}
	if (!CodeAddr)
	{
		TRACE(L"VirtualAllocEx失败");
		return 0;
	}
	char buf[allocSize];
	int bufCodeSize;
	SZAsmToBytes((DWORD)CodeAddr, asmCode, buf, &bufCodeSize);
	switch (mode)
	{
	case 1://用消息勾子注入主线程
	{
		buf[bufCodeSize] = 0x0c3;//ret 
		bufCodeSize++;
	}
	break;
	case 0://创建远线程
	default:
	{
		buf[bufCodeSize] = 0x0c2;//ret 4
		buf[bufCodeSize + 1] = 0x04;
		buf[bufCodeSize + 2] = 0x00;
		bufCodeSize += 3;
	}
	break;
	}

	if (!WriteProcessMemory(hProcess, CodeAddr, buf, bufCodeSize, NULL)) {
		TRACE(L"WriteProcessMemory失败");
		goto exit;
	}
	DWORD dwThreadID;
	switch (mode)
	{
	case 1:
		//主线程运行
	{
		if (lpMainTreadHookInfo->dwHhookAddr == 0)
		{

			//申请存放hhook句柄地址
			lpMainTreadHookInfo->dwHhookAddr = (DWORD)VirtualAllocEx(hProcess, NULL, 4, MEM_COMMIT, PAGE_READWRITE);
			lpMainTreadHookInfo->dwRetAddr = (DWORD)VirtualAllocEx(hProcess, NULL, 4, MEM_COMMIT, PAGE_READWRITE);
			lpMainTreadHookInfo->dwMsgProcAddr = (DWORD)VirtualAllocEx(hProcess, NULL, allocSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			char tempcode[100];
			char hookMyfucCode[100];
			int nSize = 0;
			char  msgProcAsm[1000];
			sprintf(msgProcAsm,
				"PUSH EBP|"
				"MOV EBP,ESP|"
				"MOV EAX,DWORD PTR SS:[EBP+8]|"
				"PUSH EBX|"
				"PUSH ESI|"
				"PUSH EDI|"
				"TEST EAX,EAX|"
				"db 75|"//JNZ SHORT 0BCD003E
				"db 3a|"
				"MOV EAX,DWORD PTR SS:[EBP+10]|"
				"CMP DWORD PTR DS:[EAX+8],0x%x|"
				"db 75|"//JNZ SHORT 0BCD0035
				"db 1f|"
				"PUSH EAX|"
				"PUSH ECX|"
				"PUSH EDX|"
				"PUSH EBX|"
				"PUSH ESI|"
				"PUSH EDI|"
				"PUSH EBP|"
				"CALL DWORD PTR DS:[EAX+4]|"
				"POP EBP|"
				"MOV EBX,DWORD PTR SS:[EBP+10]|"
				"MOV EBX,DWORD PTR SS:[EBX]|"
				"MOV DWORD PTR DS:[EBX],EAX|"
				"POP EDI|"
				"POP ESI|"
				"POP EBX|"
				"POP EDX|"
				"POP ECX|"
				"POP EAX|"
				"XOR EAX,EAX|"
				"POP EDI|"
				"POP ESI|"
				"POP EBX|"
				"POP EBP|"
				"RETN 0C|"
				"MOV ECX,DWORD PTR SS:[EBP+10]|"
				"MOV EDX,DWORD PTR SS:[EBP+0xC]|"
				"PUSH ECX|"
				"PUSH EDX|"
				"PUSH EAX|"
				"MOV EAX,DWORD PTR DS:[0x%x]|"
				"PUSH EAX|"
				"CALL 0x%x|"
				"POP EDI|"
				"POP ESI|"
				"POP EBX|"
				"POP EBP|"
				"RETN 0C",
				MainTreadMsgId,
				lpMainTreadHookInfo->dwHhookAddr,
				CallNextHookEx);
			char msgProc[256];
			//TRACE(L"CallNextHookEx=%x\n",CallNextHookEx);
			//*((UINT*)(msgProc+0x13))=MainTreadMsgId;//替换消息ID
			//*((UINT*)(msgProc+0x48))=lpMainTreadHookInfo->dwHhookAddr;//替换hhook句柄地址
			//sprintf(tempcode,"call 0x%x",CallNextHookEx);
			SZAsmToBytes(lpMainTreadHookInfo->dwMsgProcAddr, msgProcAsm, msgProc, &nSize);
			WriteProcessMemory(hProcess, (PVOID)lpMainTreadHookInfo->dwMsgProcAddr, msgProc, nSize, NULL);

			//SetWindowsHookEx设置消息勾子得到hhook并保存在目标进程内存中
			//hhook=::SetWindowsHookEx(WH_CALLWNDPROC,MyMessageProc,0,主线程句柄);
			//Sleep(0xffffffff);//如果设置勾子的线程束,那么勾子会失效,所以这里要设置完就无限休眠
			PVOID hookMyfuc = VirtualAllocEx(hProcess, NULL, allocSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
			sprintf(tempcode, "push 0x%x|push 0|push 0x%x|push 0x%x|mov eax,0x%x|call eax|MOV DWORD PTR DS:[0x%x],EAX|push 0xEfffffff|mov eax,0x%x|call eax|retn 4",
				SZGetMainThreadId(pid),
				lpMainTreadHookInfo->dwMsgProcAddr,
				WH_CALLWNDPROC,
				SetWindowsHookEx,
				lpMainTreadHookInfo->dwHhookAddr,
				Sleep
			);
			SZAsmToBytes((DWORD)hookMyfuc, tempcode, hookMyfucCode, &nSize);
			//写入自己的函数
			WriteProcessMemory(hProcess, hookMyfuc, hookMyfucCode, nSize, NULL);
			HANDLE hHookThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)hookMyfuc, NULL, 0, &dwThreadID);

			//WaitForSingleObject(hHookThread,INFINITE);
			CloseHandle(hHookThread);


		}
		SendMessage(hwnd, MainTreadMsgId, lpMainTreadHookInfo->dwCodeAddr, lpMainTreadHookInfo->dwRetAddr);
		ReadProcessMemory(hProcess, (PVOID)(lpMainTreadHookInfo->dwRetAddr), ret, 4, NULL);


	}
	break;
	case 0:
	default:
	{	//为0时
		//创建远线程
		HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)CodeAddr, NULL, 0, &dwThreadID);
		if (!hRemoteThread)
		{
			TRACE(L"CreateRemoteThread失败");
			goto exit;
		}
		WaitForSingleObject(hRemoteThread, INFINITE);
		GetExitCodeThread(hRemoteThread, ret);
		CloseHandle(hRemoteThread);
		VirtualFreeEx(hProcess, CodeAddr, allocSize, MEM_DECOMMIT);
	}
	break;
	}

exit:
	CloseHandle(hProcess);
	return 1;
}
// 由进程名获取主线程ID(需要头文件tlhelp32.h)
// 失败返回0
extern "C" DWORD SJ_API  SZGetMainThreadId(DWORD idProcess)
{
	DWORD idThread;
	if (idProcess == 0)
	{
		return 0;
	}
	// 获取进程的主线程ID
	THREADENTRY32 te;       // 线程信息
	te.dwSize = sizeof(THREADENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0); // 系统所有线程快照
	if (Thread32First(hSnapshot, &te))       // 第一个线程
	{
		do
		{
			if (idProcess == te.th32OwnerProcessID)      // 认为找到的第一个该进程的线程为主线程
			{
				idThread = te.th32ThreadID;
				break;
			}
		} while (Thread32Next(hSnapshot, &te));           // 下一个线程
	}
	CloseHandle(hSnapshot); // 删除快照
	return idThread;
}
extern "C" DWORD SJ_API SZGetAddr(HWND hwnd, LPCSTR lpszAddr)
{
	DWORD nPid;
	GetWindowThreadProcessId(hwnd, &nPid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nPid);
	DWORD dwAddr = 0;
	char lpszTemp[256];
	//"[[[<kkkk.exe>+231]+444]-455]+345"
	strcpy(lpszTemp, lpszAddr);
	SZCString::ReplaceA(lpszTemp, " ", "");
	boost::regex reg("<.+>");
	boost::regex reg2("\\[((\\+|\\-)?\\w+)?\\]");
	boost::regex reg3("(\\+|\\-)?\\w+");
	boost::cmatch mat;
	if (boost::regex_search(lpszTemp, mat, reg))
	{
		char szModuleName[100];
		strcpy(szModuleName, mat[0].str().c_str());
		SZCString::ReplaceA(lpszTemp, szModuleName, "");
		SZCString::SubStringA(szModuleName, 1, strlen(szModuleName) - 2);
		dwAddr = SZGetModuleBaseAddr(hwnd, szModuleName);
	}
	while (strlen(lpszTemp))
	{
		BOOL is_GetAddr = FALSE;
		if (boost::regex_search(lpszTemp, mat, reg2))
		{
			is_GetAddr = TRUE;
		}
		else if (boost::regex_search(lpszTemp, mat, reg3))
		{
			is_GetAddr = FALSE;
		}
		char szOffset[20];
		strcpy(szOffset, mat[0].str().c_str());
		SZCString::ReplaceA(lpszTemp, szOffset, "");
		if (is_GetAddr)
		{
			SZCString::SubStringA(szOffset, 1, strlen(szOffset) - 2);
		}
		switch (szOffset[0])
		{
		case '-':
			dwAddr -= strtol(szOffset + 1, NULL, 16);
			break;
		case '+':
			dwAddr += strtol(szOffset + 1, NULL, 16);
			break;
		default:
			dwAddr += strtol(szOffset, NULL, 16);
			break;
		}
		if (is_GetAddr)
		{
			try
			{
				ReadProcessMemory(hProcess, (LPCVOID)dwAddr, &dwAddr, 4, NULL);
			}
			catch (...)
			{
				return 0;
			}
		}
	}

	CloseHandle(hProcess);
	return dwAddr;
}
extern "C" float SJ_API  SZReadFloat(HWND hwnd, LPCSTR lpszAddr)
{
	int nAddr = SZGetAddr(hwnd, lpszAddr);
	float nRet;
	DWORD nPid;
	GetWindowThreadProcessId(hwnd, &nPid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nPid);
	ReadProcessMemory(hProcess, (LPCVOID)nAddr, &nRet, 4, NULL);
	CloseHandle(hProcess);
	return nRet;
}
extern "C" double SJ_API  SZReadDouble(HWND hwnd, LPCSTR lpszAddr)
{
	int nAddr = SZGetAddr(hwnd, lpszAddr);
	double nRet;
	DWORD nPid;
	GetWindowThreadProcessId(hwnd, &nPid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nPid);
	ReadProcessMemory(hProcess, (LPCVOID)nAddr, &nRet, sizeof(double), NULL);
	CloseHandle(hProcess);
	return nRet;
}
extern "C" DWORD SJ_API  SZReadInt(HWND hwnd, LPCSTR lpszAddr, int type)
{
	int nAddr = SZGetAddr(hwnd, lpszAddr);
	return SZReadIntAddr(hwnd, nAddr, type);
}
extern "C" DWORD SJ_API  SZReadIntAddr(HWND hwnd, int nAddr, int type)
{
	DWORD nRet = 0;
	DWORD nPid;
	GetWindowThreadProcessId(hwnd, &nPid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nPid);
	switch (type)
	{
	case 0://0 : 32位有符号
		ReadProcessMemory(hProcess, (LPCVOID)nAddr, &nRet, 4, NULL);
		;
		break;
	case 1://1 : 16 位有符号
		ReadProcessMemory(hProcess, (LPCVOID)nAddr, &nRet, 2, NULL);
		if (nRet & 0x8000)
		{
			nRet |= 0xFFFF0000;
		}
		break;
	case 2://2 : 8位有符号
		ReadProcessMemory(hProcess, (LPCVOID)nAddr, &nRet, 1, NULL);
		if (nRet & 0x80)
		{
			nRet |= 0xFFFFFF00;
		}
		break;
	case 3://3 : 16位无符号
		ReadProcessMemory(hProcess, (LPCVOID)nAddr, &nRet, 2, NULL);
		break;
	case 4://4 : 8位无符号
		ReadProcessMemory(hProcess, (LPCVOID)nAddr, &nRet, 1, NULL);
	default:
		break;
	}

	CloseHandle(hProcess);
	return nRet;
}
extern "C" int SJ_API   SZWriteData(HWND hwnd, LPCSTR lpszAddr, LPCSTR data)
{
	PVOID pAddr = (PVOID)SZGetAddr(hwnd, lpszAddr);
	char* tempdata = new char[strlen(data) + 1];
	strcpy(tempdata, data);
	SZCString::ReplaceA(tempdata, " ", "");
	DWORD bufSize = strlen(tempdata) / 2;
	char* buf = new char[bufSize];
	int i = 0;
	while (i < bufSize)
	{
		char dataItem[3];
		memcpy((PVOID)dataItem, (PVOID)(tempdata + i * 2), 2);
		dataItem[2] = 0;
		buf[i] = (char)strtol(dataItem, NULL, 16);
		i++;
	}
	DWORD nPid;
	DWORD nAccess;
	GetWindowThreadProcessId(hwnd, &nPid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nPid);
	try
	{
		WriteProcessMemory(hProcess, pAddr, buf, bufSize, (SIZE_T* ) & nAccess);
	}
	catch (...)
	{
		nAccess = 0;
	}
	delete[] tempdata;
	delete[] buf;
	return  nAccess;

}
extern "C" DWORD SJ_API  SZFindData(HWND hwnd, LPCSTR lpszData, DWORD dwStartAddr, DWORD dwEndAddr)
{
	DWORD nPid;
	GetWindowThreadProcessId(hwnd, &nPid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nPid);
	DWORD ret = 0;
	DWORD codeSize = strlen(lpszData) / 2;
	DWORD dwReadSize = 1024, dwRealitySize = 0;
	char* mydata = new char[codeSize];
	SZCString::HexStrToBytes(mydata, lpszData, &codeSize);
	char* buf = new char[dwReadSize];
	__int64 dwCur = dwStartAddr;
	while (dwCur < dwEndAddr)
	{
		ReadProcessMemory(hProcess, (LPCVOID)dwCur, buf, dwReadSize, (SIZE_T *) & dwRealitySize);

		if (dwRealitySize)
		{
			for (DWORD i = 0; i < (dwReadSize - codeSize); i++)
			{
				if (SZMemcmp(buf + i, mydata, codeSize))
				{
					ret = dwCur + i;
					goto end;
				}
			}
			dwCur += dwReadSize - codeSize;
		}
		else
		{
			dwCur -= dwCur % 0x1000;
			dwCur += 0x1000;
			TRACE(L"不可读%x\r\n", (int)dwCur);
		}
	}
end:
	delete[] buf;
	delete[] mydata;
	CloseHandle(hProcess);
	return ret;
}
//从矩形x1,y1,x2,y2内查找与位图lpszName完全匹配的起始点
//若找到,(fx,fy)为图片第一次出现的左上角顶点,并返回1
//若找不到,(fx,fy)=(-1,-1)并返回0
extern "C" int SJ_API  SZFindBmp(int x1, int y1, int x2, int y2, LPCSTR lpszName, int* fxx, int* fyy)
{
	TRACE(L"参数x1=%d,y1=%d,x2=%d,y2=%d:%s", x1, y1, x2, y2, AnsiToUnicode(lpszName));
	BITMAP bm;
	BITMAP bb;
	char* lpBufbmp;
	char* lpBufscr;
	DWORD dwX, dwY;
	lpBufbmp = GetBmpData(lpszName, bm);
	lpBufscr = GetScreenData(x2, y2, bb, dwX, dwY);
	int fx, fy;
	FindBmpBuf(x1, y1, x2, y2, fx, fy, dwX, dwY, bm, lpBufbmp, bb, lpBufscr);
	*fxx = fx;
	*fyy = fy;
	TRACE(L"图片坐标:%d,%d", fx, fy);
	HANDLE hp = GetProcessHeap();
	HeapFree(hp, 0, lpBufscr);
	HeapFree(hp, 0, lpBufbmp);
	return 0;
	if (fx > 0)return 1;
	return 0;

}

extern "C" HWND SJ_API  SZGetMousePointWindow()
{
	POINT point;
	GetCursorPos(&point);
	HWND hwnd = WindowFromPoint(point);
	return hwnd;
}
extern "C" int SJ_API  SZSetTimerCall(HWND hwnd, LPCSTR lpszASM, PTimerInfo pTimerInfo)
{
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	if (!SZPrivilgeEscalation())
	{
		TRACE(L"提权失败");
		return 0;
	}
	if (!pTimerInfo->dwInterval)
	{
		pTimerInfo->dwInterval = 1000;
	}
	if (!pTimerInfo->dwTimerId)
	{
		pTimerInfo->dwTimerId = 1000;
	}
	if (!pTimerInfo->dwProcSize)
	{
		pTimerInfo->dwProcSize = allocSize;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	PVOID pProc = VirtualAllocEx(hProcess, NULL, pTimerInfo->dwProcSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	pTimerInfo->dwProcAddr = (DWORD)pProc;
	PVOID pSetTimerProc = VirtualAllocEx(hProcess, NULL, allocSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	char procBytes[allocSize];
	char setTimerBytes[allocSize];
	int nProcSize;
	int nSetTimerProcSize;
	char setTimerAsm[100];
	char procAsm[1024];
	//::SetTimer(this->m_hWnd,0x2000,1000,TimerProc2);
	sprintf(setTimerAsm, "push 0x%x|push 0x%x|push 0x%x|push 0x%x|call 0x%x|ret 4",
		pTimerInfo->dwProcAddr, pTimerInfo->dwInterval, pTimerInfo->dwTimerId, hwnd, ::SetTimer);
	sprintf(procAsm, "push eax|push ecx|push edx|push ebx|push ebp|push esi|push edi|%s|pop edi|pop esi|pop ebp|pop ebx|pop edx|pop ecx|pop eax|ret 10", lpszASM);
	SZAsmToBytes((DWORD)pProc, procAsm, procBytes, &nProcSize);
	SZAsmToBytes((DWORD)pSetTimerProc, setTimerAsm, setTimerBytes, &nSetTimerProcSize);
	WriteProcessMemory(hProcess, pProc, procBytes, nProcSize, NULL);
	WriteProcessMemory(hProcess, pSetTimerProc, setTimerBytes, nSetTimerProcSize, NULL);
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pSetTimerProc, NULL, 0, NULL);
	if (!hRemoteThread)
	{
		TRACE(L"CreateRemoteThread失败");
		return FALSE;
	}
	WaitForSingleObject(hRemoteThread, INFINITE);
	DWORD ret;
	GetExitCodeThread(hRemoteThread, &ret);
	CloseHandle(hRemoteThread);
	VirtualFreeEx(hProcess, pSetTimerProc, allocSize, MEM_DECOMMIT);
	CloseHandle(hProcess);
	return ret;
}
extern "C" int SJ_API  SZKillTimerCall(HWND hwnd, PTimerInfo pTimerInfo)
{
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	if (!SZPrivilgeEscalation())
	{
		TRACE(L"提权失败");
		return 0;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	PVOID pKillTimerProc = VirtualAllocEx(hProcess, NULL, allocSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	char killTimerBytes[allocSize];
	int nkillTimerProcSize;
	char killTimerAsm[100];
	sprintf(killTimerAsm, "push 0x%x|push 0x%x|call 0x%x|ret 4", pTimerInfo->dwTimerId, hwnd, ::KillTimer);
	SZAsmToBytes((DWORD)pKillTimerProc, killTimerAsm, killTimerBytes, &nkillTimerProcSize);
	WriteProcessMemory(hProcess, pKillTimerProc, killTimerBytes, nkillTimerProcSize, NULL);
	HANDLE hRemoteThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pKillTimerProc, NULL, 0, NULL);
	if (!hRemoteThread)
	{
		TRACE(L"CreateRemoteThread失败");
		return FALSE;
	}
	WaitForSingleObject(hRemoteThread, INFINITE);
	DWORD ret;
	GetExitCodeThread(hRemoteThread, &ret);
	CloseHandle(hRemoteThread);
	VirtualFreeEx(hProcess, pKillTimerProc, allocSize, MEM_DECOMMIT);
	if (pTimerInfo->dwProcAddr)
	{
		VirtualFreeEx(hProcess, (PVOID)pTimerInfo->dwProcAddr, pTimerInfo->dwProcSize, MEM_DECOMMIT);
	}
	CloseHandle(hProcess);
	return ret;
}
#define  SleeptimerId 13122

extern "C" int SJ_API  SZSetTimerSleep(HWND hwnd, DWORD dwInterval, DWORD dwSleep)
{
	TimerInfo ti = { 0 };
	ti.dwInterval = dwInterval;
	ti.dwTimerId = SleeptimerId;
	char asmcode[100];
	sprintf(asmcode, "push 0x%x|call 0x%x", dwSleep, ::Sleep);
	return SZSetTimerCall(hwnd, asmcode, &ti);
}
extern "C" int SJ_API  SZKillTimerSleep(HWND hwnd)
{
	TimerInfo ti = { 0 };
	ti.dwTimerId = SleeptimerId;
	return SZKillTimerCall(hwnd, &ti);
}
extern "C" PVOID SJ_API  SZAllocMemory(HWND hwnd, DWORD dwSize)
{
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	if (!SZPrivilgeEscalation())
	{
		TRACE(L"提权失败");
		return 0;
	}
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	PVOID pret = VirtualAllocEx(hProcess, NULL, dwSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	CloseHandle(hProcess);
	return pret;
}

extern "C" HWND SJ_API  SZGetTopLevelParentHandle(HWND hwnd)
{
	//CWnd* win = CWnd::FromHandle(hwnd);
	//return win->GetTopLevelParent()->GetSafeHwnd();
	return hwnd;
}
extern "C" int SJ_API  SZTopLevelParentMoveWindow(HWND hwnd, int x, int y)
{
	hwnd = SZGetTopLevelParentHandle(hwnd);
	return SZMoveWindow(hwnd, x, y);
}

extern "C" BOOL SJ_API  SZInputString(LPCSTR str)
{
	LPCSTR temp = str;
	while (*temp)
	{
		if (*temp >= '0' && *temp <= '9')
		{
			SZKeyPress(*temp);
		}
		else if (*temp >= 'a' && *temp <= 'z')
		{
			if (LOBYTE(GetKeyState(VK_CAPITAL)))
			{
				SZKeyDown(VK_SHIFT);
				SZKeyPress(temp[0] - 'a' + 'A');
				SZKeyUp(VK_SHIFT);
			}
			else
			{
				SZKeyPress(temp[0] - 'a' + 'A');
			}

		}
		else if (*temp >= 'A' && *temp <= 'Z')
		{
			if (!LOBYTE(GetKeyState(VK_CAPITAL)))
			{
				SZKeyDown(VK_SHIFT);
				SZKeyPress(*temp);
				SZKeyUp(VK_SHIFT);
			}
			else
			{
				SZKeyPress(*temp);
			}

		}
		temp++;
	}
	return TRUE;
}
extern "C" BOOL SJ_API  SZSetWindowSize(HWND hwnd, int x, int y)
{
	return ::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, x, y, SWP_NOMOVE | SWP_NOOWNERZORDER);
}
//从矩形x1,y1,x2,y2内查找与位图lpszName条件匹配的起始点
//若有匹配的,(fx,fy)为图片第一次出现的左上角顶点,并返回匹配个数
//忽略位图backcolor颜色点，颜色偏差errorcolor，允许不匹配点的个数errorcount
extern "C" int SJ_API  SZFindBmpEx(int x1, int y1, int x2, int y2, LPCSTR lpszName, int backcolor, int errorcolor, int errorcount, int& fx, int& fy)
{

	// 	TRACE(L"参数x1=%d,y1=%d,x2=%d,y2=%d:%s",x1,y1,x2,y2,AnsiToUnicode( lpszName));
	// 	BITMAP bm;
	// 	BITMAP bb;
	// 	char * lpBufbmp;
	// 	char *lpBufscr;
	// 	DWORD dwX,dwY;
	// 	lpBufbmp=GetBmpData(lpszName,bm);
	// 	lpBufscr=GetScreenData(x2,y2,bb,dwX,dwY);
	// 	int fx,fy;
	// 	FindBmpBuf(x1,y1,x2,y2,fx,fy,dwX,dwY,bm,lpBufbmp,bb,lpBufscr);
	// 	*fxx=fx;
	// 	*fyy=fy;
	// 	TRACE(L"图片坐标:%d,%d",fx,fy);
	// 	HANDLE hp=GetProcessHeap();
	// 	HeapFree(hp,0,lpBufscr);
	// 	HeapFree(hp,0,lpBufbmp);
	// 	return 0;
	// 	if(fx>0)return 1;
	// 	return 0;

	BITMAP bm;
	BITMAP bb;
	DWORD dwX, dwY;

	char* lpBufbmp = GetBmpData(lpszName, bm);
	char* lpBufscr = GetScreenData(x2, y2, bb, dwX, dwY);
	int x, y, color = -1;
	for (y = 0; y < bm.bmHeight - 1; y++)
	{
		for (x = 0; x < bm.bmWidth - 1; x++)
			if (GetBmpPixel(x, y, dwX, dwY, bm, lpBufbmp) != errorcount)
			{
				color = GetBmpPixel(x, y, dwX, dwY, bm, lpBufbmp);
				break;
			}
		if (color > 0)break;
	}

	Points* head, * tail, * p, * q = NULL;
	head = tail = NULL;
	int xtmp = x1, countP = 0, countB = 0;
	p = (struct Points*)malloc(sizeof(struct Points));
	for (; y1 < y2; y1++)
		for (x1 = xtmp; x1 <= x2; x1++)
			if (GetScrPixel(x1, y1, dwX, dwY, bb, lpBufscr) == color)
			{
				countP++;
				p = (struct Points*)malloc(sizeof(struct Points));
				(p->pts).x = x1 - x;
				(p->pts).y = y1 - y;
				if (head == NULL)q = head = tail = p;
				else
					tail = tail->next;
				tail->next = p;
			}
	p->next = NULL;
	while (q)
	{
		if (CompareEx((q->pts), backcolor, errorcolor, errorcount, bm, bb, dwX, dwY, lpBufbmp, lpBufscr))
		{
			countB++;
			if (countB == 1)
			{
				fx = (q->pts).x;
				fy = (q->pts).y;
			}
		}
		p = q->next;
		free(q);
		q = p;
	}

	HANDLE hp = GetProcessHeap();
	HeapFree(hp, 0, lpBufscr);
	HeapFree(hp, 0, lpBufbmp);
	return countB;
}

//读取字符串
extern "C" int SJ_API  SZReadString(HWND hwnd, PVOID nAddr, LPSTR strBuff, int size)
{
	DWORD nRet = 0;
	DWORD nPid;
	GetWindowThreadProcessId(hwnd, &nPid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, nPid);
	ReadProcessMemory(hProcess, (LPCVOID)nAddr, &nRet, size, NULL);
	CloseHandle(hProcess);
	return nRet;
}
