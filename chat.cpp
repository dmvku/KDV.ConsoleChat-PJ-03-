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

        if (keyCommand == serverCommand::helloMsg)
        {
            std::system("clear");
            std::cout << receivedData;            
        }
        else if (keyCommand == serverCommand::serviceMsg)
        {
            std::cout << receivedData;
            continue;
        }
        else if (keyCommand == serverCommand::loginMsg)
        {
            currentUser_.setName(receivedData);
            std::cout << currentUser_.getName() << "\n";
            continue;
        }
        else if (keyCommand == serverCommand::passwordMsg)
        {
            std::cout << receivedData;            
            std::getline(std::cin, client.sendData_);
            client.sendData_ = sha1(client.sendData_);
            client.dataTransmission();            
            continue;
        }
        else if (keyCommand == serverCommand::newMessageMsg)
        {
            createMessage(receivedData);
            continue;
        }
        else if (keyCommand == serverCommand::beginUserListMsg)
        {
            recivingChatData('u');
            continue;
        }
        else if (keyCommand == serverCommand::beginChatListMsg)
        {
            recivingChatData('c');
            continue;
        }
        else if (keyCommand == serverCommand::exitChatMsg)
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
            std::system("clear");
            std::cout << "        CHAT:\n";
            viewChat();
        }
        else if (client.sendData_ == "u")
        {
            std::system("clear");
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
    int maxMessagesOnTheScreen{ 25 };
    int countMessagesOnTheScreen{};

    std::string addMeFrom{};
    //std::string addFrom{};
    std::string addRecipient{};

    for (auto& element : chatMessages_)
    {
        std::string from{ element.getFrom() };
        std::string to{ element.getTo() };
        std::string message{ element.getMessage() };
        addMeFrom = from == currentUser_.getName()
            ? textColor::resetColor + "(me)" : "";

        if (to == "to_all")
        {
            addRecipient = "";
        }
        else if (to == currentUser_.getName())
        {
            addRecipient = textColor::resetColor + " to myself";
        }
        else
        {
            addRecipient = textColor::resetColor + " to "
                + textColor::green + to;
        }

        std::cout << textColor::cyan << from << addMeFrom << addRecipient
            << textColor::resetColor << ": " << textColor::yellow 
            << message << textColor::resetColor << "\n";

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
            countMessagesOnTheScreen = 0;
        }        
    }  
    std::cout << "\n";
}

void Chat::viewUsers()
{
    for (auto& element : chatUsers_)
    {
        std::cout << textColor::yellow << element.getName()
            << textColor::resetColor << "\n";
    }    
}

void Chat::recivingChatData(char list)
{
    std::string recievingData;    
    std::string endList;

    if (list == 'u')
    {
        endList = serverCommand::endUserListMsg;
    }
    else
    {
        endList = serverCommand::endChatListMsg;
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
            client.sendData_ = serverCommand::confirmationMsg;
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
