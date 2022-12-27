/// @Paper         参考博客https://blog.csdn.net/github_18974657/article/details/122578897
/// @PaperAuthor   Ninsun Closear
/// @Year
/// @
/// @Descripttion  OpenGL使用基础教程
/// @version
/// @Author        陈朝
/// @Date          2022-12-15 10:09:41
/// @LastEditors   陈朝
/// @LastEditTime  2022-12-27 13:57:07

#include "threedshow.h"

ThreeDShow::ThreeDShow(QWidget *parent) : QOpenGLWidget(parent)
{
	startTimer(1000 / 60); // 开启定时器

	m_camera.move(-6, 0, 3);
	m_camera.look(0, 30, 0);
	m_camera.update();

	installEventFilter(&m_camera);
}
ThreeDShow::~ThreeDShow()
{
	makeCurrent();
}

void ThreeDShow::initializeGL()
{
	initializeOpenGLFunctions();
	glClearColor(0, 0.5, 0.7, 0.01); // 蓝色背景

	for (int i = 0; i < 3; ++i)
	{
		auto _dice = new Dice();
		_dice->init();
		_dice->setPos({0, i * 3.f, 0});
		m_models << _dice;
	}
}

void ThreeDShow::paintGL()
{
	glEnable(GL_DEPTH_TEST); // 开启深度测试
	for (auto dice : m_models)
	{
		dice->paint(m_projection, m_camera.view()); // 逐个模型渲染
	}
}

void ThreeDShow::resizeGL(int w, int h)
{
	m_projection.setToIdentity(); // 矩阵运算都会在一个单位矩阵上进行

	// 构建出一个基于你的窗口大小的近大远小的投影矩阵了。以60度视场角来进行投影
	m_projection.perspective(60, (float)w / h, 0.001, 1000); // 最近的成像面0.001，最远的成像面1000
}

// 定时实现模型旋转
void ThreeDShow::timerEvent(QTimerEvent *event)
{
	m_camera.update();
	float _speed = 1;
	for (auto dice : m_models)
	{
		float _y = dice->rotate().y() + _speed;
		if (_y >= 360)
			_y -= 360;
		dice->setRotate({0, _y, 0});
		++_speed;
	}
	repaint();
}