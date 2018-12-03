//模板匹配算法
#define N  10;   //样品类别
#define K  25;   //特征维数

double LeastDistance()
{
	double min=10000000000;
	number_no number_no;
	for(int n=0;n<N;n++)
	{
		for(int i=0;i<pattern[n].number;i++)
		{
			if(match(pattern[n].feature[i],testsample)<min)
			{
				//匹配的最小值
				min=match(pattern[n].feature[i],testsample);
				number_no.number=n;//模板类别
				number_no.no=i;//模板序号
			}
		}
	}
	return number_no;//返回样本的类别和序号
}

double match(double a[], double b[])
{
	double count=0.0;
	for(int i=0;i<K;i++)
	{
		count+=(a[i]-b[i])*(a[i]-b[i]);
	}
	return count;
}
