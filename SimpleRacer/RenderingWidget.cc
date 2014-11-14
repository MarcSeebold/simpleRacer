#include "RenderingWidget.hh"
#include <QPainter>
#include <iostream>
#include <QPaintEvent>
#include "GameLogic.hh"

using namespace simpleRacer;

RenderingWidget::RenderingWidget(QWidget *parent) : QWidget(parent)
{
   setFixedSize(800, 300);
}

void RenderingWidget::setGameLogicComponent(SharedGameLogic _logic)
{
   mGameLogic = _logic;
}

void RenderingWidget::paintEvent(QPaintEvent *event)
{
   static float animate = 0;

   if (!mGameLogic)
      return;
   _ state = mGameLogic->getGameState();
   if (!state)
      return;


   _ scaleX = size().width() / 100;
   _ scaleY = size().height() / 100;

   QPainter painter;
   painter.begin(this);
   painter.setRenderHint(QPainter::Antialiasing);
   // background
   // street is gray
   painter.fillRect(event->rect(), QBrush(Qt::gray));
   // street lines
   for (int x = 0; x < 10; ++x)
   {
      for (int y = 1; y < 4; ++y)
      {
         QRectF line(100 * x - (animate), 75 * y, 50, 5);
         painter.fillRect(line, Qt::white);
      }
   }
   animate += 0.6f;
   if (animate > 100)
      animate = 0;

   // draw cars
   _ carLength = GameLogic::sCarLength * scaleX;
   _ carWidth = GameLogic::sCarWidth * scaleY;
   QSize carTarget(carLength, carWidth);

   static QImage car(":/assets/car.png");
   static QImage carScaled;
   static QSize currSize = size();
   if (size() != currSize || !carScaled.valid(1, 1))
   {
      carScaled = car.scaled(carTarget, Qt::KeepAspectRatio, Qt::SmoothTransformation);
   }

   for (int p : {0, 1})
   {
      _ x = state->positionX[p] * scaleX;
      _ y = state->positionY[p] * scaleY;

      // convert center pos to top left
      x -= carLength / 2;
      y -= carWidth / 2;

      painter.drawImage(x, y, carScaled);
      // painter.drawRect(car);
   }

   painter.end();
}
