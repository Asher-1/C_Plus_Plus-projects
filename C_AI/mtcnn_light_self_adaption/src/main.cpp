//author: samylee
//date: 2018.4.19

#include "network.h"
#include "mtcnn.h"
#include <time.h>
#include <opencv2/opencv.hpp>

using namespace std;
int main0(void) {
    const enum CBLAS_ORDER Order=CblasRowMajor;
    const enum CBLAS_TRANSPOSE TransA=CblasNoTrans;
    const enum CBLAS_TRANSPOSE TransB=CblasNoTrans;
    const int M=4;//A的行数，C的行数
    const int N=2;//B的列数，C的列数
    const int K=3;//A的列数，B的行数
    const float alpha=1;
    const float beta=0;
    const int lda=K;//A的列
    const int ldb=N;//B的列
    const int ldc=N;//C的列
    const float A[K*M]={1,2,3,4,5,6,7,8,9,8,7,6};
    const float B[K*N]={5,4,3,2,1,0};
    float C[M*N];

    cblas_sgemm(Order, TransA, TransB, M, N, K, alpha, A, lda, B, ldb, beta, C, ldc);

    for(int i=0;i<M;i++)
    {
      for(int j=0;j<N;j++)
      {
          cout<<C[i*N+j]<<"/t";
      }
      cout<<endl;
    }

    return EXIT_SUCCESS;
}

int main()
{
	//initial models without image's width or height
	mtcnn find;

	//test_type choice
    string test_type = "video";
	if (test_type == "img_dir")
	{
		string imgpath = "images_adaption/";
        vector<cv::String> filenames;
        cv::glob(imgpath, filenames, true);
        for (size_t i = 0; i < filenames.size(); i++)
		{
            string imgname = filenames[i];
			Mat image = imread(imgname, 1);

			clock_t start_t = clock();
			//detect face by min_size(30)
			find.findFace(image, 30);
			cout << "Cost time: " << clock() - start_t << endl;

			imshow("test", image);
			waitKey(0);
		}
	}
	else if(test_type == "video")
	{
		VideoCapture cap("test.mp4");
		if (!cap.isOpened())
			cout << "fail to open!" << endl;
		Mat image;

		while (true) {
			
			cap >> image;
			if (!image.data) {
				cout << "fail to read image!" << endl;
				return -1;
			}
			clock_t start_t = clock();
			//detect face by min_size(60)
            find.findFace(image, 60);
			cout << "Cost time: " << clock() - start_t << endl;

			imshow("result", image);
			if (waitKey(1) >= 0) break;
		}
	}
	else
	{
		cout << "Unknow test type!" << endl;
	}

	return 0;
}
