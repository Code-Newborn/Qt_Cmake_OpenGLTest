#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QMatrix4x4>
#include <QVector3D>
class Camera : public QObject
{
    Q_OBJECT
public:
    Camera(QObject *parent = nullptr);
    ~Camera();

public:
    /// @brief      鼠标控制视角
    /// @msg        增量
    /// @param      {float} yaw 偏航角
    /// @param      {float} pitch 俯仰角
    /// @param      {float} roll 滚转角
    /// @return     {*}
    void look(float yaw, float pitch, float roll);

    /// @brief      键盘控制模型移动
    /// @msg        增量
    /// @param      {float} x 增量X
    /// @param      {float} y 增量Y
    /// @param      {float} z 增量Z
    /// @return     {*}
    void move(float x, float y, float z);

public:
    void update();
    QMatrix4x4 view() { return m_view; }

protected:
    /// @brief 事件过滤器
    /// @param obj 监视对象（需要调用installEventFilter(&this)才能起效）
    /// @param ev 发生的事件
    /// @return 是否捕获到事件
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    QMatrix4x4 m_view;
    QVector3D m_pos{0, 0, 0};    // 位置向量
    QVector3D m_front{0, 0, -1}; // 相机前方向量（世界坐标系下）
    QVector3D m_up{0, 1, 0};     // 相机上方向量（世界坐标系下）
    float m_yaw = 0;             // 偏航角
    float m_pitch = 0;           // 俯仰角
    float m_roll = 0;            // 滚转角
    QVector3D m_move;            // 移动增量
    float m_lookSpeed = 0.1;     // 视角速度
    float m_moveSpeed = 0.1;     // 移动速度
};

#endif // CAMERA_H