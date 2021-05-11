using System;
using System.Diagnostics;
using SPIDER;


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

            Random random = new Random();

           
            int count = 0;
            DateTime start = DateTime.Now;
            while (true)
            {
                try
                {
                    count++;
                    int argument1 = random.Next(0, 255);
                    int argument2 = random.Next(0, 255);
                    int returnValue = 0;

                    notifier.Args()
                            .Push<int>("argument1", argument1)
                            .Push<int>("argument2", argument2);


                    notifier.Call();


                    notifier.Returns()
                            .Get<int>("returnValue", ref returnValue);


                    DateTime end = DateTime.Now;
                    TimeSpan timeDiff = end - start;

                    //System.Console.WriteLine("time = " + count);
                    System.Console.WriteLine("result" + returnValue);
                    if (timeDiff.TotalMilliseconds > 100)
                    {
                        start = DateTime.Now;
                        System.Console.WriteLine("FPS:" + count);
                        count = 0;

                    }
                }
                catch(Exception e)
                {
                    System.Console.WriteLine(e.Message);

                }

            }
                

        }
    }
}
