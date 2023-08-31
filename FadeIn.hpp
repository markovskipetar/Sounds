#pragma once

template <typename U>
class FadeIn
{
public:
	FadeIn() = default;

	FadeIn(float coefficient, unsigned fadeInLength, unsigned samplesLength)
	{
		this->coefficient = coefficient;
		this->fadeInLength = fadeInLength;

		this->step = coefficient / (fadeInLength - 2);
	}

	U operator()(U sample, unsigned index) const
	{
		if (index >= fadeInLength - 1)
		{
			return sample * coefficient;
		}

		return sample * (index * step);
	}

private:
	float coefficient;
	unsigned fadeInLength;
	float step;
};