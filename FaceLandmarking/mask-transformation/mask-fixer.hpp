#pragma once

#include "../../FaceLandmarking/math/angle.hpp"
#include "../face-mask.hpp"
#include "../mask-info/mask-description.hpp"
#include "../mask-info/mask-limits.hpp"

namespace FaceLandmarking::MaskTransformation
{
	class MaskFixer
	{
	private:
		MaskInfo::MaskDescription maskDescription;
		MaskInfo::MaskLimits maskLimits;
		MaskTransformation::MaskOffset maskOffset;

		float factor(float value, const MaskInfo::Threshold& threshold)
		{
			if (value < threshold[0])
				return -2;
			if (value < threshold[1])
				return -1;
			if (value < threshold[25])
				return -0.5;
			if (value < threshold[40])
				return -0.1;

			if (value > threshold[100])
				return 2;
			if (value > threshold[99])
				return 1;
			if (value > threshold[75])
				return 0.5;
			if (value > threshold[60])
				return 0.1;

			return 0;
		}

		void fixAngle(FaceMask &mask)
		{
			for (size_t shapeIndex = 0; shapeIndex < maskDescription.shapes.size(); shapeIndex++)
			{
				const auto& shape = maskDescription.shapes[shapeIndex];

				for (size_t pointIndex = shape.firstEnclosed(); pointIndex <= shape.lastEnclosed(); pointIndex++)
				{
					auto& offsetPrev = maskOffset[shape.point(pointIndex - 1)];
					auto& offset = maskOffset[shape.point(pointIndex)];
					auto& offsetNext = maskOffset[shape.point(pointIndex + 1)];

					Math::Point<float>
						a = mask[shape.point(pointIndex - 1)],
						b = mask[shape.point(pointIndex)],
						c = mask[shape.point(pointIndex + 1)];

					float length = b.to(c).d() / a.to(b).d();
					float lengthFactor = factor(length, maskLimits.shapeLimits[shapeIndex].lengthThresholds[pointIndex]);

					auto vc = b.to(c).normalized() * std::abs(lengthFactor) * 0.25;
					auto va = b.to(a).normalized() * std::abs(lengthFactor) * 0.25;

					if (lengthFactor > 0) {
						offsetPrev = offsetPrev + va / 2;
						offset = offset + vc;
						offsetNext = offsetNext - vc / 2;
					}
					else
					{
						offsetPrev = offsetPrev - va / 2;
						offset = offset + va;
						offsetPrev = offsetPrev + vc / 2;
					}
				}
			}
		}

		void fixDistances(FaceMask &mask)
		{
			for (size_t shapeIndex = 0; shapeIndex < maskDescription.shapes.size(); shapeIndex++)
			{
				const auto& shape = maskDescription.shapes[shapeIndex];

				for (size_t pointIndex = shape.firstEnclosed(); pointIndex <= shape.lastEnclosed(); pointIndex++)
				{
					if (maskDescription.points[shape.point(pointIndex)].floating)
						continue;

					auto& offsetPrev = maskOffset[shape.point(pointIndex - 1)];
					auto& offset = maskOffset[shape.point(pointIndex)];
					auto& offsetNext = maskOffset[shape.point(pointIndex + 1)];

					Math::Point<float>
						a = mask[shape.point(pointIndex - 1)],
						b = mask[shape.point(pointIndex)],
						c = mask[shape.point(pointIndex + 1)];

					float angle = Math::Angle<float>::between(a, b, c);
					float angleFactor = factor(angle, maskLimits.shapeLimits[shapeIndex].angleThresholds[pointIndex]);

					auto v = a.to(c).normal().normalized() * angleFactor * 0.5;
					offsetPrev = offsetPrev + v / 2;
					offset = offset - v;
					offsetNext = offsetNext + v / 2;
				}
			}
		}

		void fixFloatingPoints(FaceMask &mask)
		{
			for (size_t shapeIndex = 0; shapeIndex < maskDescription.shapes.size(); shapeIndex++)
			{
				const auto& shape = maskDescription.shapes[shapeIndex];

				for (size_t firstPointIndex = 0; firstPointIndex < shape.points.size();)
				{
					if (maskDescription.points[shape.point(firstPointIndex)].floating || !maskDescription.points[shape.point(firstPointIndex + 1)].floating)
					{
						firstPointIndex++;
						continue;
					}

					size_t lastPointIndex = firstPointIndex + 1;
					while (maskDescription.points[shape.point(lastPointIndex)].floating)
						lastPointIndex++;

					float totalDistance = 0;
					for (size_t pointIndex = firstPointIndex; pointIndex < lastPointIndex; pointIndex++)
						totalDistance += mask[shape.point(pointIndex)].to(mask[shape.point(pointIndex + 1)]).d();

					Math::Point<float> point = mask[shape.point(firstPointIndex)];
					Math::Vector<float> offset = Math::Vector<float>(0, 0);

					float accumulatedDistance = 0;
					size_t distancePointIndex = firstPointIndex;

					for (size_t pointIndex = firstPointIndex + 1; pointIndex < lastPointIndex; pointIndex++)
					{
						float requiredDistance = totalDistance * (pointIndex - firstPointIndex) / (lastPointIndex - firstPointIndex);

						while (accumulatedDistance < requiredDistance)
						{
							float availableDistance = offset.d();

							if (accumulatedDistance + availableDistance < requiredDistance)
							{
								point += offset;
								accumulatedDistance += availableDistance;

								offset = mask[shape.point(distancePointIndex)].to(mask[shape.point(distancePointIndex + 1)]);
								distancePointIndex++;
							}
							else
							{
								Math::Vector<float> segment = offset.normalized() * (requiredDistance - accumulatedDistance);

								point += segment;
								offset -= segment;

								accumulatedDistance = requiredDistance;
							}
						}

						maskOffset[shape.point(pointIndex)] += mask[shape.point(pointIndex)].to(point);
					}

					firstPointIndex = lastPointIndex;
				}
			}
		}

		void floatPoints(FaceMask &mask)
		{
			for (int i = 0; i <= mask.size(); i++)
				maskOffset[i] += maskDescription.points[i].defaultDirection;
		}

	public:
		MaskFixer(MaskInfo::MaskDescription maskDescription, MaskInfo::MaskLimits maskLimits) :
			maskDescription(maskDescription),
			maskLimits(maskLimits)
		{ }

		void fix(FaceMask &mask)
		{
			maskOffset.clear();
			maskOffset.resize(maskDescription.points.size(), Math::Vector<float>(0, 0));

			fixAngle(mask);
			fixDistances(mask);
			fixFloatingPoints(mask);
			floatPoints(mask);

			mask += maskOffset;
		}
	};
}