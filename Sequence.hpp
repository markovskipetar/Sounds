#pragma once
#include "Sound.hpp"
#include "ISamples.hpp"

template <typename T>
class Sequence : public Sound<T>, ISamples<T>
{
public:
	Sequence(const Sound<T>* const* sounds, int soundsCount, T limit = DEFAULT_LIMIT_VALUE);
	T operator[](int index) const override;

	Sound<T>* clone() const override;

private:
	void setSamplesLength(const Sound<T>* const* sounds, unsigned soundsCount);
	void setSamples(const Sound<T>* const* sounds, unsigned soundsCount);

	unsigned calculateTotalLength(const Sound<T>* const* sounds, unsigned soundsCount) const;
	const T* generateSamples(const Sound<T>* const* sounds, unsigned soundsCount) const;
};

template<typename T>
Sequence<T>::Sequence(const Sound<T>* const* sounds, int soundsCount, T limit) : Sound<T>(ZERO, limit), ISamples<T>()
{
	Validator::ValidateDynamicMemory(sounds, false);
	Validator::ValidatePosiviteValue(soundsCount, NEGATIVE_VALUE_WHAT_ARG);

	setSamplesLength(sounds, soundsCount);
	setSamples(sounds, soundsCount);
}

template<typename T>
T Sequence<T>::operator[](int index) const
{
	T sample = ISamples<T>::operator[](index);

	return Sound<T>::clip(sample);
}

template<typename T>
void Sequence<T>::setSamplesLength(const Sound<T>* const* sounds, unsigned soundsCount)
{
	unsigned totalSamplesLength = calculateTotalLength(sounds, soundsCount);

	Sound<T>::setSamplesLength(totalSamplesLength);
}

template<typename T>
inline unsigned Sequence<T>::calculateTotalLength(const Sound<T>* const* sounds, unsigned soundsCount) const
{
	unsigned totalLength = 0;

	for (size_t i = 0; i < soundsCount; i++)
	{
		totalLength += sounds[i]->getSamplesLength();
	}

	return totalLength;
}

template<typename T>
inline void Sequence<T>::setSamples(const Sound<T>* const* sounds, unsigned soundsCount)
{
	const T* newSamples = generateSamples(sounds, soundsCount);

	unsigned newSamplesLength = Sound<T>::getSamplesLength();

	ISamples<T>::setSamples(newSamples, newSamplesLength);
}

template<typename T>
inline const T* Sequence<T>::generateSamples(const Sound<T>* const* sounds, unsigned soundsCount) const
{
	unsigned newSamplesLength = Sound<T>::getSamplesLength();

	T* newSamples = new (std::nothrow) T[newSamplesLength];

	Validator::ValidateDynamicMemory(newSamples);

	unsigned index = 0;

	for (size_t i = 0; i < soundsCount; i++)
	{
		unsigned currentSoundLength = sounds[i]->getSamplesLength();

		for (size_t j = 0; j < currentSoundLength; j++)
		{
			newSamples[index++] = (*sounds[i])[j];
		}
	}

	return newSamples;
}

template <typename T>
Sound<T>* Sequence<T>::clone() const
{
	return new Sequence<T>(*this);
}