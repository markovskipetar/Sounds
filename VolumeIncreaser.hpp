#pragma once

template <typename U>
class VolumeIncreaser
{
public:
	VolumeIncreaser() = default;

	VolumeIncreaser(float coefficient)
	{
		this->coefficient = coefficient;
	}

	U operator()(U sample, unsigned index) const
	{
		return sample * coefficient;
	}

private:
	float coefficient;
};