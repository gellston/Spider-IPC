[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Ask Me Anything !](https://img.shields.io/badge/Ask%20me-anything-1abc9c.svg)](https://github.com/gellston/gellston/issues)
[![Generic badge](https://img.shields.io/badge/<Build>-<Pass>-<COLOR>.svg)](https://shields.io/)

# Spider-IPC
SPIDER-IPC is a C++/C# library that provides inter-process communication using shared memory on Windows <br>
This library provides remote function calls and variable sharing functions

# Build
This library was developed and modified with Visual Studio 2019 <br>

# Download
[Download C++ v120-x86 version ](https://github.com/gellston/Spider-IPC/raw/main/lib/SpiderNative-x86-v120.zip)<br>
[Download C++ v120-x64 version](https://github.com/gellston/Spider-IPC/raw/main/lib/SpiderNative-x64-v120.zip) <br>
[Download C++ v142-x64-windows10 version](https://github.com/gellston/Spider-IPC/raw/main/lib/SpiderNative-x64-v142-windows10.zip) <br>
[Download C# NetCore3.1-x64-windows10 version](https://github.com/gellston/Spider-IPC/raw/main/lib/SpiderSharp-x64-NetCore3.1-windows10.zip) <br>


# Getting Started 
Start with [C++](https://github.com/gellston/Spider-IPC/tree/main/SpiderNative) and [C#](https://github.com/gellston/Spider-IPC/tree/main/SpiderSharp) examples <br>



# C++ Basic Example

```cpp
/// <summary>
/// Server Side
/// </summary>
///
int main() {

	spider::function notifier("test");
	notifier
		.args()
		.arg<int>("argument1")
		.arg<int>("argument2")
		.returns()
		.ret<int>("returnValue")
		.complete();
	while (true) {
		
		try {
			int argument1 = 11;
			int argument2 = 11;
			int returnValue = 0;

			notifier
				.args()
				.push<int>("argument1", argument1)
				.push<int>("argument2", argument2);

			notifier();

			notifier
				.returns()
				.get("returnValue", &returnValue);
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}
	}
	return 0;
}

/// <summary>
/// Client Side
/// </summary>
/// 
int main() {

	spider::function notifier("test", [&](spider::function* function) {
		try {
			int argument1 = 0;
			int argument2 = 0;
			function->args()
				.get("argument1", &argument1)
				.get("argument2", &argument2);
			int result = argument1 + argument2;
			function->returns()
				.push("returnValue", result);
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}

	});
	notifier
		.args()
		.arg<int>("argument1")
		.arg<int>("argument2")
		.returns()
		.ret<int>("returnValue")
		.complete();

	while (true) {
		getchar();
	}
	return 0;
}
```

# C# Basic Example 
```csharp
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
/// <summary>
/// Client Side
/// </summary>
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

```


# License 
Licensed under the MIT License.
