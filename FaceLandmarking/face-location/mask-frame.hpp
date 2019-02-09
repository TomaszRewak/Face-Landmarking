#pragma once

#include "mask-frame.hpp"
#include "../FaceLandmarking/mask-transformation/mask-normalizer.hpp"
#include "../FaceLandmarking/math/rect.hpp"
#include "../FaceLandmarking/math/offset.hpp"

namespace FaceLandmarking::FaceLocation
{
	template<size_t N>
	class MaskFrame
	{
	private:
		Math::Size<float> desiredSize;
		Math::Offset<float> defaultOffset;

		Math::Rect<float> getRect(const FaceMask<N> &mask) const
		{
			float
				minX = std::numeric_limits<float>::max(),
				maxX = std::numeric_limits<float>::min(),
				minY = std::numeric_limits<float>::max(),
				maxY = std::numeric_limits<float>::min();

			for (size_t i = 0; i < N; i++)
			{
				minX = std::min(minX, mask[i].x);
				maxX = std::max(maxX, mask[i].x);
				minY = std::min(minY, mask[i].y);
				maxY = std::max(maxY, mask[i].y);
			}

			return Math::Rect<float>(
				Math::Point<float>((minX + maxX) / 2, (minY + maxY) / 2),
				Math::Size<float>(maxX - minX, maxY - minY)
			);
		}

	public:
		MaskFrame(const FaceMask<N> &averageMask, Math::Size<float> desiredSize)
		{
			Math::Rect<float> fullRect = averageMask.faceRect();
			Math::Rect<float> partialRect = getRect(averageMask);

			defaultOffset.left = std::abs(fullRect.left() - partialRect.left()) / fullRect.size.width * desiredSize.width;
			defaultOffset.right = std::abs(fullRect.right() - partialRect.right()) / fullRect.size.width * desiredSize.width;
			defaultOffset.top = std::abs(fullRect.top() - partialRect.top()) / fullRect.size.height * desiredSize.height;
			defaultOffset.down = std::abs(fullRect.down() - partialRect.down()) / fullRect.size.height * desiredSize.height;

			desiredSize.width *= partialRect.size.width / fullRect.size.width;
			desiredSize.height *= partialRect.size.height / fullRect.size.height;

			this->desiredSize = desiredSize;
		}

		float getScale(const FaceMask<N>& mask) const
		{
			Math::Size<float> currentSize = getRect(mask).size;

			return std::min(
				std::max(
					desiredSize.width / currentSize.width,
					desiredSize.height / currentSize.height
				),
				2.f
			);
		}

		Math::Rect<float> getFrame(const FaceMask<N>& mask) const
		{
			Math::Rect<float> partRect = getRect(mask);

			float scale = getScale(mask);
			partRect += defaultOffset * scale;

			return partRect;
		}
	};
}