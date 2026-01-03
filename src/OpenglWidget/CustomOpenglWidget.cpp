 // #define GLFW_INCLUDE_NONE

#include "CustomOpenglWidget.h"

CustomOpenglWidget::CustomOpenglWidget(QWidget *parent):
    QOpenGLWidget(parent)
{
	widget_ = new HoverToolWidget(this);
	widget_->show();
}


void CustomOpenglWidget::BindManageEngine(const std::shared_ptr<ManageEngine>& engine)
{
    manage_engine_moudle_ = engine;
   
}

void CustomOpenglWidget::initializeGL()
{

    initializeOpenGLFunctions();
    const GLubyte* version = glGetString(GL_VERSION);
    qDebug() << "OpenGL Version:" << reinterpret_cast<const char*>(version);
	

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_MULTISAMPLE);
	glHint(GL_LINE_SMOOTH, GL_NICEST);




    if (manage_engine_moudle_)
    {
        manage_engine_moudle_->setViewSize(this->width(), this->height());
        manage_engine_moudle_->initializeGl();
       
    }
    timer.start(12, this);
    glViewport(0, 0, width(), height());

}

void CustomOpenglWidget::resizeGL(int w, int h)
{
	m_width = w;
	m_height = h;
	makeCurrent();
	glViewport(0, 0, m_width, m_height);
	if (manage_engine_moudle_)
	{
		manage_engine_moudle_->setViewSize(w, h);
		manage_engine_moudle_->initializeGl();

	}
	doneCurrent();
    update();
    LogInfo("resizeGL: width: {} height:{}", w, h);
}

void CustomOpenglWidget::paintGL()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    if (manage_engine_moudle_)
    {
        manage_engine_moudle_->setEngineScaleAndTranslate(m_sUUid, glm::vec3(scaleFactor, scaleFactor, scaleFactor),
           glm::vec3(m_TranlstorPosition.x(), -m_TranlstorPosition.y(), -18.0f), rotationAxisold);
        manage_engine_moudle_->paintGl();
    }
}

void CustomOpenglWidget::timerEvent(QTimerEvent *)
{
    update();
}

void CustomOpenglWidget::mousePressEvent(QMouseEvent *e)
{
    m_MousePressPosition = QVector2D(e->localPos());
    if (e->type() == QEvent::MouseButtonPress && e->button() == Qt::LeftButton)
    {
        update();
        int readX = m_MousePressPosition.x();
        int readY = this->height() - m_MousePressPosition.y() ;       
        makeCurrent();
        MvpDataPtr  mvp_data =  manage_engine_moudle_->pickModel(readX, readY);
        doneCurrent();
        if (mvp_data)
        {
            m_TranlstorPosition = m_TranlstorPositionOld = mvp_data->tranlstor_position_;
            rotationAxistemp = rotationAxisold = mvp_data->model_;

        }
    }
}

void CustomOpenglWidget::mouseReleaseEvent(QMouseEvent *e)
{
    m_MouseReleasePosintion =   QVector2D(e->localPos()) ;

    rotationAxistemp = rotationAxisold;
    m_NormaliedOld = m_Normalied;
    angularSpeed = 0.0;
    m_TotalAngleX += m_Anglex;
    m_Angley = 0.0f;
    m_Anglex = 0.0f;
    this->setCursor(QCursor(Qt::ArrowCursor));
}

void CustomOpenglWidget::mouseMoveEvent(QMouseEvent *e)
{

    if(e->type() == QEvent::MouseMove && (e->buttons() & Qt::LeftButton ))
    {
        switch (widget_->getModelState())
        {
        case HoverToolWidget::MoveState:
			m_TranlstorPosition = (QVector2D(e->localPos()) - m_MousePressPosition) / m_TranslateSpeed + m_TranlstorPositionOld;
			this->setCursor(QCursor(Qt::SizeAllCursor));
            break;
        case HoverToolWidget::RotateState:
        {
        
			float diffx = /*glm::length*/(e->localPos().x() - m_MousePressPosition.x());
			float diffy =/* glm::length*/(e->localPos().y() - m_MousePressPosition.y());
			if (abs(diffx) - m_MouseMovePositionABS.x() <= -1)
			{
				m_MousePressPosition.setX(e->localPos().x());
				mouseReleaseEvent(e);
			}
			else
			{
				m_Anglex += glm::radians(diffy / m_AngularSpeed);
			}
			if (abs(diffy) - m_MouseMovePositionABS.y() <= -1)
			{
				m_MousePressPosition.setY(e->localPos().y());
				mouseReleaseEvent(e);
			}
			else
			{
				m_Angley += glm::radians(diffx / m_AngularSpeed);
			}
			qDebug() << abs(diffx) - m_MouseMovePositionABS.x() << abs(diffy) - m_MouseMovePositionABS.y();
			m_MouseMovePositionABS = QVector2D(abs(diffx), abs(diffy));
			rotationAxisold = glm::mat4(1.0f);
			rotationAxisold = glm::rotate(rotationAxisold, m_Angley, glm::vec3(0.0f, 1.0f, 0.0f));
			rotationAxisold = glm::rotate(rotationAxisold, m_Anglex, glm::vec3(1.0f, 0.0f, 0.0f));
			rotationAxisold *= rotationAxistemp;
        
        }
			break;
		case HoverToolWidget::ScaleState:
			break;
		case HoverToolWidget::SelectState:
			break;
        default:
            break;
        }

    }
}

void CustomOpenglWidget::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        // 计算缩放因子
        qreal delta = event->angleDelta().y() / 120.0;
        scaleFactor += delta * 0.7; // 缩放因子的调整量
        LogInfo("wheelEvent:{}", scaleFactor);
    }
    else
    {
        event->ignore(); // 忽略滚轮事件，将其传递给默认处理程序
    }


}