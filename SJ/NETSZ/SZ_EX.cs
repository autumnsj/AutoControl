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
using Win32;
namespace NETSZ
{
    public static partial class SZ
    {
        public static int GetMousePointWindow()
        {
            Win32.POINT point = new POINT();//必须用与之相兼容的结构体，类也可以  
            User.GetCursorPos(out point);//获取当前鼠标坐标
            int hwnd = User.WindowFromPoint(point.x, point.y);//获取指定坐标处窗口的句柄  
            return hwnd;
        }
        public static string GetWindowClass(int hwnd)
        {
            StringBuilder className =new StringBuilder(100);
            IntPtr temp = new IntPtr(hwnd);
            User.GetClassName(temp, className, 100);
            return className.ToString();
        }
        public static int RunApp(string exePath, int mode)
        {
            System.Diagnostics.ProcessStartInfo info = new System.Diagnostics.ProcessStartInfo(exePath);
            info.WindowStyle = ProcessWindowStyle.Normal;
            info.WorkingDirectory = Path.GetDirectoryName(exePath); System.Diagnostics.Process.Start(info);
            return 1;
        }
        public static string RunCmd(string strCmd)
        {
            string rInfo;
            try
            {
                Process myProcess = new Process();
                ProcessStartInfo myProcessStartInfo = new ProcessStartInfo("cmd.exe");
                myProcessStartInfo.UseShellExecute = false;
                myProcessStartInfo.CreateNoWindow = true;
                myProcessStartInfo.RedirectStandardOutput = true;
                myProcess.StartInfo = myProcessStartInfo;
                myProcessStartInfo.Arguments = "/c " + strCmd;
                myProcess.Start();
                StreamReader myStreamReader = myProcess.StandardOutput;
                rInfo = myStreamReader.ReadToEnd();
                myProcess.Close();
                rInfo = strCmd + "\r\n" + rInfo;
                return rInfo;
            }
            catch (Exception ex)
            {
                return ex.Message;
            }
        }
        private delegate bool WNDENUMPROC(IntPtr hWnd, int lParam);
        [DllImport("user32.dll")]
        private static extern bool EnumWindows(WNDENUMPROC lpEnumFunc, int lParam);
        public static WindowInfo[] CP_GetAllDesktopWindows()
        {
            List<WindowInfo> wndList = new List<WindowInfo>();
       
            //enum all desktop windows
            EnumWindows(delegate(IntPtr hWnd, int lParam)
            {
                WindowInfo wnd = new WindowInfo();
                StringBuilder sb = new StringBuilder(256);
                //get hwnd
                wnd.hWnd = hWnd;
                //get window name
                User.GetWindowText(hWnd, sb, sb.Capacity);
                wnd.szWindowName = sb.ToString();
                //get window class
                User.GetClassName(hWnd, sb, sb.Capacity);
                wnd.szClassName = sb.ToString();
                //add it into list
                wndList.Add(wnd);
                return true;
            }, 0);
            return wndList.ToArray();
        }

        public static bool CP_CheckWindowHandle(int hwnd)
        {
            WindowInfo[] allwindow = CP_GetAllDesktopWindows();

            foreach (var item in allwindow)
            {
                if (item.hWnd.ToInt32() == hwnd)
                {
                    return true;
                }
            }
            return false;
        }

        /// <summary>
        /// 读取字符串
        /// </summary>
        /// <param name="handle">进程句柄</param>
        /// <param name="address">地址</param>
        /// <param name="type">类型,0 : ASCII字符串,1 : Unicode字符串</param>
        /// <param name="len">长度</param>
        /// <returns></returns>
        public static string CP_ReadString(int handle, int address, int type, int len = 100)
        {
            IntPtr ret = Marshal.AllocHGlobal(len);
            try
            {
                int temp = 0;
                Win32.Kernel.ReadProcessMemory(new IntPtr(handle), new IntPtr(address), ret, len,ref temp);
            }
            catch (Exception ex)
            {
                return "CP_ReadString错误:" + ex.Message;
            }
            string result = string.Empty;
            switch (type)
            {
                case 0:
                    result=Marshal.PtrToStringAnsi(ret);
                    break;
                case 1:
                    result=Marshal.PtrToStringUni(ret);
                    break;
            }
            Marshal.FreeHGlobal(ret);
            return result;
        }
        public static string CP_ReadStringByWindowHandle(int hwnd, int address, int type, int len = 100)
        {
            int pid = 0;
            Win32.User.GetWindowThreadProcessId(new IntPtr( hwnd), ref pid);
            int handle = Win32.Kernel.OpenProcess(0x0010 | 0x0020, 0, pid);
            string result = CP_ReadString(handle, address, type, len);
            Kernel.CloseHandle(handle);
            return result;
        } 

        public static int GetProcAddress(string lpModuleName,string lpProcName)
        {
            int temp = Win32.Kernel.GetModuleHandle(lpModuleName);
            return Win32.Kernel.GetProcAddress(new IntPtr(temp), lpProcName);
        }
        public static float CP_ReadFloat(int handle,int address)
        {
            IntPtr result = Marshal.AllocHGlobal(4);
            int read = 0;
            Win32.Kernel.ReadProcessMemory(new IntPtr(handle), new IntPtr(address), result, 4,ref read);
            float temp = Marshal.ReadInt32(result).ToFloatP();
            Marshal.FreeHGlobal(result);
            return temp;
        }
        public static float CP_ReadFloatByHwnd(int hwnd,int address)
        {

            int pid = 0;
            Win32.User.GetWindowThreadProcessId(new IntPtr(hwnd), ref pid);
            int handle = Win32.Kernel.OpenProcess(0x0010 | 0x0020, 0, pid);

            IntPtr result = Marshal.AllocHGlobal(4);
            int read = 0;
            Win32.Kernel.ReadProcessMemory(new IntPtr(handle), new IntPtr(address), result, 4,ref read);
            float temp = Marshal.ReadInt32(result).ToFloatP();
            Marshal.FreeHGlobal(result);
            Win32.Kernel.CloseHandle(handle);
            return temp;
        }
        
    }
}
