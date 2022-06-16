#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <string>
#include <vector>
#include <queue>
#include <map>

#include "Utils.h"

namespace Willow {
	//Class used for storing HTTP Responses
	//It has setters and getters so the response can be modified before sending to client
	class HttpResponse
	{
	private:
		//Version for http
		std::string m_Version;
		
		//Status code
		size_t m_StatusCode;

		//Reason
		std::string m_Reason;

		//Headers
		std::vector<Header> m_Headers;

		//Body
		std::string m_Body;

		//Private functions
		void ClearMemberVariables();
	public:
		//Constructors / Destructors
		HttpResponse();
		HttpResponse(const std::string& response);
		HttpResponse(const std::string& version, const size_t& statusCode, const std::string& reason, const std::vector<Header>& headers, const std::string& body);
		~HttpResponse();

		//Copy constructor
		HttpResponse(const HttpResponse& other);

		//Getters
		const std::string& GetHttpVersion() const;
		const size_t& GetHttpStatusCode() const;
		const std::string& GetHttpReason() const;
		const std::vector<Header>& GetHttpHeaders() const;
		const std::string& GetHttpBody() const;

		//Setters
		void SetHttpVersion(const std::string& version);
		void SetHttpStatusCode(const size_t statusCode);
		void SetHttpHeaders(const std::vector<Header>& headers);
		void SetHttpHeaders(const std::map<std::string, std::string>& headers);
		void SetHttpBody(const std::string& body);

		//Append functions
		void AppendHttpBody(const std::string& body);
		void AppendHttpHeaders(const std::vector<Header>& headers);
		void AppendHttpHeaders(const std::map<std::string, std::string>& headers);
		void AppendHttpHeader(const Header& header);
		void AppendHttpHeader(const std::string& name, const std::string& value);

		//Clear functions
		void ClearHttpBody();
		void ClearHttpHeaders();

		//Raw messsage
		const std::string GetRawHttpResponse() const;

		//Debug print
		void PrintResponse() const;
	};
}

#endif