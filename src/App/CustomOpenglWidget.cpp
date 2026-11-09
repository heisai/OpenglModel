// #define GLFW_INCLUDE_NONE

#include "CustomOpenglWidget.h"
#include <cmath>

CustomOpenglWidget::CustomOpenglWidget(QWidget *parent) : QOpenGLWidget(parent)
{
    widget_ = new HoverToolWidget(this);
    widget_->show();
}

void CustomOpenglWidget::BindManageEngine(const std::shared_ptr<ManageEngine> &engine)
{
    manage_engine_moudle_ = engine;
}

void CustomOpenglWidget::initializeGL()
{

    initializeOpenGLFunctions();
    const GLubyte *version = glGetString(GL_VERSION);
    qDebug() << "OpenGL Version:" << reinterpret_cast<const char *>(version);

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
        int readY = this->height() - 1 - m_MousePressPosition.y();
        if (!manage_engine_moudle_)
        {
            return;
        }
        makeCurrent();
        MvpDataPtr mvp_data = manage_engine_moudle_->pickModel(readX, readY);
        doneCurrent();
        drag_data_ = mvp_data;
        if (drag_data_)
        {
            drag_position_ = drag_data_->position_;
            drag_rotation_ = drag_data_->rotation_;
            drag_scale_ = drag_data_->scale_;
        }
    }
}

void CustomOpenglWidget::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    this->setCursor(QCursor(Qt::ArrowCursor));
}

void CustomOpenglWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (drag_data_ && manage_engine_moudle_ && (e->buttons() & Qt::LeftButton))
    {
        const QVector2D delta = QVector2D(e->localPos()) - m_MousePressPosition;
        auto position = drag_position_;
        auto rotation = drag_rotation_;
        if (widget_->getModelState() == HoverToolWidget::MoveState)
        {
            if (width() <= 0 || height() <= 0)
            {
                return;
            }
            const glm::mat4 camera = drag_data_->projection_ * drag_data_->view_;
            const glm::vec4 clip = camera * glm::vec4(drag_position_, 1.0f);
            if (std::abs(clip.w) < 1e-6f)
            {
                qWarning() << "Cannot drag at camera plane";
                return;
            }
            const glm::mat4 inverseCamera = glm::inverse(camera);
            // Unproject both cursor positions at the object's original depth, retaining the grab offset.
            auto unproject = [&](const QVector2D &cursor)
            {
                const glm::vec4 world =
                    inverseCamera * glm::vec4(2.0f * cursor.x() / width() - 1.0f, 1.0f - 2.0f * cursor.y() / height(),
                                              clip.z / clip.w, 1.0f);
                return glm::vec3(world) / world.w;
            };
            position += unproject(QVector2D(e->localPos())) - unproject(m_MousePressPosition);
            position.z = drag_position_.z;
        }
        else if (widget_->getModelState() == HoverToolWidget::RotateState)
        {
            rotation = glm::rotate(glm::mat4(1.0f), delta.x() / m_AngularSpeed, glm::vec3(0, 1, 0)) *
                       glm::rotate(glm::mat4(1.0f), delta.y() / m_AngularSpeed, glm::vec3(1, 0, 0)) * drag_rotation_;
        }
        else
        {
            return;
        }
        manage_engine_moudle_->updateSelectedTransform(position, rotation, drag_scale_);
        update();
    }
}

void CustomOpenglWidget::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
        // 计算缩放因子
        qreal delta = event->angleDelta().y() / 120.0;
        if (drag_data_ && manage_engine_moudle_)
        {
            const float factor = std::pow(1.1f, static_cast<float>(delta));
            const glm::vec3 newScale = glm::max(drag_data_->scale_ * factor, glm::vec3(0.01f));
            manage_engine_moudle_->updateSelectedTransform(drag_data_->position_, drag_data_->rotation_, newScale);
            update();
        }
    }
    else
    {
        event->ignore(); // 忽略滚轮事件，将其传递给默认处理程序
    }
}
