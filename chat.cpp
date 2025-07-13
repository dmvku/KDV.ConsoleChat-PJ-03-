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
    client.startClient();
    
    std::cout << "The connection to the server is established\n";    

    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true)
    {
        if (keyCommand == serverCommand::exitChatMsg)
        {
            std::cout << "Exit chat...\n";
            close(client.socket_file_descriptor);
            return;
        }

        recievingData();
        keyCommand = dataParsing();

        if (keyCommand == serverCommand::helloMsg)
        {
            std::system("clear");
            std::cout << recievedData;            
        }
        else if (keyCommand == serverCommand::serviceMsg)
        {
            std::cout << recievedData;
            continue;
        }
        else if (keyCommand == serverCommand::loginMsg)
        {                     
            loginUser();
            continue;
        }        
        else if (keyCommand == serverCommand::newMessageMsg)
        {            
            newMessage(); 
            continue;
        }
        else if (keyCommand == serverCommand::deleteMessageMsg)
        {            
            deleteMessage();
            continue;
        }
        else if (keyCommand == serverCommand::beginUserListMsg)
        {            
            recievingChatData('U'); // reciving (U)sers
            continue;
        }
        else if (keyCommand == serverCommand::beginChatListMsg)
        {            
            recievingChatData('C'); // reciving (C)hat
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
            std::cout << recievedData;
        }
        
        std::getline(std::cin, client.sendData_);
        client.dataTransmission();
        
        if (client.sendData_ == "v")
        {
            std::system("clear");
            std::cout << "\t\t\tCHAT:\n";
            viewChat();            
        }
        else if (client.sendData_ == "u")
        {
            std::system("clear");
            std::cout << "\t\t\tUSER LIST:\n";
            viewUsers();            
        } 
    }    
}

void Chat::loginUser()
{    
    while (true)
    {        
        recievingData();
        keyCommand = dataParsing();

        if (keyCommand == serverCommand::passwordMsg)
        {
            std::cout << recievedData;
            std::getline(std::cin, client.sendData_);
            client.sendData_ = sha1(client.sendData_);
            client.dataTransmission();
            continue;
        }
        else if (keyCommand == serverCommand::loginMsg)
        {            
            std::string userID = dataParsing();
            currentUser_.setUserID(std::stoul(userID));
            std::string name = dataParsing();
            currentUser_.setName(name);
            std::string surname = dataParsing();
            currentUser_.setSurname(surname);

            std::cout << "\nUser " << currentUser_.getName() << " "
                << currentUser_.getSurname() << " is login...\n";
            return;
        }
        else if (keyCommand == serverCommand::serviceMsg)
        {
            std::cout << recievedData;
            return;
        }
        else if (keyCommand == serverCommand::exitChatMsg)
        {                        
            return;
        }
        else
        {
            std::cout << recievedData;
        }

        std::getline(std::cin, client.sendData_);
        client.dataTransmission();
    } 
}

void Chat::newMessage()
{
    while (true)
    {
        recievingData();
        keyCommand = dataParsing();

        if (keyCommand == serverCommand::serviceMsg)
        {
            std::cout << recievedData;
            continue;
        }
        else if (keyCommand == serverCommand::newMessageMsg)
        {            
            addMessage();
            return;
        }        
        else if (keyCommand == serverCommand::exitChatMsg)
        {
            return;
        }
        else
        {
            std::cout << recievedData;
        }

        std::getline(std::cin, client.sendData_);
        client.dataTransmission();
    }
}

void Chat::addMessage()
{        
    chatMessages_.push_back(
        Message{ std::stoul(dataParsing()),
            std::stoul(dataParsing()),
            std::stoul(dataParsing()),
            dataParsing(),
            dataParsing() });
}

void Chat::deleteMessage()
{
    unsigned long deletingMessageID{ 0 };

    while (true)
    {
        recievingData();
        keyCommand = dataParsing();

        if (keyCommand == serverCommand::beginUserListMsg)
        {            
            client.sendData_ = showUsersMessage();
            client.dataTransmission();
            continue;
        }
        else if (keyCommand == serverCommand::deleteMessageMsg)
        {           
            std::string messageID = dataParsing();
            deletingMessageID = std::stoul(messageID);            
            for (auto& element : chatMessages_)
            {
                if (element.getMessageID() == deletingMessageID)
                {
                    element.setMessage("Deleted");
                    return;
                }                
            }
        }
        else if (keyCommand == serverCommand::serviceMsg)
        {
            std::cout << recievedData;
            return;
        }
        else
        {
            std::cout << recievedData;
        }

        std::getline(std::cin, client.sendData_);
        client.dataTransmission();
    }
}

void Chat::addUsersData()
{    
    chatUsers_.push_back(User{ std::stoul(dataParsing()),
            dataParsing(),
            dataParsing() });
}

