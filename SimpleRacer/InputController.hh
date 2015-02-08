#pragma once
#include <vector>
#include "Common.hh"

namespace simpleRacer
{
/// Processes user input
/// Mainly used for delaying user input as effect of lag simulation
class InputController
{
public:
    enum class KeyType : char
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

public:
    /// c'tor
    InputController();

    /// inform us about a keypress
    void sendKeyPress(KeyType _type);
    /// process keypresses
    void update();
    /// Set user-input delay
    void setDelay(int64_t _delay) { mDelay = _delay; }

private:
    struct KeyEvent
    {
        int64_t timestamp;
        KeyType type;
    };
    SHARED(struct, KeyEvent);

private:
    std::vector< UniqueKeyEvent > mKeyEvents; ///< Unprocessed key events
    int64_t mDelay = 0; ///< How much delay should we add to user input?
};
} // namespace
