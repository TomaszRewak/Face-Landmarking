#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "../feature-selector.hpp"

namespace FaceLandmarking::FeatureSelection {
	class SimpleFeatureSelector : public FeatureSelector {
	private:
		cv::Mat __hsv;
		cv::Mat __hsvSplit[3];

		cv::Mat* h;
		cv::Mat* s;
		cv::Mat* v;

	public:
		void setImage(const cv::Mat& image) override {
			cv::cvtColor(image, __hsv, cv::COLOR_BGR2HSV_FULL);
			cv::split(__hsv, __hsvSplit);

			h = &__hsvSplit[0];
			s = &__hsvSplit[1];
			v = &__hsvSplit[2];
		}

		void selectFeatures(cv::Mat *features, int x, int y) const override {
			features->create(1, 4, CV_64F);
		}
	};
}