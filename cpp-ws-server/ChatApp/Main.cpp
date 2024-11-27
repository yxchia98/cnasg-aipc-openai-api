// ---------------------------------------------------------------------
// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause
// ---------------------------------------------------------------------
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "ChatApp.hpp"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <thread>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

//------------------------------------------------------------------------------
constexpr const int c_chat_separater_length = 80;

void ChatSplit(bool end_line = true)
{
    std::string split_line(c_chat_separater_length, '-');
    std::cout << "\n" << split_line;
    if (end_line)
    {
        std::cout << "\n";
    }
}

// Echoes back all received WebSocket messages
void
do_session(tcp::socket socket, App::ChatApp& app)
{
    try
    {
        // Construct the stream by moving in the socket
        websocket::stream<tcp::socket> ws{ std::move(socket) };

        // Set a decorator to change the Server of the handshake
        ws.set_option(websocket::stream_base::decorator(
            [](websocket::response_type& res)
            {
                res.set(http::field::server,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                    "snapdragon-websocket-server-sync");
            }));

        // Accept the websocket handshake
        ws.accept();

        for (;;)
        {
            // This buffer will hold the incoming message
            beast::flat_buffer buffer;
            std::string model_response;

            // Read a message
            ws.read(buffer);

            std::string received_message = beast::buffers_to_string(buffer.data());
            ChatSplit();
            std::cout << "PROMPT:" << beast::make_printable(buffer.data()) << std::endl;

            if (received_message == "exit")
            {
                model_response = app.ChatWithUserOnce("exit");
            }
            else
            {
                model_response = app.ChatWithUserOnce(received_message);
            }

            // Prepend "SERVER" to the received message
            std::string response_message = model_response;

            // Clear the buffer and store the modified message
            buffer.consume(buffer.size());
            buffer.commit(net::buffer_copy(buffer.prepare(response_message.size()),
                net::buffer(response_message)));

            // Echo the message back
            ws.text(ws.got_text());
            ws.write(buffer.data());

            std::cout << "RESPONSE:" << beast::make_printable(buffer.data()) << std::endl;
        }
    }
    catch (beast::system_error const& se)
    {
        // This indicates that the session was closed
        if (se.code() != websocket::error::closed)
            std::cerr << "Error: " << se.code().message() << std::endl;
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}

//------------------------------------------------------------------------------

namespace
{

constexpr const std::string_view c_option_genie_config = "--genie-config";
constexpr const std::string_view c_option_base_dir = "--base-dir";
constexpr const std::string_view c_option_help = "--help";
constexpr const std::string_view c_option_help_short = "-h";
constexpr const std::string_view c_option_address = "--address";
constexpr const std::string_view c_option_port = "--port";

void PrintHelp()
{
    std::cout << "\n:::::::: Chat with " << App::c_bot_name << " options ::::::::\n\n";
    std::cout << c_option_genie_config << " <Local file path>: [Required] Path to local Genie config for model.\n";
    std::cout << c_option_base_dir
              << " <Local directory path>: [Required] Base directory to set as the working directory.\n";
    std::cout << c_option_address << " [Required] Address to host websocket server (e.g. 0.0.0.0).\n";
    std::cout << c_option_port << " [Required] Port to host websocket server (e.g. 8000).\n";
    std::cout << "\nDuring chat with " << App::c_bot_name << ", please type " << App::c_exit_prompt
              << " as a prompt to terminate chat.\n ";
}

void PrintWelcomeMessage()
{
    std::cout << "\n:::::::: Welcome to Chat with " << App::c_bot_name << " ::::::::\n ";
    std::cout << App::c_bot_name << " will use provided configuration file for conversation.\n";
    std::cout << "At any time during chat, please type `" << App::c_exit_prompt
              << "` to terminate the conversation.\n\n";
    std::cout << "Let's begin with an introduction,\n";
    std::cout << "I'm `" << App::c_bot_name << "`! What's your name? ";
}

} // namespace

int main(int argc, char* argv[])
{
    std::string genie_config_path;
    std::string base_dir;
    std::string config;
    std::string ws_address;
    std::string ws_port;
    std::string ws_threads;
    int ws_address_argv_idx = 0;
    int ws_port_argv_idx = 0;
    int ws_threads_argv_idx = 0;

    bool invalid_arguments = false;

    // Check if argument file path is accessible
    auto check_arg_path = [&invalid_arguments](const std::string_view& arg_name, const std::string& value)
    {
        if (value.empty() || !std::filesystem::exists(value))
        {
            std::cout << "\nInvalid argument for " << arg_name
                      << ": It must be a valid and accessible local path. Provided: " << value << std::endl;
            invalid_arguments = true;
        }
    };

    // Arg parser
    for (int i = 1; i < argc; ++i)
    {
        if (c_option_genie_config == argv[i])
        {
            if (i + 1 < argc)
            {
                genie_config_path = argv[++i];
                check_arg_path(c_option_genie_config, genie_config_path);
            }
            else
            {
                std::cout << "\nMissing value for " << c_option_genie_config << " option.\n";
                invalid_arguments = true;
            }
        }
        else if (c_option_base_dir == argv[i])
        {
            if (i + 1 < argc)
            {
                base_dir = argv[++i];
                check_arg_path(c_option_base_dir, base_dir);
            }
            else
            {
                std::cout << "\nMissing value for " << c_option_base_dir << " option.\n";
                invalid_arguments = true;
            }
        }
        else if (c_option_help == argv[i] || c_option_help_short == argv[i])
        {
            PrintHelp();
            return 0;
        }
        else if (c_option_address == argv[i])
        {
            if (i + 1 < argc)
            {
                ws_address = argv[++i];
                ws_address_argv_idx = i;
            }
            else
            {
                std::cout << "\nMissing value for " << c_option_address << " option.\n";
                invalid_arguments = true;
            }
        }
        else if (c_option_port == argv[i])
        {
            if (i + 1 < argc)
            {
                ws_port = argv[++i];
                ws_port_argv_idx = i;
            }
            else
            {
                std::cout << "\nMissing value for " << c_option_port << " option.\n";
                invalid_arguments = true;
            }
        }
        else
        {
            std::cout << "Unsupported option " << argv[i] << " provided.\n";
            invalid_arguments = true;
        }
    }

    // If invalid arguments or required arguments are missing, print help and exit.
    if (invalid_arguments || genie_config_path.empty() || base_dir.empty() || argc < 9)
    {
        PrintHelp();
        return 1;
    }

    try
    {
        auto const address = net::ip::make_address(argv[ws_address_argv_idx]);
        auto const port = static_cast<unsigned short>(std::atoi(argv[ws_port_argv_idx]));

        // Load genie_config_path into std::string config before changing directory
        std::ifstream config_file(genie_config_path);
        if (!config_file)
        {
            throw std::runtime_error("Failed to open Genie config file: " + genie_config_path);
        }

        config.assign((std::istreambuf_iterator<char>(config_file)), std::istreambuf_iterator<char>());

        std::filesystem::current_path(base_dir);

        std::string user_name = "USER";

        App::ChatApp app(config);

        // The io_context is required for all I/O
        net::io_context ioc{ 1 };

        // The acceptor receives incoming connections
        tcp::acceptor acceptor{ ioc, {address, port} };
        for (;;)
        {
            // This will receive the new connection
            tcp::socket socket{ ioc };

            // Block until we get a connection
            acceptor.accept(socket);

            // Launch the session, transferring ownership of the socket
            std::thread(
                &do_session,
                std::move(socket),
                std::ref(app)).detach();
        }


        // Load genie_config_path into std::string config before changing directory
        //std::ifstream config_file(genie_config_path);
        //if (!config_file)
        //{
        //    throw std::runtime_error("Failed to open Genie config file: " + genie_config_path);
        //}

        //config.assign((std::istreambuf_iterator<char>(config_file)), std::istreambuf_iterator<char>());


        //std::filesystem::current_path(base_dir);

        //std::string user_name = "USER";

        //App::ChatApp app(config);

        // Get user name to chat with
        //PrintWelcomeMessage();
        //std::getline(std::cin, user_name);

        // Interactive chat
        //app.ChatWithUser(user_name);
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    catch (...)
    {
        std::cerr << "Unknown error.\n";
        return 1;
    }
    return 0;
}
