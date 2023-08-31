#pragma once

template <typename T>
class FadeOut
{
public:
	FadeOut(unsigned left, unsigned right, float zatihvane, unsigned sampleLength)
	{
		this->left = left;
		this->right = right;
		this->zatihvane = zatihvane;
		this->sampleLength = sampleLength;

		step = (1 - zatihvane) / (left - 1);

		stepToZero = (1 - step) / right;
	}

	T operator() (T sample, int index)
	{
		int currentIndex = sampleLength - index;

		if (currentIndex >= left && currentIndex < right)
		{
			return sample * (currentIndex * step);
		}
		else if (currentIndex >= right)
		{
			return sample * (currentIndex * stepToZero);
		}

		return sample;
	}

private:
	unsigned left;
	unsigned right;
	unsigned sampleLength;
	float zatihvane;

	float step;
	float stepToZero;
};