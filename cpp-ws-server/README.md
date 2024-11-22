# C++ Websocket Server

## Step 1: Install Boost:Beast from [https://www.boost.org/doc/libs/1_86_0/more/getting_started/unix-variants.html](https://www.boost.org/doc/libs/1_86_0/more/getting_started/unix-variants.html)

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

## Step 4: Build binaries for C++ Websocket Server
```
cd cpp-ws-server
g++ -I ./boost_1_86_0 -o websocket_server_async websocket_server_async.cpp
```

## Step 5: Start C++ Websocket Server
```
./websocket_server_async 0.0.0.0 8001 1
```
Test Websocket Server using python scripts provided in `../python-ws-server` 