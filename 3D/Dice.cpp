#include "Dice.h"

Dice::Dice()
{
	setVertices({
		// 顶点				纹理
		// 1
		{{-1, 1, 1}, {0.50, 0.25}},	  // 左上
		{{-1, -1, 1}, {0.50, 0.50}},  // 左下
		{{1, -1, 1}, {0.75, 0.50}},	  // 右下
		{{1, 1, 1}, {0.75, 0.25}},	  // 右上
									  // 6
		{{1, 1, -1}, {0.00, 0.25}},	  // 左上
		{{1, -1, -1}, {0.00, 0.50}},  // 左下
		{{-1, -1, -1}, {0.25, 0.50}}, // 右下
		{{-1, 1, -1}, {0.25, 0.25}},  // 右上
									  // 2
		{{1, 1, 1}, {0.75, 0.25}},	  // 左上
		{{1, -1, 1}, {0.75, 0.50}},	  // 左下
		{{1, -1, -1}, {1.00, 0.50}},  // 右下
		{{1, 1, -1}, {1.00, 0.25}},	  // 右上
									  // 5
		{{-1, 1, -1}, {0.25, 0.25}},  // 左上
		{{-1, -1, -1}, {0.25, 0.50}}, // 左下
		{{-1, -1, 1}, {0.50, 0.50}},  // 右下
		{{-1, 1, 1}, {0.50, 0.25}},	  // 右上
									  // 3
		{{-1, 1, -1}, {0.00, 0.00}},  // 左上
		{{-1, 1, 1}, {0.00, 0.25}},	  // 左下
		{{1, 1, 1}, {0.25, 0.25}},	  // 右下
		{{1, 1, -1}, {0.25, 0.00}},	  // 右上
									  // 4
		{{1, -1, 1}, {0.00, 0.50}},	  // 左上
		{{1, -1, -1}, {0.00, 0.75}},  // 左下
		{{-1, -1, -1}, {0.25, 0.75}}, // 右下
		{{-1, -1, 1}, {0.25, 0.50}},  // 右上
	});

	setTexture(new QOpenGLTexture(QImage(":/shaders/dice1000.png"))); // 加载纹理

	auto _program = new QOpenGLShaderProgram();
	_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/shapes.vert");	 // 顶点着色器
	_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/shapes.frag"); // 片段着色器
	setShaderProgram(_program);
}

Dice::~Dice()
{
}

void Dice::init()
{
	initializeOpenGLFunctions();
	if (!m_vao.isCreated())
		m_vao.create(); // 创建VAO
	if (!m_vbo.isCreated())
		m_vbo.create(); // 创建VBO
	if (!m_program->isLinked())
		m_program->link(); // 链接着色器

	// 创建顶点和纹理坐标
	if (m_vertexCount < m_vertices.count())
	{
		if (m_vertexBuffer)
			delete[] m_vertexBuffer;
		m_vertexBuffer = new float[m_vertices.count() * VertexFloatCount];
		m_vertexCount = m_vertices.count();
		int _offset = 0;
		for (auto &vertex : m_vertices)
		{
			m_vertexBuffer[_offset] = vertex.pos.x();
			_offset++;
			m_vertexBuffer[_offset] = vertex.pos.y();
			_offset++;
			m_vertexBuffer[_offset] = vertex.pos.z();
			_offset++;
			m_vertexBuffer[_offset] = vertex.texture.x();
			_offset++;
			m_vertexBuffer[_offset] = vertex.texture.y();
			_offset++;
		}
	}

	m_vao.bind();
	m_vbo.bind();
	m_vbo.allocate(m_vertexBuffer, sizeof(float) * m_vertices.count() * VertexFloatCount);

	m_program->bind();
	// 绑定顶点坐标信息, 从0 * sizeof(float)字节开始读取3个float, 因为一个顶点有5个float数据, 所以下一个数据需要偏移5 * sizeof(float)个字节
	m_program->setAttributeBuffer("vPos", GL_FLOAT, 0 * sizeof(float), 3, 5 * sizeof(float));
	m_program->enableAttributeArray("vPos");
	// 绑定纹理坐标信息, 从3 * sizeof(float)字节开始读取2个float, 因为一个顶点有5个float数据, 所以下一个数据需要偏移5 * sizeof(float)个字节
	m_program->setAttributeBuffer("vTexture", GL_FLOAT, 3 * sizeof(float), 2, 5 * sizeof(float));
	m_program->enableAttributeArray("vTexture");
	m_program->release();

	m_vbo.release();
	m_vao.release();
}

void Dice::update()
{
}

void Dice::paint(const QMatrix4x4 &projection, const QMatrix4x4 &view)
{
	for (auto index : m_textures.keys())
	{
		m_textures[index]->bind(index);
	}
	m_vao.bind();
	m_program->bind();
	// 绑定变换矩阵
	m_program->setUniformValue("projection", projection);
	m_program->setUniformValue("view", view);
	m_program->setUniformValue("model", model());
	// 绘制
	for (int i = 0; i < 6; ++i)
	{
		glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
	}
	m_program->release();
	m_vao.release();
	for (auto texture : m_textures)
	{
		texture->release();
	}
}
