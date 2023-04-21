using NETSZ;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.IO.Compression;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace TestExe
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            
        }
        IniFile ini = new IniFile("./kkk/ffff/kkk.ini");
        private void button1_Click(object sender, EventArgs e)
        {
            
           // List<Point> ret = BmpColor.FindPicScreen(628, 432, 777,532, @"C:\Users\Administrator\Desktop\自由对战取消.bmp", 20);
            this.pictureBox1.Image= BmpColor.CopyScreen(0, 0, 1000, 1000);
  
        }
        public byte[] Decompress(byte[] data)
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
        private void button2_Click(object sender, EventArgs e)
        {
            SZ.FindScreenPic("1.bmp");
           
        }

        private void button3_Click(object sender, EventArgs e)
        {

        }
    }
}
