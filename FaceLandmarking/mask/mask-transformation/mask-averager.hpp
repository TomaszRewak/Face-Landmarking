#include "../face-mask.hpp"

namespace FaceLandmarking::Mask::MaskTransformation
{

	template<size_t N>
	class MaskAverager
	{
	private:
		FaceMask<N> avgMask;
		int count;

	public:
		MaskAverager() :
			count(0)
		{
			std::fill(avgMask.begin(), avgMask.end(), Math::Point<float>(0, 0));
		}

		void addMask(const FaceMask<N>& mask)
		{
			for (size_t i = 0; i < N; i++)
			{
				avgMask[i].x += mask[i].x;
				avgMask[i].y += mask[i].y;
			}

			count++;
		}

		FaceMask<N> getAvg() const
		{
			FaceMask<N> mask = avgMask;

			for (auto& point : mask)
			{
				point.x /= count;
				point.y /= count;
			}

			return mask;
		}
	};
}