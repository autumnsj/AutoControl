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
using System.Security.Cryptography;
using System.IO.Compression;

namespace NETSZ
{
    unsafe public static class Ex
    {
        public static int ToInt(this float d)
       {
           return Convert.ToInt32(d) - (d % 1 > 0.5 ? 1 : 0);
       }
       public static int ToInt(this double d)
       {
           return Convert.ToInt32(d) - (d % 1 >0.5 ? 1 : 0);
       }
       public static int ChromaticAberration(this Color a,Color b)
       {
           return Math.Abs(a.R - b.R) + Math.Abs(a.G - b.G) + Math.Abs(a.B - b.B);
       }
       public static int ChromaticAberration(this string a, string b)
       {
           int r1 = Convert.ToInt32(a.Substring(0, 2), 16);
           int g1 = Convert.ToInt32(a.Substring(2, 2), 16);
           int b1 = Convert.ToInt32(a.Substring(4, 2), 16);
           int r2 = Convert.ToInt32(b.Substring(0, 2), 16);
           int g2 = Convert.ToInt32(b.Substring(2, 2), 16);
           int b2 = Convert.ToInt32(b.Substring(4, 2), 16);
           return Math.Abs(r1 - r2) + Math.Abs(g1 - g2) + Math.Abs(b1 - b2);
       }
      
       public static int ToIntP(this float d)
       {
           int* p = (int*)&d;
           return *p;
       }
       public static float ToFloatP(this int d)
       {
           float* p = (float*)&d;
           return *p;
       }
       public static string ToHexString(this float f)
       {
           int* p = (int*)&f;
           return string.Format("{0:X}", *p);
       }
       public static string ToHexString(this double f)
       {
           int* p = (int*)&f;
           return string.Format("{0:X}", *p);
       }
       public static string ToHexString(this int f)
       {
           return string.Format("{0:X}", f);
       }
       public static string ToHexString(this uint f)
       {
           int* p = (int*)&f;
           return string.Format("{0:X}", *p);
       }
       public static int ToHexInt(this string s)
       {
          return Convert.ToInt32(s, 16);
       }
       public static float ToFloat(this byte[] data)
       {
           float fTemp = BitConverter.ToSingle(data, 0);
           return fTemp;
       }
       public static int ToInt(this byte[] data)
       {
           int result = BitConverter.ToInt32(data, 0);
           return result;
       }
       public static string ToUnicodeString(this byte[] data)
       {
           string result = System.Text.Encoding.Unicode.GetString(data);
           return result;
       }

       /// <summary>
       /// 反序列化
       /// </summary>
       /// <param name="type">类型</param>
       /// <param name="xml">XML字符串</param>
       /// <returns></returns>
       public static T XMLDeserialize<T>(this string xml)
       {
           try
           {
               using (StringReader sr = new StringReader(xml))
               {
                   XmlSerializer xmldes = new XmlSerializer(typeof(T));
                   return (T)xmldes.Deserialize(sr);
               }
           }
           catch 
           {

               return default(T);
           }
       }
       /// <summary>
       /// 序列化
       /// </summary>
       /// <param name="type">类型</param>
       /// <param name="obj">对象</param>
       /// <returns></returns>
       public static string XMLSerializer(this object obj)
       {
           MemoryStream Stream = new MemoryStream();
           XmlSerializer xml = new XmlSerializer((obj).GetType());
           //序列化对象
           xml.Serialize(Stream, obj);
          
           Stream.Position = 0;
           StreamReader sr = new StreamReader(Stream);
           string str = sr.ReadToEnd();

           sr.Dispose();
           Stream.Dispose();

           return str;
       }

       public static string ToJSON(this object obj)
       {
           return JsonConvert.SerializeObject(obj, Formatting.Indented, new JsonSerializerSettings
           {
               ReferenceLoopHandling = ReferenceLoopHandling.Ignore
           });
       }
       public static T JsonToObj<T>(this string data)
       {
           return JsonConvert.DeserializeObject<T>(data, new JsonSerializerSettings
           {
               ReferenceLoopHandling = ReferenceLoopHandling.Ignore
           });
       }
        //strcmd="regsvr32 -s com组件";
      
       
        /// <summary>  
        /// 
        /// 检查指定CLSID的COM控件是否注册  
        /// </summary>  
        ///  <param name="clsid">COM控件的CLSID，不带大括号</param>  
        ///   <returns>是否已经注册</returns> 
        public static bool GetRegisterStatus(string clsid) 
        { 
            bool hasRegister = false;  
            RegistryKey root = RegistryKey.OpenBaseKey(RegistryHive.ClassesRoot, RegistryView.Registry32); 
           // string cld = String.Format("\\CLSID\\{0}{1}{2}", "{", clsid, "}"); 
            string cld = String.Format("{0}{1}{2}", "{", clsid, "}");
            RegistryKey comKey = root.OpenSubKey(cld); 
            hasRegister = comKey != null; 
            return hasRegister; 
        }
         /// <summary>  
         /// 检查指定CLSID的COM控件是否注册，并返回文件信息  
         /// </summary> 
         /// <param name="clsid">COM控件的CLSID，不带大括号</param>  
         /// <param name="file">文件信息</param> 
         /// <returns>是否已经注册</returns> 
        public static bool GetRegisterStatus(string clsid, out FileInfo file)
        {
            file = null;
            RegistryKey root = RegistryKey.OpenBaseKey(RegistryHive.ClassesRoot, RegistryView.Registry32);
            string cld = String.Format("\\CLSID\\{0}{1}{2}", "{", clsid, "}");
            RegistryKey comKey = root.OpenSubKey(cld);
            if (comKey == null) return false;
            RegistryKey fileKey = comKey.OpenSubKey("InprocServer32");
            if (fileKey == null) return false;
            string filename = fileKey.GetValue("").ToString();
            if (!string.IsNullOrEmpty(filename))
            {
                file = new FileInfo(filename);
            }
            return true;
        }
        /// <summary> 
        /// 根据指定的文件名创建一个运行Regsvr32的Process  
        /// </summary>  
        /// <param name="filename">文件绝对路径</param>  
        /// <param name="register">注册/反注册</param> 
        /// <returns></returns>  
        private static Process GetRegsvr32(string filename, bool register)
        {
            if (!File.Exists(filename)) { return null; }
            Process p = new Process();
            p.StartInfo.FileName = string.Format("{0}\\{1}", Environment.GetFolderPath(Environment.SpecialFolder.System),
                "regsvr32.exe");
            p.StartInfo.Arguments = string.Format("/s {0}", filename);
            if (!register)
            {
                p.StartInfo.Arguments += " /u";
            }

            return p;
        }

