#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>

namespace Willow {
	
	//Structure for Header
	struct Header {
		std::string name;
		std::string value;
	};

	//Enum for supported HTTP methods
	enum class HttpMethod {
		GET,
		POST,
	};

	size_t ConvertFromHexString(const std::string& hexString);
	
}

#endif