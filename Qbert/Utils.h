#pragma once
#include <json.hpp>
#include <fstream>
#include <iostream>

namespace utils
{
	inline static nlohmann::json ReadJson(const std::string& fileName)
	{
		nlohmann::json jsonArray = nlohmann::json::array();
		std::ifstream inFile(fileName, std::ios::binary);
		if (!inFile.is_open())
		{
			std::cerr << "Failed to open " << fileName << "\n";
			return jsonArray;
		}

		std::vector<char> buffer((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
		inFile.close();

		try
		{
			if (!buffer.empty())
				jsonArray = nlohmann::json::from_cbor(buffer);
		}
		catch (const nlohmann::json::parse_error& e)
		{
			std::cerr << "Parse error: " << e.what() << std::endl;
			return nlohmann::json::array();
		}
		inFile.close();

		return jsonArray;
	}

	inline static void WriteJson(const std::string& filename, nlohmann::json data)
	{
		std::ofstream outFile(filename, std::ios::binary);
		if (!outFile.is_open())
		{
			std::cerr << "Error opening file for writing\n";
			return;
		}

		std::vector<uint8_t> outBuffer = nlohmann::json::to_cbor(data);
		outFile.write(reinterpret_cast<const char*>(outBuffer.data()), outBuffer.size());
		outFile.close();
	}
}