#ifndef SERVEREXCEPTIONS_H
#define SERVEREXCEPTIONS_H

#include <iostream>

namespace Willow{
    class ServerException : public std::exception{
    private:
        std::string m_Message;
        int m_ErrorCode;
        std::string m_CompleteMessage;
    public:
        ServerException(std::string message, int errorCode) : std::exception(), m_Message(message), m_ErrorCode(errorCode) {
            this->m_CompleteMessage = "Error: " + std::to_string(errorCode) + ", Message: " + message;
        }

        const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override final{
            return this->m_CompleteMessage.c_str();
        }
    };
}

#endif