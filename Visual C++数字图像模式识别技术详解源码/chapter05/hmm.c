// 动态数组申请
float *vector(long nl,long nh)
{
	float *v;
	v=(float *)calloc((unsigned) (nh-nl+1),sizeof(float));
	if (!v) nrerror("allocation failure in vector()");
	return v-nl;
}

int *ivector(long nl,long nh)
{
	int *v;
	v=(int *)calloc((unsigned) (nh-nl+1),sizeof(int));
	if (!v) nrerror("allocation failure in ivector()");
	return v-nl;
}

unsigned char *cvector(long nl,long nh)
{
	unsigned char *v;
	v=(unsigned char *)calloc((unsigned) (nh-nl+1),sizeof(unsigned char));
	if (!v) nrerror("allocation failure in cvector()");
	return v-nl;
}

unsigned long *lvector(long nl,long nh)
{
	unsigned long *v;
	v=(unsigned long *)calloc((unsigned) (nh-nl+1),sizeof(unsigned long));
	if (!v) nrerror("allocation failure in lvector()");
	return v-nl;
}

double *dvector(long nl,long nh)
{
	double *v;
	v=(double *)calloc((unsigned) (nh-nl+1),sizeof(double));
	if (!v) nrerror("allocation failure in dvector()");
	return v-nl;
}

float **matrix(long nrl,long nrh,long ncl,long nch)
{
	int i;
	float **m;
	m=(float **) calloc((unsigned) (nrh-nrl+1),sizeof(float*));
	if (!m) nrerror("allocation failure 1 in matrix()");
	m -= nrl;

	for(i=nrl;i<=nrh;i++) {
		m[i]=(float *) calloc((unsigned) (nch-ncl+1),sizeof(float));
		if (!m[i]) nrerror("allocation failure 2 in matrix()");
		m[i] -= ncl;
	}
	return m;
}

double **dmatrix(long nrl,long nrh,long ncl,long nch)
{
	int i;
	double **m;
	m=(double **) calloc((unsigned) (nrh-nrl+1),sizeof(double*));
	if (!m) nrerror("allocation failure 1 in dmatrix()");
	m -= nrl;

	for(i=nrl;i<=nrh;i++) {
		m[i]=(double *) calloc((unsigned) (nch-ncl+1),sizeof(double));
		if (!m[i]) nrerror("allocation failure 2 in dmatrix()");
		m[i] -= ncl;
	}
	return m;
}

int **imatrix(long nrl,long nrh,long ncl,long nch)
{
	int i,**m;
	m=(int **)calloc((unsigned) (nrh-nrl+1),sizeof(int*));
	if (!m) nrerror("allocation failure 1 in imatrix()");
	m -= nrl;

	for(i=nrl;i<=nrh;i++) {
		m[i]=(int *)calloc((unsigned) (nch-ncl+1),sizeof(int));
		if (!m[i]) nrerror("allocation failure 2 in imatrix()");
		m[i] -= ncl;
	}
	return m;
}

unsigned char **cmatrix(long nrl,long nrh,long ncl,long nch)
{
	int i;
	unsigned char **m;
	m=(unsigned char **) calloc((unsigned) (nrh-nrl+1),sizeof(unsigned char*));
	if (!m) nrerror("allocation failure 1 in dmatrix()");
	m -= nrl;

	for(i=nrl;i<=nrh;i++) {
		m[i]=(unsigned char *) calloc((unsigned) (nch-ncl+1),sizeof(unsigned char));
		if (!m[i]) nrerror("allocation failure 2 in dmatrix()");
		m[i] -= ncl;
	}
	return m;
}

//求一个二维矩阵的子矩阵
float **submatrix(float **a,long oldrl,long oldrh,long oldcl,long oldch,long newrl,long newcl)
{
	int i,j;
	float **m;
	m=(float **) calloc((unsigned) (oldrh-oldrl+1),sizeof(float*));
	if (!m) nrerror("allocation failure in submatrix()");
	m -= newrl;

	for(i=oldrl,j=newrl;i<=oldrh;i++,j++) m[j]=a[i]+oldcl-newcl;
	return m;
}

