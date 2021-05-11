using System;

namespace console_test_client
{
    class Program
    {
        static void Main(string[] args)
        {
            SPIDER.Function notifier = new SPIDER.Function("test", (instance)=>
            {
                try
                {
                    int argument1 = 0;
                    int argument2 = 0;

                    instance.Args()
                            .Get<int>("argument1", ref argument1)
                            .Get<int>("argument2", ref argument2);

                    int result = 5;

                    instance.Returns()
                            .Push<int>("returnValue", result);


                }catch(Exception e)
                {
                    System.Console.WriteLine(e.Message);
                }

            });




            while (true)
            {
                System.Threading.Thread.Sleep(1000);
            }

        }
    }
}
