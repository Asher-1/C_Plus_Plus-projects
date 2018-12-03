//神经网络BP算法
CString m_result,m_result1;
int  decimal, sign;
char  buffer[200];
double x_out[InputN];				//输入层的输出值
double hn_out[HN];               	//隐含层的输出值
double y_out[OutN];              	//输出层的输出值
double y[OutN];                  	//输出层的期望输出值
double w[InputN][HN];            	//输入层到隐含层的权值
double v[HN][OutN];              	//隐含层到输出层的权值
double deltaw[InputN][HN];
double deltav[HN][OutN];
double hn_delta[HN];             	//隐含层的delta
double y_delta[OutN];            	//输出层的delta
double error;                    	//全局误差
double errlimit=0.001;            	//预设的全局误差极限
double alpha=0.1, beta=0.1;        	//预设的学习速率
int loop=0;                      	//训练次数
int times=50000;                 	//预设的训练次数上限
int i, j, m;
double max, min;
double sumtemp;
double errtemp;

struct{                      		//训练样本对.
	double input[InputN];
	double teach[OutN];
}data[datanum];

//初始化. 将连接权值、偏置系数置为(-1,1)之间的随机数
for(i=0; i<InputN; i++)        		//w的随机输入
	for(j=0; j<HN; j++){
		w[i][j] = ((double)rand()/32767.0)*2-1;
		deltaw[i][j] = 0;
	}

for(i=0; i<HN; i++)   			//v的随机输入
	for(j=0;j<OutN;j++){		
		v[i][j] = ((double)rand()/32767.0)*2-1;
		deltav[i][j] = 0;
	}
//训练.
while(loop < times){
	loop++;
	error = 0.0;
	
	for(m=0; m<datanum; m++){  	//对datanum个样本进行增量训练,m为样本个数
		//输入正向传播
		max=0.0;
		min=0.0;
		for(i=0; i<InputN; i++){ 	//设置输入层的输出
			x_out[i] = data[m].input[i];
			if (max < x_out[i]) 	//比较得出输入的最大最小值
		    	max = x_out[i];
	
		if (min > x_out[i]) 
				min = x_out[i];
		}

		for(i=0; i<InputN; i++){ 	//归一化后的输入值
			x_out[i] = (x_out[i] - min) / (max-min);
		}

		for(i=0; i<OutN; i++){  	//输出层的期望输出
			y[i] = data[m].teach[i];
		}			
	
		for(i=0; i<HN; i++){    	//计算隐含层的输出
			sumtemp = 0.0;
			for(j=0; j<InputN; j++)
			sumtemp += w[j][i] * x_out[j];
			hn_out[i] = tanh(sumtemp);   	//隐含层作用函数为y=tanh(x)
		}
				
		for(i=0; i<OutN; i++){           	//计算输出层的输出
			sumtemp = 0.0;
			for(j=0; j<HN; j++) sumtemp += v[j][i] * hn_out[j];
			y_out[i] = g(sumtemp);     	//输出层作用函数为sigmod函数
		}
	
//误差反向传播
		for(i=0; i<OutN; i++){
			errtemp = y[i] - y_out[i]; 
			y_delta[i] = errtemp * g(y_out[i]) * (1.0 - g(y_out[i]));
			error += (errtemp * errtemp);  	//所有样本的全局误差
		}
		
		for(i=0; i<HN; i++){               	//计算隐含层的delta
			errtemp = 0.0;
			for(j=0; j<OutN; j++)	errtemp += y_delta[j] * v[i][j];
			hn_delta[i] = errtemp * (1.0 + hn_out[i]) * (1.0 - hn_out[i]);
		}
		
		for(i=0; i<OutN; i++){       		//调整输出层的权值和偏置系数
			for(j=0; j<HN; j++){
				deltav[j][i] = alpha * deltav[j][i] + beta * y_delta[i] * hn_out[j];
				v[j][i] += deltav[j][i];
			}
		}
		
		for(i=0; i<HN; i++){        	//调整隐含层的权值和偏置系数
			for(j=0; j<InputN; j++){
				deltaw[j][i] = alpha * deltaw[j][i] + beta * hn_delta[i] * x_out[j];
				w[j][i] += deltaw[j][i];
			}
		}
	}
		
//全局误差判断
	error = error/2;
	if(loop%1000==0){
	    m_result="全局误差Error=";
		sprintf( buffer, "%f", error);
		m_result+=buffer;
		m_result+="\r\n";
		m_result+=m_result1;
	}
	if(error < errlimit)	break;
}
