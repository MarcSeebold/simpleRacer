#pragma once

#include <QWidget>
#include "Common.hh"

namespace simpleRacer
{
SHARED(class, GameLogic);
struct GameState;

class RenderingWidget : public QWidget
{
   Q_OBJECT
public:
   explicit RenderingWidget(QWidget *parent = 0);
   void setGameLogicComponent(SharedGameLogic _logic);

   /// Specify how old the rendered data should be
   void setPaintDelay(const int64_t &_value);

protected:
   void paintEvent(QPaintEvent *event) override;

private:
   SharedGameLogic mGameLogic;
   int64_t mPaintDelay = 0; //< Specifies how old the rendered data should be
};
}
