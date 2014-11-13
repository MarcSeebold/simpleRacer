#include "Common.hh"
#include <cassert>
#include <iostream>

void _sr_assert_fail(const char *_expr, const char *_file, int _line, const char *_function)
{
   std::cerr << "Assertion `" << _expr << "' failed." << std::endl;
   std::cerr << "File " << _file << " @line " << _line << " @function " << _function << std::endl;

   assert(false);
}