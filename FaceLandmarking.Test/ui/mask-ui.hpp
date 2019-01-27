#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../../FaceLandmarking/face-mask.hpp"

namespace FaceLandmarking::Test::UI
{
	class MaskUI
	{
	public:
		static void drawMask(const cv::Mat& image, const FaceMask& mask, const MaskInfo::MaskDescription& maskDescription, cv::Scalar color = cv::Scalar(255, 255, 0))
		{
			for (auto& shape : maskDescription.shapes)
			{
				if (!shape.visible)
					continue;
			
				int pointsNumber = shape.points.size() - (shape.closed ? 0 : 1);
			
				/*for (size_t pointIndex = 0; pointIndex < pointsNumber; pointIndex++)
				{
					const auto& point = mask[shape.point(pointIndex)];
					const auto& nextPoint = mask[shape.point(pointIndex + 1)];
			
					cv::line(
						image,
						cv::Point(point.x, point.y),
						cv::Point(nextPoint.x, nextPoint.y),
						color / 2
					);
				}*/
			}

			for (size_t pointIndex = 0; pointIndex < mask.size(); pointIndex++)
			{
				if (!maskDescription.points[pointIndex].inUse)
					continue;

				const auto& point = mask[pointIndex];

				cv::circle(
					image,
					cv::Point(point.x, point.y),
					2,
					color,
					1);

				//cv::putText(
				//	image,
				//	std::to_string(pointIndex),
				//	cv::Point(point.x, point.y),
				//	cv::FONT_HERSHEY_SIMPLEX,
				//	0.3,
				//	cv::Scalar(125, 125, 0));
			}
		}
	};
}