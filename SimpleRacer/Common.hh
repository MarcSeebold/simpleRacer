#pragma once
#include <memory>
#include <vector>
#include <QObject>
#include <functional>

/**
 * Common used stuff
 */

class QJsonObject;
class QByteArray;

namespace common
{
/// Returns the current system time in nanoseconds
int64_t getCurrentTimestamp();
/// convert CSV data in _csv to JSON and store it in _json
void csvToJSON(QJsonObject& _json, const QString& _csv, const QString _jsonKey);
}

class QTimer;
/**
 * @brief The DelayedActions class allows to enqueue std::functions and executes them after a given time.
 *        Note that update() actually executed the functions and must be called as often as possible.
 *        If update() is called too late, enqueued functions may be executed too late!
 */
class DelayedActions : public QObject
{
   Q_OBJECT

private:
   struct DelayedAction
   {
      int64_t timestamp;
      std::function<void()> function;
   };

public:
   // c'tor, d'tor
   DelayedActions();
   ~DelayedActions();

   /// Call as often as possible. Actually executes enqueued functions whos execution time is passed.
   void update();

   enum class DelayedActionType : char
   {
      SERVER_TO_CLIENT,
      CLIENT_TO_SERVER
   };
   /// Simulates lag by executing the given function delayed
   void pushDelayedAction(std::function<void()> _function, DelayedActionType _type);

   /// Execute the given function after _delay seconds
   void pushDelayedAction(std::function<void()> _function, float _delay);

   /// Stops all delayed actions and delets them
   void clear();

private:
   std::vector<DelayedAction*> mDelayedActions;
};

/// Unique player id
/// P1 is human player, P2 is AI.
enum class PlayerID : int
{
   P1 = 0,
   P2 = 1
};

#define DO_STRING_JOIN2(arg1, arg2) arg1##arg2
#define STRING_JOIN2(arg1, arg2) DO_STRING_JOIN2(arg1, arg2)

// GETTER(name) SETTER(name) PROPERTY(name) as non-typed getter/setter macros
#define GETTER(name)                                                     \
   auto get##name() const->decltype(m##name) const & { return m##name; } \
   friend class STRING_JOIN2(___get_, __LINE__)
#define SETTER(name)                                                   \
   void set##name(decltype(m##name) const& value) { m##name = value; } \
   friend class STRING_JOIN2(___set_, __LINE__)
// A property has a setter and a getter
#define PROPERTY(name) \
   GETTER(name);       \
   SETTER(name)

// Use an underscore instead of auto
#define _ auto

// Defines a {shared,unique}_ptr type
// e.g., SHARED(class, myClass);
#define SHARED(type, class_or_struct_name)                                                \
   type class_or_struct_name;                                                             \
                                                                                          \
   using Shared##class_or_struct_name = std::shared_ptr<class_or_struct_name>;            \
   using ConstShared##class_or_struct_name = std::shared_ptr<const class_or_struct_name>; \
                                                                                          \
   using Weak##class_or_struct_name = std::weak_ptr<class_or_struct_name>;                \
   using ConstWeak##class_or_struct_name = std::weak_ptr<const class_or_struct_name>;     \
                                                                                          \
   using Unique##class_or_struct_name = std::unique_ptr<class_or_struct_name>;            \
   using ConstUnique##class_or_struct_name = std::unique_ptr<const class_or_struct_name>

#define _SR_STRING(x) #x
#ifdef WIN32
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

// assertion define
void _sr_assert_fail(const char* _expr, const char* _file, int _line, const char* _function);
#define SR_ASSERT(expr) \
   ((expr) ? static_cast<void>(0) : (_sr_assert_fail(_SR_STRING(expr), __FILE__, __LINE__, __PRETTY_FUNCTION__)))