        public static string GetMD5(this string myString)
        {
            MD5 md5 = new MD5CryptoServiceProvider();
            byte[] fromData = System.Text.Encoding.Unicode.GetBytes(myString);
            byte[] targetData = md5.ComputeHash(fromData);
            string byte2String = null;

            for (int i = 0; i < targetData.Length; i++)
            {
                byte2String += targetData[i].ToString("x");
            }

            return byte2String;
        }
        /// 进行DES加密。
        /// </summary>
        /// <param name="pToEncrypt">要加密的字符串。</param>
        /// <param name="sKey">密钥，且必须为8位。</param>
        /// <returns>以Base64格式返回的加密字符串。</returns>
        public static string Encrypt(this string pToEncrypt, string sKey)
        {
            using (DESCryptoServiceProvider des = new DESCryptoServiceProvider())
            {
                byte[] inputByteArray = Encoding.UTF8.GetBytes(pToEncrypt);
                des.Key = ASCIIEncoding.UTF8.GetBytes(sKey);
                des.IV = ASCIIEncoding.UTF8.GetBytes(sKey);
                System.IO.MemoryStream ms = new System.IO.MemoryStream();
                using (CryptoStream cs = new CryptoStream(ms, des.CreateEncryptor(), CryptoStreamMode.Write))
                {
                    cs.Write(inputByteArray, 0, inputByteArray.Length);
                    cs.FlushFinalBlock();
                    cs.Close();
                }
                string str = Convert.ToBase64String(ms.ToArray());
                ms.Close();
                return str;
            }
        }

        /// <summary>
        /// 进行DES解密。
        /// </summary>
        /// <param name="pToDecrypt">要解密的以Base64</param>
        /// <param name="sKey">密钥，且必须为8位。</param>
        /// <returns>已解密的字符串。</returns>
        public static string Decrypt(this string pToDecrypt, string sKey)
        {
            byte[] inputByteArray = Convert.FromBase64String(pToDecrypt);
            using (DESCryptoServiceProvider des = new DESCryptoServiceProvider())
            {
                des.Key = ASCIIEncoding.UTF8.GetBytes(sKey);
                des.IV = ASCIIEncoding.UTF8.GetBytes(sKey);
                System.IO.MemoryStream ms = new System.IO.MemoryStream();
                using (CryptoStream cs = new CryptoStream(ms, des.CreateDecryptor(), CryptoStreamMode.Write))
                {
                    cs.Write(inputByteArray, 0, inputByteArray.Length);
                    cs.FlushFinalBlock();
                    cs.Close();
                }
                string str = Encoding.UTF8.GetString(ms.ToArray());
                ms.Close();
                return str;
            }
        }
        public static byte[] DeGzip(this byte[] data)
        {
            MemoryStream srcMs = new MemoryStream(data);
            GZipStream zipStream = new GZipStream(srcMs, CompressionMode.Decompress);
            MemoryStream ms = new MemoryStream();
            byte[] bytes = new byte[40960];
            int n;
            while ((n = zipStream.Read(bytes, 0, bytes.Length)) > 0)
            {
                ms.Write(bytes, 0, n);
            }
            zipStream.Close();
            return ms.ToArray();
        }
        /// <summary>  
        /// 压缩数据  
        /// </summary>  
        /// <param name="data"></param>  
        /// <returns></returns>  
        public static byte[] EnGzip(this byte[] data)
        {
            MemoryStream ms = new MemoryStream();
            GZipStream zipStream = new GZipStream(ms, CompressionMode.Compress);
            zipStream.Write(data, 0, data.Length);//将数据压缩并写到基础流中  
            zipStream.Close();
            return ms.ToArray();
        }  
      
    }
}
