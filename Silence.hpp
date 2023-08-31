#pragma once
#include "Sound.hpp"
#include "ISamples.hpp"

template <typename T>
class Silence : public Sound<T>
{
public:
	Silence(int samplesLength = ZERO);
	T operator[](int index) const override;

	Sound<T>* clone() const override;
};

template <typename T>
Silence<T>::Silence(int samplesLength)
	: Sound<T>(samplesLength, ZERO) {}

template <typename T>
T Silence<T>::operator[](int index) const
{
	return ZERO;
}

template <typename T>
Sound<T>* Silence<T>::clone() const
{
	return new Silence<T>(*this);
}