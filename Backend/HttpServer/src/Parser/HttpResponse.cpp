#include "HttpResponse.h"

#include "HttpExceptions.h"

#include <iostream>

namespace Willow {
	//Map with all the available codes supported
	static const std::map<size_t, std::string> StatusCodesAndReason{
		{200, "OK"},
		{400, "Bad request"},
		{404, "Not Found"}
	};

	//Private functions
	void HttpResponse::ClearMemberVariables()
	{
		//Clears the member varialbes
		this->m_Version.clear();
		this->m_Reason.clear();
		//for (auto& header : this->m_Headers)
		//	delete header;
		this->m_Headers.clear();
		this->m_Body.clear();
	}

	//Split the request into lines and push lines into a queue
	std::queue<std::string> HttpResponse::SplitResponseLines(const std::string& request){
		//Splits the request string by \r\n and pushes every line into a queue, then returns the queue
		size_t start = 0, end = 0;
		std::queue<std::string> lines;
		for (size_t i = 0; i < request.size(); i++) {
			end = i;
			if (request.at(i) == '\r' || request.at(i) == '\n') {
				lines.push(request.substr(start, end - start));
				start = end + 2;
				i = i + 1;
			}
		}
		if(start < request.size())
			lines.push(request.substr(start));
		return lines;
	}

	//Constructors / Destructors
	HttpResponse::HttpResponse()
		: m_StatusCode(0)
	{
		//Creates an empty object
	}

	HttpResponse::HttpResponse(const std::string& response)
		:m_StatusCode(0)
	{
		//Constructs a response object based on a raw response
		std::queue<std::string> responseLines = this->SplitResponseLines(response);

		//First line contains information about the method used, location and HTTP version
		std::string firstLine = responseLines.front();
		responseLines.pop();

		//Extract the information from the first line
		size_t space0 = firstLine.find_first_of(' ', 0);
		size_t space1 = firstLine.find_first_of(' ', space0 + 1);

		//If any of the space values are std::string::npos then the first line does not contain the right information
		if (space0 == std::string::npos || space1 == std::string::npos)
			throw HttpException("Invalid HTTP format!", 5);

		this->m_Version = firstLine.substr(0, space0);
		this->m_StatusCode = stoi(firstLine.substr(space0 + 1, space1 - space0 - 1));
		this->m_Reason = firstLine.substr(space1 + 1);

		//Go through every line and save the data into the class members
		while (!responseLines.empty()) {
			std::string line = responseLines.front();
			responseLines.pop();

			//Find the ':' in the line and split the line into name and value
			//If the line does not contain ':' then it is part of the body so add the line to the body and break the loop
			size_t doubleDot = line.find_first_of(':', 0);
			if (doubleDot == std::string::npos) {
				if(!line.empty())
					this->m_Body += line + "\r\n";
				break;
			}

			//Remove any spaces between ':' and name and between ':' and value
			std::string name = line.substr(0, doubleDot);
			std::string value = line.substr(doubleDot + 1);

			//Name
			size_t nonSpace = name.find_last_not_of(' ');
			name = name.substr(0, nonSpace + 1);

			//Value	
			nonSpace = value.find_first_not_of(' ');
			value = value.substr(nonSpace);

			this->m_Headers.push_back(Header{ name, value });
		}

		//Rest of the string is the body of the response
		while (!responseLines.empty()) {
			if(!responseLines.front().empty())
				this->m_Body += responseLines.front() + "\r\n";
			responseLines.pop();
		}
	}
	
	HttpResponse::HttpResponse(const std::string& version, const size_t& statusCode, const std::string& reason, const std::vector<Header>& headers, const std::string& body)
		: m_Version(version), m_StatusCode(statusCode), m_Reason(reason), m_Headers(headers), m_Body(body)
	{
		//Constructs a response from the given parameters
	}

	HttpResponse::~HttpResponse()
	{
		this->ClearMemberVariables();
	}

	//Copy constructor
	HttpResponse::HttpResponse(const HttpResponse& other)
		: m_Version(other.m_Version), m_StatusCode(other.m_StatusCode), m_Reason(other.m_Reason), m_Headers(other.m_Headers), m_Body(other.m_Body)
	{

	}

	//Getters
	const std::string& HttpResponse::GetHttpVersion() const
	{
		// Returns the version of the http response object
		return this->m_Version;
	}
	
	const size_t& HttpResponse::GetHttpStatusCode() const
	{
		// Returns the status code of the http response object
		return this->m_StatusCode;
	}

