#pragma once
#include "Sound.hpp"

template <typename T, typename U>
class Effect : public Sound<T>
{
public:
	Effect(const Sound<T>* sound, U transform, T limit = DEFAULT_LIMIT_VALUE);
	T operator[](int index)  const override;

	Effect(const Effect<T, U>& other);
	Effect<T, U>& operator=(const Effect<T, U>& other);

	~Effect();

	Sound<T>* clone() const override;

private:
	const Sound<T>* sound;
	U transform;

	void setSound(const Sound<T>* sound);

	void copyFrom(const Effect<T, U>& other);
	void free();
};

template <typename T, typename U>
Effect<T, U>::Effect(const Sound<T>* sound, U transform, T limit) : Sound<T>(ZERO, limit), sound(nullptr), transform(transform)
{
	setSound(sound);

	Sound<T>::setSamplesLength(sound->getSamplesLength());
}

template <typename T, typename U>
void Effect<T, U>::setSound(const Sound<T>* sound)
{
	Validator::ValidateDynamicMemory(sound, false);

	this->sound = sound->clone();
}

template <typename T, typename U>
T Effect<T, U>::operator[](int index) const
{
	if (index < 0 || index >= sound->getSamplesLength())
	{
		throw std::invalid_argument("Index out of range!");
	}

	T transformedSample = transform((*sound)[index], index);

	return Sound<T>::clip(transformedSample);
}

template<typename T, typename U>
inline Effect<T, U>::Effect(const Effect<T, U>& other) : Sound<T>(other.sound->getSamplesLength(), other.sound->getLimit())
{
	copyFrom(other);
}

template<typename T, typename U>
inline Effect<T, U>& Effect<T, U>::operator=(const Effect<T, U>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}

	return *this;
}

template<typename T, typename U>
inline Effect<T, U>::~Effect()
{
	free();
}

template<typename T, typename U>
inline void Effect<T, U>::copyFrom(const Effect<T, U>& other)
{
	sound = other.sound->clone();
	transform = other.transform;
}

template<typename T, typename U>
inline void Effect<T, U>::free()
{
	delete sound;
}

template<typename T, typename U>
Sound<T>* Effect<T, U>::clone() const
{
	return new Effect(*this);
}