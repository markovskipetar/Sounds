#pragma once
#include <iostream>
#include "FileSound.hpp"
#include "Silence.hpp"
#include "Periodic.hpp"
#include "Mix.hpp"
#include "Sequence.hpp"
#include <cstdlib>
#include "Effect.hpp"
#include "Normalizator.hpp"
#include <sstream>
#include "Playlist.hpp"

void testNormalization()
{
	int* arr = new int [3] {2, -4, 6};

	std::ofstream file("numbers.dat", std::ios::binary);

	file.write((const char*)arr, 12);

	file.flush();
	file.close();

	Sound<int>* fileSound = new FileSound<int>("numbers.dat", 10);

	Normalizator<int> normalizator(fileSound, 12);
	//Normalizator<int> normalizator(fileSound);

	Sound<int>* normalizedFileSound = new Effect<int, Normalizator<int>>(fileSound, normalizator, 15);
	//Sound<int>* normalizedFileSound = new Effect<int, Normalizator<int>>(fileSound, normalizator, 5);

	for (size_t i = 0; i < normalizedFileSound->getSamplesLength(); i++)
	{
		std::cout << (*normalizedFileSound)[i] << " ";
	}
}

void testNullptr()
{
	try
	{
		//Sound<int>* fileSound = new FileSound<int>("nullptr", 6);
		//Sound<int>* periodic = new Periodic<int>(nullptr, 5, 3, 10);
		Sound<int>* mix = new Mix<int>(nullptr, 3, 10);
	}
	catch (const std::invalid_argument& exc)
	{
		std::cout << exc.what() << std::endl;
	}
	catch (const std::runtime_error& exc)
	{
		std::cout << exc.what() << std::endl;
	}
}

void testFileSound()
{
	int* arr = new int [5] {1, -12, 4, -6, 10};

	std::ofstream file("numbers.dat", std::ios::binary);

	file.write((const char*)arr, 20);

	file.flush();
	file.close();

	Sound<int>* fileSound = new FileSound<int>("numbers.dat", 6);
	//Sound<int>* fileSound = new FileSound<int>("numbers.dat");

	for (size_t i = 0; i < 5; i++)
	{
		std::cout << (*fileSound)[i] << " ";
	}

	std::cout << std::endl;

	Sound<int>* fileSound2 = fileSound->clone();

	delete fileSound;

	for (size_t i = 0; i < 5; i++)
	{
		std::cout << (*fileSound2)[i] << " ";
	}

	std::cout << std::endl;
}

void testPeriodic()
{
	int* arr = new int [5] {9, 6, -14, -6, 13};

	Sound<int>* periodic = new Periodic<int>(arr, 5, 3, 10);
	//Sound<int>* periodic = new Periodic<int>(arr, 5, 3);

	for (size_t i = 0; i < periodic->getSamplesLength(); i++)
	{
		std::cout << (*periodic)[i] << " ";
	}

	std::cout << std::endl;

	Sound<int>* periodic2 = periodic->clone();

	delete periodic;

	for (size_t i = 0; i < periodic2->getSamplesLength(); i++)
	{
		std::cout << (*periodic2)[i] << " ";
	}

	std::cout << std::endl;
}

void testMix()
{
	int* arr = new int [3] {-7, -3, -9};

	std::ofstream file("mixnumbers.dat", std::ios::binary);

	file.write((const char*)arr, 12);

	file.flush();
	file.close();

	Sound<int>* fileSound = new FileSound<int>("mixnumbers.dat", 10);

	int* arr2 = new int[2] {-3, 4};

	Sound<int>* periodic = new Periodic<int>(arr2, 2, 2, 10);

	int* arr3 = new int[2] {6, -1};

	Sound<int>* periodic2 = new Periodic<int>(arr3, 2, 1, 10);

	Sound<int>** playlist = new Sound<int>*[3] {fileSound, periodic, periodic2};

	Sound<int>* mix = new Mix<int>(playlist, 3, 2);

	for (size_t i = 0; i < mix->getSamplesLength(); i++)
	{
		std::cout << (*mix)[i] << " ";
	}
}

void testSequence()
{
	int* arr = new int [3] {-7, -3, -9};

	std::ofstream file("mixnumbers.dat", std::ios::binary);

	file.write((const char*)arr, 12);

	file.flush();
	file.close();

	Sound<int>* fileSound = new FileSound<int>("mixnumbers.dat", 10);

	int* arr2 = new int[2] {-3, 4};
	Sound<int>* periodic = new Periodic<int>(arr2, 2, 2, 10);

	int* arr3 = new int[2] {6, -1};
	Sound<int>* periodic2 = new Periodic<int>(arr3, 2, 1, 10);

	Sound<int>** playlist = new Sound<int>*[3] {fileSound, periodic, periodic2};

	Sound<int>* sequence = new Sequence<int>(playlist, 3, 30);
	//Sound<int>* sequence = new Sequence<int>(playlist, 3, 3);

	for (size_t i = 0; i < sequence->getSamplesLength(); i++)
	{
		std::cout << (*sequence)[i] << " ";
	}
}

template <typename U>
struct Mult
{
	U operator()(U sample, int mult) const
	{
		return sample * 2;
	}
};

template <typename U>
U mult(U element, int index)
{
	return element * 2;
}

template <typename U>
U reverse(U element, int index)
{
	return -element;
}

void testEffect()
{
	//Mult<int> multFunc;

	int* arr = new int [3] {-7, -3, -9};

	std::ofstream file("55.55.33", std::ios::binary);

	file.write((const char*)arr, 12);

	file.flush();
	file.close();

	Sound<int>* fileSound = new FileSound<int>("55.55.33", 10);

	//Sound<int>* effect = new Effect<int, Mult<int>>(fileSound, multFunc,30); // if using class, its operator() must be const
	Sound<int>* reversedFileSound = new Effect<int, int(*)(int, int)>(fileSound, reverse, 30);

	Sound<int>** playlist = new Sound<int>*[2] {fileSound, reversedFileSound};

	Sound<int>* mute = new Mix<int>(playlist, 2, 30);

	for (size_t i = 0; i < mute->getSamplesLength(); i++)
	{
		std::cout << (*mute)[i];
	}
}

void f(int*& ptr)
{
	ptr = new int[1] {999};
}

int main()
{



	int* per = new int[3] {1, 2, 3};
	Sound<int>* perS = new Periodic<int>(per, 3, 4, 15);

	int* arr = new int [8] {7, 2, 4, 9, 4, 3, 6, 9};

	std::ofstream file("numbers.dat", std::ios::binary);
	file.write((const char*)arr, 32);

	file.flush();
	file.close();

	try
	{
		Playlist<int> playlist("playlist.txt");

		playlist.writeToFile(); //wwrites in text file - test purpose onlue

		playlist.play();

		Sound<int>* fileSound = new FileSound<int>("defaultFile.dat", 50);

		for (size_t i = 0; i < fileSound->getSamplesLength(); i++)
		{
			std::cout << fileSound->operator[](i) << " ";
		}

	}
	catch (const std::exception& e)
	{
		std::cout << e.what();
	}


	//testEffect();

	//testNormalization();

	//testEffect();

	//testMix();

	//testNullptr();

	//testFileSound();
	//std::cout << "--------------------------" << std::endl;
	//testPeriodic();*/
}
