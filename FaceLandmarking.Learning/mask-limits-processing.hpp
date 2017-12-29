#pragma once

#include <filesystem>

#include "../FaceLandmarking.Reader/dataset-reader.hpp"
#include "../FaceLandmarking.Reader/mask-limits-io.hpp"
#include "mask/threshold-builder.hpp"
#include "../FaceLandmarking/mask-transformation/mask-normalizer.hpp"
#include "../FaceLandmarking/mask-info/mask-description.hpp"
#include "../FaceLandmarking/mask-info/mask-limits.hpp"
#include "../FaceLandmarking/math/angle.hpp"

namespace FaceLandmarking::Learning
{
	namespace fs = std::experimental::filesystem;

	class MaskLimitsProcessing
	{
	private:
		MaskInfo::MaskDescription maskDescription;

		fs::path dataPath;
		fs::path maskFile;

	public:
		MaskLimitsProcessing(MaskInfo::MaskDescription maskDescription, fs::path dataPath) :
			maskDescription(maskDescription),
			dataPath(dataPath),
			maskFile(dataPath / "mask" / "mask-limits.mask")
		{ }

		void compute() const
		{
			Reader::DatasetReader reader(dataPath);

			std::vector<std::vector<Mask::ThresholdBuilder>> angleThresholdBuildes;
			std::vector<std::vector<Mask::ThresholdBuilder>> lengthThresholdBuildes;

			for (auto& shape : maskDescription.shapes)
			{
				angleThresholdBuildes.push_back(
					std::vector<Mask::ThresholdBuilder>(shape.points.size(), Mask::ThresholdBuilder(101))
				);

				lengthThresholdBuildes.push_back(
					std::vector<Mask::ThresholdBuilder>(shape.points.size(), Mask::ThresholdBuilder(101))
				);
			}

			while (reader.hasNext())
			{
				auto example = reader.loadNext(false);
				auto& mask = example.mask;

				for (int i = 0; i < maskDescription.shapes.size(); i++)
				{
					auto& shape = maskDescription.shapes[i];

					for (int j = 0; j < shape.points.size(); j++)
					{
						Math::Point<float>
							a = mask[shape.point(j - 1)],
							b = mask[shape.point(j)],
							c = mask[shape.point(j + 1)];

						float angle = Math::Angle<float>::between(a, b, c);
						float length = b.to(c).d() / a.to(b).d();

						angleThresholdBuildes[i][j].addValue(angle);
						lengthThresholdBuildes[i][j].addValue(length);
					}
				}
			}

			MaskInfo::MaskLimits maskLimits;

			for (int i = 0; i < maskDescription.shapes.size(); i++)
			{
				MaskInfo::MaskShapeLimits maskShapeLimits;

				auto& shape = maskDescription.shapes[i];

				for (int j = 0; j < shape.points.size(); j++)
				{
					maskShapeLimits.angleThresholds.push_back(
						angleThresholdBuildes[i][j].getThreshold()
					);

					maskShapeLimits.lengthThresholds.push_back(
						lengthThresholdBuildes[i][j].getThreshold()
					);
				}

				maskLimits.shapeLimits.push_back(maskShapeLimits);
			}

			Reader::MaskLimitsIO maskLimitsIO(maskFile);
			maskLimitsIO.save(maskLimits);
		}

		MaskInfo::MaskLimits load() const
		{
			if (!fs::exists(maskFile))
				compute();

			Reader::MaskLimitsIO maskLimitsIO(maskFile);
			return maskLimitsIO.load();
		}
	};
}