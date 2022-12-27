#include <QMatrix4x4>

#include <QOpenGLExtraFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
struct Vertex
{
    QVector3D pos;
    QVector2D texture;
};

class Model : public QOpenGLExtraFunctions
{
public:
    Model();
    ~Model();

public:
    void setScale(float val) { m_scale = val; }
    void setRotate(const QVector3D &rotate) { m_rotate = rotate; }
    void setPos(const QVector3D &pos) { m_pos = pos; }
    QVector3D rotate() { return m_rotate; }

public:
    void setVertices(const QVector<Vertex> &vertices) { m_vertices = vertices; }
    void setTexture(QOpenGLTexture *texture, int index = -1);
    void setShaderProgram(QOpenGLShaderProgram *program) { m_program = program; }

public:
    QMatrix4x4 model();

public:
    virtual void init();
    virtual void update();
    virtual void paint(const QMatrix4x4 &projection, const QMatrix4x4 &view);

protected:
    QVector3D m_pos{0, 0, 0};
    QVector3D m_rotate{0, 0, 0};
    float m_scale = 1;
    QVector<Vertex> m_vertices;
    QMap<int, QOpenGLTexture *> m_textures;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program = nullptr;
};
