#ifndef THREEDSHOW_H
#define THREEDSHOW_H

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>

#include "Camera.h"
#include "dice.h"

// QOpenGL窗口类继承于QOpenGLWidget
class ThreeDShow : public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT

public:
    ThreeDShow(QWidget *parent = nullptr);
    ~ThreeDShow();

protected:
    /// @brief 设置OpenGL呈现上下文，定义显示列表等。只在程序开始时运行一次,之后不会再运行
    virtual void initializeGL() override;

    /// @brief 渲染OpenGL场景。会在窗口位置移动时被调用，或是窗口在隐藏和显示之间变换时，被调用
    virtual void paintGL() override;

    /// @brief 设置OpenGL视区、投影等。只在在窗口尺寸发生变化时，会自动运行，然后触发paint事件，从而调用paintGL()事件处理函数
    /// @param w 窗口宽度
    /// @param h 窗口高度
    virtual void resizeGL(int w, int h) override;

    /// @brief 定时器事件
    /// @param event
    virtual void timerEvent(QTimerEvent *event) override;

private:
    QMatrix4x4 m_projection;

    Camera m_camera;
    QVector<Model *> m_models;
};

#endif // THREEDSHOW_H
