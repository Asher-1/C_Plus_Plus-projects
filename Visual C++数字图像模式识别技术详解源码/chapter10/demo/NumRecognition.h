// NumRecognition.h: interface for the NumRecognition class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NUMRECOGNITION_H__3582881A_B701_43D1_9863_7841D24C4E55__INCLUDED_)
#define AFX_NUMRECOGNITION_H__3582881A_B701_43D1_9863_7841D24C4E55__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct number_no
{
	int number;
	int no;
};


class NumRecognition  
{
public:
	NumRecognition();
	virtual ~NumRecognition();
	number_no LeastDistance();
	double match(double s1[], double s2[]);

};

#endif // !defined(AFX_NUMRECOGNITION_H__3582881A_B701_43D1_9863_7841D24C4E55__INCLUDED_)
