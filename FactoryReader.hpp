#pragma once
#include "Sound.hpp"
#include <iostream>
#include <string>
#include "Silence.hpp";
#include "Periodic.hpp";
#include "FadeIn.hpp"
#include "VolumeIncreaser.hpp"
#include "FadeOut.hpp"

template <typename T>
class FactoryReader
{
public:
	static FactoryReader<T>* getInstance();
	Sound<T>* readSound(std::istream& fileReader);

private:
	static FactoryReader<T>* instance;

	FactoryReader() = default;

private:
	template <typename U>
	struct NumberParsedResult
	{
		bool isNumber = true;
		U number;
	};

	template <typename U>
	NumberParsedResult<U> getNumber(const char* line) const;

	Sound<T>* parsePause(const char* first) const;
	Sound<T>* parseFadeIn(const char* fileName, const char* fadeInSamplesAsText, const char* coeffAsText) const;
	Sound<T>* parseSound(const char* fileName, const char* coefficient) const;

	void setPointer(const char*& ptr, char*& line) const;
	bool isTransition(const char* transitionLength, const char* secondSongSamples, const char* attenuationLevel) const;
	bool isLineOfWhitespaces(const char* first, const char* second, const char* third) const;
	bool isFileNameValid(const char* fileName) const;

	Sound<T>* getNextSound(const Sound<T>* firstSong, std::ifstream& fileReader) const;
};

template <typename T>
FactoryReader<T>* FactoryReader<T>::instance = nullptr;

template <typename T>
FactoryReader<T>* FactoryReader<T>::getInstance()
{
	if (!instance)
	{
		instance = new FactoryReader();
	}

	return instance;
}

template <typename T>
Sound<T>* FactoryReader<T>::readSound(std::istream& fileReader)
{
	char line[1000];
	fileReader.getline(line, 1000, '\n');

	if (line[0] == '\0')
	{
		return nullptr;
	}

	char* ptr = line;

	const char* firstElement = nullptr;
	const char* secondElement = nullptr;
	const char* thirdElement = nullptr;

	setPointer(firstElement, ptr);
	setPointer(secondElement, ptr);
	setPointer(thirdElement, ptr);

	// line only with whitespaces
	if (isLineOfWhitespaces(firstElement, secondElement, thirdElement))
	{
		return nullptr;
	}

	// onlye one element
	if (!secondElement && !thirdElement)
	{
		return parsePause(firstElement);
	}

	// numbers, describing transition
	// if (isTransition(firstElement, secondElement, thirdElement))
	// {
	// 	return nullptr;
	// }

	// song
	if (!thirdElement)
	{
		return parseSound(firstElement, secondElement);
	}

	// fade in
	return parseFadeIn(firstElement, secondElement, thirdElement);
}

template <typename T>
Sound<T>* FactoryReader<T>::parsePause(const char* first) const
{
	NumberParsedResult<int> pauseParseResult = getNumber<int>(first);

	if (!pauseParseResult.isNumber || pauseParseResult.number < 0)
	{
		return nullptr;
	}

	return new Silence<int>(pauseParseResult.number);
}

template <typename T>
Sound<T>* FactoryReader<T>::parseFadeIn(const char* fileName, const char* fadeInSamplesAsText, const char* coeffAsText) const
{
	if (!isFileNameValid(fileName))
	{
		return nullptr;
	}

	NumberParsedResult<int> fadeInSamples = getNumber<int>(fadeInSamplesAsText);

	if (!fadeInSamples.isNumber || fadeInSamples.number < 0)
	{
		return nullptr;
	}

	NumberParsedResult<float> coeffParseResult = getNumber<float>(coeffAsText);

	if (!coeffParseResult.isNumber || coeffParseResult.number < 0)
	{
		return nullptr;
	}

	Sound<T>* fileSound = new FileSound<T>(fileName, 50);

	FadeIn<T> fadeIn(coeffParseResult.number, fadeInSamples.number, fileSound->getSamplesLength());

	Sound<T>* fadedFileSound = new Effect<T, FadeIn<T>>(fileSound, fadeIn, 50);

	//Sound<T>* tryTransition = getNextSound(fadedFileSound, fileReader); Please check only the logic, no time to do it ;(

	return fadedFileSound;

	// if limit is passed explicitly
	// T limit = 12;
	// return new FileSound<T>(fileName, limit);
}

