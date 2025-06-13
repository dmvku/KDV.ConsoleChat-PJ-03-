#include "chat_class.h"
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

void Chat::startClient()
{        
    configuringTheServerConnection();
    if (serverAddress_ == "0" || serverPort_ == 0)
    {
        std::cout << "Exit chat...\n";
        return;
    }
    createSocket();    
    interactionWithTheServer();
}

void Chat::configuringTheServerConnection()
{
    serverAddress_ = serverConfig_.getAddressIP();
    serverPort_ = serverConfig_.getPort();
}

void Chat::createSocket()
{
    // Создадим сокет
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1)
    {
        std::cout << "Creation of Socket failed!\n";
        exit(1);
    }
    std::cout << "Socket is creation...\n";    

    // Установим адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr(serverAddress_.c_str());
    // Зададим номер порта
    serveraddress.sin_port = htons(serverPort_);
    // Используем IPv4
    serveraddress.sin_family = AF_INET;
    std::cout << "Apply server parameters...\n";
}

void Chat::connectionToTheServer()
{    
    // Установим соединение с сервером
    int numberOfAttempts{ 3 };    
    for (int attempts = 1; attempts <= numberOfAttempts; attempts++)
    {
        std::cout << "Connecting to the server. Attempts - " << attempts << "\n";
        connection = connect(socket_file_descriptor,
            (struct sockaddr*)&serveraddress, sizeof(serveraddress));
        if (connection == -1)
        {            
            if (attempts == numberOfAttempts)
            {
                std::cout << "Connection with the server failed!\n";
                exit(1);
            }
            std::cout << "Not connection to the server!\nPause 10 sec.\n";
            sleep(10);
        }
        else
        {
            break;
        }
    }
}

void Chat::interactionWithTheServer()
{
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (1)
    {
        connectionToTheServer();

        std::cout << "The connection to the server is established\n";
        std::string receivedData;        

        while (1)
        {
            dataRecieving();
            receivedData = std::string(message);
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
                std::cin.clear();
                std::getline(std::cin, sendData_);               
                sendData_ = passwordHashing(sendData_);                
                dataTransmission();
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
                close(socket_file_descriptor);
                return;
            }            
            else
            {
                std::cout << receivedData;
            }
            
            std::cin.clear();
            std::getline(std::cin, sendData_);

            if (sendData_ == "v")
            {
                std::cout << "        CHAT:\n";
                viewChat();
            }
            else if (sendData_ == "u")
            {
                std::cout << "        USER LIST:\n";
                viewUsers();
            }            
                    
            dataTransmission();
        }
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

void Chat::dataTransmission()
{
    bzero(message, MESSAGE_LENGTH);
    std::copy(sendData_.begin(), sendData_.end(), message);

    ssize_t bytes = write(socket_file_descriptor, message, sizeof(message));
    // Если передали >= 0  байт, значит пересылка прошла успешно
    if (bytes >= 0) {
        std::cout << "Data successfully sent to the server.!\n";
    }
}

void Chat::dataRecieving()
{
    bzero(message, MESSAGE_LENGTH);
    read(socket_file_descriptor, message, MESSAGE_LENGTH);
    std::cout << "The data is received from the server: "
        << message << "\n";
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
        dataRecieving();
        recievingData = std::string(message);        
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
            sendData_ = "#OK";
            dataTransmission();
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
