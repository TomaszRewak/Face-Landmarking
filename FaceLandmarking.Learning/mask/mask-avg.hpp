#pragma once

#include "../../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::Learning::Mask
{
	class AverageMask
	{
	private:
		FaceMask avgMask;
		int count;

		void tryInitialize(const FaceMask& mask)
		{
			if (avgMask.size() == 0)
			{
				avgMask.resize(mask.size(), Math::Point<float>(0, 0));
			}
			else if (mask.size() != avgMask.size())
				throw "Points count doesn't match.";
		}

	public:
		AverageMask() :
			count(0)
		{ }

		void addMask(const FaceMask& mask)
		{
			tryInitialize(mask);

			for (size_t i = 0; i < mask.size(); i++)
			{
				avgMask[i].x += mask[i].x;
				avgMask[i].y += mask[i].y;
			}

			count++;
		}

		FaceMask getAvg() const
		{
			FaceMask mask = avgMask;

			for (auto& point : mask)
			{
				point.x /= count;
				point.y /= count;
			}

			return mask;
		}
	};
}