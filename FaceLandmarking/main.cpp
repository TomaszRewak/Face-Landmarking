#include "app/examples.hpp"
#include "app/video.hpp"
#include "app/features.hpp"

enum class ProcessType
{
	Video,
	Example,
	Features
};

int main(int argc, char** argv)
{
	const int N = 194;

	ProcessType processType = ProcessType::Example;
	std::string dataPath = "./../Data";
	std::string videoPath = "./../Data/video-examples/ja6.mp4";
	int steps = 15;

	bool transform = true;
	int transformRotate = cv::ROTATE_90_COUNTERCLOCKWISE;
	int transformWidth = 350;
	int transformHeight = 600;

	for (int i = 1; i < argc;)
	{
		std::string param(argv[i++]);

		if (param == "-type")
		{
			std::string type(argv[i++]);

			if (type == "video")
				processType = ProcessType::Video;
			if (type == "example")
				processType = ProcessType::Example;
			if (type == "features")
				processType = ProcessType::Features;
		}
		if (param == "-data")
			dataPath = argv[i++];
		if (param == "-video")
			videoPath = argv[i++];
		if (param == "-steps")
			steps = std::stoi(argv[i++]);
		if (param == "-transform")
			transform = true;
		if (param == "-transform-rotate")
			transformRotate = std::stoi(argv[i++]);
		if (param == "-transform-width")
			transformWidth = std::stoi(argv[i++]);
		if (param == "-transform-height")
			transformHeight = std::stoi(argv[i++]);
	}

	switch (processType)
	{
	case ProcessType::Video:
		FaceLandmarking::App::video<N>(dataPath, videoPath, steps, transform, transformRotate, transformWidth, transformHeight);
		break;
	case ProcessType::Example:
		FaceLandmarking::App::example<N>(dataPath, steps);
		break;
	case ProcessType::Features:
		FaceLandmarking::App::features<N>(dataPath);
		break;
	}
}
