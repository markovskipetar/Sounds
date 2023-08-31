#pragma once
#include <utility>
#include <stdexcept>
#include "Validator.hpp"

template <typename T>
class ISamples
{
protected:
	ISamples();
	ISamples(const T* samples, unsigned samplesLength);

	ISamples(const ISamples<T>& other);
	ISamples& operator=(const ISamples<T>& other);

	~ISamples();

	T operator[](int index) const; // 'T' can be repalced with 'const T&' if working with abstract data types

protected:
	void setSamples(const T*& samples, unsigned samplesLength);

private:
	const T* samples;
	unsigned samplesLength;

	void free();
	void copyFrom(const ISamples<T>& other);
};

template<typename T>
ISamples<T>::ISamples() : samples(nullptr) {}

template<typename T>
ISamples<T>::ISamples(const T* samples, unsigned samplesLength)
{
	T* newSamples = new (std::nothrow) T[samplesLength];

	Validator::ValidateDynamicMemory(newSamples);

	for (size_t i = 0; i < samplesLength; i++)
	{
		newSamples[i] = samples[i];
	}

	this->samples = newSamples;
	this->samplesLength = samplesLength;
}

template<typename T>
ISamples<T>::ISamples(const ISamples<T>& other)
{
	copyFrom(other);
}

template<typename T>
ISamples<T>& ISamples<T>::operator=(const ISamples<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}

	return *this;
}

template<typename T>
ISamples<T>::~ISamples()
{
	free();
}

template <typename T>
void ISamples<T>::free()
{
	delete[] samples;
	samples = nullptr;
}

template <typename T>
void ISamples<T>::copyFrom(const ISamples<T>& other)
{
	samplesLength = other.samplesLength;

	T* samplesCopy = new T[samplesLength];

	for (size_t i = 0; i < samplesLength; i++)
	{
		samplesCopy[i] = other.samples[i];
	}

	this->samples = samplesCopy;
}

template <typename T>
T ISamples<T>::operator[](int index) const
{
	if (index < 0 || index >= samplesLength)
	{
		throw std::invalid_argument("Index out of range!");
	}

	return samples[index];
}

template <typename T>
void ISamples<T>::setSamples(const T*& samples, unsigned samplesLength)
{
	this->samples = samples;
	samples = nullptr;

	this->samplesLength = samplesLength;
}
