#pragma once
#include "Sound.hpp"
#include "ISamples.hpp"

const char* NEGATIVE_VALUE_WHAT_ARG = "Sounds count must be positive number!";

template <typename T>
class Mix : public Sound<T>, public ISamples<T>
{
public:
	Mix(const Sound<T>* const*, int soundsCount, T limit = DEFAULT_LIMIT_VALUE);
	T operator[](int index) const override;

	Sound<T>* clone() const override;

private:
	void setSamples(const Sound<T>* const* sounds, unsigned soundsCount);
	void setSamplesLength(const Sound<T>* const* sounds, unsigned soundsCount);

	unsigned getLongestSoundSamplesLength(const Sound<T>* const* sounds, unsigned soundsCount) const;
	const T* generateSamples(const Sound<T>* const* sounds, unsigned soundsCount) const;

	T calculateAverageSample(const Sound<T>* const* sounds, int soundsCount, unsigned index) const;
};

template <typename T>
Mix<T>::Mix(const Sound<T>* const* sounds, int soundsCount, T limit) : Sound<T>(ZERO, limit), ISamples<T>()
{
	Validator::ValidateDynamicMemory(sounds, false);
	Validator::ValidatePosiviteValue(soundsCount, NEGATIVE_VALUE_WHAT_ARG);

	setSamplesLength(sounds, soundsCount);
	setSamples(sounds, soundsCount);
}

template <typename T>
T Mix<T>::operator[](int index) const
{
	T sample = ISamples<T>::operator[](index);

	return Sound<T>::clip(sample);
}

template <typename T>
void Mix<T>::setSamplesLength(const Sound<T>* const* sounds, unsigned soundsCount)
{
	unsigned longestSoundSamplesLength = getLongestSoundSamplesLength(sounds, soundsCount);

	Sound<T>::setSamplesLength(longestSoundSamplesLength);
}

template <typename T>
unsigned Mix<T>::getLongestSoundSamplesLength(const Sound<T>* const* sounds, unsigned soundsCount) const
{
	unsigned longestSoundSamplesLength = sounds[0]->getSamplesLength();

	for (size_t i = 1; i < soundsCount; i++)
	{
		unsigned currentSongSamplesLength = sounds[i]->getSamplesLength();

		if (currentSongSamplesLength > longestSoundSamplesLength)
		{
			longestSoundSamplesLength = currentSongSamplesLength;
		}
	}

	return longestSoundSamplesLength;
}

template <typename T>
void Mix<T>::setSamples(const Sound<T>* const* sounds, unsigned soundsCount)
{
	const T* newSamples = generateSamples(sounds, soundsCount);

	unsigned newSamplesLength = Sound<T>::getSamplesLength();

	ISamples<T>::setSamples(newSamples, newSamplesLength);
}

template <typename T>
const T* Mix<T>::generateSamples(const Sound<T>* const* sounds, unsigned soundsCount) const
{
	unsigned newSamplesLength = Sound<T>::getSamplesLength();

	T* newSamples = new (std::nothrow) T[newSamplesLength];

	Validator::ValidateDynamicMemory(newSamples);

	for (size_t currentIndex = 0; currentIndex < newSamplesLength; currentIndex++)
	{
		newSamples[currentIndex] = calculateAverageSample(sounds, soundsCount, currentIndex);
	}

	return newSamples;
}

template <typename T>
T Mix<T>::calculateAverageSample(const Sound<T>* const* sounds, int soundsCount, unsigned index) const
{
	T currentSample = 0;

	for (size_t j = 0; j < soundsCount; j++)
	{
		try
		{
			currentSample += (*sounds[j])[index];
		}
		catch (const std::invalid_argument& exc) {}
	}

	return currentSample / soundsCount;
}

template <typename T>
Sound<T>* Mix<T>::clone() const
{
	return new Mix<T>(*this);
}