// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 SJ_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// SJ_API extern "C" 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifdef SJ_EXPORTS
#define  SJ_API   __declspec(dllexport)
#else
#define  SJ_API  __declspec(dllimport)
#endif

//#define  __declspec(dll)
//#define  
#define WINDOW_TEXT_LENGTH 256    
///////////////////////////////非导出模块START///////////////////////////////////////////
//定义一个点的链表
struct Points
{
	POINT pts;
	struct Points* next;
};
typedef struct
{
	HWND hWnd;
	DWORD dwPid;
	CHAR sCaption[WINDOW_TEXT_LENGTH];
	CHAR sClass[WINDOW_TEXT_LENGTH];
	BOOL success;
}WNDINFO, * PWNDINFO;
typedef struct
{
	DWORD dwHhookAddr;//句柄
	DWORD dwMsgProcAddr;//hook消息处理地址
	DWORD dwCodeAddr;//代码地址
	DWORD dwRetAddr;//返回值
} MainTreadHookInfo, * PMainTreadHookInfo;
typedef struct
{
	DWORD dwTimerId;//ID
	DWORD dwProcAddr;//地址
	DWORD dwProcSize;//申请内存大小
	DWORD dwInterval;//调用频率
} TimerInfo, * PTimerInfo;
void GetAbsolutePoints(int nX_i, int nY_i, POINT* point);
wchar_t* AnsiToUnicode(const char* str);
void Alert(LPCSTR msg);
BOOL  CALLBACK EnumWindowCallBack(HWND hWnd, LPARAM lParam);
//对比内存
BOOL SZMemcmp(
	const char* lpData,//对比源
	const char* lpFeatureBytes,//特征码 00为通配码
	DWORD dwSize //对比长度
);
char* GetBmpData(LPCSTR lpszName, BITMAP& bm);
//从数据char *lpBufscr中查找*lpBufbmp;
int FindBmpBuf(int x1, int y1, int x2, int y2, int& fx, int& fy, DWORD dwX, DWORD dwY, BITMAP& bm, char* lpBufbmp, BITMAP& bb, char* lpBufscr);
/**在屏幕中找与图片第一个点相同颜色的坐标**/
POINT FindFirstPoint(int x1, int y1, int x2, int y2, DWORD dwX, DWORD dwY, BITMAP& bm, char* lpBufbmp, BITMAP& bb, char* lpBufscr);
char* GetScreenData(int dx, int dy, BITMAP& bb, DWORD& dwX, DWORD& dwY);
/**获得图片指定位置的颜色值**/
int GetBmpPixel(int x, int y, DWORD dwX, DWORD dwY, BITMAP& bm, char* lpBufbmp);
/**获得屏幕指定位置的颜色值**/
int GetScrPixel(int x, int y, DWORD dwX, DWORD dwY, BITMAP& bb, char* lpBufscr);
//比较屏幕pt点位图矩形大小的所有颜色和位置是否与位图相同
int Compare(POINT pt, BITMAP& bm, char* lpBufbmp, BITMAP& bb, char* lpBufscr, DWORD dwX, DWORD dwY);
int CompareEx(POINT pt, int backcolor, int errorcolor, int errorcount, BITMAP& bm, BITMAP& bb, DWORD dwX, DWORD dwY, char* lpBufbmp, char* lpBufscr); //扩展去除一些颜色
///////////////////////////////非导出模块END///////////////////////////////////////////
extern "C" HWND SJ_API   SZFindWindow(LPCSTR lpClassName, LPCSTR lpWindowName);
extern "C" HWND SJ_API   SZFindWindowByProcess(LPCSTR lpProcessName, LPCSTR lpClassName, LPCSTR lpWindowName);
extern "C" int SJ_API   SZGetWindowRect(HWND hwnd, RECT* rect);
//移动鼠标
extern "C" int SJ_API   SZMoveTo(int x, int y);
//鼠标左键
extern "C" int SJ_API   SZLeftClick();
//移动窗口
extern "C" int SJ_API   SZMoveWindow(HWND hwnd, int x, int y);
//设置窗口状态
extern "C" int SJ_API   SZSetWindowState(
	HWND hwnd,
	int flag//1:激活指定窗口 8:置顶指定窗口 9:取消置顶指定窗口 15:使指定的窗口获取输入焦点
);
//按键单击
extern "C" int SJ_API   SZKeyPress(int vk);
//屏幕截图
extern "C" int SJ_API   SZCapture(int x1, int y1, int x2, int y2,
	LPCSTR file //文件存放路径
);
//发送向指定窗口发送字符串
extern "C" int SJ_API   SZSendString(HWND hwnd, LPCSTR str);
//获取指定模块基址
extern "C" int SJ_API   SZGetModuleBaseAddr(HWND hwnd, LPCSTR module_name);
//提权
extern "C" BOOL  SJ_API  SZPrivilgeEscalation();
//汇编转字节
extern "C" int  SJ_API  SZAsmToBytes(
	DWORD addr, //地址
	LPCSTR lpAsmCode,//汇编代码
	char* lpByteBuff, //输出字节buff
	int* pByteBuffSize);//机器码长度

