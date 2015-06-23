#pragma once

#include <QOpenGLWidget>
#include "Common.hh"

SHARED(class, GameLogic);

class RenderingWidget : public QOpenGLWidget
{
   Q_OBJECT
public:
   explicit RenderingWidget(QWidget *parent = 0);

protected:
   void paintEvent(QPaintEvent *event) override;

private:
   float mOpacity = 1; ///< 0 = black screen

public:
   PROPERTY(Opacity);
};