//将一个实数一维数组转化为二维数组
float **convert_matrix(float *a,long nrl,long nrh,long ncl,long nch)
{
	int i,j,nrow,ncol;
	float **m;
	nrow=nrh-nrl+1;
	ncol=nch-ncl+1;
	m = (float **) calloc((unsigned) (nrow),sizeof(float*));
	if (!m) nrerror("allocation failure in convert_matrix()");
	m -= nrl;
	for(i=0,j=nrl;i<=nrow-1;i++,j++) m[j]=a+ncol*i-ncl;
	return m;
}

//前后向算法
//前向算法估计参数
void Forward(HMM *phmm,int T,int *O,double **alpha,double *pprob)
{
	int		i,j;	
	int		t;	
	double sum;	

	//初始化
	for(i=1;i<=phmm->N;i++)
		alpha[1][i]=phmm->pi[i]*phmm->B[i][O[1]];
	//递归
	for(t=1;t<T;t++){
		for(j=1;j<=phmm->N;j++){
			sum=0.0;
		for(i=1;j<=phmm->N;i++)
			sum+=alpha[t][i]*(phmm->A[i][j]);
		alpha[t+1][j]=sum*(phmm->B[j][O[t+1]]);
		}
	}
	//终止
	*pprob=0.0;
	for(i=1;i<=phmm->N;i++)
		*pprob+=alpha[T][i];
}

void ForwardWithScale(HMM*phmm,int T,int *O,double **alpha,double *scale,double *pprob)
// pprob是对数概率
{
	int		i,j;
	int		t;
	double sum;
	//初始化
	scale[1]=0.0;
	for(i=1;i<=phmm->N;i++){
		alpha[1][i]=phmm->pi[i]*(phmm->B[i][O[1]+1]);
		scale[1]+=alpha[1][i];
	}
	for(i=1;i<=phmm->N;i++)
		alpha[1][i]/=scale[1];
	//递归
	for(t=1;t<T;t++){
		scale[t+1]=0.0;
		for(j=1;j<=phmm->N;j++){
			sum=0.0;
		    for(i=1;i<=phmm->N;i++)
				sum+=alpha[t][i]*(phmm->A[i][j]);
			alpha[t+1][j]=sum*(phmm->B[j][O[t+1]]);
			scale[t+1]+=alpha[t+1][j];
		}
		for(j=1;j<=phmm->N;j++)
			alpha[t+1][j]/=scale[t+1];
	}
	//终止
	*pprob=0.0;
	for(t=1;t<=T;t++)
		*pprob+=log(scale[t]);
}

//后向算法估计参数
void Backward(HMM *phmm, int T, int *O, double **beta, double *pprob)
{
	int     i, j;
	int     t;
	double sum;
	//初始化
	for (i = 1; i <= phmm->N; i++)
		beta[T][i] = 1.0;
	//递归
	for (t = T - 1; t >= 1; t--) 
	{
		for (i = 1; i <= phmm->N; i++) 
		{
			sum = 0.0;
			for (j = 1; j <= phmm->N; j++)
				sum += phmm->A[i][j] * (phmm->B[j][O[t+1]])*beta[t+1][j];
			beta[t][i] = sum;
		}
	}
	//终止
	*pprob = 0.0;
	for (i = 1; i <= phmm->N; i++)
		*pprob += beta[1][i];
}

void BackwardWithScale(HMM *phmm, int T, int *O, double **beta, double *scale, double *pprob)
{
	int     i, j;  
	int     t;   
	double sum;
	//初始化
	for (i = 1; i <= phmm->N; i++)
		beta[T][i] = 1.0/scale[T]; 
	//递归
	for (t = T - 1; t >= 1; t--) 
	{
		for (i = 1; i <= phmm->N; i++) 
		{
			sum = 0.0;
			for (j = 1; j <= phmm->N; j++)
				sum += phmm->A[i][j] * (phmm->B[j][O[t+1]])*beta[t+1][j];
			beta[t][i] = sum/scale[t];
		}
	}
}