extern "C" int SJ_API   SZAsmCall(HWND hwnd,
	LPCSTR asmCode,
	int mode,
	DWORD* ret,//返回值
	PMainTreadHookInfo lpMainTreadHookInfo //如果是主线程代码申请内存空间地址，
);
//获取主线程ID
extern "C" DWORD SJ_API   SZGetMainThreadId(
	DWORD idProcess//进程ID
);
//从指定地址中取个四字节
extern "C" DWORD SJ_API   SZGetAddr(HWND hwnd, LPCSTR lpszAddr);
//从指定地址中取个float
extern "C" float SJ_API   SZReadFloat(HWND hwnd, LPCSTR lpszAddr);
//从指定地址中取个int
extern "C" DWORD SJ_API   SZReadInt(
	HWND hwnd,
	LPCSTR lpszAddr,
	int type //0:32位有符号,1:16 位有符号 ,2:8位有符号,3:16位无符号,4:8位无符号
);
//从指定地址中取个double
extern "C" double SJ_API   SZReadDouble(HWND hwnd, LPCSTR lpszAddr);
//从指定地址中取个int
extern "C" DWORD SJ_API   SZReadIntAddr(
	HWND hwnd,
	int nAddr, //地址
	int type  //0:32位有符号,1:16 位有符号 ,2:8位有符号,3:16位无符号,4:8位无符号
);
//写内存
extern "C" int SJ_API    SZWriteData(HWND hwnd, LPCSTR lpszAddr, LPCSTR data);
//找特征码
extern "C" DWORD SJ_API   SZFindData(HWND hwnd, LPCSTR lpszData, DWORD dwStartAddr, DWORD dwEndAddr);
//找图
extern "C" int SJ_API   SZFindBmp(int x1, int y1, int x2, int y2, LPCSTR lpszName, int* fx, int* fy);
//取得鼠标指向的窗口句柄
extern "C" HWND SJ_API   SZGetMousePointWindow();
//鼠标右键
extern "C" int SJ_API   SZRightClick();
//鼠标左键按下
extern "C" int SJ_API   SZLeftDown();
//鼠标左键弹起
extern "C" int SJ_API   SZLeftUp();
//鼠标右键起按下
extern "C" int SJ_API   SZRightDown();
//鼠标右键弹起
extern "C" int SJ_API   SZRightUp();
//某键按下
extern "C" int SJ_API   SZKeyDown(int vk);
//某键弹起
extern "C" int SJ_API   SZKeyUp(int vk);
//定时调用自定义汇编
extern "C" int SJ_API  SZSetTimerCall(HWND hwnd, LPCSTR lpszASM, PTimerInfo pTimerInfo);
//取消定时调用汇编
extern "C" int SJ_API  SZKillTimerCall(HWND hwnd, PTimerInfo pTimerInfo);
//设置定时休眠 
extern "C" int SJ_API  SZSetTimerSleep(
	HWND hwnd,//窗口句柄
	DWORD dwInterval, //休眠频率
	DWORD dwSleep	//休眠时间
);
//取消定时休眠
extern "C" int SJ_API  SZKillTimerSleep(HWND hwnd);
//申请内存,返回内存地址
extern "C" PVOID SJ_API  SZAllocMemory(
	HWND hwnd, //窗口句柄
	DWORD dwSize //大小
);
//取上级窗口句柄
extern "C" HWND SJ_API  SZGetTopLevelParentHandle(HWND hwnd);
//移动上级窗口
extern "C" int SJ_API  SZTopLevelParentMoveWindow(HWND hwnd, int x, int y);
//输入字符串
extern "C" BOOL SJ_API  SZInputString(LPCSTR str);
//设置窗口大小
extern "C" BOOL SJ_API  SZSetWindowSize(HWND hwnd, int x, int y);
//找图
extern "C" int SJ_API  SZFindBmpEx(int x1, int y1, int x2, int y2, LPCSTR lpszName, int backcolor, int errorcolor, int errorcount, int& fx, int& fy);
//读取字符串
extern "C" int SJ_API  SZReadString(HWND hwnd, PVOID nAddr, LPSTR strBuff, int size);