
void GutUserControl_RotateObject(Matrix4x4 &object_matrix, float time_advance)
{
	// 读取鼠标
	GutMouseInfo mouse;
	if ( GutReadMouse(&mouse) )
	{
		float rotation_speed = 1.0 * time_advance;

		// 如果左击，就旋转镜头
		if ( mouse.button[0] ) 
		{
			Matrix4x4 rotate_matrix;
			
			rotate_matrix.RotateY_Replace(mouse.x * rotation_speed);
			rotate_matrix.RotateX(mouse.y * rotation_speed);

			object_matrix = object_matrix * rotate_matrix;
		}

		// 用滚轮来缩放对象
		if ( mouse.z )
		{
			float scale = 1.0f + mouse.z / 1000.0f;
			object_matrix.Scale(scale, scale, scale);
		}
	}
}

void GetUserControl_FPSCamera(Vector4 &vEye, Vector4 &vUp, Vector4 &vLookAt, float time_advance)
{
	// 读取鼠标
	GutMouseInfo mouse;
	GutReadMouse(&mouse);
	// 读取键盘
	char keyboard_state[256];
	GutReadKeyboard(keyboard_state);

	float moving_speed = 2.0f * time_advance;
	float rotation_speed = 1.0f * time_advance;

	// 极坐标系统
	static float theta = -MATH_PI * 0.5f;
	static float phi = 0.0f;

	// 如果左击，就旋转镜头
	if ( mouse.button[0] ) 
	{
		theta += mouse.x * rotation_speed;
		phi -= mouse.y * rotation_speed;
	}

	float sin_phi, cos_phi;
	float sin_theta, cos_theta;
	
	FastMath::SinCos(phi, sin_phi, cos_phi);
	FastMath::SinCos(theta, sin_theta, cos_theta);

	// 计算镜头的法线
	Vector4 camera_facing;

	camera_facing[0] = cos_phi * cos_theta;
	camera_facing[1] = sin_phi;
	camera_facing[2] = cos_phi * sin_theta;

	// 计算镜头正上方的轴向
	Vector4 camera_up;
	FastMath::SinCos(phi + MATH_PI*0.5f, sin_phi, cos_phi);

	camera_up[0] = cos_phi * cos_theta;
	camera_up[1] = sin_phi;
	camera_up[2] = cos_phi * sin_theta;

	// 获得镜面右方的方向
	Vector4 camera_right = Vector3CrossProduct(camera_up, camera_facing);

	// 按下W或方向键向上
	if ( keyboard_state[GUTKEY_W] || keyboard_state[GUTKEY_UP] )
	{
		vEye += camera_facing * moving_speed;
	}
	// 按下S或方向键向下
	if ( keyboard_state[GUTKEY_S] || keyboard_state[GUTKEY_DOWN] )
	{
		vEye -= camera_facing * moving_speed;
	}
	// 按下A或方向键向左
	if ( keyboard_state[GUTKEY_A] || keyboard_state[GUTKEY_LEFT] )
	{
		vEye += camera_right * moving_speed;
	}
	// 按下D或方向键向右
	if ( keyboard_state[GUTKEY_D] || keyboard_state[GUTKEY_RIGHT] )
	{
		vEye -= camera_right * moving_speed;
	}

	// 计算出镜头对准的点, 生成镜头转换矩阵时会用到.
	vLookat = vEye + camera_facing;
	// 因为是对2个轴转动, 需要更新镜头朝上的轴
	vUp = camera_up;

	vEye[3] = vUp[3] = vLookAt[3] = 1.0f;
}
