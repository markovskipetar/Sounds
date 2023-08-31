#pragma once
#include "Sound.hpp"
#include "Validator.hpp"
#include <iostream>
#include "FactoryReader.hpp"

const int INITIAL_COUNT = 0;
const int INITIAL_CAPACITY = 8;
const char* FAILED_READING_WHAT_ARG = "Reading songs from file failed!";

template <typename T>
class Playlist
{
public:
	Playlist(const char* fileName);

	Playlist(const Playlist<T>& other);
	Playlist<T>& operator=(const Playlist<T>& other);

	const Sound<T>* operator[] (int index) const;
	Sound<T>*& operator[] (int index);
	Sound<T>* operator * () const;

	void writeToFile() const; // only for test purposes

	void play(const char* fileName = "defaultFile.dat") const;

	~Playlist();

private:
	Sound<T>** sounds;

	unsigned count;
	unsigned capacity;

private:
	void initiazizeSongs();

	void parsePlaylsitFromFile(const char* fileName);
	void addSound(Sound<T>* sound);

	void copyFrom(const Playlist<T>& other);
	void free();

	void resize(unsigned newCapacity);
};

template <typename T>
Playlist<T>::Playlist(const char* fileName) : sounds(nullptr)
{
	Validator::ValidateFileName(fileName);

	initiazizeSongs();

	parsePlaylsitFromFile(fileName);
}

template <typename T>
Playlist<T>::Playlist(const Playlist<T>& other)
{
	copyFrom(other);
}

template <typename T>
Playlist<T>& Playlist<T>::operator=(const Playlist<T>& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}

	return *this;
}

template <typename T>
Sound<T>* Playlist<T>::operator *() const
{
	T maxLimit = -1;

	for (size_t i = 0; i < count; i++)
	{
		if (sounds[i]->getLimit() > maxLimit)
		{
			maxLimit = sounds[i]->getLimit();
		}
	}

	return new Sequence<T>(sounds, count, maxLimit);
}

template <typename T>
void Playlist<T>::copyFrom(const Playlist<T>& other)
{
	capacity = other.capacity;
	count = other.count;

	sounds = new Sound<T>*[capacity];

	for (size_t i = 0; i < count; i++)
	{
		sounds[i] = other.sounds[i]->clone();
	}
}

template <typename T>
const Sound<T>* Playlist<T>::operator[] (int index) const
{
	if (index < 0 || index >= count)
	{
		return nullptr;
	}

	return sounds[index];
}

template <typename T>
Sound<T>*& Playlist<T>::operator[] (int index)
{
	return sounds[index];
}

template <typename T>
void Playlist<T>::initiazizeSongs()
{
	count = INITIAL_COUNT;
	capacity = INITIAL_CAPACITY;

	sounds = new (std::nothrow) Sound<T>*[capacity] {nullptr};

	Validator::ValidateDynamicMemory(sounds);
}

template <typename T>
void Playlist<T>::addSound(Sound<T>* soundToAdd)
{
	if (count >= capacity)
	{
		resize(capacity * 1.5);
	}

	sounds[count++] = soundToAdd;
}

template <typename T>
void Playlist<T>::parsePlaylsitFromFile(const char* fileName)
{
	FactoryReader<T>* factoryReader = FactoryReader<T>::getInstance();

	std::ifstream fileReader(fileName);

	Validator::ValidteStream(fileReader, FAILED_READING_WHAT_ARG);

	while (true)
	{
		Sound<T>* currentSound = factoryReader->readSound(fileReader);

		if (!currentSound)
		{
			free();
			sounds = nullptr;
			break;
		}

		addSound(currentSound);

		if (fileReader.eof())
		{
			break;
		}
	}

	fileReader.close();
}

template <typename T>
void Playlist<T>::writeToFile() const
{
	if (!sounds)
	{
		std::cout << "INVALID SOUNDS!" << std::endl;

		return;
	}

	std::ofstream writer("output.txt");

	for (size_t i = 0; i < count; i++)
	{
		writer << "[ " << sounds[i]->getSamplesLength() << " ]:";

		for (size_t j = 0; j < sounds[i]->getSamplesLength(); j++)
		{
			writer << sounds[i]->operator[](j) << " ";
		}

		writer << std::endl;
	}

	writer.flush();
	writer.close();
}

template <typename T>
void Playlist<T>::resize(unsigned newCapacity)
{
	capacity = newCapacity;

	Sound<T>** newSounds = new Sound<T>*[capacity];

	for (size_t i = 0; i < count; i++)
	{
		newSounds[i] = sounds[i];
		sounds[i] = nullptr;
	}

	delete[] sounds;

	sounds = newSounds;
}

template <typename T>
void Playlist<T>::free()
{
	for (size_t i = 0; i < count; i++)
	{
		delete[] sounds[i];
		sounds[i] = nullptr;
	}

	delete[] sounds;
}

template <typename T>
void Playlist<T>::play(const char* fileName) const
{
	std::ofstream fileWriter(fileName, std::ios::binary);

	if (!fileWriter.good())
	{
		return;
	}

	for (size_t i = 0; i < count; i++)
	{
		unsigned currentSoundLength = sounds[i]->getSamplesLength();

		for (size_t j = 0; j < currentSoundLength; j++)
		{
			T sample = (*sounds[i])[j];
			fileWriter.write((const char*)&sample, sizeof(T));
		}
	}

	fileWriter.flush();
	fileWriter.close();
}

template <typename T>
Playlist<T>::~Playlist()
{
	free();
}