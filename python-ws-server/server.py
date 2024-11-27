import asyncio
import json
import time

import websockets
import sys

from typing import Optional, List

from pydantic import BaseModel, Field

from starlette.responses import StreamingResponse
from fastapi import FastAPI, HTTPException, Request

from uvicorn import Config, Server
import uvicorn


QUALCOMM_HOST: str = 'ws://localhost:8001/'
INTEL_HOST: str = 'ws://localhost:8002/'

app = FastAPI(title="OpenAI-compatible API")

main_loop = asyncio.new_event_loop()

# data models
class Message(BaseModel):
    role: str
    content: str


class ChatCompletionRequest(BaseModel):
    model: Optional[str] = "snapdragon-sealion-v2-1"
    messages: List[Message]
    max_tokens: Optional[int] = 512
    temperature: Optional[float] = 0.1
    stream: Optional[bool] = False


# async def _resp_async_generator(text_resp: str):
#     # let's pretend every word is a token and return it over time
#     tokens = text_resp.split(" ")

#     for i, token in enumerate(tokens):
#         chunk = {
#             "id": i,
#             "object": "chat.completion.chunk",
#             "created": time.time(),
#             "model": request.model,
#             "choices": [{"delta": {"content": token + " "}}],
#         }
#         yield f"data: {json.dumps(chunk)}\n\n"
#         await asyncio.sleep(1)
#     yield "data: [DONE]\n\n"

# async websocket inference 
async def inference(host: str, prompt: str):
    async with websockets.connect(host) as websocket:
        await websocket.send(prompt)
        response = await websocket.recv()
        return response

@app.post("/chat/completions")
async def chat_completions(request: ChatCompletionRequest):
    host = ''

    if request.model == "snapdragon-sealion-v2-1":
        host = QUALCOMM_HOST
        print(f"using Qualcomm AI PC: {host}")
    if request.model == "lunarlake-sealion-v2-1":
        host = INTEL_HOST
        print(f"using Intel AI PC: {host}")

    if request.messages:
        # resp_content = (
        #     "As a mock AI Assitant, I can only echo your last message: "
        #     + request.messages[-1].content
        # )

        prompt = request.messages[-1].content

        # Get loop and do inference
        # asyncio.set_event_loop(loop)
        try:
            # response = loop.run_until_complete(inference(host, prompt))
            print(f"PROMPT:\n{prompt}")
            response = await inference(host, prompt)
            print(f"RESPONSE:\n{response}")

        except KeyboardInterrupt:
            pass
        # resp_content = response
        resp_content = response

    else:
        resp_content = "As an AI Assitant, I will need prompts, but there wasn't one!"
    if request.stream:
        resp_content = "Sorry, I only support non-streaming outputs!"
        # return StreamingResponse(
        #     _resp_async_generator(resp_content), media_type="application/x-ndjson"
        # )

    #TODO : send prompt to AI PC via websocket or socket , we need implement that
    return {
        "id": "1337",
        "object": "chat.completion",
        "created": time.time(),
        "model": request.model,
        "choices": [{"message": Message(role="assistant", content=resp_content)}],
    }

@app.post("/aipc/register")
async def aipc_register(req:Request, aipc_type: str, support_model:str):
    print(req.headers.get("authorization"))
    #TODO: create and maintain a socket connection between AIPC and api server. Then server forward prompt to correct ai pc
    #ai pc will inital the connection and listen the incoming prompt , and reponse back to api server
    return {

    }


if __name__ == "__main__":

    uvicorn.run(app, host="0.0.0.0", port=8000)
    # config = Config(app=app, loop=main_loop)
    # server = Server(config)
    # main_loop.create_task(server.serve())
    # main_loop.run_forever()
    # main_loop.run_until_complete(server.serve())