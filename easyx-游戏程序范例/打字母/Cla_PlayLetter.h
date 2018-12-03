/***********************************************
//函数名称：Cla_PlayLetter类
//函数说明：继承自Cla_LivingThings
//函数作者：百度:haha1111111113
//函数版本：1.1.4
//完成时间：2013.4.25
//最后修改时间：2013.5.19
//版权：GPL标准
***********************************************/
#pragma   once
#include "Cla_LivingThings.h"

#ifndef Cla_PlayLetter_H
#define Cla_PlayLetter_H

struct Stru_PLSLF:Stru_LTSMC
{
	char letter;
	int LetterSize;
	int last_x_coo;
	int last_y_coo;
};

class Cla_PLGLSMC:public Cla_GLSMC
{
public:
	char letter;
	int LetterSize;
	Cla_PLGLSMC()
	{
		letter = 'A';
		LetterSize = 36;
	}
	Cla_PLGLSMC(char &letter,int LetterSize,int letter_x,int letter_y,int ASB_x,int ASB_y,int ASE_x,int ASE_y)
	{
		this->letter = letter;
		this->LetterSize = LetterSize;
		this->LivingThings_x_coo = letter_x;
		this->LivingThings_y_coo = letter_y;
		this->ActionSpaceBegin_x_coo = ASB_x;
		this->ActionSpaceBegin_y_coo = ASB_y;
		this->ActionSpaceEnd_x_coo = ASE_x;
		this->ActionSpaceEnd_y_coo = ASE_y;
	}
};

class Cla_PlayLetter:public Cla_LivingThings
{
public:
	Cla_PlayLetter();
	Cla_PlayLetter(Cla_PLGLSMC &GetMsg);
	void AgainGaveFeature(Cla_PLGLSMC &GetMsg);
	Stru_PLSLF SendLetterFeature();
	void sqecial(int value);
private:
	char letter;
	int LetterSize;
};



#endif