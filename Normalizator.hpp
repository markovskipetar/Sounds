#pragma once
#include "Sound.hpp"

template <typename T>
class Normalizator
{
public:
	Normalizator() = default;
	Normalizator(const Sound<T>* sound);
	Normalizator(const Sound<T>* sound, T limit);

	T operator()(T sample, int index) const;

private:
	T limit;
	T coefficient;

private:
	void setCoefficient(const Sound<T>* sound);
	T getMaxSample(const Sound<T>* sound);
};

template <typename T>
Normalizator<T>::Normalizator(const Sound<T>* sound) : Normalizator(sound, sound->getLimit()) {}

template <typename T>
Normalizator<T>::Normalizator(const Sound<T>* sound, T limit)
{
	this->limit = limit < 0 ? -limit : limit;

	setCoefficient(sound);
}

template <typename T>
void Normalizator<T>::setCoefficient(const Sound<T>* sound)
{
	T maxSample = getMaxSample(sound);

	coefficient = limit / maxSample;
}

template <typename T>
T Normalizator<T>::getMaxSample(const Sound<T>* sound)
{
	unsigned samplesLength = sound->getSamplesLength();

	T maxSample = std::abs((*sound)[0]);

	for (size_t i = 1; i < samplesLength; i++)
	{
		T currentSample = std::abs((*sound)[i]);

		if (currentSample > maxSample)
		{
			maxSample = currentSample;
		}
	}

	return maxSample;
}

template <typename T>
T Normalizator<T>::operator()(T sample, int index) const
{
	return sample * coefficient;
}