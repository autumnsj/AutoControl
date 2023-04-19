using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.IO;
using System.Xml.Serialization;
using System.Runtime.InteropServices;
using System.Diagnostics;
using Microsoft.Win32;
using System.Reflection;
using Newtonsoft.Json;
namespace NETSZ
{
   
    public partial class SZ
    {
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZFindWindow(string lpClassName, string lpWindowName);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZFindWindowByProcess(string lpProcessName, string lpClassName, string lpWindowName);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZGetWindowRect(int hwnd,ref RECT rect);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZMoveTo(int x, int y);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZLeftClick();
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZMoveWindow(int hwnd, int x, int y);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZSetWindowState(int hwnd, int flag);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZKeyPress(int vk);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZCapture(int x1,int y1,int x2,int y2,  string file);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZSendString(int hwnd, string str);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZGetModuleBaseAddr(int hwnd, string module_name);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZPrivilgeEscalation();
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZAsmToBytes(int addr, string lpAsmCode, byte[] lpByteBuff, out int pByteBuffSize);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZAsmCall(int hwnd,
                                string asmCode,
                                int mode,
                                out int ret,//返回值
                                ref MainTreadHookInfo lpMainTreadHookInfo //如果是主线程代码申请内存空间地址，
                                );
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZGetMainThreadId(int idProcess);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZGetAddr(int hwnd, string lpszAddr);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern float SZReadFloat(int hwnd, string lpszAddr);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZReadInt(int hwnd, string lpszAddr, int type);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern double SZReadDouble(int hwnd, string lpszAddr);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZReadIntAddr(int hwnd, int nAddr, int type);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZWriteData(int hwnd, string lpszAddr, string data);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZFindData(int hwnd, string lpszData, int dwStartAddr, int dwEndAddr);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZFindBmp(int x1, int y1, int x2, int y2, string lpszName, out int fxx, out int fyy);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int  SZRightClick();
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZLeftDown();
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZLeftUp();
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZRightDown();
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZRightUp();
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZKeyDown(int vk);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZKeyUp(int vk);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZSetTimerCall(int hwnd, string lpszASM, TimerInfo pTimerInfo);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZKillTimerCall(int  hwnd, TimerInfo pTimerInfo);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZSetTimerSleep(int hwnd, int dwInterval, int dwSleep);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZKillTimerSleep(int hwnd);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZInputString(string str);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZAllocMemory(int hwnd, int dwSize);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZGetTopLevelParentHandle(int hwnd);
        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZTopLevelParentMoveWindow(int hwnd, int x, int y);
         [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SZSetWindowSize(int hwnd, int x, int y);
        public SZ()
        {


        }
        MainTreadHookInfo mainTreadHookInfo = new MainTreadHookInfo();
        public  int GetTopLevelParentHandle(int hwnd, int x, int y)
        {
            return SZGetTopLevelParentHandle(hwnd);
        }
        public  int TopLevelParentMoveWindow(int hwnd, int x, int y)
        {
            return SZTopLevelParentMoveWindow(hwnd, x, y);
        }
        public int AllocMemory(int hwnd, int dwSize)
        {
            return SZAllocMemory(hwnd,dwSize);
        }
        public int FindWindow(string lpClassName, string lpWindowName)
        {

            return SZFindWindow(lpClassName, lpWindowName);
        }

        public int FindWindowByProcess(string lpProcessName, string lpClassName, string lpWindowName)
        {
            return SZFindWindowByProcess(lpProcessName, lpClassName, lpWindowName);
        }

        public int GetWindowRect(int hwnd, out object x1, out object y1, out object x2, out object y2)
        {
            RECT rect = new RECT();
            SZGetWindowRect(hwnd,ref rect);
            x1 = rect.left; y1 = rect.top; x2 = rect.right; y2 = rect.bottom;
            return 1;
        }
        public int MoveTo(int x, int y)
        {
            return SZMoveTo(x, y);
        }

        public int LeftClick()
        {
            return SZLeftClick();
        }

        public int MoveWindow(int hwnd, int x, int y)
        {
            return SZMoveWindow(hwnd, x, y);
        }

        public int SetWindowState(int hwnd, int flag)
        {

            return SZSetWindowState(hwnd, flag);
        }

        public int KeyPress(int vk)
        {
            return SZKeyPress(vk);
        }

        public int Capture(int x1, int y1, int x2, int y2, string file)
        {
            return SZCapture(x1,y1,x2,y2, file);
        }

        public int SendString(int hwnd, string str)
        {
            return SZSendString(hwnd, str);
        }

        public int GetModuleBaseAddr(int hwnd, string module_name)
        {
            return SZGetModuleBaseAddr(hwnd, module_name);
        }

        public int PrivilgeEscalation()
        {
            return SZPrivilgeEscalation();
        }

        public int AsmToBytes(int addr, string lpAsmCode, byte[] lpByteBuff, out int pByteBuffSize)
        {
            return SZAsmToBytes(addr, lpAsmCode, lpByteBuff, out  pByteBuffSize);
        }

        public int AsmCall(int hwnd,
                                string asmCode,
                                int mode,
                                out int ret
                                )
        {
            return SZAsmCall(hwnd,
                                 asmCode,
                                 mode,
                                out  ret,//返回值
                                ref  mainTreadHookInfo //如果是主线程代码申请内存空间地址，
                                );
        }
        public int AsmCall(int hwnd, int mode)
        {
            switch (mode)
            {
                case 2:
                    mode = 0;
                    break;
                case 3:
                    mode = 1;
                    break;
                default:
                    mode = 0;
                    break;
            }
            StringBuilder str = new StringBuilder();
            foreach (var item in asmList)
            {
                str.Append(item + "|");
            }
            string asmstring = str.ToString();

            asmstring = asmstring.Substring(0, asmstring.Length - 1);
            int ret;
            SZAsmCall(hwnd, asmstring, mode, out  ret, ref  mainTreadHookInfo);
            return ret;
        }

        public int GetMainThreadId(int idProcess)
        {
            return SZGetMainThreadId(idProcess);
        }

        public int GetAddr(int hwnd, string lpszAddr)
        {
            return SZGetAddr(hwnd, lpszAddr);
        }

        public float ReadFloat(int hwnd, string lpszAddr)
        {
            return SZReadFloat(hwnd, lpszAddr);
        }

        public int ReadInt(int hwnd, string lpszAddr, int type)
        {
            return SZReadInt(hwnd, lpszAddr, type);
        }

        public double ReadDouble(int hwnd, string lpszAddr)
        {
            return SZReadDouble(hwnd, lpszAddr);
        }

        public int ReadIntAddr(int hwnd, int nAddr, int type)
        {
            return SZReadIntAddr(hwnd, nAddr, type);
        }

        public int WriteData(int hwnd, string lpszAddr, string data)
        {
            return SZWriteData(hwnd, lpszAddr, data);
        }

        public int FindData(int hwnd, string lpszData, int dwStartAddr, int dwEndAddr)
        {
            return SZFindData(hwnd, lpszData, dwStartAddr, dwEndAddr);
        }
        public string FindData(int hwnd, string addr_range, string lpszData)
        {
            int dwStartAddr, dwEndAddr;
            dwStartAddr = addr_range.Split('-')[0].ToHexInt();
            dwEndAddr = addr_range.Split('-')[1].ToHexInt();
            return SZFindData(hwnd, lpszData, dwStartAddr, dwEndAddr).ToHexString();
        }

        public int FindPic(int x1, int y1, int x2, int y2, string pic_name, string delta_color, double sim, int dir, out object x, out object y)
        {

            int xx, yy;
            int ret = SZFindBmp(x1, y1, x2, y2, pic_name, out xx, out yy);
            x = xx; y = yy;
            return ret;
        }
        
        List<string> asmList = new List<string>();
        /// <summary>
        /// 为了兼容大漠，不推荐使用
        /// </summary>
        /// <returns></returns>
        public int AsmClear()
        {
            asmList.Clear();
            return 1;
        }
        /// <summary>
        /// 为了兼容大漠，不推荐使用
        /// </summary>
        /// <param name="addr"></param>
        /// <returns></returns>
        public string AsmCode(int addr)
        {
            byte[] buf = new byte[20];
            int bufSize;
            AsmToBytes(addr, asmList[0], buf, out bufSize);
            StringBuilder str = new StringBuilder();
            for (int i = 0; i < bufSize; i++)
            {
                str.Append(string.Format("{0:X2}", buf[i]));
            }
            return str.ToString();
        }
        /// <summary>
        /// 为了兼容大漠，不推荐使用
        /// </summary>
        /// <param name="asmCode"></param>
        /// <returns></returns>
        public int AsmAdd(string asmCode)
        {
            asmList.Add(asmCode);
            return 1;
        }
        public string FindDataEx(int hwndZ, string addr, string data, int int1, int int2, int int3)
        {
            return SZFindData(hwndZ, data, addr.Split('-')[0].ToHexInt(), addr.Split('-')[1].ToHexInt()).ToHexString();
        }
        //return dm.ReadString(hwndZ,string.Format("[[{0:X}+10]+bc]",edx),1,0);
        public string ReadString(int hwnd, string addr, int type, int len = 100)
        {
            int nAddr = GetAddr(hwnd, addr);
            // int 
            return CP_ReadStringByWindowHandle(hwnd, nAddr, type, len);
        }
        public int RightClick()
        {
            return SZRightClick();
        }
        public int Reg(string ss, string sd)
        {
            return 1;
        }
        public int ForceUnBindWindow(int ddd)
        {
            return 1;
        }
        public int BindWindowEx(int dd, string kk, string kks, string asdf, string kkl, int llkd)
        {
            return 1;
        }
        public int UnBindWindow()
        {
            return 1;
        }
        public int RightDown()
        {
            return SZRightDown();
        }
        public string GetColor(int x, int y)
        {
            return "";
        }
        public int RightUp()
        {
            return SZRightUp();
        }
        public int WheelDown()
        {
            return 1;
        }
        public int KeyDown(int key)
        {
            return SZKeyDown(key);
        }
        public int KeyUp(int key)
        {
            return SZKeyUp(key);
        }
        public int LeftUp()
        {
            return SZLeftUp();
        }
        public int LeftDown()
        {
            return SZLeftDown();
        }
        public int WheelUp()
        {
            return 1;
        }
        public bool SetWindowSize(int hwnd, int cx, int cy)
        {
            return SZSetWindowSize(hwnd,cx,cy);
        }
        [StructLayout(LayoutKind.Sequential)]//定义与API相兼容结构体，实际上是一种内存转换  
        public struct POINTAPI
        {
            public int X;
            public int Y;
        }
        public   int SetTimerCall(int hwnd, string lpszASM, TimerInfo pTimerInfo)
        {
            return SZSetTimerCall(hwnd, lpszASM, pTimerInfo);
        }
      
        public   int KillTimerCall(int  hwnd, TimerInfo pTimerInfo)
        {
            return SZKillTimerCall(hwnd, pTimerInfo);
        }

        public   int SetTimerSleep(int hwnd, int dwInterval, int dwSleep)
        {
            return SZSetTimerSleep(hwnd, dwInterval, dwSleep);
        }

        public   int KillTimerSleep(int hwnd)
        {
            return SZKillTimerSleep(hwnd);
        }
       
        public   int InputString(string str)
        {
            return SZInputString(str);
        }
    
    }
}
