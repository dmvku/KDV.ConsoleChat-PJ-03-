#include "socket.h"
#include <iostream>

Socket::Socket()
{
    configuringTheServerConnection();
}

Socket::~Socket()
{

}

void Socket::configuringTheServerConnection()
{
    serverAddress_ = serverConfig_.getAddressIP();
    serverPort_ = serverConfig_.getPort();
}

void Socket::startClient()
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

void Socket::dataTransmission()
{
    bzero(message, MESSAGE_LENGTH);
    std::copy(sendData_.begin(), sendData_.end(), message);

    ssize_t bytes = write(socket_file_descriptor, message, sizeof(message));
    // Если передали >= 0  байт, значит пересылка прошла успешно
    /*if (bytes >= 0)
    {
        std::cout << "Data successfully sent to the server.!\n";
    }*/
}

void Socket::dataRecieving()
{
    bzero(message, MESSAGE_LENGTH);
    read(socket_file_descriptor, message, MESSAGE_LENGTH);
    // std::cout << "The data is received from the server: "
    //    << message << "\n";
}
