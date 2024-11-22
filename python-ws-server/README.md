# cnasg-aipc-openai-api
OpenAI API Compatible server adapted for Dell AI PCs running NPUs


# Getting Started

## Install Python dependencies
```
pip install uvicorn fastapi openai websocket
```
or install from `requirements.txt`
```
pip install -r requirements.txt
```

## Serve OpenAI API Compatible Server
```
uvicorn server:app
```

## Run sample Client to test Server for `chat.completions` 
```
python client.py
```

## Run sample Client appp to test C++ Websocket server
```
python echoapp_client.py
```