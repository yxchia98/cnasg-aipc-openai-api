# C++ Websocket Server

## Step 1: Install Boost:Beast v1.86.0 from [https://www.boost.org/doc/libs/1_86_0/more/getting_started/unix-variants.html](https://www.boost.org/doc/libs/1_86_0/more/getting_started/unix-variants.html)
1. Extract them to this folder `e.g. boost_1_86_0/`
2. Open Developer Command Prompt For VS 2022 and cd into extracted boost directory
3. Issue the command 
```
bootstrap vc143
```
4. Next issue the command 
```
.\b2
```
and go grab a coffee \
5. Go to Project properties → C/C++ → General → Additional Include Directories, and add a path to the boost library root (in my case C:\Program Files (x86)\Boost_1_53). \
Include a .hpp file in your sources, like #include <boost/lexical_cast/lexical_cast_old.hpp> \

## Step 2: Build Binaries
Click on the green button after opening the `ChatApp.sln` file, to build the binaries and test the application

## Step 3: Run Binaries
Go to `./cpp-ws-server/ChatApp/` and run `.\ARM64\Debug\ChatApp.exe`, parsing in arguments for config files, address, and port.
```
cd {Project directory}
```
```
.\ARM64\Debug\ChatApp.exe --genie-config .\\genie_bundle\\genie_config.json --base-dir .\\genie_bundle\\ --address 0.0.0.0 --port 8001
```