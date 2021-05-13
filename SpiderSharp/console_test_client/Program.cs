using System;
using SPIDER;
namespace console_test_client
{
    class Program
    {
        static void Main(string[] args)
        {

            SPIDER.Function function = new SPIDER.Function("test", (function) =>
            {
                try
                {
                    int argument1 = 0;
                    int argument2 = 0;

                    function.Args()
                            .Get<int>("argument1", out argument1)
                            .Get<int>("argument2", out argument2);

                    int result = argument1 + argument2;

                    function.Returns()
                            .Push<int>("returnValue", result);

                }
                catch(Exception e)
                {
                    System.Console.WriteLine(e.Message);
                }
            });

            function.Args()
                .Arg<int>("argument1")
                .Arg<int>("argument2")
                .Returns()
                .Ret<int>("returnValue")
                .Complete();

            while (true)
            {
                System.Threading.Thread.Sleep(100);
            }
        }
    }
}
