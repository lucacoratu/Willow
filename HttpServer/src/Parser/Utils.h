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

	size_t ConvertFromHexString(const std::string& hexString);
	
}

#endif