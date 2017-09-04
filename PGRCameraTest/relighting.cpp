#include "relighting.h"

using namespace FlyCapture2;
using namespace std;



int RunSingleCamera16(LightStage& stage, LightStageCamera& cam, std::string out_path)
{
	// Connect to a camera
	cam.connect();
	// Get the camera information
	cam.getCameraInfo();
	cam.PrintCameraInfo16();
	cam.initCurrentCamera();
	// CAMERA
	cam.setNumImages(168);

	// Start capturing images
	cam.StartCapture();
	Image rawImage;
	std::vector<Image> vecImages;
	vecImages.resize(cam.getNumImages());
	int counter = 0;

	stage.adjustAll(turn_off);
	Sleep(100); // wait for the lights to turn on

	int i = 0;
	for(unsigned int arc = 0; arc < 24; arc++)
	{
		for(unsigned int index = 0; index < 7; index++)
		{
			stage(arc, 2*index)->setValue(255, 0, 255, 0, 255, 0);
			stage(arc, 2*index + 1)->setValue(255, 0, 255, 0, 255, 0);


			stage.go();
			Sleep(1000);

			cam.RetrieveBuffer(vecImages, &rawImage, i);
			i++;
			stage.adjustAll(turn_off);
		}
	}
	


	// turn off all lights
	stage.adjustAll(turn_off);


	// Stop capturing images
	cam.StopCapture();

	// Disconnect the camera
	cam.Disconnect();

	// Save images to disk
	for (int imageCnt = 0; imageCnt < cam.getNumImages(); imageCnt++)
	{
		// Create a converted image
		Image convertedImage;
		// Convert the raw image
		cam.performFunc(vecImages[imageCnt].Convert(PIXEL_FORMAT_RGB, &convertedImage));
		printf("%dth picture, take %d\n", imageCnt + 1);

		// Create a unique filename
		ostringstream filename;
		filename << out_path << "image_" << imageCnt << ".png"; 

		// Save the image. If a file format is not passed in, then the file
		// extension is parsed to attempt to determine the file format.
		cam.performFunc(convertedImage.Save(filename.str().c_str()));
	}

	return 0;
}





void compute_relighting_table(std::string out_path)
{
	ofstream ofs(out_path);

	int i = 0;
	for(unsigned int arc = 0; arc < 24; arc++)
	{
		for(unsigned int index = 0; index < 7; index++)
		{
			ofs << i << ' ' << arc << ' ' << 2*index << ' ' << endl;
			i++;
		}
	}

}

bool i_to_arc_index(int& i, int& arc, int& index, std::string in_path)
{
	ifstream ifs(in_path); //TABLE text file

    int k, l, j;
    while(ifs >> k >> l >> j)
    {
        if(i == k)
		{
			arc = l;
			index = j;

			return true;
		}
    }
	return false;
}


bool arc_index_to_rgb(int& a, int& i, float& red, float& green, float& blue, std::string in_path)
{
	ifstream ifs(in_path); //TABLE text file

	int no, arc, index;
	float r, g, b;
	while(ifs >> no >> arc >> index >> r >> g >> b && !(no == 174 && index == 13))
	{
		if(a == arc && index == i)
		{
			red = r;
			green = g;
			blue = b;

			return true;
		}
	}
	return false;
}




void multiply_image_i(std::string in_path, std::string out_path, int k, string environment)
{
	int arc, index;
	i_to_arc_index(k, arc, index, "C:\\CO417-HW1\\relighting_table.txt");

	
	float r, g, b;
	string table_path;
	table_path = "C:\\CO417-HW1\\voronoi_";
	table_path += environment;
	table_path += "\\txt\\global_rgb_w.txt";
	arc_index_to_rgb(arc, index, r, g, b, table_path);


	r = r / 255;
	g = g / 255;
	b = b / 255;


	in_path += "image_";
	in_path += to_string(k);
	in_path += ".png";

	cout << "in_path:" << in_path << endl; 

	Mat img;
	img = imread(in_path, IMREAD_UNCHANGED); 
	int width(img.cols), height(img.rows);


	

	Mat img_ret;
	img.Mat::convertTo(img_ret , CV_32FC3, 1/255.0);

	
	for(int i = 0 ; i < height ; i++) //y
    {
        for(int j = 0 ; j < width ; j++) //x
        {
            Vec3f color = Vec3f(0.0, 0.0, 0.0);

			float R, G, B;
			R = img_ret.at<Vec3f>(Point(j,i)).val[2];
			G = img_ret.at<Vec3f>(Point(j,i)).val[1];
			B = img_ret.at<Vec3f>(Point(j,i)).val[0];


			color[0] = saturate_cast<float>( (B * b) / 168 );
			color[1] = saturate_cast<float>( (G * g) / 168 );
			color[2] = saturate_cast<float>( (R * r) / 168 );



			img_ret.at<Vec3f>(Point2i(j,i)) = color;
        }
    }

	string pfm_path(out_path);
	pfm_path += "image_";
	pfm_path += to_string(k);
	pfm_path += ".pfm";

	savePFM(img_ret, pfm_path);



	apply_gamma( img_ret );

	out_path += "image_";
	out_path += to_string(k);
	out_path += ".png";
	
	imwrite(out_path, img_ret);
}



void sum_images(std::string in_path, std::string out_path)
{
	Mat img_sum = Mat::zeros(2048, 2048, CV_32FC3);


	for(int i = 0 ; i < 168 ; i++)
    {
		string img_path(in_path);
		img_path += "image_";
		img_path += to_string(i);
		img_path += ".pfm";

		cout << "img_path:" << img_path << endl; 

		Mat img;
		img = loadPFM(img_path);

		img_sum += img;
    }

	string pfm_path(out_path);
	pfm_path += "image_sum";
	pfm_path += ".pfm";


	Mat img_minus = Mat::zeros(2048, 2048, CV_32FC3);
	img_minus.setTo(cv::Scalar(0.56833348, 0.29896541, 0.36213938));

	//img_sum -= img_minus;


	savePFM(img_sum, pfm_path);


	/*
	apply_gamma( img_sum );
	out_path += "image_sum";
	out_path += ".png";
	imwrite(out_path, img_sum);
	*/
	
}