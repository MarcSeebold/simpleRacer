#include "Common.hh"
#include <cassert>
#include <iostream>
#include <QDateTime>
#include "Settings.hh"
#include <QJsonObject>
#include <QJsonArray>
#include <QString>
#include <QStringList>

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
   float delay = (_type == DelayedActionType::CLIENT_TO_SERVER) ? Settings::the()->getCurrentLatencyClientToServer()
                                                                : Settings::the()->getCurrentLatencyServerToClient();
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

/// Splits a CSV-Row and removed quote-chars
QStringList csvRowSplit(const QString &_csvRow)
{
   QStringList res;
   bool insideEntry = false;
   QString currItem;
   for (int i = 0; i < _csvRow.size(); ++i)
   {
      const _ &c = _csvRow[i];
      if (c == ',' && !insideEntry)
      { // next item. store old and begin with new.
         res.push_back(currItem);
         currItem = "";
         continue;
      }
      if (c == '\"')
      { // quote-chars start and end entries
         insideEntry = !insideEntry;
         continue;
      }
      else
      {
         if (insideEntry)
            currItem += c;
      }
   }
   // push last item
   res.push_back(currItem);
   return res;
}

void common::csvToJSON(QJsonObject &_json, const QString &_csv, const QString _jsonKey)
{
   if (_csv.size() == 0)
      return;
   QJsonArray jArray;
   // CSV: first line are row names, following lines are data
   const _ lines = _csv.split('\n');
   SR_ASSERT(lines.size() != 0 && "No input");
   // first line: row names, seperated by comma
   const _ rowNames = csvRowSplit(lines.at(0));
   SR_ASSERT(rowNames.size() != 0 && "No row names");
   // Parse each line
   bool first = true;
   for (const _ &curr : lines)
   {
      if (curr == "")
         continue;  // skip empty lines
      if (first)
      { // skip first entry (i.e., row names)
         first = false;
         continue;
      }
      QJsonObject jEntry;
      const _ rows = csvRowSplit(curr);
      SR_ASSERT(rows.size() == rowNames.size() && "row count missmatch");
      int idx = 0;
      for (const _ &rowName : rowNames)
      { // parse rows

         jEntry[rowName] = rows.at(idx);
         idx++;
      }
      jArray.push_back(jEntry);
   }
   _json.insert(_jsonKey, jArray);
}
