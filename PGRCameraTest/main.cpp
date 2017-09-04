#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <chrono>
#include <conio.h>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "loadPNM.h"
#include "convert.h"
#include "voronoi.h"
#include "edit.h"
#include "interface.h"
#include "FlyCapture2.h"
#include "relighting.h"

#define turn_on 255,255,255,0,0,0
#define turn_half_lit 128,128,128,0,0,0
#define turn_off 0,0,0,0,0,0

#pragma comment (lib, "Ws2_32.lib")

using namespace cv;
using namespace std;


int main( int argc, char** argv)
{
	int choice_mode;
	bool demo;
	bool capture;
	bool precomp;
	bool tablecomp;

	cout << "Choose a mode: " << endl; 
	cout << "1: DEMO" << endl; 
	cout << "2: CAPTURE" << endl;  
	cout << "3: PRE-COMPUTATION" << endl;  
	cout << "4: TABLE COMPUTATION" << endl; 
	cin >> choice_mode; 
	cin.ignore();


	if(choice_mode == 1) 
	{ 
		demo = true; 
		capture = false;
		precomp =  false;
		tablecomp =  false;
	}
	if(choice_mode == 2) 
	{ 
		demo = false; 
		capture = true;
		precomp =  false;
		tablecomp =  false;
	}
	if(choice_mode == 3) 
	{ 
		demo = false; 
		capture = false;
		precomp =  true;
		tablecomp =  false;
	}
	if(choice_mode == 4) 
	{ 
		demo = false; 
		capture = false;
		precomp =  false;
		tablecomp =  true;

		cout << "!!WARNING!! ONLY USE THIS MODE IF YOU WISH TO CHANGE THE LIGHTS POSITIONS!" << endl; 
	}



	if(tablecomp)
	{
		int choice_table;
		bool pointscomp;

		cout << "Which table do you want to compute? " << endl; 
		cout << "1: LATITUDE LONGITUDE TABLE" << endl; 
		cout << "2: RELIGHTING TABLE" << endl;  
		cin >> choice_table; 
		cin.ignore();


		if(choice_table == 1) { pointscomp = true; }
		if(choice_table == 2) { pointscomp = false; }


		if(pointscomp)
		{
			//CONVERT XYZ COORDINATES INTO LATITUDE-LONGITUDE COORDINATES
			int width = 1024;
			int height = 512;
			convert_data("C:\\CO417-HW1\\points.txt", "C:\\CO417-HW1\\latlon.txt", width, height);

			//CREATE CONVERSION TABLE BETWEEN (ARC, INDEX) (Huseng's) AND NUMBER (Alix's) LIGHT ADDRESSING WAYS
			vector<Point2f> points;
			sort_data("C:\\CO417-HW1\\latlon.txt", points);
			build_table("C:\\CO417-HW1\\table.txt", points, width, height);
			invert_table("C:\\CO417-HW1\\table.txt", "C:\\CO417-HW1\\table_inv_minus.txt");
			fix_table("C:\\CO417-HW1\\table_inv_minus.txt", "C:\\CO417-HW1\\table_inv.txt");
		}


		if(!pointscomp)
		{
			//CREATE TABLE THAT LINKS AND IMAGE NUMBER (0-360) TO A LIGHT REFERED AS (ARC, INDEX)
			string table_path;
			table_path = "C:\\CO417-HW1\\relighting_table.txt";

			compute_relighting_table(table_path);
		}
	}



	if(precomp)
	{
		int choice_env;
		string environment;

		cout << "Choose an envrionment: " << endl; 
		cout << "1: ENNIS" << endl; 
		cout << "2: GLACIER" << endl;  
		cout << "3: GRACE" << endl; 
		cout << "4: PISA" << endl; 
		cout << "5: UFFIZI" << endl; 
		cout << "6: ROTATED PISA" << endl;
		cout << "7: Z-GRADIENT" << endl;
		cout << "8: Y-GRADIENT" << endl;
		cout << "9: X-GRADIENT" << endl;
		cin >> choice_env; 
		cin.ignore();


		if(choice_env == 1) { environment = "ENNIS"; }
		if(choice_env == 2) { environment = "GLACIER"; }
		if(choice_env == 3) { environment = "GRACE"; }
		if(choice_env == 4) { environment = "PISA"; }
		if(choice_env == 5) { environment = "UFFIZI"; }
		if(choice_env == 6) { environment = "PISA_NEW"; }
		if(choice_env == 7) { environment = "Z-gradient"; }
		if(choice_env == 8) { environment = "Y-gradient"; }
		if(choice_env == 9) { environment = "X-gradient"; }



		




		
		cout << "Creating folders" << endl; 

		//FOLDERS CREATION:
		string dir_env;
		dir_env = "C:\\CO417-HW1\\voronoi_";
		dir_env += environment;
		CreateDirectory(dir_env.c_str(), NULL);


		string dir_txt(dir_env);
		dir_txt += "\\txt";
		CreateDirectory(dir_txt.c_str(), NULL);


		string dir_rot_conv(dir_env);
		dir_rot_conv += "\\txt\\rotations_converted";
		CreateDirectory(dir_rot_conv.c_str(), NULL);


		string dir_rot_x12(dir_env);
		dir_rot_x12 += "\\rotation_x12";
		CreateDirectory(dir_rot_x12.c_str(), NULL);


		string dir_rot(dir_env);
		dir_rot += "\\rotation";
		CreateDirectory(dir_rot.c_str(), NULL);
		//END FOLDERS CREATION







		cout << "Creating voronoi diagram" << endl; 

		//VORONOI DIAGRAM CREATION
		string PFM_path;
		PFM_path = "C:\\CO417-HW1\\GraceCathedral\\";
		PFM_path += environment;
		PFM_path += ".pfm";
	
		Mat img = loadPFM(PFM_path);
		int width(img.cols), height(img.rows);
	
		


		string voro_path;
		voro_path = "C:\\CO417-HW1\\voronoi_";
		voro_path += environment;
		string no_gamma_path(voro_path);

		voro_path += "\\gamma_corrected.png";
		no_gamma_path += "\\weighted.pfm";

		Mat img_vo = apply_voronoi( img, environment, "C:\\CO417-HW1\\latlon.txt", voro_path, true);


		delete_gamma( img_vo );
		savePFM(img_vo, no_gamma_path);
		//END VORONOI DIAGRAM CREATION









		cout << "Renaming the rotated images" << endl; 
		
		//RENAME ROTATED IMAGES
		string in;
		in = "C:\\CO417-HW1\\voronoi_";
		in += environment;
		in += "\\rotation_x12\\rotated";

		string out;
		out = "C:\\CO417-HW1\\voronoi_";
		out += environment;
		out += "\\rotation\\rotated";

		rename_rotation(in, out);
		//END RENAME ROTATED IMAGES






		cout << "Storing data" << endl; 
		
		//STORE DATA
		for(unsigned int i = 0; i < get_rot_complete().size(); i++) //image i
		{
			string out_path;
			out_path = "C:\\CO417-HW1\\voronoi_";
			out_path += environment;
			out_path += "\\txt\\rotations_converted\\rotation";
			out_path += to_string(i);
			out_path += ".txt";




			save_vec(out_path, get_rot_complete()[i]);

			cout << "rotation no:" << i << endl; 
		}
		//END STORE DATA
		







		cout << "Compressing data" << endl; 
		
		//COMPRESS DATA 1 file
		for(unsigned int i = 0; i < 361; i++) //image i
		{


			string in_path;
			in_path = "C:\\CO417-HW1\\voronoi_";
			in_path += environment;
			in_path += "\\txt\\rotations_converted\\rotation";
			in_path += to_string(i);
			in_path += ".txt";

			ifstream ifs(in_path); 

		
			cout << "rotation no:" << i << endl; 



			string out_path;
			out_path = "C:\\CO417-HW1\\voronoi_";
			out_path += environment;
			out_path += "\\txt\\global_converted.txt";

			ofstream ofs;
			ofs.open(out_path, std::ios_base::app);

			int no, arc, index;
			float r, g, b;
			while(ifs >> no >> arc >> index >> r >> g >> b)
			{
				ofs << no << ' ' << arc << ' ' << index << ' ' << r << ' ' << g << ' ' << b << endl;
			}
		}
		//END COMPRESS DATA 1 file


	
		


		cout << "Converting to HSV" << endl; 
		
		//CONVERSION RGB TO HSV
		string rgb_path;
		rgb_path = "C:\\CO417-HW1\\voronoi_";
		rgb_path += environment;
		rgb_path += "\\txt\\global_converted.txt";

		string hsv_path;
		hsv_path = "C:\\CO417-HW1\\voronoi_";
		hsv_path += environment;
		hsv_path += "\\txt\\global_rgb_hsv.txt";

		rgb_to_hsv_file(rgb_path, hsv_path);
		//END CONVERSION RGB TO HSV
		





		cout << "Converting to RGB and W" << endl; 
		
		//CONVERSION TO WEIGHTED RGB AND W*3
		string rgb_hsv_path;
		rgb_hsv_path = "C:\\CO417-HW1\\voronoi_";
		rgb_hsv_path += environment;
		rgb_hsv_path += "\\txt\\global_rgb_hsv.txt";

		string rgb_w_path;
		rgb_w_path = "C:\\CO417-HW1\\voronoi_";
		rgb_w_path += environment;
		rgb_w_path += "\\txt\\global_rgb_w.txt";

		compute_ratio_file(rgb_hsv_path, rgb_w_path);
		//END CONVERSION TO WEIGHTED RGB AND W*3
		


		//re_compute_file("C:\\CO417-HW1\\voronoi_Y-gradient\\txt\\global_rgb_w.txt", "C:\\CO417-HW1\\voronoi_Y-gradient\\txt\\global_rgb_w_plus.txt");
		
	}




	if(capture)
	{
		
		int choice_capture;
		bool acquisition;

		cout << "Choose a BRDF mode: " << endl; 
		cout << "1: AQUISITION" << endl; 
		cout << "2: PROCESSING" << endl;  
		cin >> choice_capture; 
		cin.ignore();


		if(choice_capture == 1) { acquisition = true; }
		if(choice_capture == 2) { acquisition = false; }
		






		
		if(acquisition)
		{
			//FOLDERS CREATION:
			string dir_IBRL = "IBRL";
			CreateDirectory(dir_IBRL.c_str(), NULL);


			string dir_imgs(dir_IBRL);
			dir_imgs += "\\images";
			CreateDirectory(dir_imgs.c_str(), NULL);


			string dir_imgs_multiplied(dir_IBRL);
			dir_imgs_multiplied += "\\images_multiplied";
			CreateDirectory(dir_imgs_multiplied.c_str(), NULL);
			//END FOLDERS CREATION




			//CAPTURE:
			WSADATA wsaData;
			int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != 0){
				printf("WSAStartup failed with error: %d\n", iResult);
				return 1;
			}

			LightStage stage;
			LightStageCamera cam;
			RunSingleCamera16( stage, cam, "C:\\CO417-HW1\\IBRL\\images\\" );
			//END CAPTURE
		}

		
		if(!acquisition)
		{
			//IMAGES MULTIPLICATION:
			int choice_env;
			string environment;

			cout << "Choose an envrionment: " << endl; 
			cout << "1: ENNIS" << endl; 
			cout << "2: GLACIER" << endl;  
			cout << "3: GRACE" << endl; 
			cout << "4: PISA" << endl; 
			cout << "5: UFFIZI" << endl; 
			cout << "6: ROTATED PISA" << endl;
			cout << "7: Z-GRADIENT" << endl;
			cout << "8: Y-GRADIENT" << endl;
			cout << "9: X-GRADIENT" << endl;
			cin >> choice_env; 
			cin.ignore();


			if(choice_env == 1) { environment = "ENNIS"; }
			if(choice_env == 2) { environment = "GLACIER"; }
			if(choice_env == 3) { environment = "GRACE"; }
			if(choice_env == 4) { environment = "PISA"; }
			if(choice_env == 5) { environment = "UFFIZI"; }
			if(choice_env == 6) { environment = "PISA_NEW"; }
			if(choice_env == 7) { environment = "Z-gradient"; }
			if(choice_env == 8) { environment = "Y-gradient"; }
			if(choice_env == 9) { environment = "X-gradient"; }

		
			for(unsigned int i = 0; i < 168; i++) //image i
			{
				multiply_image_i("C:\\CO417-HW1\\IBRL\\images\\", "C:\\CO417-HW1\\IBRL\\images_multiplied\\", i, environment);

				cout << "image no:" << i << endl; 
			}
			


			sum_images("C:\\CO417-HW1\\IBRL\\images_multiplied\\", "C:\\CO417-HW1\\IBRL\\");
		}




		




	}

	if(demo)
	{
		int choice_env;
		string environment;

		cout << "Choose an envrionment: " << endl; 
		cout << "1: ENNIS" << endl; 
		cout << "2: GLACIER" << endl;  
		cout << "3: GRACE" << endl; 
		cout << "4: PISA" << endl; 
		cout << "5: UFFIZI" << endl; 
		cout << "6: ROTATED PISA" << endl;
		cout << "7: Z-GRADIENT" << endl;
		cout << "8: Y-GRADIENT" << endl;
		cout << "9: X-GRADIENT" << endl;
		cin >> choice_env; 
		cin.ignore();


		if(choice_env == 1) { environment = "ENNIS"; }
		if(choice_env == 2) { environment = "GLACIER"; }
		if(choice_env == 3) { environment = "GRACE"; }
		if(choice_env == 4) { environment = "PISA"; }
		if(choice_env == 5) { environment = "UFFIZI"; }
		if(choice_env == 6) { environment = "PISA_NEW"; }
		if(choice_env == 7) { environment = "Z-gradient"; }
		if(choice_env == 8) { environment = "Y-gradient"; }
		if(choice_env == 9) { environment = "X-gradient"; }



		
		int choice_rot;
		bool rotation_on;

		cout << "Do you want rotation or static? " << endl; 
		cout << "1: ROTATION" << endl; 
		cout << "2: STATIC" << endl;  
		cin >> choice_rot; 
		cin.ignore();


		if(choice_rot == 1) { rotation_on = true; }
		if(choice_rot == 2) { rotation_on = false; }

	
	
	
		
		//LIGHT STAGE:
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0){
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		}
		LightStage stage;


		
		//DEMO:
		string show_path_0;
		show_path_0 = "C:\\CO417-HW1\\voronoi_";
		show_path_0 += environment;
		show_path_0 += "\\original.png";

		Mat fr_0;
		fr_0 = imread(show_path_0, CV_LOAD_IMAGE_COLOR);
		imshow( "Original", fr_0);
		waitKey(1000);


		string show_path_1;
		show_path_1 = "C:\\CO417-HW1\\voronoi_";
		show_path_1 += environment;
		show_path_1 += "\\centers.png";

		Mat fr_1;
		fr_1 = imread(show_path_1, CV_LOAD_IMAGE_COLOR); 
		imshow( "Demo", fr_1);
		waitKey(1000);

		string show_path_2;
		show_path_2 = "C:\\CO417-HW1\\voronoi_";
		show_path_2 += environment;
		show_path_2 += "\\centers_lines.png";

		Mat fr_2;
		fr_2 = imread(show_path_2, CV_LOAD_IMAGE_COLOR); 
		imshow( "Demo", fr_2);
		waitKey(1000);

		string show_path_3;
		show_path_3 = "C:\\CO417-HW1\\voronoi_";
		show_path_3 += environment;
		show_path_3 += "\\rotation_x12\\rotated0.0.png";

		Mat fr_3;
		fr_3 = imread(show_path_3, CV_LOAD_IMAGE_COLOR); 
		imshow( "Demo", fr_3);
		waitKey(1000);



		/*
		//SWITCH MENU:
		bool end = false;
		bool orig = false;

		while(!end)
		{
			cout << "Press 'c' to switch map OR 'e' to continue" << endl;
			char ch;
			cin >> ch; 
			cin.ignore();

			if(ch == 'c')
			{
				if(orig)
				{
					imshow( "Demo", fr_3);
					waitKey(1000);
					orig = false;
				}

				if(!orig)
				{
					imshow( "Demo", fr_0);
					waitKey(1000);
					orig = true;
				}
			}

			if(ch == 'e')
			{
				end = true;
			}

			if(ch != 'c' && ch != 'e')
			{
				cout << "You did not press 'c' or 'e'!" << endl;
			}

		}
		//END SWITCH MENU
		*/



		//END DEMO






		




		
		if(rotation_on)
		{
			int choice_rgbw;
			bool rgbw;
			bool rgb;
			bool end = false;





			while(!end)
			{
				cout << "Do you want RGBW or RGB? " << endl; 
				cout << "1: RGBW" << endl; 
				cout << "2: RGB" << endl;  
				cout << "3: END DEMO" << endl;  
				cin >> choice_rgbw; 
				cin.ignore();


				if(choice_rgbw == 1) 
				{ 
					rgbw = true; 
					rgb = false; 
					end = false;
				}
				if(choice_rgbw == 2) 
				{ 
					rgbw = false; 
					rgb = true;
					end = false;
				}
				if(choice_rgbw == 3) 
				{ 
					rgbw = false; 
					rgb = false; 
					end = true; 
				}





				if(rgb)
				{
					stage.adjustAll(turn_off);

					while(_kbhit() == 0) 
					{
						//LAUNCH LIGHT STAGE 1 file
						int i = 0;


						string in_path;
						in_path = "C:\\CO417-HW1\\voronoi_";
						in_path += environment;
						in_path += "\\txt\\global_converted.txt";

						ifstream ifs(in_path); 

						int no, arc, index;
						float r, g, b;
						while(ifs >> no >> arc >> index >> r >> g >> b)
						{
							stage(arc, index)->setValue(r, 0, g, 0, b, 0);

							if(no == 174)
							{
								stage.go();

			
			
								string animation_path;
								animation_path = "C:\\CO417-HW1\\voronoi_";
								animation_path += environment;
								animation_path += "\\rotation\\rotated";
								animation_path += to_string(i);
								animation_path += ".png";
								play_one_rotation(animation_path, i);
			

								i++;
								waitKey(20);
							}
						}
					}

					cin.ignore();


					string show_path_0;
					show_path_0 = "C:\\CO417-HW1\\voronoi_";
					show_path_0 += environment;
					show_path_0 += "\\original.png";

					Mat fr_0;
					fr_0 = imread(show_path_0, CV_LOAD_IMAGE_COLOR);
					imshow( "Original", fr_0);
					waitKey(1000);



					/*
					//SWITCH MENU:
					bool end = false;
					bool orig = false;

					while(!end)
					{
						cout << "Press 'c' to switch map OR 'e' to continue" << endl;
						char ch;
						cin >> ch; 
						cin.ignore();

						if(ch == 'c')
						{
							if(orig)
							{
								imshow( "Demo", fr_3);
								waitKey(1000);
								orig = false;
							}

							if(!orig)
							{
								imshow( "Demo", fr_0);
								waitKey(1000);
								orig = true;
							}
						}

						if(ch == 'e')
						{
							end = true;
						}

						if(ch != 'c' && ch != 'e')
						{
							cout << "You did not press 'c' or 'e'!" << endl;
						}

					}
					//END SWITCH MENU
					*/


				}


				if(rgbw)
				{
					stage.adjustAll(turn_off);

					while(_kbhit() == 0) 
					{
						//LAUNCH LIGHT STAGE 1 file RGB AND W*3
						int i = 0;


						string in_path;
						in_path = "C:\\CO417-HW1\\voronoi_";
						in_path += environment;
						in_path += "\\txt\\global_rgb_w.txt";

						ifstream ifs(in_path); 

						int no, arc, index;
						float r, g, b;
						while(ifs >> no >> arc >> index >> r >> g >> b)
						{
							stage(arc, index)->setValue(r, 0, g, 0, b, 0);

							if(no == 174 && index == 13)
							{
								stage.go();

			
			
								string animation_path;
								animation_path = "C:\\CO417-HW1\\voronoi_";
								animation_path += environment;
								animation_path += "\\rotation\\rotated";
								animation_path += to_string(i);
								animation_path += ".png";
								play_one_rotation(animation_path, i);
			

								i++;
								waitKey(20);
							}
						}
					}

					cin.ignore();
					

					string show_path_0;
					show_path_0 = "C:\\CO417-HW1\\voronoi_";
					show_path_0 += environment;
					show_path_0 += "\\original.png";

					Mat fr_0;
					fr_0 = imread(show_path_0, CV_LOAD_IMAGE_COLOR);
					imshow( "Original", fr_0);
					waitKey(1000);


					/*
					//SWITCH MENU:
					bool end = false;
					bool orig = false;

					while(!end)
					{
						cout << "Press 'c' to switch map OR 'e' to continue" << endl;
						char ch;
						cin >> ch; 
						cin.ignore();

						if(ch == 'c')
						{
							if(orig)
							{
								imshow( "Demo", fr_3);
								waitKey(1000);
								orig = false;
							}

							if(!orig)
							{
								imshow( "Demo", fr_0);
								waitKey(1000);
								orig = true;
							}
						}

						if(ch == 'e')
						{
							end = true;
						}

						if(ch != 'c' && ch != 'e')
						{
							cout << "You did not press 'c' or 'e'!" << endl;
						}

					}
					//END SWITCH MENU
					*/



				}

			}


		}
	


	
		



		
		if(!rotation_on)
		{
			int choice_rgbw;
			bool rgbw;
			bool rgb;
			bool end = false;



			while(!end)
			{
				cout << "Do you want RGBW or RGB? " << endl; 
				cout << "1: RGBW" << endl; 
				cout << "2: RGB" << endl;  
				cout << "3: END DEMO" << endl;  
				cin >> choice_rgbw; 
				cin.ignore();


				if(choice_rgbw == 1) 
				{ 
					rgbw = true; 
					rgb = false; 
					end = false;
				}
				if(choice_rgbw == 2) 
				{ 
					rgbw = false; 
					rgb = true;
					end = false;
				}
				if(choice_rgbw == 3) 
				{ 
					rgbw = false; 
					rgb = false; 
					end = true; 
				}


				if(rgb)
				{
					stage.adjustAll(turn_off);

					//NO ROTATION ONLY RGB:
					string in_path;
					in_path = "C:\\CO417-HW1\\voronoi_";
					in_path += environment;
					in_path += "\\txt\\global_converted.txt";


					ifstream ifs(in_path); 

					int no, arc, index;
					float r, g, b;
					while(ifs >> no >> arc >> index >> r >> g >> b && !(no == 174 && index == 13))
					{
						stage(arc, index)->setValue(r, 0, g, 0, b, 0);
					}

					stage.go();
					Sleep(1000);


				}




				if(rgbw)
				{
					stage.adjustAll(turn_off);

					//NO ROTATION RGB AND W*3:
					string in_path;
					in_path = "C:\\CO417-HW1\\voronoi_";
					in_path += environment;
					in_path += "\\txt\\global_rgb_w.txt";


					ifstream ifs(in_path); 

					int no, arc, index;
					float r, g, b;
					while(ifs >> no >> arc >> index >> r >> g >> b && !(no == 174 && index == 13))
					{
						stage(arc, index)->setValue(r, 0, g, 0, b, 0);
					}

					stage.go();
					Sleep(1000);


				/*
					LightStageCamera cam;

					// Connect to a camera
					cam.connect();
					// Get the camera information
					cam.getCameraInfo();
					cam.PrintCameraInfo16();
					cam.init_control();
					// CAMERA
					cam.setNumImages(1);


					// Start capturing images
					cam.StartCapture();
					Image rawImage;
					std::vector<Image> vecImages;
					vecImages.resize(cam.getNumImages());
		
					cam.RetrieveBuffer(vecImages, &rawImage, 0);


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
						filename << "C:\\CO417-HW1\\image_rgbw_" << imageCnt << ".png"; 

						// Save the image. If a file format is not passed in, then the file
						// extension is parsed to attempt to determine the file format.
						cam.performFunc(convertedImage.Save(filename.str().c_str()));
					}
				*/
				}

			}


		}
	
		cin.ignore();

		
		int choice_off;
		bool off;

		cout << "Do you want to turn the lights off? " << endl; 
		cout << "1: YES" << endl; 
		cout << "2: NO" << endl;  
		cin >> choice_off; 
		cin.ignore();


		if(choice_off == 1) { off = true; }
		if(choice_off == 2) { off = false; }

		if(off)
		{
			stage.adjustAll(turn_off);
		}
		
	




		//END LIGHT STAGE



	}



	


	cout << "Done! Press Enter to exit..." << endl;
	cin.ignore();
	return 0;
}