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
#include "stdafx.h"
#include "Cla_PlayLetter.h"

Cla_PlayLetter::Cla_PlayLetter()
{
	letter = 'A';
	LetterSize = 36;
}
Cla_PlayLetter::Cla_PlayLetter(Cla_PLGLSMC &GetMsg)
{
	letter = GetMsg.letter;
	LetterSize = GetMsg.LetterSize;
	ActionSpaceBegin_x_coo = GetMsg.ActionSpaceBegin_x_coo;
	ActionSpaceBegin_y_coo = GetMsg.ActionSpaceBegin_y_coo;
	ActionSpaceEnd_x_coo = GetMsg.ActionSpaceEnd_x_coo;
	ActionSpaceEnd_y_coo = GetMsg.ActionSpaceEnd_y_coo;
	now_x_coo = GetMsg.LivingThings_x_coo;
	now_y_coo = GetMsg.LivingThings_y_coo;
	x_coo_increment = GetMsg.LivingThings_x_coo_increment;
	y_coo_increment = GetMsg.LivingThings_y_coo_increment;
}

void Cla_PlayLetter::AgainGaveFeature(Cla_PLGLSMC &GetMsg)
{
	life = true;
	letter = GetMsg.letter;
	LetterSize = GetMsg.LetterSize;
	ActionSpaceBegin_x_coo = GetMsg.ActionSpaceBegin_x_coo;
	ActionSpaceBegin_y_coo = GetMsg.ActionSpaceBegin_y_coo;
	ActionSpaceEnd_x_coo = GetMsg.ActionSpaceEnd_x_coo;
	ActionSpaceEnd_y_coo = GetMsg.ActionSpaceEnd_y_coo;
	now_x_coo = GetMsg.LivingThings_x_coo;
	now_y_coo = GetMsg.LivingThings_y_coo;
	x_coo_increment = GetMsg.LivingThings_x_coo_increment;
	y_coo_increment = GetMsg.LivingThings_y_coo_increment;
	LivingThingsRecord(0,1,1);
}

Stru_PLSLF Cla_PlayLetter::SendLetterFeature()
{
	Stru_PLSLF STFeature;
	Stru_PLSLF &stf = STFeature;
	stf.letter = letter;
	stf.LetterSize = LetterSize;
	stf.now_x_coo = now_x_coo;
	stf.now_y_coo = now_y_coo;
	stf.last_x_coo = last_x_coo;
	stf.last_y_coo = last_y_coo;
	stf.life = life;
	stf.NaturalDeath = NaturalDeath;
	stf.UnnaturalDeath = UnnaturalDeath;
	stf.transmigrate = transmigrate;
	stf.CreateLifeTotalNumber = CreateLifeTotalNumber;
	return stf;
}
void Cla_PlayLetter::sqecial(int value)
{
	y_coo_increment += value;
}