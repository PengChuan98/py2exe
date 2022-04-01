# py2exe
py2exe by using python3.dll.

> ~~脑子抽抽了用`LoadLibrary`去调用`pythonx.dll`了，重写。~~

**TODO LIST :**

>- Start a separate thread to call pythonxx.dll
>- ...........

- **`studio` cannot be used after using `AttachConsole`** 

>`cmd.exe` is a application so `freopen("CONIN$", "r+t", stdin);` after using  `AttachConsole` is not completely valid!In addition, another reason for the input problem is that `cmd.exe` does not know if it needs to wait. (*WinMainCRTStartup*) Can see [c++ - Console showing when running winapi program - Stack Overflow](https://stackoverflow.com/questions/28650971/console-showing-when-running-winapi-program)
