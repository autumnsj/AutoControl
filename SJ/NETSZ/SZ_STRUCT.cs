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
        [StructLayout(LayoutKind.Sequential)]
        public struct RECT
        {
            public int left;
            public int top;
            public int right;
            public int bottom;

        }
        [StructLayout(LayoutKind.Sequential)]
        public struct MainTreadHookInfo
        {
            public int dwHhookAddr;//句柄
            public int dwMsgProcAddr;//hook消息处理地址
            public int dwCodeAddr;//代码地址
            public int dwRetAddr;//代码地址
        }
        public struct WindowInfo
        {
            public IntPtr hWnd;
            public string szWindowName;
            public string szClassName;
        }
        [StructLayout(LayoutKind.Sequential)]
        public struct TimerInfo
        {
            public int dwTimerId;
            public int dwProcAddr;
            public int dwProcSize;
            public int dwInterval;
        } 
    
    }
}