//Viterbi算法
#define VITHUGE  100000000000.0
void Viterbi(HMM *phmm, int T, int *O, double **delta, int **psi, int *q, double *pprob)
{
	int 	i, j;
	int  	t;
	int	maxvalind;
	double	maxval, val;

	//初始化
	for (i = 1; i <= phmm->N; i++) 
	{
		delta[1][i] = phmm->pi[i] * (phmm->B[i][O[1]]);
		psi[1][i] = 0;
	}	
	//递归
	for (t = 2; t <= T; t++) 
	{
		for (j = 1; j <= phmm->N; j++) 
		{
			maxval = 0.0;
			maxvalind = 1;	
			for (i = 1; i <= phmm->N; i++) 
			{
				val = delta[t-1][i]*(phmm->A[i][j]);
				if (val > maxval) {
					maxval = val;	
					maxvalind = i;	
				}
			}
			
			delta[t][j] = maxval*(phmm->B[j][O[t]]);
			psi[t][j] = maxvalind; 
		}
	}
	//终止
	*pprob = 0.0;
	q[T] = 1;
	for (i = 1; i <= phmm->N; i++) 
	{
		if (delta[T][i] > *pprob) 
		{
			*pprob = delta[T][i];	
			q[T] = i;
		}
	}
	for (t = T - 1; t >= 1; t--)
		q[t] = psi[t+1][q[t+1]];
}

void ViterbiLog(HMM *phmm, int T, int *O, double **delta, int **psi, int *q, double *pprob)
{
	int     i, j;
	int     t;
	int     maxvalind;
	double  maxval, val;
	double  **biot;

	//预处理 
	for (i = 1; i <= phmm->N; i++) 
		phmm->pi[i] = log(phmm->pi[i]);
	for (i = 1; i <= phmm->N; i++) 
		for (j = 1; j <= phmm->N; j++) 
		{
			phmm->A[i][j] = log(phmm->A[i][j]);
		}
	biot = dmatrix(1, phmm->N, 1, T);
	for (i = 1; i <= phmm->N; i++) 
		for (t = 1; t <= T; t++) 
		{
			biot[i][t] = log(phmm->B[i][O[t]]);
		}
	//初始化
	for (i = 1; i <= phmm->N; i++) 
	{
		delta[1][i] = phmm->pi[i] + biot[i][1];
		psi[1][i] = 0;
	}
	//递归
	for (t = 2; t <= T; t++) 
	{
		for (j = 1; j <= phmm->N; j++) 
		{
			maxval = -VITHUGE;
			maxvalind = 1;
			for (i = 1; i <= phmm->N; i++) 
			{
				val = delta[t-1][i] + (phmm->A[i][j]);
				if (val > maxval) 
				{
					maxval = val;
					maxvalind = i;
				}
			}
			delta[t][j] = maxval + biot[j][t]; 
			psi[t][j] = maxvalind;
		}
	}
	//终止
	*pprob = -VITHUGE;
	q[T] = 1;
	for (i = 1; i <= phmm->N; i++) 
	{
		if (delta[T][i] > *pprob) 
		{
			*pprob = delta[T][i];
			q[T] = i;
		}
	}
	//回朔
	for (t = T - 1; t >= 1; t--)
		q[t] = psi[t+1][q[t+1]];
}

