#include "RenderingWidget.hh"
#include <QPainter>
#include <iostream>
#include <QPaintEvent>
#include "GameLogic.hh"
#include <cassert>

#include <qdebug.h>

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

   if (!mGameLogic || !mGameLogic->getRunning())
      return;

   _ convFac = GameLogic::sConversionFactor;

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
   _ carWidth = GameLogic::sCarWidth / convFac;
   _ carHeight = GameLogic::sCarHeight / convFac;
   QSize carTarget(carWidth, carHeight);

   static QImage car(":/assets/car.png");
   static QImage carScaled;
   static QSize currSize = size();
   if (size() != currSize || !carScaled.valid(1, 1))
   {
      carScaled = car.scaled(carTarget, Qt::KeepAspectRatio, Qt::SmoothTransformation);
   }

   for (int p : {0, 1})
   {
      _ carPos = mGameLogic->getCarCenterPosition((PlayerID)p);
      _ x = carPos.x() / convFac;
      _ y = (GameLogic::sGameHeight - carPos.y()) / convFac;

      // convert center pos to top left
      x -= carWidth / 2;
      y -= carHeight / 2;


      painter.drawImage(x, y, carScaled);
   }

   // draw coins
   static QImage coinImg(":/assets/coin.png");
   static QImage coinScaled;
   coinScaled = coinImg.scaled(QSize(GameLogic::sCoinSize / convFac, GameLogic::sCoinSize / convFac),
                               Qt::KeepAspectRatio, Qt::SmoothTransformation);

   _ coins = mGameLogic->getCoins();
   for (_ const &coin : coins)
   {
      _ x = coin.x() / convFac;
      _ y = (GameLogic::sGameHeight - coin.y()) / convFac;
      // convert center pos to top left
      x -= GameLogic::sCoinSize/ convFac / 2;
      y -= GameLogic::sCoinSize/ convFac / 2;
      painter.drawImage(x, y, coinScaled);
   }

   painter.end();
}
