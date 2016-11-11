#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

class Helper;

class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(Helper *helper, QWidget *parent);

public slots:
    void animate();

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    Helper *helper;
    int elapsed;
};

#endif
