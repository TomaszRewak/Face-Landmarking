#pragma once

#include "mask-frame.hpp"
#include "../FaceLandmarking/mask-info/mask-description.hpp"
#include "../FaceLandmarking/mask-transformation/mask-normalizer.hpp"
#include "../FaceLandmarking/math/rect.hpp"

namespace FaceLandmarking::FaceLocator
{
	class MaskFrame
	{
	private:
		const MaskInfo::MaskDescription& maskDescription;

		Math::Size<float> desiredSize;
		
		Math::Size<float> getSize(const FaceMask &mask) const
		{
			float
				minX = std::numeric_limits<float>::max(),
				maxX = std::numeric_limits<float>::min(),
				minY = std::numeric_limits<float>::max(),
				maxY = std::numeric_limits<float>::min();

			for (size_t i = 0; i < mask.size(); i++)
			{
				if (maskDescription.points[i].inUse)
				{
					minX = std::min(minX, mask[i].x);
					maxX = std::max(maxX, mask[i].x);
					minY = std::min(minY, mask[i].y);
					maxY = std::max(maxY, mask[i].y);
				}
			}

			return Math::Size<float>(maxX - minX, maxY - minY);
		}

	public:
		MaskFrame(const FaceMask &averageMask, const MaskInfo::MaskDescription& maskDescription, Math::Size<float> desiredSize) :
			maskDescription(maskDescription)
		{
			Math::Size<float> fullSize = averageMask.faceSize();
			Math::Size<float> partialSize = getSize(averageMask);

			desiredSize.width *= partialSize.width / fullSize.width;
			desiredSize.height *= partialSize.height / fullSize.height;

			this->desiredSize = desiredSize;
		}

		float getScale(const FaceMask& mask) const
		{
			Math::Size<float> currentSize = getSize(mask);

			return std::min(
				std::max(
					desiredSize.width / currentSize.width,
					desiredSize.height / currentSize.height
				), 
				2.f
			);
		}
	};
}