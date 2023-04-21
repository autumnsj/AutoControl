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
using System.Diagnostics.Eventing.Reader;
using Win32;
using System.Drawing.Imaging;
using System.Windows.Forms;
using OpenCvSharp;

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

        [DllImport("SJ.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SZFindBmpEx(int x1, int y1, int x2, int y2, string lpszName, int backcolor, int errorcolor, int errorcount, out object fx, out object fy);

        public static int FindBmpEx(int x1, int y1, int x2, int y2, string lpszName, int backcolor, int errorcolor, int errorcount, out object fx, out object fy)
        {
            return SZFindBmpEx( x1,  y1,  x2,  y2,  lpszName,  backcolor,  errorcolor,  errorcount, out  fx, out  fy);
        }

        static  MainTreadHookInfo  mainTreadHookInfo = new MainTreadHookInfo();
     
        public static int GetWindowRect(int hwnd, out object x1, out object y1, out object x2, out object y2)
        {
            RECT rect = new RECT();
            SZGetWindowRect(hwnd,ref rect);
            x1 = rect.left; y1 = rect.top; x2 = rect.right; y2 = rect.bottom;
            return 1;
        }
        public static int MoveTo(int x, int y)
        {
            return SZMoveTo(x, y);
        }

        public static int LeftClick()
        {
            return SZLeftClick();
        }

        public static int MoveWindow(int hwnd, int x, int y)
        {
            return SZMoveWindow(hwnd, x, y);
        }

        public static int SetWindowState(int hwnd, int flag)
        {

            return SZSetWindowState(hwnd, flag);
        }

        public static int KeyPress(int vk)
        {
            return SZKeyPress(vk);
        }

        public static int Capture(int x1, int y1, int x2, int y2, string file)
        {
            return SZCapture(x1,y1,x2,y2, file);
        }

        public static int SendString(int hwnd, string str)
        {
            return SZSendString(hwnd, str);
        }

        public static int GetModuleBaseAddr(int hwnd, string module_name)
        {
            return SZGetModuleBaseAddr(hwnd, module_name);
        }

        public static int PrivilgeEscalation()
        {
            return SZPrivilgeEscalation();
        }

        public static int AsmToBytes(int addr, string lpAsmCode, byte[] lpByteBuff, out int pByteBuffSize)
        {
            return SZAsmToBytes(addr, lpAsmCode, lpByteBuff, out  pByteBuffSize);
        }

        public static int AsmCall(int hwnd,
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
        public static int AsmCall(int hwnd, int mode)
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

        public static int GetMainThreadId(int idProcess)
        {
            return SZGetMainThreadId(idProcess);
        }

        public static int GetAddr(int hwnd, string lpszAddr)
        {
            return SZGetAddr(hwnd, lpszAddr);
        }

        public static float ReadFloat(int hwnd, string lpszAddr)
        {
            return SZReadFloat(hwnd, lpszAddr);
        }

        public static int ReadInt(int hwnd, string lpszAddr, int type)
        {
            return SZReadInt(hwnd, lpszAddr, type);
        }

        public static double ReadDouble(int hwnd, string lpszAddr)
        {
            return  SZReadDouble(hwnd, lpszAddr);
        }

        public static int ReadIntAddr(int hwnd, int nAddr, int type)
        {
            return SZReadIntAddr(hwnd, nAddr, type);
        }

        public static int WriteData(int hwnd, string lpszAddr, string data)
        {
            return SZWriteData(hwnd, lpszAddr, data);
        }

        public static int FindData(int hwnd, string lpszData, int dwStartAddr, int dwEndAddr)
        {
            return SZFindData(hwnd, lpszData, dwStartAddr, dwEndAddr);
        }
        public static string FindData(int hwnd, string addr_range, string lpszData)
        {
            int dwStartAddr, dwEndAddr;
            dwStartAddr = addr_range.Split('-')[0].ToHexInt();
            dwEndAddr = addr_range.Split('-')[1].ToHexInt();
            return SZFindData(hwnd, lpszData, dwStartAddr, dwEndAddr).ToHexString();
        }

        
        static List<string>  asmList = new List<string>();
        /// <summary>
        /// 为了兼容大漠，不推荐使用
        /// </summary>
        /// <returns></returns>
        public static int AsmClear()
        {
            asmList.Clear();
            return 1;
        }
        /// <summary>
        /// 为了兼容大漠，不推荐使用
        /// </summary>
        /// <param name="addr"></param>
        /// <returns></returns>
        public static string AsmCode(int addr)
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
        public static int AsmAdd(string asmCode)
        {
            asmList.Add(asmCode);
            return 1;
        }
        public static string FindDataEx(int hwndZ, string addr, string data, int int1, int int2, int int3)
        {
            return SZFindData(hwndZ, data, addr.Split('-')[0].ToHexInt(), addr.Split('-')[1].ToHexInt()).ToHexString();
        }
        //return dm.ReadString(hwndZ,string.Format("[[{0:X}+10]+bc]",edx),1,0);
        public static string ReadString(int hwnd, string addr, int type, int len = 100)
        {
            int nAddr = GetAddr(hwnd, addr);
            // int 
            return CP_ReadStringByWindowHandle(hwnd, nAddr, type, len);
        }
        public static int RightClick()
        {
            return SZRightClick();
        }
        public static int Reg(string ss, string sd)
        {
            return 1;
        }
        public static int ForceUnBindWindow(int ddd)
        {
            return 1;
        }
        public static int BindWindowEx(int dd, string kk, string kks, string asdf, string kkl, int llkd)
        {
            return 1;
        }
        public static int UnBindWindow()
        {
            return 1;
        }
        public static int RightDown()
        {
            return SZRightDown();
        }
        public static string GetColor(int x, int y)
        {
            return "";
        }
        public static int RightUp()
        {
            return SZRightUp();
        }
        public static int WheelDown()
        {
            return 1;
        }
        public static int KeyDown(int key)
        {
            return SZKeyDown(key);
        }
        public static int KeyUp(int key)
        {
            return SZKeyUp(key);
        }
        public static int LeftUp()
        {
            return SZLeftUp();
        }
        public static int LeftDown()
        {
            return SZLeftDown();
        }
        public static int WheelUp()
        {
            return 1;
        }
        public static bool SetWindowSize(int hwnd, int cx, int cy)
        {
            return SZSetWindowSize(hwnd,cx,cy);
        }
        [StructLayout(LayoutKind.Sequential)]//定义与API相兼容结构体，实际上是一种内存转换  
        public struct POINTAPI
        {
            public int X;
            public int Y;
        }
        public static int SetTimerCall(int hwnd, string lpszASM, TimerInfo pTimerInfo)
        {
            return SZSetTimerCall(hwnd, lpszASM, pTimerInfo);
        }
      
        public static int KillTimerCall(int  hwnd, TimerInfo pTimerInfo)
        {
            return SZKillTimerCall(hwnd, pTimerInfo);
        }

        public static int SetTimerSleep(int hwnd, int dwInterval, int dwSleep)
        {
            return SZSetTimerSleep(hwnd, dwInterval, dwSleep);
        }

        public static int KillTimerSleep(int hwnd)
        {
            return SZKillTimerSleep(hwnd);
        }
       
        public static int InputString(string str)
        {
            return SZInputString(str);
        }


        /// <summary>
        /// 图片转成字节
        /// </summary>
        /// <param name="img"></param>
        /// <returns></returns>
        public static byte[] ImageToBytes(Image img)
        {
            MemoryStream ms = new MemoryStream();
            byte[] imagedata = null;
            img.Save(ms, ImageFormat.Bmp);
            imagedata = ms.GetBuffer();
            return imagedata;
        }

        /// <summary>
        /// 字节转换成图片
        /// </summary>
        /// <param name="bytes"></param>
        /// <returns></returns>
        public static Image BytesToImage(byte[] bytes)
        {
            MemoryStream ms = new MemoryStream(bytes);
            Image img = Image.FromStream(ms);
            return img;
        }
        public static Bitmap GetScreenCapture()
        {
            Rectangle tScreenRect = new Rectangle(0, 0, Screen.PrimaryScreen.Bounds.Width, Screen.PrimaryScreen.Bounds.Height);
            Bitmap tSrcBmp = new Bitmap(tScreenRect.Width, tScreenRect.Height); // 用于屏幕原始图片保存
            Graphics gp = Graphics.FromImage(tSrcBmp);
            gp.CopyFromScreen(0, 0, 0, 0, tScreenRect.Size);
            gp.DrawImage(tSrcBmp, 0, 0, tScreenRect, GraphicsUnit.Pixel);
            return tSrcBmp;
        }
        public static Rectangle FindScreenPic(string path, double threshold =1)
        {
            Image matchImage = Image.FromFile(path);
            Image sourceImage = GetScreenCapture();
            return FindPic(sourceImage, matchImage, threshold);
        }
        public static Rectangle FindPic(Image sourceImage, Image matchImage, double threshold =1)
        {
            var refMat = Mat.FromImageData(ImageToBytes(sourceImage), ImreadModes.AnyColor);//大图
            var tplMat = Mat.FromImageData(ImageToBytes(matchImage), ImreadModes.AnyColor);//小图
            using (Mat res = new Mat(refMat.Rows - tplMat.Rows + 1, refMat.Cols - tplMat.Cols + 1, MatType.CV_32FC1))
            {
                Mat gref = refMat.CvtColor(ColorConversionCodes.BGR2GRAY);
                Mat gtpl = tplMat.CvtColor(ColorConversionCodes.BGR2GRAY);

                Cv2.MatchTemplate(gref, gtpl, res, TemplateMatchModes.CCoeffNormed);
                Cv2.Threshold(res, res, 0.8, 1.0, ThresholdTypes.Tozero);

                double minval, maxval;
                OpenCvSharp.Point minloc, maxloc;

                Cv2.MinMaxLoc(res, out minval, out maxval, out minloc, out maxloc);

                if (maxval >= threshold)
                {
                    return new Rectangle(maxloc.X, maxloc.Y, tplMat.Width, tplMat.Height);
                }
                return Rectangle.Empty;
            }
        }
    }
}
