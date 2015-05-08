#include "Common.hh"
#include <cassert>
#include <iostream>
#include <QDateTime>
#include <QTimer>
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


void DelayedActions::pushDelayedAction(std::function<void ()> _function, DelayedActionType _type)
{
   _ delayed = new QTimer();
   mDelayedActions.push_back(delayed);
   delayed->setSingleShot(true);
   connect(delayed, &QTimer::timeout, [this, _function]()
           {
              _function();
           });
   float delay = (_type == DelayedActionType::CLIENT_TO_SERVER) ? LagSettings::the()->getLatencyClientToServer() : LagSettings::the()->getLatencyServerToClient();
   delayed->start(delay * 1000); // secondsToMilliseconds
}


DelayedActions::DelayedActions()
{

}

DelayedActions::~DelayedActions()
{
   for (_& a : mDelayedActions)
      delete a;
   mDelayedActions.clear();
}

void DelayedActions::update()
{
   // clean up delayed actions
   for (_ it = mDelayedActions.begin(); it != mDelayedActions.end();)
   {
      if (!(*it)->isActive())
      {
         _ obj = *it;
         it = mDelayedActions.erase(it);
         delete obj;
      }
      else
      {
         ++it;
      }
   }
}
