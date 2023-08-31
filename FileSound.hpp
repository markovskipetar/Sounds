#pragma once
#include "Sound.hpp"
#include "ISamples.hpp"
#include "fstream"
#include "Validator.hpp"

const char* READING_SAMPLES_LENGTH_WHAT_ARG = "Reading samples length failed!";
const char* READING_SAMPLES_WHAT_ARG = "Reading samples failed!";

template <typename T>
class FileSound : public Sound<T>, public ISamples<T>
{
public:
	FileSound(const char* fileName, T limit = DEFAULT_LIMIT_VALUE);
	T operator[](int index) const override;

	Sound<T>* clone() const override;

private:
	void setSamplesLength(const char* fileName, std::ifstream& fileReader);
	void setSamples(const char* fileName, std::ifstream& fileReader);

	unsigned readSamplesLengthFromFile(const char* fileName, std::ifstream& fileReader) const;
	const T* readSamplesFromFile(std::ifstream& fileReader, unsigned samplesLength) const;
};

template <typename T>
FileSound<T>::FileSound(const char* fileName, T limit) : Sound<T>(ZERO, limit), ISamples<T>()
{
	Validator::ValidateFileName(fileName);

	std::ifstream fileReader(fileName, std::ios::binary);

	setSamplesLength(fileName, fileReader);
	setSamples(fileName, fileReader);

	fileReader.close();
}

template <typename T>
void FileSound<T>::setSamplesLength(const char* fileName, std::ifstream& fileReader)
{
	unsigned samplesLength = readSamplesLengthFromFile(fileName, fileReader);

	Sound<T>::setSamplesLength(samplesLength);
}

template <typename T>
unsigned FileSound<T>::readSamplesLengthFromFile(const char* fileName, std::ifstream& fileReader) const
{
	Validator::ValidteStream(fileReader, READING_SAMPLES_LENGTH_WHAT_ARG);

	unsigned bytesCount = fileReader.seekg(ZERO, std::ios::end).tellg();

	fileReader.seekg(ZERO, std::ios::beg);

	return bytesCount / sizeof(T);
}

template <typename T>
void FileSound<T>::setSamples(const char* fileName, std::ifstream& fileReader)
{
	unsigned samplesLength = Sound<T>::getSamplesLength();

	const T* samples = readSamplesFromFile(fileReader, samplesLength);

	ISamples<T>::setSamples(samples, samplesLength);
}

template <typename T>
const T* FileSound<T>::readSamplesFromFile(std::ifstream& fileReader, unsigned samplesLength) const
{
	Validator::ValidteStream(fileReader, READING_SAMPLES_WHAT_ARG);

	T* samples = new (std::nothrow) T[samplesLength];

	Validator::ValidateDynamicMemory(samples);

	fileReader.read((char*)samples, samplesLength * sizeof(T));

	return samples;
}

template <typename T>
T FileSound<T>::operator[](int index) const
{
	T sample = ISamples<T>::operator[](index);

	return Sound<T>::clip(sample);
}

template <typename T>
Sound<T>* FileSound<T>::clone() const
{
	return new FileSound<T>(*this);
}