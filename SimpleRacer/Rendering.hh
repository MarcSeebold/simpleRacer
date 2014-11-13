#pragma once
#include <memory>
#include <QObject>

namespace simpleRacer
{
struct GameState;

class Rendering : public QObject
{
   Q_OBJECT

public:
   Rendering();
   ~Rendering();

public slots:
   /// Render the given gamestate
   void render(const GameState *_gameState);

};

} // namespace simpleRacer
