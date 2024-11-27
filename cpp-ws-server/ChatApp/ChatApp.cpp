// ---------------------------------------------------------------------
// Copyright (c) 2024 Qualcomm Innovation Center, Inc. All rights reserved.
// SPDX-License-Identifier: BSD-3-Clause
// ---------------------------------------------------------------------
#include "ChatApp.hpp"
#include "PromptHandler.hpp"
#include <fstream>
#include <iostream>

using namespace App;

namespace
{

constexpr const int c_chat_separater_length = 80;

//
// ChatSplit - Line to split during Chat for UX
// Adds split line to separate out sections in output.
//
void ChatSplit(bool end_line = true)
{
    std::string split_line(c_chat_separater_length, '-');
    std::cout << "\n" << split_line;
    if (end_line)
    {
        std::cout << "\n";
    }
}

//
// GenieCallBack - Callback to handle response from Genie
//   - Captures response from Genie into user_data
//   - Print response to stdout
//   - Add ChatSplit upon sentence completion
//
void GenieCallBack(const char* response_back, const GenieDialog_SentenceCode_t sentence_code, const void* user_data)
{
    std::string* user_data_str = static_cast<std::string*>(const_cast<void*>(user_data));
    user_data_str->append(response_back);

    // Write user response to output.
    std::cout << response_back;
    if (sentence_code == GenieDialog_SentenceCode_t::GENIE_DIALOG_SENTENCE_END)
    {
        ChatSplit(false);
    }
}

} // namespace

ChatApp::ChatApp(const std::string& config)
{
    // Create Genie config
    if (GENIE_STATUS_SUCCESS != GenieDialogConfig_createFromJson(config.c_str(), &m_config_handle))
    {
        throw std::runtime_error("Failed to create the Genie Dialog config. Please check config.");
    }

    // Create Genie dialog handle
    if (GENIE_STATUS_SUCCESS != GenieDialog_create(m_config_handle, &m_dialog_handle))
    {
        throw std::runtime_error("Failed to create the Genie Dialog.");
    }
}

ChatApp::~ChatApp()
{
    if (m_config_handle != nullptr)
    {
        if (GENIE_STATUS_SUCCESS != GenieDialogConfig_free(m_config_handle))
        {
            std::cerr << "Failed to free the Genie Dialog config.";
        }
    }

    if (m_dialog_handle != nullptr)
    {
        if (GENIE_STATUS_SUCCESS != GenieDialog_free(m_dialog_handle))
        {
            std::cerr << "Failed to free the Genie Dialog.";
        }
    }
}

void ChatApp::ChatWithUser(const std::string& user_name)
{
    AppUtils::PromptHandler prompt_handler;

    // Initiate Chat with infinite loop.
    // User to provide `exit` as a prompt to exit.
    while (true)
    {
        std::string user_prompt;
        std::string model_response;

        // Input user prompt
        ChatSplit();
        std::cout << user_name << ": ";
        std::getline(std::cin, user_prompt);

        // Exit prompt
        if (user_prompt.compare(c_exit_prompt) == 0)
        {
            std::cout << "Exiting chat as per " << user_name << "'s request.";
            return;
        }
        // User provides an empty prompt
        if (user_prompt.empty())
        {
            std::cout << "\nPlease enter prompt.\n";
            continue;
        }

        std::string tagged_prompt = prompt_handler.GetPromptWithTag(user_prompt);

        // Bot's response
        std::cout << c_bot_name << ":";
        if (GENIE_STATUS_SUCCESS != GenieDialog_query(m_dialog_handle, tagged_prompt.c_str(),
                                                      GenieDialog_SentenceCode_t::GENIE_DIALOG_SENTENCE_COMPLETE,
                                                      GenieCallBack, &model_response))
        {
            throw std::runtime_error("Failed to get response from GenieDialog. Please restart the ChatApp.");
        }

        if (model_response.empty())
        {
            // If model response is empty, reset dialog to re-initiate dialog.
            // During local testing, we found that in certain cases,
            // model response bails out after few iterations during chat.
            // If that happens, just reset Dialog handle to continue the chat.
            if (GENIE_STATUS_SUCCESS != GenieDialog_reset(m_dialog_handle))
            {
                throw std::runtime_error("Failed to reset Genie Dialog.");
            }
            if (GENIE_STATUS_SUCCESS != GenieDialog_query(m_dialog_handle, tagged_prompt.c_str(),
                                                          GenieDialog_SentenceCode_t::GENIE_DIALOG_SENTENCE_COMPLETE,
                                                          GenieCallBack, &model_response))
            {
                throw std::runtime_error("Failed to get response from GenieDialog. Please restart the ChatApp.");
            }
        }
    }
}

void ChatApp::ChatWithUserOnce(const std::string& user_prompt)
{
    AppUtils::PromptHandler prompt_handler;

    //std::string user_prompt;
    std::string model_response;

    // Input user prompt
    ChatSplit();
    //std::getline(std::cin, user_prompt);

    // Exit prompt
    if (user_prompt.compare(c_exit_prompt) == 0)
    {
        std::cout << "Exiting chat as per request.";
        return;
    }
    // User provides an empty prompt
    if (user_prompt.empty())
    {
        std::cout << "\nPlease enter prompt.\n";
        return;
    }

    std::string tagged_prompt = prompt_handler.GetPromptWithTag(user_prompt);

    // Bot's response
    std::cout << c_bot_name << ":";
    if (GENIE_STATUS_SUCCESS != GenieDialog_query(m_dialog_handle, tagged_prompt.c_str(),
        GenieDialog_SentenceCode_t::GENIE_DIALOG_SENTENCE_COMPLETE,
        GenieCallBack, &model_response))
    {
        throw std::runtime_error("Failed to get response from GenieDialog. Please restart the ChatApp.");
    }

    if (model_response.empty())
    {
        // If model response is empty, reset dialog to re-initiate dialog.
        // During local testing, we found that in certain cases,
        // model response bails out after few iterations during chat.
        // If that happens, just reset Dialog handle to continue the chat.
        if (GENIE_STATUS_SUCCESS != GenieDialog_reset(m_dialog_handle))
        {
            throw std::runtime_error("Failed to reset Genie Dialog.");
        }
        if (GENIE_STATUS_SUCCESS != GenieDialog_query(m_dialog_handle, tagged_prompt.c_str(),
            GenieDialog_SentenceCode_t::GENIE_DIALOG_SENTENCE_COMPLETE,
            GenieCallBack, &model_response))
        {
            throw std::runtime_error("Failed to get response from GenieDialog. Please restart the ChatApp.");
        }
    }
}
