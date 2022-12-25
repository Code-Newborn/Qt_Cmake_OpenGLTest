#include "Camera.h"

#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLWidget>

Camera::Camera(QObject *parent)
    : QObject(parent)
{
}

Camera::~Camera()
{
}

void Camera::update()
{
    auto _move = m_move.normalized() * m_moveSpeed;
    move(_move.x(), _move.y(), _move.z());

    m_view.setToIdentity();

    // 第一个参数为摄像机位置
    // 第二个参数为目标位置
    // 第三个参数为世界空间中的上向量（用来计算右向量使用的那个上向量）
    m_view.lookAt(m_pos, m_pos + m_front, m_up);
}

void Camera::look(float yaw, float pitch, float roll)
{
    m_yaw += yaw;
    while (m_yaw >= 360)
        m_yaw -= 360;
    while (m_yaw < 0)
        m_yaw += 360;
    m_pitch += pitch;
    if (m_pitch > 89)
        m_pitch = 89;
    if (m_pitch < -89)
        m_pitch = -89;

    QVector3D _front{0, 0, -1}; // 视角方向
    QMatrix4x4 _mat;
    _mat.setToIdentity();
    _mat.rotate(m_pitch, 1, 0, 0);
    _mat.rotate(m_yaw, 0, 1, 0);
    m_front = _front * _mat;
}

void Camera::move(float x, float y, float z)
{
    auto _right = QVector3D::crossProduct(m_front, m_up).normalized(); // 向量叉乘得右向量（单位向量）
    auto _front = QVector3D::crossProduct(m_up, _right).normalized();  // 向量叉乘得前向量（单位向量）
    m_pos += _front * x;
    m_pos += _right * y;
    m_pos += m_up * z;
}

bool Camera::eventFilter(QObject *obj, QEvent *ev)
{
    auto _widget = qobject_cast<QOpenGLWidget *>(obj); // 需要在UI中设置focusPolicy为ClickFocus
    if (_widget)
    {
        if (ev->type() == QEvent::KeyPress) // 按键按下
        {
            auto event = static_cast<QKeyEvent *>(ev);
            if (event->key() == Qt::Key_Escape) // 按下Esc键
            {
                _widget->setMouseTracking(false);
                _widget->setCursor(Qt::ArrowCursor);
            }
            else if (event->key() == Qt::Key_W)
            {
                m_move.setX(1); // X向增1
            }
            else if (event->key() == Qt::Key_S)
            {
                m_move.setX(-1); // X向减1
            }
            else if (event->key() == Qt::Key_A)
            {
                m_move.setY(-1); // Y向减1
            }
            else if (event->key() == Qt::Key_D)
            {
                m_move.setY(1); // Y向增1
            }
            else if (event->key() == Qt::Key_Space)
            {
                m_move.setZ(1); // Z向增1
            }
            else if (event->key() == Qt::Key_C)
            {
                m_move.setZ(-1); // Z向减1
            }
        }
        else if (ev->type() == QEvent::KeyRelease) // 按键释放
        {
            auto event = static_cast<QKeyEvent *>(ev);
            if (event->key() == Qt::Key_W)
            {
                m_move.setX(0);
            }
            else if (event->key() == Qt::Key_S)
            {
                m_move.setX(0);
            }
            else if (event->key() == Qt::Key_A)
            {
                m_move.setY(0);
            }
            else if (event->key() == Qt::Key_D)
            {
                m_move.setY(0);
            }
            else if (event->key() == Qt::Key_Space)
            {
                m_move.setZ(0);
            }
            else if (event->key() == Qt::Key_C)
            {
                m_move.setZ(0);
            }
        }
        else if (ev->type() == QEvent::MouseButtonPress)
        {
            auto _lastPos = _widget->mapToGlobal(_widget->rect().center()); // 记录按下的鼠标全局位置
            QCursor::setPos(_lastPos);

            _widget->setMouseTracking(true);     // 窗口部件跟踪鼠标生效
            _widget->setCursor(Qt::BlankCursor); // 隐藏鼠标
        }
        else if (ev->type() == QEvent::MouseMove)
        {
            auto event = static_cast<QMouseEvent *>(ev);
            auto _lastPos = _widget->mapToGlobal(_widget->rect().center()); // mapToGlobal将当前控件的相对位置转换为屏幕绝对位置
            QCursor::setPos(_lastPos);                                      // 鼠标始终置于QopenGLWidget中心

            auto _move = event->globalPos() - _lastPos;                // 鼠标移动量
            look(_move.x() * m_lookSpeed, _move.y() * m_lookSpeed, 0); // 观察角度增量
        }
        else if (ev->type() == QEvent::Leave) // 鼠标移出
        {
            _widget->setMouseTracking(false);
            _widget->setCursor(Qt::ArrowCursor);
        }
    }
    return false;
}
