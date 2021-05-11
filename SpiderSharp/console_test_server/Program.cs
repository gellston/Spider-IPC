using System;
using SPIDER;


namespace console_test_server
{
    class Program
    {
        static void Main(string[] args)
        {

            SPIDER.SPIDER_ACCESS access = SPIDER_ACCESS.READ_WRITE;



            // SPIDER.Variable<int> variable = new Variable<int>("test", SPIDER_MODE.CREATE, SPIDER_ACCESS.READ_WRITE);
            while (true)
            {
                try
                {
                   

                    SPIDER.Function function = new SPIDER.Function("test");

                    function.Delay(1).Call();

                }
                catch(Exception e)
                {
                    System.Console.WriteLine(e.Message);

                }

            }
                

        }
    }
}
