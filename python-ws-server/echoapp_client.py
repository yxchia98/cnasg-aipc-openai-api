import sys
import time
from threading import Thread

import websockets


def on_message(ws, message):
    print(f"Received: {message}")


def on_error(ws, error):
    print(error)


def on_close(ws, close_status_code, close_msg):
    print("### closed ###")


def on_open(ws):
    def run(*args):
        for i in range(3):
            # send the message, then wait
            # so thread doesn't exit and socket
            # isn't closed
            await ws.send("Hello %d" % i)

        for i in range(3):
            # send the message, then wait
            # so thread doesn't exit and socket
            # isn't closed
            await ws.send("Hello %d" % i)
        ws.send("exit")
        time.sleep(1)
        # ws.close()
        print("Thread terminating...")

    Thread(target=run).start()


if __name__ == "__main__":
    websocket.enableTrace(True)
    if len(sys.argv) < 2:
        host = "ws://localhost:8001/"
    else:
        host = sys.argv[1]
    ws = websocket.WebSocketApp(
        host, on_message=on_message, on_error=on_error, on_close=on_close
    )
    ws.on_open = on_open
    ws.run_forever()