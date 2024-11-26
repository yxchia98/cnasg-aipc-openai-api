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
and go grab a coffee

## Step 2: Pull and start GCC container
```
docker run --rm --gpus all --name=cpp-env -v $PWD:$PWD -w $PWD --network=host -itd gcc:14.2.0
```
```
docker exec -it cpp-env bash
```

## Step 3: Update Packages
```
apt-get update
```
```
apt-get install build-essential gdb
```

## Step 4: Move `genie_bundle` to `ChatApp/` folder
Move the compiled `genie_bundle` along with the binaries, tokenizers, HTP configs, etc.\
Refer to `/ai-hub-apps/apps/windows/cpp/ChatApp` from root of this repo for more info.

## Step 5: Build binaries for C++ Websocket Server
```
cd cpp-ws-server
g++ -I ./boost_1_86_0 -o websocket_server_async websocket_server_async.cpp
```

## Step 6: Start C++ Websocket Server
```
./websocket_server_async 0.0.0.0 8001 1
```
Test Websocket Server using python scripts provided in `../python-ws-server` 