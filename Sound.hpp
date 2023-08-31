#pragma once
#include <stdexcept>

#define ZERO 0
#define DEFAULT_LIMIT_VALUE 1

template <typename T>
class Sound
{
public:
	Sound(int samplesLength, T inputLimit);

	T virtual operator[](int index) const = 0;
	unsigned getSamplesLength() const;

	virtual Sound<T>* clone() const = 0;

	virtual ~Sound() = default;
	T getLimit() const;

protected:
	const char* getType() const;

	T clip(T sample) const;
	void setSamplesLength(int samplesLength);

private:
	int samplesLength;
	const T limit;
};


template <typename T>
Sound<T>::Sound(int samplesLength, T inputLimit)
	: limit(inputLimit < ZERO ? -inputLimit : inputLimit)
{
	setSamplesLength(samplesLength);
}

template <typename T>
unsigned Sound<T>::getSamplesLength() const
{
	return samplesLength;
}

template <typename T>
void Sound<T>::setSamplesLength(int samplesLength)
{
	if (samplesLength < ZERO)
	{
		throw std::invalid_argument("Samples length must be non-negative");
	}

	this->samplesLength = samplesLength;
}

template <typename T>
const char* Sound<T>::getType() const
{
	return samplesLength <= ZERO ? "Pause" : "Song";
}

template <typename T>
T Sound<T>::getLimit() const
{
	return limit;
}

template <typename T>
T Sound<T>::clip(T sample) const
{
	if (sample < -limit)
	{
		return -limit;
	}
	else if (sample > limit)
	{
		return limit;
	}

	return sample;
}

