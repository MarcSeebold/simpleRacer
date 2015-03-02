#pragma once

#include <QWidget>
#include "Common.hh"

SHARED(class, GameLogic);
struct GameState;

class RenderingWidget : public QWidget
{
   Q_OBJECT
public:
   explicit RenderingWidget(QWidget *parent = 0);

protected:
   void paintEvent(QPaintEvent *event) override;
};
