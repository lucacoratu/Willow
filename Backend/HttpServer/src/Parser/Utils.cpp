#include "Utils.h"

namespace Willow {
	static const unsigned char HexAlphabet[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	
	size_t ConvertFromHexString(const std::string& hexString)
	{
		//Converts hex string to size_t value
		size_t aux = 0;

		//Take characters from hex string 2 by 2 and add them to aux
		std::string byte = hexString.substr(0, 2);
		size_t currentPos = 2;
		while (currentPos <= hexString.size()) {
			//Shift the value 8 positions to the left
			aux <<= 8;

			//Convert to integer value
			int letter = -1;
			for (size_t i = 0; i < sizeof(HexAlphabet); i++) {
				if (byte[0] == HexAlphabet[i]) {
					letter = i;
					break;
				}
			}

			unsigned char byteValue = letter * 16;

			for (size_t i = 0; i < sizeof(HexAlphabet); i++) {
				if (byte[1] == HexAlphabet[i]) {
					letter = i;
					break;
				}
			}

			byteValue += letter;
			aux += byteValue;

			byte = hexString.substr(currentPos, 2);
			currentPos += 2;
		}

		return aux;
	}
}