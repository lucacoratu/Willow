#include "HttpRequest.h"

#include "HttpExceptions.h"
#include "Utils.h"

#include <iostream>

namespace Willow {
	//Private functions
	std::queue<std::string> HttpRequest::SplitRequestLines(const std::string& request)
	{
		//Splits the request string by \r\n and pushes every line into a queue, then returns the queue
		int start = 0, end = 0;
		std::queue<std::string> lines;
		for (int i = 0; i < request.size(); i++) {
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

	void HttpRequest::ClearMemberVariables()
	{
		//Clears the member variables of the class
		this->m_HttpMethod.clear();
		this->m_HttpVersion.clear();
		this->m_Location.clear();
		this->m_Body.clear();

		//for (auto& header : this->m_Headers) {
		//	delete header;
		//}
		this->m_Headers.clear();
	}

	bool HttpRequest::HasValueInHeader(const Header header, const std::string& value)
	{
		//Parse the value string 
		std::string headerValue = header.value;

		//Get the values separated by commas
		size_t currentPos = 0;
		size_t start = 0;
		while (true) {
			currentPos = headerValue.find(',', currentPos);
			if (currentPos == std::string::npos)
				break;
			
			//Parse the value and check if it is equal to value passed as parameter
			std::string auxValue = headerValue.substr(start, currentPos - start);
			//Remove any spaces
			size_t notSpaceStart = auxValue.find_first_not_of(' ');
			size_t notSpaceEnd = auxValue.find_last_not_of(' ');
			std::string checkValue = auxValue.substr(notSpaceStart, notSpaceEnd - notSpaceStart);

			if (checkValue == value)
				return true;
		}

		//If there is no comma then parse the single value that is in the header
		size_t notSpaceStart = headerValue.find_first_not_of(' ');
		size_t notSpaceEnd = headerValue.find_last_not_of(' ');
		std::string checkValue = headerValue.substr(notSpaceStart, (notSpaceEnd - notSpaceStart + 1));

		if (checkValue == value)
			return true;

		return false;
	}

	HttpRequest::HttpRequest(const std::string& request)
	{
		//Create a queue with every line in the request
		std::queue<std::string> requestLines = this->SplitRequestLines(request);

		//First line contains information about the method used, location and HTTP version
		std::string firstLine = requestLines.front();
		requestLines.pop();

		//Extract the information from the first line
		size_t space0 = firstLine.find_first_of(' ', 0);
		size_t space1 = firstLine.find_first_of(' ', space0 + 1);

		//If any of the space values are std::string::npos then the first line does not contain the right information
		if (space0 == std::string::npos || space1 == std::string::npos)
			throw HttpException("Invalid HTTP format!", 5);

		this->m_HttpMethod = firstLine.substr(0, space0);
		this->m_Location = firstLine.substr(space0 + 1, space1 - space0);
		this->m_HttpVersion = firstLine.substr(space1 + 1);

		//Go through every line and save the data into the class members
		while (!requestLines.empty()) {
			std::string line = requestLines.front();
			requestLines.pop();

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

		//Rest of the string is the body of the request
		while (!requestLines.empty()) {
			if(!requestLines.front().empty())
				this->m_Body += requestLines.front() + "\r\n";
			requestLines.pop();
		}

		//Parse the request if it is chunked encoded
		this->ParseChunkedRequest();
	}

	//Public functions
	//Constructors / Destructors
	HttpRequest::HttpRequest(const std::string& method, const std::string& location, const std::string& version, const std::vector<Header>& headers, const std::string& body)
		: m_HttpMethod(method), m_Location(location), m_HttpVersion(version), m_Headers(headers), m_Body(body)
	{

	}

	HttpRequest::HttpRequest(const HttpRequest& other)
		: m_HttpMethod(other.m_HttpMethod), m_HttpVersion(other.m_HttpVersion), m_Location(other.m_Location), m_Headers(other.m_Headers), m_Body(other.m_Body)
	{

	}

	HttpRequest::~HttpRequest()
	{
		this->ClearMemberVariables();
	}

	const std::string& HttpRequest::GetHttpMethod() const
	{
		//If the method string is empty throws an exception (can be caught with std::exception)
		//Else it returns the method of the HTTP Request
		if (this->m_HttpMethod.empty())
			throw HttpException("Method is empty, make sure to call Parse!", 2);
		return this->m_HttpMethod;
	}

	const std::string& HttpRequest::GetHttpLocation() const
	{
		//If the location string is empty throws an exception (can be caught with std::exception)
		//Else it returns the location of the HTTP Request
		if (this->m_Location.empty())
			throw HttpException("Location is empty, make sure to call Parse!", 3);
		return this->m_Location;
	}

	const std::string& HttpRequest::GetHttpVersion() const
	{
		//If the version string is empty throws an exception (can be caught with std::exception)
		//Else it returns the version of the HTTP Request
		if (this->m_HttpVersion.empty())
			throw HttpException("Version is empty, make sure to call Parse!", 4);
		return this->m_HttpVersion;
	}

	
	const std::vector<Header>& HttpRequest::GetHttpHeaders() const
	{
		//Returns the list of Headers of the HTTP Request, parsed by the parse function
		//Throws an error if headers are empty
		if (this->m_Headers.size() == 0)
			throw HttpException("Headers are empty, make sure to call Parse!", 1);

		return this->m_Headers;
	}

	const std::string& HttpRequest::GetHttpBody() const
	{
		//Returns the body of the HTTP Request
		return this->m_Body;
	}

	void HttpRequest::ParseChunkedRequest()
	{
		//Parses the Http request and modifies the class members after parsing the request
		
		//Check if the request has Transfer-Encoding: chunked in it
		for (auto& header : this->m_Headers) {
			if (header.name == "Transfer-Encoding") {
				//See if it has chunked in it
				if (this->HasValueInHeader(header, "chunked")) {
					//Parse the chunked request data

					//Body is the form
					//DataSize (hexadecimal)\r\n
					//Data of size DataSize

					std::string parsedBody = "";

					//Take data size
					size_t start = 0;
					size_t end = this->m_Body.find_first_of('\n');
					while (end != std::string::npos) {
						std::string dataSizeString;
						try {
							//If index is out of bounds then the body has all been parsed
							dataSizeString = this->m_Body.substr(start, (end - start));
						}
						catch (std::exception& e) {
							this->m_Body = parsedBody;
							return;
						}

						//If dataSizeString is empty then all the chunked body was parsed so return from function
						if (dataSizeString == "")
							return;
						
						//Remove /r at the end if it has
						if (dataSizeString.at(dataSizeString.size() - 1) == '\r' || dataSizeString.at(dataSizeString.size() - 1) == '\n') {
							dataSizeString = dataSizeString.substr(0, dataSizeString.size() - 1);
						}
						
						//If dataSizeString is empty then all the chunked body was parsed so return from function
						if (dataSizeString == "")
							return;

						//Convert hex string to size_t
						size_t dataSize = ConvertFromHexString(dataSizeString);
						
						//Take dataSize characters from the body
						size_t dataStart = end + 1;
						size_t dataEnd = dataStart + dataSize;

						//Check if data is the size specified before, if it isn't throw an exception
						if (dataEnd > this->m_Body.size())
							throw HttpException("Invalid chunked body data!", 7);

						//Add the parsed data to the body
						parsedBody += this->m_Body.substr(dataStart, dataEnd);

						start = dataEnd + 1;
						end = this->m_Body.find_first_of('\n', start);
					}

					this->m_Body = parsedBody;
				}
				else {
					//The request is not chunked encoded.
					return;
				}
			}
		}
	}

	//Debug print
	void HttpRequest::PrintRequest() const
	{
		//Prints the data in the request

		//Print the HTTP version
		std::cout << "HTTP Version: " << this->m_HttpVersion << "\n";
		std::cout << "HTTP Method: " << this->m_HttpMethod << "\n";
		std::cout << "HTTP Path: " << this->m_Location << "\n";
		std::cout << "Headers:\n";
		for (auto& [name, value] : this->m_Headers) {
			std::cout << "Name: " << name << ", Value: " << value << "\n";
		}

		std::cout << "Body: " << this->m_Body << std::endl;
	}
}
