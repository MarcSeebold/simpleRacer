#include "Common.hh"
#include <cassert>
#include <iostream>
#include <QDateTime>

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


void common::srassert(bool _statement, const char *_text, const char *_function, const char *_file, int _line)
{
   if (_statement)
      return;
   std::cerr << "[" << _file << ":" << _line << ":" << _function << "] Assertion failed: " << _text << std::endl;
   assert(0);
}