void Chat::viewChat()
{        
    int maxMessagesOnTheScreen{ 15 };
    int countMessagesOnTheScreen{};

    std::string addMeFrom{};
    //std::string addFrom{};
    std::string addRecipient{};
    unsigned long lastviewedMessage{ 0 };

    for (auto& element : chatMessages_)
    {        
        if (element.getMessage() == "Deleted")
        {
            continue;
        }
        
        std::string from{ getNameByUserID(element.getFrom()) };
        std::string to{ element.getTo() == 1 
            ? "to_all" : getNameByUserID(element.getTo()) };        
        std::string message{ element.getMessage() };
        std::string createTime{ element.getCreateTime() };
        if (from == currentUser_.getName()
            && currentUser_.getUserID() == element.getFrom())
        {
            addMeFrom = textColor::cyan
                + from
                + textColor::resetColor
                + "(me)";
        }
        else
        {
            addMeFrom = textColor::cyan
                + from + " " + getSurnameByUserID(element.getFrom())
                + textColor::resetColor;
        }

        if (to == "to_all")
        {
            addRecipient = "";
        }
        else if (to == currentUser_.getName()
            && currentUser_.getUserID() == element.getTo())
        {
            addRecipient = textColor::resetColor + " to myself";
        }
        else 
        {
            addRecipient = textColor::green + " to "
                + to + " " + getSurnameByUserID(element.getTo())
                + textColor::resetColor;
        }
        
        std::cout << textColor::magenta << createTime << " "
            << addMeFrom << addRecipient
            << textColor::resetColor << ": " << textColor::yellow 
            << message << textColor::resetColor << "\n";

        lastviewedMessage = element.getMessageID();

        countMessagesOnTheScreen++;
        
        if (countMessagesOnTheScreen == maxMessagesOnTheScreen)
        {
            client.sendData_ = std::to_string(lastviewedMessage);
            client.dataTransmission();
            char action{ '\0' };
            std::cout << "Continue (y - yes, other key - no)? ";
            std::cin >> action;
            if (action != 'y' && action != 'Y')
            {
                client.sendData_ = serverCommand::confirmationMsg;
                client.dataTransmission();
                return;
            }
            countMessagesOnTheScreen = 0;
        }        
    }  
    client.sendData_ = std::to_string(lastviewedMessage);
    client.dataTransmission();
    client.sendData_ = serverCommand::confirmationMsg;
    client.dataTransmission();
    std::cout << "\n";
}

void Chat::viewUsers()
{
    for (auto& element : chatUsers_)
    {
        std::cout << textColor::yellow << element.getName() << " "
            << element.getSurname() << textColor::resetColor << "\n";
    }    
}

std::string Chat::getNameByUserID(unsigned long userID)
{    
    for (auto& element : chatUsers_)
    {        
        if (element.getUserID() == userID)
        {
            return element.getName();            
        }        
    }

    return "Unknown";
}

std::string Chat::getSurnameByUserID(unsigned long userID)
{    
    for (auto& element : chatUsers_)
    {
        if (element.getUserID() == userID)
        {
            return element.getSurname();
        }
    }

    return "Unknown";
}

std::string Chat::showUsersMessage()
{    
    std::string addRecipient;
    std::string action{ "0" };
    int maxMessagesOnTheScreen{ 15 };
    int countMessages{ 0 };

    for (auto& element : chatMessages_)
    {
        if (element.getFrom() == currentUser_.getUserID()
            && element.getMessage() != "Deleted")
        {
            std::string messageID{ std::to_string(element.getMessageID()) };
            std::string to{ element.getTo() == 1
                ? "to_all" : getNameByUserID(element.getTo()) };
            std::string message{ element.getMessage() };
            std::string createTime{ element.getCreateTime() };

            if (to == currentUser_.getName()
                && currentUser_.getUserID() == element.getTo())
            {
                addRecipient = textColor::resetColor + " to myself";
            }
            else
            {
                addRecipient = textColor::green + " to "
                    + to + " " + getSurnameByUserID(element.getTo())
                    + textColor::resetColor;
            }            

            std::cout << " ID: " << textColor::cyan << messageID << "\t"
                << textColor::magenta << createTime << "  "
                << textColor::blue << addRecipient << ": "
                << textColor::yellow << message
                << textColor::resetColor << "\n";

            countMessages++;
            maxMessagesOnTheScreen--;

            if (maxMessagesOnTheScreen == 0)
            {                
                std::cout << "\nEnter ID of the message you want to delete,\n" 
                    << "'0' to exit or other key to continue: ";
                std::getline(std::cin, action);
                if (action.at(0) < '0' || action.at(0) > '9')
                {
                    maxMessagesOnTheScreen = 15;
                    continue;
                }
                return action;
            }
        }
    }
    if (countMessages)
    {
        std::cout << textColor::yellow
            << "Enter ID of the message you want to delete "
            << "or other key to continue: "
            << textColor::resetColor;
        std::getline(std::cin, action);
        if (action.at(0) < '0' || action.at(0) > '9')
        {
            action = "0";
        }
    }
    else
    {
        std::cout << textColor::yellow
            << "No messages found...\n" << textColor::resetColor;
    }
    
    return action;
}

void Chat::recievingChatData(char list)
{
    std::string endList;

    if (list == 'U')
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
        recievedData = std::string(client.message);           
        if (recievedData != endList)
        {
            switch (list)
            {
            case 'U':
                addUsersData();
                break;
            case 'C':
                addMessage();
                break;
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

void Chat::recievingData()
{
    client.dataRecieving();
    recievedData = std::string(client.message);
}

std::string Chat::dataParsing()
{
    std::string separator{ "|" };
    size_t separatorPoint{ recievedData.find(separator) };
    if (separatorPoint == std::string::npos)
    {
        keyCommand = "";
    }
    std::string value{ recievedData.substr(0, separatorPoint) };
    recievedData.erase(0, separatorPoint + 1);
    return value;
}
