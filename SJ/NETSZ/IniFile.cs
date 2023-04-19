using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace NETSZ
{

    /// <summary>
    /// INI文件的操作类
    /// </summary>
    public class IniFile
    {
        
        [DllImport("kernel32")]
        public static extern bool WritePrivateProfileString(byte[] section, byte[] key, byte[] val, string filePath);
        [DllImport("kernel32")]
        public static extern int GetPrivateProfileString(byte[] section, byte[] key, byte[] def, byte[] retVal, int size, string filePath);//与ini交互必须统一编码格式
        private static byte[] getBytes(string s, string encodingName) 
        { 
            return null == s ? null : Encoding.GetEncoding(encodingName).GetBytes(s); 
        }
        public static string ReadString(string section, string key, string def, string fileName, string encodingName = "utf-8", int size = 1024)
        {
            byte[] buffer = new byte[size]; int count = GetPrivateProfileString(getBytes(section, encodingName), getBytes(key, encodingName), getBytes(def, encodingName), buffer, size, fileName); return Encoding.GetEncoding(encodingName).GetString(buffer, 0, count).Trim();
        }
        public static bool WriteString(string section, string key, string value, string fileName, string encodingName = "utf-8") 
        { 
            return WritePrivateProfileString(getBytes(section, encodingName), getBytes(key, encodingName), getBytes(value, encodingName), fileName); 
        }
        public string path;
        public string encodingName;
        public IniFile(string _path = "./config.ini", string _encodingName = "utf-8")
        {
            encodingName = _encodingName;
            this.path =Path.GetFullPath(_path);
            string directoryName = Path.GetDirectoryName(this.path);
            try
            {
                if (!Directory.Exists(directoryName))
                {
                    Directory.CreateDirectory(directoryName);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message+"\r\n"+"请尝试以管理员身份运行");
            }
            
        }
  
        /// <summary>
        /// 写INI文件
        /// </summary>
        /// <param name="section">段落</param>
        /// <param name="key">键</param>
        /// <param name="iValue">值</param>
        public void IniWriteValue(string section, string key, string iValue)
        {
             WriteString(section, key, iValue, path, encodingName);
        }


        /// <summary>
        /// 读取INI文件
        /// </summary>
        /// <param name="section">段落</param>
        /// <param name="key">键</param>
        /// <returns>返回的键值</returns>
        public string IniReadValue(string section, string key, string def="",int size=1024)
        {
            return ReadString(section, key, def, path, encodingName, size);
        }

        
    }
}
