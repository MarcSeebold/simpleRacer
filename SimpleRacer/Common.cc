#include "Common.hh"
#include <cassert>
#include <iostream>
#include <QDateTime>
#include "LagSettings.hh"

void _sr_assert_fail(const char *_expr, const char *_file, int _line, const char *_function)
{
   std::cerr << "Assertion `" << _expr << "' failed." << std::endl;
   std::cerr << "File " << _file << " @line " << _line << " @function " << _function << std::endl;

   assert(false);
}


int64_t common::getCurrentTimestamp()
{
   return QDateTime::currentMSecsSinceEpoch();
}


void DelayedActions::pushDelayedAction(std::function<void()> _function, DelayedActionType _type)
{
   float delay = (_type == DelayedActionType::CLIENT_TO_SERVER) ? Settings::the()->getLatencyClientToServer()
                                                                : Settings::the()->getLatencyServerToClient();
   if (delay == 0)
      _function();
   else
   {
      _ a = new DelayedAction{common::getCurrentTimestamp() + int64_t(delay * 1000.f) /*s to ms*/, _function};
      mDelayedActions.push_back(a);
   }
}

void DelayedActions::pushDelayedAction(std::function<void()> _function, float _delay)
{
   if (_delay == 0)
      _function();
   else
   {
      _ a = new DelayedAction{common::getCurrentTimestamp() + int64_t(_delay * 1000.f) /*s to ms*/, _function};
      mDelayedActions.push_back(a);
   }
}

void DelayedActions::clear()
{
   for (const _ &a : mDelayedActions)
      delete a;
   mDelayedActions.clear();
}


DelayedActions::DelayedActions()
{
}

DelayedActions::~DelayedActions()
{
   for (_ &a : mDelayedActions)
      delete a;
   mDelayedActions.clear();
}

void DelayedActions::update()
{
   // execute delayed actions
   _ currT = common::getCurrentTimestamp();
   for (_ it = mDelayedActions.begin(); it != mDelayedActions.end();)
   {
      DelayedAction *a = *it;
      if (a->timestamp <= currT)
      {
         // execute func and erase action
         a->function();
         delete a;
         a = nullptr;
         it = mDelayedActions.erase(it);
      }
      else
         ++it;
   }
}