	const std::string& HttpResponse::GetHttpReason() const
	{
		// Returns the reason of the http response object
		return this->m_Reason;
	}
	const std::vector<Header>& HttpResponse::GetHttpHeaders() const
	{
		// Returns the headers of the http response object
		return this->m_Headers;
	}
	const std::string& HttpResponse::GetHttpBody() const
	{
		// Returns the body of the http response object
		return this->m_Body;
	}

	//Setters
	void HttpResponse::SetHttpVersion(const std::string& version)
	{
		//Sets the http version of the object
		//If the version is not HTTP/1.1 or HTTP/1.0 or HTTP/2.0 then return
		if (version != "HTTP/1.1" && version != "HTTP/1.0" && version != "HTTP/2.0")
			return;

		this->m_Version = version;
	}

	void HttpResponse::SetHttpStatusCode(const size_t statusCode)
	{
		//Sets the status code of the object
		//If the status code is not in the list of supported status codes then return
		if (StatusCodesAndReason.at(statusCode) == "")
			return;

		this->m_StatusCode = statusCode;
		this->m_Reason = StatusCodesAndReason.at(statusCode);
	}

	void HttpResponse::SetHttpHeaders(const std::vector<Header>& headers)
	{
		//Sets the headers of the object
		//If the headers parameter is empty then return (use the clear headers function instead)
		if (headers.empty())
			return;

		this->m_Headers = headers;
	}

	void HttpResponse::SetHttpHeaders(const std::map<std::string, std::string>& headers)
	{
		//Sets the headers of the object from an std::map<>
		//If the map is empty then return (use the clear headers function instead)
		if (headers.empty())
			return;

		//Clear the current headers
		//for (auto& header : this->m_Headers)
		//	delete header;
		this->m_Headers.clear();

		for (auto& [name, value] : headers) {
			this->m_Headers.push_back(Header{ name, value });
		}
	}

	void HttpResponse::SetHttpBody(const std::string& body)
	{
		//Clears the previous body and sets the new body
		//If the body is empty then return (use clear body function instead)
		if (body.empty())
			return;

		this->m_Body.clear();
		this->m_Body = body;
	}

	//Append functions
	void HttpResponse::AppendHttpBody(const std::string& body)
	{
		this->m_Body += body;
	}

	void HttpResponse::AppendHttpHeaders(const std::vector<Header>& headers)
	{
		for (auto& header : headers)
			this->m_Headers.push_back(header);
	}

	void HttpResponse::AppendHttpHeaders(const std::map<std::string, std::string>& headers)
	{
		for (auto& [name, value] : headers)
			this->m_Headers.push_back(Header{ name, value });
	}

	void HttpResponse::AppendHttpHeader(const Header& header)
	{
		//Append the header from parameter to the list of current headers
		this->m_Headers.push_back(header);
	}

	void HttpResponse::AppendHttpHeader(const std::string& name, const std::string& value)
	{
		//Append the name and the value to the list of current headers
		this->m_Headers.push_back(Header({ name, value }));
	}

	//Clear functions
	void HttpResponse::ClearHttpBody()
	{
		this->m_Body.clear();
	}

	void HttpResponse::ClearHttpHeaders()
	{
		this->m_Body.clear();
	}

	//Raw http response
	const std::string HttpResponse::GetRawHttpResponse() const
	{
		//Creates a raw http response from the data in the object and returns it

		std::string rawResponse = "";
		//Append the status line
		rawResponse += this->m_Version + " " + std::to_string(this->m_StatusCode) + " " + this->m_Reason + "\r\n";
		
		//Append the headers
		for (auto& [name, value] : this->m_Headers) {
			rawResponse += name + ": " + value + "\r\n";
		}

		//Append an empty line
		rawResponse += "\r\n";

		//Append the message body
		rawResponse += this->m_Body;

		return rawResponse;
	}

	//Debug print function
	void HttpResponse::PrintResponse() const
	{
		//Prints the member variables of the object
		std::cout << "=====HTTP Response=====\n";
		std::cout << "Version: " << this->m_Version << "\n";
		std::cout << "Status code: " << this->m_StatusCode << "\n";
		std::cout << "Reason: " << this->m_Reason << "\n";
		std::cout << "Headers:\n";
		for (auto& [name, value] : this->m_Headers)
			std::cout << "Name: " << name << ", Value: " << value << "\n";
		std::cout << "Body:\n";
		std::cout << this->m_Body << "\n";
		std::cout << "=======================" << std::endl;
	}
}