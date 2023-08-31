#pragma once
#include <stdexcept>
#include <iostream>

class Validator
{
public:
	static void ValidateFileName(const char* fileName);
	static void ValidteStream(std::istream& stream, const char* whatArgument);
	static void ValidatePosiviteValue(int value, const char* whatArgument);

	template<typename T>
	static void ValidateDynamicMemory(const T* dynamicMemomryPtr, bool isCurrentAllocation = true);
};

void Validator::ValidateFileName(const char* fileName)
{
	if (!fileName)
	{
		throw std::invalid_argument("Filename cannot be nullptr");
	}
}

void Validator::ValidteStream(std::istream& stream, const char* whatArgument)
{
	if (!stream.good())
	{
		throw std::runtime_error(whatArgument);
	}
}

template<typename T>
void Validator::ValidateDynamicMemory(const T* dynamicMemoryPtr, bool isCurrentAllocation)
{
	if (!dynamicMemoryPtr)
	{
		if (isCurrentAllocation)
		{
			throw std::runtime_error("Memory allocation failed!");
		}

		throw std::runtime_error("Samples cannon be nullptr");
	}
}

void Validator::ValidatePosiviteValue(int value, const char* whatArgument)
{
	if (value <= 0)
	{
		throw std::invalid_argument(whatArgument);
	}
}