template <typename T>
Sound<T>* FactoryReader<T>::parseSound(const char* fileName, const char* coefficient) const
{
	if (!isFileNameValid(fileName))
	{
		return nullptr;
	}

	NumberParsedResult<float> coeffParseResult = getNumber<float>(coefficient);

	if (!coeffParseResult.isNumber || coeffParseResult.number < 0)
	{
		return nullptr;
	}

	Sound<T>* fileSound = new FileSound<T>(fileName, 50);

	VolumeIncreaser<T> volumeIncreaser(coeffParseResult.number);

	Sound<T>* fileSoundIncreasedVolume = new Effect<T, VolumeIncreaser<T>>(fileSound, volumeIncreaser, 50);

	//Sound<T>* tryTransition = getNextSound(fileSoundIncreasedVolume, fileReader); Please check only the logic, no time to do it ;(

	return fileSoundIncreasedVolume;
}

template <typename T>
template <typename U>
typename FactoryReader<T>::template NumberParsedResult<U> FactoryReader<T>::getNumber(const char* line) const
{
	NumberParsedResult<U> result;
	size_t processedCharacters;

	try
	{
		float parseResult = std::stof(line, &processedCharacters);

		if ((U)parseResult != parseResult)
		{
			result.isNumber = false;
			return result;
		}

		result.number = (U)parseResult;
	}
	catch (const std::invalid_argument&)
	{
		result.isNumber = false;
		return result;
	}

	if (line[processedCharacters])
	{
		result.isNumber = false;
	}

	return result;
}

template <typename T>
Sound<T>* FactoryReader<T>::getNextSound(const Sound<T>* firstSong, std::ifstream& fileReader) const
{
	if (fileReader.eof)
	{
		return nullptr;
	}

	unsigned index = fileReader.tellg();

	int transitionLength;
	int secondSongFadeIn;
	float zatihvane;

	fileReader >> transitionLength >> secondSongFadeIn >> zatihvane;

	if (transitionLength < 0 || secondSongFadeIn < 0 || zatihvane < 0)
	{
		return nullptr;
	}

	if (fileReader.eof)
	{
		fileReader.seekg(index, std::ios::beg);
		return nullptr;
	}

	int left = transitionLength - secondSongFadeIn;

	float diff = 1 - zatihvane;

	FadeOut<T> fadeOut(left, secondSongFadeIn, zatihvane, firstSong->getSamplesLength());

	Sound<T>* fadeOutFirst = new Effect<T, FadeOut<T>>(firstSong, fadeOut, 30);

	//Sound<T>* nextSound = readNext(); // no time ;(

	Sound<T>* secondSong = nullptr; // only for the logic

	FadeIn<T> fadeIn(zatihvane, secondSongFadeIn, secondSong->getSamplesLength());

	Sound<T>* fadeInSecond = new FadeIn<T, FadeOut<T>>(secondSong, fadeIn, 30);

	Silence<T> silence = new Silence<int>(firstSong->getSamplesLength() - left);

	Sound<T>** temp = new Sound<T>*[2] {silence, fadeInSecond};

	Sound<T>* periodic = new Sequence<int>(temp, 2, 50);

	Sound<T>** temp2 = new Sound<T>*[2] {fadeOutFirst, periodic};

	Sound<T>* transition = new Mix<T>(temp2, 2, 50);

	return transition;
}

template <typename T>
void FactoryReader<T>::setPointer(const char*& ptr, char*& line) const
{
	while (*line)
	{
		if (*line != ' ')
		{
			ptr = line;
			break;
		}

		line++;
	}

	while (*line)
	{
		if (*line == ' ')
		{
			*line = '\0';
			line++;
			break;
		}

		line++;
	}
}

template <typename T>
bool FactoryReader<T>::isTransition(const char* transitionLength, const char* secondSongSamples, const char* attenuationLevel) const
{
	//return result.isNumber;
	return 1;
}

template <typename T>
bool FactoryReader<T>::isLineOfWhitespaces(const char* firstElement, const char* secondElement, const char* thirdElement) const
{
	return !firstElement && !secondElement && !thirdElement;
}

template <typename T>
bool FactoryReader<T>::isFileNameValid(const char* fileName) const
{
	while (*fileName)
	{
		if (*fileName == '.' && *(fileName + 1) != '.')
		{
			return true;
		}

		fileName++;
	}

	return false;
}