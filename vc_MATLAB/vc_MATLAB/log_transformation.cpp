void log_transformation()
{
	Engine *ep;
	if (!(ep = engOpen(NULL)))
	{
		printf("Cannot open MATLAB Engine.");
	}
	// mxCreateDoubleMatrix函数生成矩阵，本则例子需要创建3个256x256的矩阵
	mxArray *I = mxCreateDoubleMatrix(256, 256, mxREAL);//256行列
	mxArray *J = mxCreateDoubleMatrix(256, 256, mxREAL);//256行列
	mxArray *H = mxCreateDoubleMatrix(256, 256, mxREAL);//256行列
														//下面通过MATLAB引擎调用MATLAB算法
	engPutVariable(ep, "I", I);
	engPutVariable(ep, "J", J);
	engPutVariable(ep, "H", H);
	//第18行—第20行：将三个矩阵写入MATLAB空间，并为其在MATLAB空间中命名
	engEvalString(ep, "I=imread('cameraman.tif')");
	engEvalString(ep, "J=double(I)");
	//进行对数变换
	engEvalString(ep, "J=40*(log(J+1))");
	engEvalString(ep, "H=uint8(J)");
	engEvalString(ep, "subplot(1,2,1), imshow(I)");
	engEvalString(ep, "subplot(1,2,2), imshow(H)");
	printf("press any key to exit!");
	getchar();
	mxDestroyArray(I);
	mxDestroyArray(J);
	mxDestroyArray(H);
	engClose(ep);
}
