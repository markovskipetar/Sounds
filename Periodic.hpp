#pragma once
#include "Sound.hpp"
#include "ISamples.hpp"

template <typename T>
class Periodic : public Sound<T>, public ISamples<T>
{
public:
	Periodic(const T* samples, int initialSamplesLength, int repeats, T limit = DEFAULT_LIMIT_VALUE);
	T operator[](int index) const override;

	Sound<T>* clone() const override;

private:
	const int initialSamplesLength;
	const int repeats;

	void setSamplesLength(int samplesLength, int repeats);
	void setSamples(const T* samples);
};

template <typename T>
void Periodic<T>::setSamplesLength(int initialSamplesLength, int repeats)
{
	if (initialSamplesLength < 0 && repeats < 0)
	{
		initialSamplesLength *= -1;
	}

	Sound<T>::setSamplesLength(initialSamplesLength * repeats);
}

template <typename T>
Periodic<T>::Periodic(const T* samples, int initialSamplesLength, int repeats, T limit) :Sound<T>(ZERO, limit), ISamples<T>(), initialSamplesLength(initialSamplesLength), repeats(repeats)
{
	Validator::ValidateDynamicMemory(samples, false);

	setSamplesLength(initialSamplesLength, repeats);
	setSamples(samples);
}

template <typename T>
void Periodic<T>::setSamples(const T* samples)
{
	T* samplesCopy = new (std::nothrow) T[initialSamplesLength];

	Validator::ValidateDynamicMemory(samplesCopy);

	for (size_t i = 0; i < initialSamplesLength; i++)
	{
		samplesCopy[i] = samples[i];
	}

	const T* samplesCopyConst = samplesCopy;

	ISamples<T>::setSamples(samplesCopyConst, initialSamplesLength);
}

template <typename T>
T Periodic<T>::operator[](int index) const
{
	if (index < 0 || index >= initialSamplesLength * repeats)
	{
		index = -(initialSamplesLength + 1);
	}

	if (initialSamplesLength == 1)
	{
		index = 0;
	}

	T sample = ISamples<T>::operator[](index% initialSamplesLength);

	return Sound<T>::clip(sample);
}

template <typename T>
Sound<T>* Periodic<T>::clone() const
{
	return new Periodic<T>(*this);
}