from openai import OpenAI

# init client and connect to localhost server
client = OpenAI(
    api_key="fake-api-key",
    base_url="http://localhost:8000" # change the default port if needed
)

# call API
chat_completion = client.chat.completions.create(
    messages=[
        {
            "role": "user",
            "content": "Say this is a test",
        }
    ],
    model="gpt-1337-turbo-pro-max",
)

# SCENARIO 1: Non-streamining chat completion
# print the top "choice" 
print(chat_completion.choices[0].message.content)


stream = client.chat.completions.create(
    model="mock-gpt-model",
    messages=[{"role": "user", "content": "Say this is a test"}],
    stream=True,
)

# SCENARIO 2: Streamining chat completion
# print the stream
for chunk in stream:
    print(chunk.choices[0].delta.content or "")