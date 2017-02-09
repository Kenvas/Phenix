using System;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;
using System.Collections.Generic;

namespace EnumExtracter
{
    class Program
    {
        static void Main(string[] args)
        {
            string[] cmdargs = Environment.GetCommandLineArgs();
            if (cmdargs.Length <= 2)
            {
                var filename = "./test.txt";
                if (File.Exists(filename))
                {
                    ExtractEnum(filename, "WM");
                }
                else
                {
                    var procname = Path.GetFileNameWithoutExtension(cmdargs[0]);
                    Console.WriteLine("usage: {0} <filename> <enum_prefix>", procname);
                }
                return;
            }
        }

        private static void ExtractEnum(string filename, string prefix)
        {
            var text = File.ReadAllText(filename);
            var pattern = string.Format(@"#define\s*({0}_\w*)\s*(0x[0-9a-fA-F]*)", prefix);
            var regexpr = new Regex(pattern);
            var matches = regexpr.Matches(text);
            var dictionary = new Dictionary<int, string>(matches.Count);
            foreach (Match match in matches)
            {
                var groups = match.Groups;
                var name = groups[1].Value;
                var value = Convert.ToInt32(groups[2].Value, 16);
                //Console.WriteLine("name = {0,-28}, value = 0x{1:X4}", name, value);
                dictionary[value] = name;
            }
            GenerateCppCode(filename, dictionary);
        }

        private static void GenerateCppCode(string filename, Dictionary<int, string> dictionary)
        {
            var sb = new StringBuilder();
            sb.AppendLine("char const * const GetWindowMessageName(UINT message)");
            sb.AppendLine("{");
            sb.AppendLine("    switch (message)");
            sb.AppendLine("    {");
            foreach (var p in dictionary)
            {
                sb.AppendFormat("    case 0x{0:X4}: return \"{1}\";\n", p.Key, p.Value);
            }
            sb.AppendFormat("    default: return \"\";\n");
            sb.AppendLine("    }");
            sb.AppendLine("}");
            var newname = Path.ChangeExtension(filename, ".h");
            File.WriteAllText(newname, sb.ToString());
            sb.Length = 0;
        }
    }
}
