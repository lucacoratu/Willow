#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <string>
#include <vector>
#include <queue>

#include "Utils.h"

namespace Willow {
	//Class used to store parsed Headers and Body
	//Use the Parse method with a string to parse the HTTP Request
	//Get specific data by calling the getter
	class HttpRequest
	{
	private:
		//Method of the HTTP Request (POST / GET)
		std::string m_HttpMethod;

		//Location of the request
		std::string m_Location;

		//Version of the HTTP
		std::string m_HttpVersion;

		//Headers of the HTTP Request
		std::vector<Header> m_Headers;

		//Body of the HTTP Request
		std::string m_Body;

		//Private functions
		//Split the request into lines and push lines into a queue
		std::queue<std::string> SplitRequestLines(const std::string& request);

		//Clear the member variables
		void ClearMemberVariables();

		//Has Value
		bool HasValueInHeader(const Header header, const std::string& value);

		//Parse the request if it is chunked
		void ParseChunkedRequest();
	public:
		//Constructors / Destructors
		HttpRequest(const std::string& request);
		HttpRequest(const std::string& method, const std::string& location, const std::string& version, const std::vector<Header>& headers, const std::string& body);
		~HttpRequest();

		//Copy constructor
		HttpRequest(const HttpRequest& other);

		//Getters
		const std::string& GetHttpMethod() const;
		const std::string& GetHttpLocation() const;
		const std::string& GetHttpVersion() const;
		const std::vector<Header>& GetHttpHeaders() const;
		const std::string& GetHttpBody() const;

		//Debug print
		void PrintRequest() const;
	};
}

#endif

