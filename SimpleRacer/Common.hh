#pragma once
#include <memory>
#include <vector>
#include <QObject>
#include <functional>
// some usefull stuff

namespace common
{
int64_t getCurrentTimestamp();
}

class QTimer;
class DelayedActions : public QObject
{
   Q_OBJECT
public:
   DelayedActions();
   ~DelayedActions();

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
   std::vector<QTimer*> mDelayedActions;
};

/// Unique player id
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
#define PROPERTY(name) \
   GETTER(name);       \
   SETTER(name)

#define _ auto

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

// assertion define
void _sr_assert_fail(const char* _expr, const char* _file, int _line, const char* _function);
#define _SR_STRING(x) #x
#ifdef WIN32
#define __PRETTY_FUNCTION__ __FUNCTION__
#endif

#define SR_ASSERT(expr) \
   ((expr) ? static_cast<void>(0) : (_sr_assert_fail(_SR_STRING(expr), __FILE__, __LINE__, __PRETTY_FUNCTION__)))
