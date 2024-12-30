from openai import OpenAI

import httpx

httpx_client = httpx.Client(verify=False)

# init client and connect to localhost server
client = OpenAI(
    api_key="<API-KEY>",
    http_client=httpx_client,
    base_url="http://localhost:8000" # change the default port if needed
)

# client = OpenAI(
#     api_key="<API-KEY-HERE>",
#     http_client=httpx_client,
#     base_url="https://203.126.124.228" # change the default port if needed
# )

# call API
chat_completion = client.chat.completions.create(
    messages=[
        {
            "role": "user",
            "content": "Who are you?",
        }
    ],
    model="snapdragon-sealion-v3-0",
)

# Non-streaming chat completion
print(chat_completion.choices[0].message.content)