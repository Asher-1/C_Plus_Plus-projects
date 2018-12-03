/***********************************************
//函数名称：Cla_LivingThings类
//函数说明：为活体运动类创建的父类
//函数作者：百度:haha1111111113
//函数版本：1.1.4
//完成时间：2013.4.25
//最后修改时间：2013.5.19
//版权：GPL标准
***********************************************/
#pragma   once
#ifndef Cla_LivingThings_H
#define Cla_LivingThings_H

class Cla_GLSMC
{
public:
	int ActionSpaceBegin_x_coo;
	int ActionSpaceBegin_y_coo;
	int ActionSpaceEnd_x_coo;
	int ActionSpaceEnd_y_coo;
	int LivingThings_x_coo;
	int LivingThings_y_coo;
	int LivingThings_x_coo_increment;
	int LivingThings_y_coo_increment;
	Cla_GLSMC()
	{
		int ActionSpaceBegin_x_coo = 0;
		int ActionSpaceBegin_y_coo = 0;
		int ActionSpaceEnd_x_coo = 0;
		int ActionSpaceEnd_y_coo = 0;
		LivingThings_x_coo = 0;
		LivingThings_y_coo = 0;
		LivingThings_x_coo_increment = 0;
		LivingThings_y_coo_increment = 0;	
	}
	Cla_GLSMC(int &ASB_x,int &ASB_y,int &ASE_x,int &ASE_y,int &LT_x,int &LT_y,int &LT_x_inc,int &LT_y_inc)
	{
		int ActionSpaceBegin_x_coo = ASB_x;
		int ActionSpaceBegin_y_coo = ASB_y;
		int ActionSpaceEnd_x_coo = ASE_x;
		int ActionSpaceEnd_y_coo = ASE_y;
		LivingThings_x_coo = LT_x;
		LivingThings_y_coo = LT_y;
		LivingThings_x_coo_increment = LT_x_inc;
		LivingThings_y_coo_increment = LT_y_inc;		
	}
};

struct Stru_LTSMC
{
	bool life;
	int now_x_coo;
	int now_y_coo;
	int NaturalDeath;
	int UnnaturalDeath;
	int transmigrate;
	int CreateLifeTotalNumber;
};

class Cla_LivingThingsControlCenter
{
protected:
	Cla_GLSMC LTMsg;
	static void ProcessingCenter();
	static void DataCenter();
public:
	static void MessageCenter(Cla_GLSMC &GetMsg);
};

class Cla_LivingThings
{
public:
	Cla_LivingThings();
	Cla_LivingThings(Cla_GLSMC &GLMsg);
	void LivingThingsAction();
	void GaveLife(Cla_GLSMC &GLMsg);
	void LivingThingsAccident();
	Stru_LTSMC SendMessageCenter();
	friend void Cla_LivingThingsControlCenter::MessageCenter(Cla_GLSMC &GetMsg);
protected:
	void LivingThingsRecord(int UD = 0,int TS = 0,int CL = 0);
	int ActionSpaceBegin_x_coo;
	int ActionSpaceBegin_y_coo;
	int ActionSpaceEnd_x_coo;
	int ActionSpaceEnd_y_coo;
	int now_x_coo;
	int now_y_coo;
	int x_coo_increment;
	int y_coo_increment;
	int last_x_coo;
	int last_y_coo;
	int later_x_coo;
	int later_y_coo;
	bool life;
	int NaturalDeath;
	int UnnaturalDeath;
	int transmigrate;
	static int CreateLifeTotalNumber;
};

#endif