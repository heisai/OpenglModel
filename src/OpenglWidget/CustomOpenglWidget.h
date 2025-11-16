#ifndef CUSTOMOPENGLWIDGET_H
#define CUSTOMOPENGLWIDGET_H



#include<QOpenGLWidget>
#include"src/Utils/OpengHearder.h"
#include"src/Graphics/ManageEngine.h"
#include<QBasicTimer>
#include<QMouseEvent>
#include<QVector3D>
#include<string.h>
#include<iostream>
struct AABB {
	glm::vec3 min; // Minimum vertex of AABB
	glm::vec3 max; // Maximum vertex of AABB
};
#include<QTimer>
#include<QHBoxLayout>
#include<QPushButton>
#include<QDockWidget>
#include <QOpenGLFunctions_4_5_Core>
#include<QOpenGLContext>
#include"src/OpenglWidget/HoverToolWidget.h"
class CustomOpenglWidget : public QOpenGLWidget, QOpenGLFunctions_4_5_Core
{
    Q_OBJECT
public:
    CustomOpenglWidget(QWidget *parent = nullptr);
    void BindManageEngine(const std::shared_ptr<ManageEngine> &engine);
    void initializeGL() override;
protected:
    void resizeGL(int w, int h) override;
    void paintGL() override;
    void timerEvent(QTimerEvent *)override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseReleaseEvent(QMouseEvent *e) override;
    void mouseMoveEvent(QMouseEvent *e) override;
	void wheelEvent(QWheelEvent* event)override;


	
signals:
    void CheckBoxType(QString type,bool check);


private:
    QVector2D m_MousePressPosition;
    QVector2D m_MouseMovePositionABS;
    QVector2D m_TranlstorPosition;
    QVector2D m_TranlstorPositionOld;
    QVector2D m_MouseReleasePosintion;
    QBasicTimer timer;
    float angularSpeed = 0.0;
    QVector3D m_Normalied = QVector3D(1.0,1.0,0.0);
    QVector3D m_NormaliedOld = QVector3D(1.0,1.0,0.0);
    qreal scaleFactor = 1.0;
    float m_TranslateSpeed = 50.0;
    float m_AngularSpeed = 100.0;
    QVector2D m_MaxRange = QVector2D(2.0,2.0);

    glm::vec3 rotationAxis = glm::vec3(0.0,0.0,0.0);
    glm::mat4 rotationAxisold = glm::mat4(1.0f);
    glm::mat4 rotationAxistemp = glm::mat4(1.0f);


	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);


    float diff1 = 0.0f;
    float diff2 = 0.0f;


  
    float m_RangeAngleX =  glm::radians(90.0);

    float m_Angley = 0.0f;
    float m_Anglex = 0.0f;
    float m_TotalAngleX = 0.0f;
	AABB cubeBoundingBox = {
		glm::vec3(-0.5f, -0.5f, -0.5f), // 最小顶点
		glm::vec3(0.5f, 0.5f, 0.5f)     // 最大顶点
	};


	std::vector<float> m_Vertice = {
		 -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	QString  m_sUUid = "";
	std::shared_ptr<ManageEngine>m_PtrManageEngine;
	int m_width, m_height;
	//QWidget* widget_;

	HoverToolWidget* widget_;
};

#endif // CUSTOMOPENGLWIDGET_H
