using System;
using SPIDER;
/// <summary>
/// Server Side
/// </summary>
namespace console_test_server
{
    class Program
    {
        static void Main(string[] args)
        {
            SPIDER.Function notifier = new SPIDER.Function("test");
            notifier.Args()
                    .Arg<int>("argument1")
                    .Arg<int>("argument2")
                    .Returns()
                    .Ret<int>("returnValue")
                    .Complete();
            while (true)
            {
                try
                {
                    int argument1 = 11;
                    int argument2 = 11;
                    int returnValue = 0;
                    notifier.Args()
                            .Push<int>("argument1", argument1)
                            .Push<int>("argument2", argument2);

                    notifier.Call();

                    notifier.Returns()
                            .Get<int>("returnValue", out returnValue);
                }
                catch(Exception e)
                {
                    System.Console.WriteLine(e.Message);
                }
            }
        }
    }
}