//Baum-Welch算法
#define DELTA 0.001
void BaumWelch(HMM *phmm, int T, int *O, double **alpha, double **beta, double **gamma, int *pniter, double *plogprobinit, double *plogprobfinal)
{
	int	i, j, k;
	int	t, l = 0;
	double	logprobf, logprobb;
	double	numeratorA, denominatorA;
	double	numeratorB, denominatorB;
	double ***xi, *scale;
	double delta, deltaprev, logprobprev;
	deltaprev = 10e-70;
	xi = AllocXi(T, phmm->N);
	scale = dvector(1, T);

	ForwardWithScale(phmm, T, O, alpha, scale, &logprobf);
	*plogprobinit = logprobf; 
	BackwardWithScale(phmm, T, O, beta, scale, &logprobb);
	ComputeGamma(phmm, T, alpha, beta, gamma);
	ComputeXi(phmm, T, O, alpha, beta, xi);
	logprobprev = logprobf;

	do{	
		//重新估计t=1时，状态为i的频率
		for (i = 1; i <= phmm->N; i++) 
			phmm->pi[i] = .001 + .999*gamma[1][i];
		//重新估计转移矩阵和观察矩阵
		for (i = 1; i <= phmm->N; i++) 
		{ 
			denominatorA = 0.0;
			for (t = 1; t <= T - 1; t++) 
				denominatorA += gamma[t][i];

			for (j = 1; j <= phmm->N; j++) 
			{
				numeratorA = 0.0;
				for (t = 1; t <= T - 1; t++) 
					numeratorA += xi[t][i][j];
				phmm->A[i][j] = .001 + .999*numeratorA/denominatorA;
			}

			denominatorB = denominatorA + gamma[T][i]; 
			for (k = 1; k <= phmm->M; k++) 
			{
				numeratorB = 0.0;
				for (t = 1; t <= T; t++) 
				{
					if (O[t] == k) 
						numeratorB += gamma[t][i];
				}

				phmm->B[i][k] = .001 +.999*numeratorB/denominatorB;
			}
		}

		ForwardWithScale(phmm, T, O, alpha, scale, &logprobf);
		BackwardWithScale(phmm, T, O, beta, scale, &logprobb);
		ComputeGamma(phmm, T, alpha, beta, gamma);
		ComputeXi(phmm, T, O, alpha, beta, xi);

		//计算两次直接的概率差
		delta = logprobf - logprobprev; 
		logprobprev = logprobf;
		l++;
	}
	while (delta > DELTA); 
	*pniter = l;
	*plogprobfinal = logprobf; /* log P(O|estimated model) */
	FreeXi(xi, T, phmm->N);
	free_dvector(scale, 1, T);
}

void ComputeGamma(HMM *phmm, int T, double **alpha, double **beta, double **gamma)
{
	int 	i, j;
	int	t;
	double	denominator;

	for (t = 1; t <= T; t++) 
	{
		denominator = 0.0;
		for (j = 1; j <= phmm->N; j++) 
		{
			gamma[t][j] = alpha[t][j]*beta[t][j];
			denominator += gamma[t][j];
		}

		for (i = 1; i <= phmm->N; i++) 
			gamma[t][i] = gamma[t][i]/denominator;
	}
}

void ComputeXi(HMM* phmm, int T, int *O, double **alpha, double **beta, double ***xi)
{
	int i, j;
	int t;
	double sum;

	for (t = 1; t <= T - 1; t++) {
		sum = 0.0;	
		for (i = 1; i <= phmm->N; i++) 
			for (j = 1; j <= phmm->N; j++) 
			{
				xi[t][i][j] = alpha[t][i]*beta[t+1][j]
					*(phmm->A[i][j])
					*(phmm->B[j][O[t+1]]);
				sum += xi[t][i][j];
			}

		for (i = 1; i <= phmm->N; i++) 
			for (j = 1; j <= phmm->N; j++) 
				xi[t][i][j]  /= sum;
	}
}

double *** AllocXi(int T, int N)
{
	int t;
	double ***xi;
	xi = (double ***) malloc(T*sizeof(double **));
	xi --;
	for (t = 1; t <= T; t++)
		xi[t] = dmatrix(1, N, 1, N);
	return xi;
}

void FreeXi(double *** xi, int T, int N)
{
	int t;
	for (t = 1; t <= T; t++)
		free_dmatrix(xi[t], 1, N, 1, N);
	xi ++;
	free(xi);
}
