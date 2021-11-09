# Telegram-Bot-C-CPP
A simple Telegram bot using the tgbot library that sends messages to a particular chat once a file inside a give directory gets modified.

___

<img src="images/results.gif" width="1024" />

# Main Idea
___
The main idea of this program is to automate the prodcess of sending resource information about a particular machine using the Telegram API.
You can find a detailed explanation here in my Medium article.

# Compilation
___
This bot uses the TgBot library. There is a detailed explanation how to compile such programs (with and without CMake) in the [library repository](https://github.com/reo7sp/tgbot-cpp).

Use the code below to compile without CMake:
``g++ telegram_bot.cpp -o telegram_bot --std=c++11 -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread



