import asyncio
import websockets
import sys

async def inference(host, prompt):
    async with websockets.connect(host) as websocket:
        await websocket.send(prompt)
        response = await websocket.recv()
        return response


if __name__ == "__main__":
    host = ''
    if len(sys.argv) < 2:
        host = "ws://localhost:8001/"
    else:
        host = sys.argv[1]
    loop = asyncio.new_event_loop()
    asyncio.set_event_loop(loop)
    try:
        print(loop.run_until_complete(inference(host, "Hello! How are you?")))
        print(loop.run_until_complete(inference(host, "What is Dell Technologies?")))
        print(loop.run_until_complete(inference(host, "exit")))
    except KeyboardInterrupt:
        pass

    # res1 = asyncio.get_event_loop().run_until_complete(inference(host, "Hello! How are you?"))
    # print(res1)
    # res2 = asyncio.get_event_loop().run_until_complete(inference(host, "exit"))
    # print(res2)