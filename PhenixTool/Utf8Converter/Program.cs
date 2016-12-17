using System;
using System.IO;
using System.Text;

namespace Utf8Converter
{
    class Program
    {
        static void Main(string[] args)
        {
            string[] cmdargs = Environment.GetCommandLineArgs();
            if (cmdargs.Length <= 1)
            {
                var filename = "./test.txt";
                if (File.Exists(filename))
                {
                    ConvertToUtf8(filename);
                }
                else
                {
                    var procname = Path.GetFileNameWithoutExtension(cmdargs[0]);
                    Console.WriteLine("usage: {0} <dir>", procname);
                }
                return;
            }

            var dirpath = cmdargs[1];
            if (!Directory.Exists(dirpath))
            {
                Console.WriteLine("error: {0} not exists.", dirpath);
                return;
            }

            var filenames = Directory.GetFiles(dirpath, "*", SearchOption.AllDirectories);
            foreach (var filename in filenames)
            {
                ConvertToUtf8(filename);
            }
        }

        private static void ConvertToUtf8(string filename)
        {
            try
            {
                var info = new FileInfo(filename);
                var name = filename;
                var text = File.ReadAllText(name);
                var utf8_no_bom = new UTF8Encoding(false);
                File.WriteAllText(name, text, utf8_no_bom);
                Console.ForegroundColor = ConsoleColor.DarkCyan;
                Console.Write(name);
                Console.ResetColor();
                Console.WriteLine(" converted to utf8 done.");
            }
            catch (Exception e)
            {
                Console.WriteLine("errro: convert [{0}] fail. reason: {1}", filename, e.ToString());
            }
        }
    }
}
