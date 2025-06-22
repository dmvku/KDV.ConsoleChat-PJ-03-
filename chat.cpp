#include "chat.h"
#include "sha1.h"

#include <iostream>
#include <string>
#include <limits>

Chat::Chat()
{
    
}

Chat::~Chat()
{

}

void Chat::startChat()
{
    if (!client.startClient())
    {
        return;
    }

    std::cout << "The connection to the server is established\n";
    std::string receivedData;

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (1)
    {
        client.dataRecieving();
        receivedData = std::string(client.message);
        std::string keyCommand = dataParsing(receivedData);

        if (keyCommand == serviceMsg)
        {
            std::cout << receivedData;
            continue;
        }
        else if (keyCommand == loginMsg)
        {
            currentUser_.setName(receivedData);
            std::cout << currentUser_.getName() << "\n";
            continue;
        }
        else if (keyCommand == passwordMsg)
        {
            std::cout << receivedData;            
            std::getline(std::cin, client.sendData_);
            client.sendData_ = sha1(client.sendData_);
            client.dataTransmission();            
            continue;
        }
        else if (keyCommand == newMessageMsg)
        {
            createMessage(receivedData);
            continue;
        }
        else if (keyCommand == beginUserListMsg)
        {
            recivingChatData('u');
            continue;
        }
        else if (keyCommand == beginChatListMsg)
        {
            recivingChatData('c');
            continue;
        }
        else if (keyCommand == exitChatMsg)
        {
            std::cout << "Exit chat...\n";
            close(client.socket_file_descriptor);
            return;
        }
        else
        {
            std::cout << receivedData;
        }
        
        std::getline(std::cin, client.sendData_);
        
        if (client.sendData_ == "v")
        {
            std::cout << "        CHAT:\n";
            viewChat();
        }
        else if (client.sendData_ == "u")
        {
            std::cout << "        USER LIST:\n";
            viewUsers();
        }

        client.dataTransmission();
    }    
}

void Chat::createMessage(std::string& newMessage)
{    
    chatMessages_.push_back(Message{ dataParsing(newMessage),
                    dataParsing(newMessage), dataParsing(newMessage) });
}

void Chat::viewChat()
{    
    int maxMessagesOnTheScreen{ 15 };
    int countMessagesOnTheScreen{};

    std::string addMeFrom{};
    //std::string addFrom{};
    std::string addRecipient{};

    for (auto& element : chatMessages_)
    {
        std::string from{ element.getFrom() };
        std::string to{ element.getTo() };
        std::string message{ element.getMessage() };
        addMeFrom = from == currentUser_.getName() ? "\033[0m(me)" : "";

        if (to == "to_all")
        {
            addRecipient = "";
        }
        else if (to == currentUser_.getName())
        {
            addRecipient = "\033[0m to myself";
        }
        else
        {
            addRecipient = "\033[0m to \033[1;4;36m" + to;
        }

        std::cout << "\033[1;4;33m" << from << addMeFrom << addRecipient
            << "\033[0m: " << message << "\n";

        countMessagesOnTheScreen++;
        
        if (countMessagesOnTheScreen == maxMessagesOnTheScreen)
        {
            char action{ '\0' };
            std::cout << "Continue (y - yes, other key - no)? ";
            std::cin >> action;
            if (action != 'y' && action != 'Y')
            {
                return;
            }

            system("clear");
            std::cout << "           CHAT:\n";
            countMessagesOnTheScreen = 0;
        }        
    }  
    std::cout << "\n";
}

void Chat::viewUsers()
{
    for (auto& element : chatUsers_)
    {
        std::cout << "\033[1;4;33m" << element.getName() << "\033[0m\n";
    }
    std::cout << "\n";
}

void Chat::recivingChatData(char list)
{
    std::string recievingData;    
    std::string endList;

    if (list == 'u')
    {
        endList = endUserListMsg;
    }
    else
    {
        endList = endChatListMsg;
    }

    do
    {
        client.dataRecieving();
        recievingData = std::string(client.message);        
        if (recievingData != endList)
        {
            switch (list)
            {
            case 'u':
                chatUsers_.push_back(User{ recievingData });
                break;
            case 'c':
                createMessage(recievingData);
            }
            client.sendData_ = "#OK";
            client.dataTransmission();
        }
        else
        {
            return;
        }
    } while (1);
}

std::string Chat::dataParsing(std::string& data)
{
    std::string separator{ "|" };
    size_t separatorPoint{ data.find(separator) };
    if (separatorPoint == std::string::npos)
    {
        return "Data error...\n";
    }
    std::string value{ data.substr(0, separatorPoint) };
    data.erase(0, separatorPoint + 1);
    return value;
}
