#include <glut.h> 
#include "stdio.h"
#include "GLfont.h"
#include <vector>
//#include "iostream"

#define WIN_WIDTH 600
#define WIN_HEIGHT 600

#define BORDER_SIZE 40

// font class
GLfont font;

void myDisplay(void) 
{    
	std::vector<std::vector<double> > data_;
	std::vector<double> p_temp;

	p_temp.push_back(0.0130162);
	p_temp.push_back(0.94);
	data_.push_back(p_temp);

	p_temp.clear();
	p_temp.push_back(0.0432221);
	p_temp.push_back(0.902222);
	data_.push_back(p_temp);

	p_temp.clear();
	p_temp.push_back(0.132769);
	p_temp.push_back(0.864444);
	data_.push_back(p_temp);

	p_temp.clear();
	p_temp.push_back(0.351267);
	p_temp.push_back(0.826667);
	data_.push_back(p_temp);

	p_temp.clear();
	p_temp.push_back(0.775798);
	p_temp.push_back(0.788889);
	data_.push_back(p_temp);

	p_temp.clear();
	p_temp.push_back(1.47011);
	p_temp.push_back(0.751111);
	data_.push_back(p_temp);

	p_temp.clear();
	p_temp.push_back(2.4696);
	p_temp.push_back(0.713333);
	data_.push_back(p_temp);

	p_temp.clear();
	p_temp.push_back(3.73663);
	p_temp.push_back(0.675556);
	data_.push_back(p_temp);

	p_temp.clear();
	p_temp.push_back(5.19858);
	p_temp.push_back(0.637778);
	data_.push_back(p_temp);

	p_temp.clear();
	p_temp.push_back(6.72799);
	p_temp.push_back(0.6);
	data_.push_back(p_temp);

	double y_max = 0;
	double y_min = 0;
	double y_scale = 0;
	double x_max = 0;
	double x_min = 0;
	double x_scale = 0;

	for (size_t i = 0; i < data_.size(); i++)
	{

			if (data_[i][0] < x_min)
			{
				x_min = data_[i][0];
			}
			else if (data_[i][0] > x_max)
			{
				x_max = data_[i][0];
			}

			if (data_[i][1] < y_min)
			{
				y_min = data_[i][1];
			}
			else if (data_[i][1] > y_max)
			{
				y_max = data_[i][1];
			}
	}

	const float half_win_width = WIN_WIDTH / 2, half_win_height = WIN_HEIGHT / 2;
	const float x_win_range = WIN_WIDTH - 2 * BORDER_SIZE;
	const float y_win_range = WIN_HEIGHT - 2 * BORDER_SIZE;

	double x_range = x_max - x_min;
	double y_range = y_max - y_min;
	x_scale = x_win_range / x_range;
	y_scale = y_win_range / y_range;

	const float arrow_delta = 4;
	const float origin_x = -half_win_width + BORDER_SIZE;
	const float origin_y = -half_win_height + BORDER_SIZE;

	const float head_x = origin_x + WIN_HEIGHT -  1.2*BORDER_SIZE;;
	const float head_y = origin_y + WIN_HEIGHT -  1.2*BORDER_SIZE;

	int chw = 6;
	int chh = 10;

	int x_step = chw * 6;
	int y_step = chh * 2;

	glClear(GL_COLOR_BUFFER_BIT);
	// draw grid
	glLineWidth(1);
	float i; int j;
	for (i = origin_y, j = 0; i < head_y; i += y_step, ++j)
	{
		glBegin(GL_LINES);
		if (j%2==0) glColor3f(0.6f,0.6f,0.6f);
		else  glColor3f(0.9,0.9,0.9);
		glVertex3f(origin_x,i,-0.9);
		glVertex3f(head_x,i,-0.9);
		glEnd();
	}
	for (i= origin_x, j=0; i < head_x ; i += x_step, j++)
	{         
		glBegin(GL_LINES);
		if (j%2==0) glColor3f(0.6f,0.6f,0.6f);
		else  glColor3f(0.9,0.9,0.9);
		glVertex3f(i, origin_y, -0.9);
		glVertex3f(i, head_y, -0.9);
		glEnd();
	}

	// draw title
	font.CreateFont("宋体", 15, FW_DONTCARE, false, false, false);
	char str[256];
	sprintf(str, "Polarization  Curve");
	font.ShowText(origin_x + half_win_width - 2* BORDER_SIZE, half_win_height - BORDER_SIZE/2, str, 1, 0, 0);

	// draw axis
	glLineWidth(3);
	glColor3f(0, 0, 0);
	glBegin(GL_LINES);
	glVertex3f(origin_x, origin_y, 0.0);
	glVertex3f(head_x, origin_y, 0.0);
	glVertex3f(origin_x, origin_y,0.0);
	glVertex3f(origin_x, head_y, 0.0);
	glEnd();

	// draw x arrow 
	glLineWidth(1);
	glBegin(GL_TRIANGLES);
	glVertex3f(head_x - arrow_delta, origin_y - arrow_delta,0.0);
	glVertex3f(head_x - arrow_delta, origin_y + arrow_delta,0.0);
	glVertex3f(head_x, origin_y, 0.0);
	// draw y arrow
	glBegin(GL_TRIANGLES);
	glVertex3f(origin_x - arrow_delta, head_y - arrow_delta,0.0);
	glVertex3f(origin_x + arrow_delta, head_y - arrow_delta,0.0);
	glVertex3f(origin_x, head_y,0.0);
	glEnd();


	// draw x scale
	font.CreateFont("宋体", 10, FW_DONTCARE, false, false, false);
	double x_scale_pixes = chw * 6;
	for (int i = 1; i < ceil(x_win_range / x_scale_pixes) + 1; i++)
	{
		char str[256];
		sprintf(str, "%.4f", float( i * x_scale_pixes / x_scale));
		font.ShowText(origin_x + i * x_scale_pixes - 10, origin_y - BORDER_SIZE/2, str, 1, 0, 0);
	}

	font.CreateFont("宋体", 12, FW_DONTCARE, false, false, false);
	font.ShowText(head_x - 2*BORDER_SIZE, origin_y + BORDER_SIZE / 2, "Current [A/cm2]", 0, 0, 1);

	// draw y scale
	font.CreateFont("宋体", 10, FW_DONTCARE, false, false, false);
	double y_scale_pixes = chh * 2;
	for (int i = 0; i < ceil(y_win_range / y_scale_pixes) + 1; i++)
	{
		char str[256];
		sprintf(str, "%.5f", float(i * y_scale_pixes / y_scale));
		font.ShowText(origin_x - BORDER_SIZE, origin_y + i * y_scale_pixes - chh/2, str, 1, 0, 0);
	}

	font.CreateFont("宋体", 12, FW_DONTCARE, false, false, false);
	font.ShowText(origin_x + BORDER_SIZE / 2, head_y - BORDER_SIZE/2, "cell voltage [V]", 0, 0, 1);


	glPopMatrix();
	glFlush();

	// draw original point
	glColor3f( 1.0f, 0.0f, 0.0f ); 
    glPointSize( 4 );
    glBegin( GL_POINTS );
    {
        glVertex3f(origin_x, origin_y, 0.0f);  // draw point at coordinate (0,0,0)
    }
    glEnd();

	// draw polarization curve data
	float pre_x, pre_y, cur_x, cur_y;
	for (size_t i = 1; i < data_.size(); i++)
	{
		pre_x = origin_x + (data_[i-1][0] - x_min) * x_scale;
		pre_y = origin_y + (data_[i-1][1] - y_min) * y_scale;
		cur_x = origin_x + (data_[i][0] - x_min) * x_scale;
		cur_y = origin_y + (data_[i][1] - y_min) * y_scale;

		glBegin(GL_LINES);
		glVertex3f(pre_x, pre_y, 0.0f);
		glVertex3f(cur_x, cur_y, 0.0f);
		glEnd();
	}

	glFlush();
}  


void ChangeSize( GLsizei w, GLsizei h )
{
    GLfloat nRange = (GLfloat)WIN_WIDTH / 2;
    // Prevent a divide by zero
    if(h == 0)
        h = 1;
	if (w== 0)
		w = 1;
    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);
    // Reset projection matrix stack
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // Establish clipping volume (left, right, bottom, top, near, far)
    if (w <= h) 
        glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    else 
        glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);

    // Reset Model view matrix stack
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


int main(int argc, char *argv[])
{   
	glutInit(&argc, argv);     
	glutInitDisplayMode(GLUT_RGB |            
		GLUT_SINGLE);    
	glutInitWindowPosition(100, 100);    
	glutInitWindowSize(WIN_WIDTH, WIN_HEIGHT);
	glutCreateWindow("第一个OpenGL程序");  
	glutDisplayFunc(&myDisplay);
	glutReshapeFunc( ChangeSize );
	// glutDisplayFunc(&Display);
	
	glClearColor(1.0, 1.0, 1.0, 1);
	glColor3f(1.0f, 0.0f, 0.0f);
	
	glutMainLoop();     
	return 0; 
}