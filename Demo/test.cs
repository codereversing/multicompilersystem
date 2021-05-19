using System;

namespace HelloWorld
{
    class Hello {         
        static int Main(string[] args)
        {
            System.Console.WriteLine("Hello, World! stdout");
            Console.Error.WriteLine("Hello, World! stderr");
            // System.Console.WriteLine("arg[0] = " + args[0]);
            return 0;
        }
    }
}
