from openai import OpenAI

import httpx

httpx_client = httpx.Client(verify=False)

# init client and connect to localhost server
# client = OpenAI(
#     api_key="<API-KEY-HERE>",
#     http_client=httpx_client,
#     base_url="http://localhost:8000" # change the default port if needed
# )

client = OpenAI(
    api_key="<API-KEY-HERE>",
    http_client=httpx_client,
    base_url="https://203.126.124.228" # change the default port if needed
)

# call API
chat_completion = client.chat.completions.create(
    messages=[
        {
            "role": "user",
            "content": "What is Dell Technologies?",
        }
    ],
    model="snapdragon-sealion-v2-1",
    max_tokens = 100,
    temperature = 0.1,
)

# Non-streaming chat completion
print(chat_completion.choices[0].message.content)