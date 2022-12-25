/// @Paper         参考博客https://blog.csdn.net/github_18974657/article/details/122578897
/// @PaperAuthor   Ninsun Closear
/// @Year          
/// @              
/// @Descripttion  OpenGL使用基础教程
/// @version       
/// @Author        陈朝
/// @Date          2022-12-15 10:09:41
/// @LastEditors   陈朝
/// @LastEditTime  2022-12-25 16:23:17

#include "threedshow.h"

ThreeDShow::ThreeDShow(QWidget *parent)
    : QOpenGLWidget(parent), VBO(QOpenGLBuffer::VertexBuffer), EBO(QOpenGLBuffer::IndexBuffer)
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
    initializeOpenGLFunctions();          // 初始化OpenGL函数
    glEnable(GL_DEPTH_TEST);              // 开启深度测试，前后遮挡
    glClearColor(0.0f, 0.5f, 0.9f, 1.0f); // 设置刷新时的背景颜色
    glClear(GL_COLOR_BUFFER_BIT);
    VAO.create(); // 创建VAO对象 Vertex Array Object，用来管理VBO。
    VAO.bind();   // 通知OpenGL使用

    EBO.create();                // 创建EBO对象 Element Buffer Object，用来存放顶点索引数据
    EBO.bind();                  // 通知OpenGL使用
    uint elements[] = {0, 1, 3}; // 使用的点索引
    EBO.allocate(elements, sizeof(elements));

    VBO.create(); // 创建VBO对象 Vertex Buffer Object，用来缓存用户传入的顶点数据。
    VBO.bind();   // 通知OpenGL使用

    // 顶点						纹理
    float vertices[] = {
        // 点数1面
        -1, 1, 1, 0.50, 0.25,  // 左上
        -1, -1, 1, 0.50, 0.50, // 左下
        1, -1, 1, 0.75, 0.50,  // 右下
        1, 1, 1, 0.75, 0.25,   // 右上
        // 点数6面
        1, 1, -1, 0.00, 0.25,   // 左上
        1, -1, -1, 0.00, 0.50,  // 左下
        -1, -1, -1, 0.25, 0.50, // 右下
        -1, 1, -1, 0.25, 0.25,  // 右上
        // 点数2面
        1, 1, 1, 0.75, 0.25,   // 左上
        1, -1, 1, 0.75, 0.50,  // 左下
        1, -1, -1, 1.00, 0.50, // 右下
        1, 1, -1, 1.00, 0.25,  // 右上
        // 点数5面
        -1, 1, -1, 0.25, 0.25,  // 左上
        -1, -1, -1, 0.25, 0.50, // 左下
        -1, -1, 1, 0.50, 0.50,  // 右下
        -1, 1, 1, 0.50, 0.25,   // 右上
        // 点数3面
        -1, 1, -1, 0.00, 0.00, // 左上
        -1, 1, 1, 0.00, 0.25,  // 左下
        1, 1, 1, 0.25, 0.25,   // 右下
        1, 1, -1, 0.25, 0.00,  // 右上
        // 点数4面
        -1, -1, 1, 0.00, 0.50,  // 左上
        -1, -1, -1, 0.00, 0.75, // 左下
        1, -1, -1, 0.25, 0.75,  // 右下
        1, -1, 1, 0.25, 0.50,   // 右上
    };

    VBO.allocate(vertices, sizeof(vertices)); // 绑定定义的顶点

    shaderProgram.create();
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shapes.vert");   // 通过资源文件的方式加载Shader的顶点着色器
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shapes.frag"); // 通过资源文件的方式加载Shader的片段着色器
    shaderProgram.link();

    // 绑定顶点坐标信息, 从0 * sizeof(float)字节开始读取3个float, 因为一个顶点有5个float数据, 所以下一个数据需要偏移5 * sizeof(float)个字节
    shaderProgram.setAttributeBuffer("vPos", GL_FLOAT, 0 * sizeof(float), 3, 5 * sizeof(float));
    shaderProgram.enableAttributeArray("vPos");

    // 绑定纹理坐标信息, 从3 * sizeof(float)字节开始读取2个float, 因为一个顶点有5个float数据, 所以下一个数据需要偏移5 * sizeof(float)个字节
    shaderProgram.setAttributeBuffer("vTexture", GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
    shaderProgram.enableAttributeArray("vTexture");

    m_texture = new QOpenGLTexture(QImage(":/shaders/dice1000.png")); // 加载纹理图片
    VAO.release();                                                    // 与调用bind的对象相对应
    VBO.release();
    EBO.release();
}

void ThreeDShow::paintGL()
{
    VAO.bind();
    m_texture->bind();
    shaderProgram.bind();

    m_view.setToIdentity();
    m_view.lookAt(QVector3D(3, 3, 3), QVector3D(0, 0, 0), QVector3D(0, 1, 0)); // 视图矩阵用于模拟摄像机

    // 绑定变换矩阵
    shaderProgram.setUniformValue("projection", m_projection);
    shaderProgram.setUniformValue("view", m_camera.view());
    shaderProgram.setUniformValue("model", m_model);

    // 绘制
    for (int i = 0; i < 6; ++i)
    {
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }
    // glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
    VAO.release();
    shaderProgram.release();
    m_texture->release();
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

    m_angle += 1;
    if (m_angle >= 360)
        m_angle = 0;
    m_model.setToIdentity();
    m_model.rotate(m_angle, 0, 1, 0); // 绕（0，1，0）轴进行一定角度的旋转
    repaint();                        // 重新绘制
}