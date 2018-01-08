#include "examples.hpp"
#include "video.hpp"
#include "features.hpp"

using namespace cv;
using namespace std;
using namespace FaceLandmarking;

enum class ProcessType
{
	Video,
	Example,
	Features
};

int main(int argc, char** argv)
{
	ProcessType processType = ProcessType::Video;
	string videoPath = "D:/Programy/FaceLandmarking/Data/examples/ja4b.mp4";

	for (int i = 1; i < argc;)
	{
		string param(argv[i++]);

		if (param == "-type")
		{
			string type(argv[i++]);

			if (type == "video")
				processType = ProcessType::Video;
			if (type == "example")
				processType = ProcessType::Example;
			if (type == "features")
				processType = ProcessType::Features;
		}

		if (param == "-video")
		{
			string path(argv[i++]);

			videoPath = path;
		}
	}

	switch (processType)
	{
	case ProcessType::Video:
		video_test(videoPath);
		break;
	case ProcessType::Example:
		example_test();
	case ProcessType::Features:
		features_test();
	}
}
