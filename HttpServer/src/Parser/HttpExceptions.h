#ifndef HTTPEXCEPTION_H
#define HTTPEXCEPTION_H

#include <string>

//Derived class of the std::exception class to be used for throwing exceptions in the Http.Parse methods
namespace Willow {
	class HttpException : public std::exception {
	private:
		std::string m_Message;
        int m_ErrorCode;
        std::string m_CompleteMessage;
	public:
		HttpException(std::string message, int errorCode) : std::exception(), m_Message(message), m_ErrorCode(errorCode) {
			 this->m_CompleteMessage = "Error: " + std::to_string(errorCode) + ", Message: " + message;
        }

        const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override final{
            return this->m_CompleteMessage.c_str();
        }

		virtual ~HttpException() {

		}
	};
}

#endif