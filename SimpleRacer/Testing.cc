#if 0
#include "Testing.hh"
#include "GameLogic.hh"
#include <queue>
#include <Common.hh>
#include <random>
#include <time.h>
#include <cassert>
#include <functional>
#include <QMessageBox>

#define TESTING_RUN_TEST(func)        \
   ok &= func();                      \
   if (!ok)                           \
   {                                  \
      SR_ASSERT(0 && "Test failed."); \
   }                                  \
   while (0)

using namespace simpleRacer;

Testing::Testing()
{
}

void Testing::runTests()
{
   srand(time(nullptr));
   bool ok = true;
   for (int i = 0; i < 100; ++i)
   {
      TESTING_RUN_TEST(gameLogicGetOldCarDataClosest);
   }

   QMessageBox msgBox;
   msgBox.setText(QString("Test result: ") + (ok ? "Success" : "Failed"));
   msgBox.exec();
}

bool Testing::gameLogicGetOldCarDataClosest()
{
   GameLogic logic;
   // create some random, sorted values
   std::priority_queue<int, std::vector<int>, std::greater<int>> values;
   for (int i = 0; i < 99; ++i)
      values.push(rand() % 200);
   // insert specific value we want to search for later
   int needle = rand() % 200;
   values.push(needle);
   // put these values in mOldCarData vector
   for (int i = 0; i < 100; ++i)
   {
      logic.mOldCarData.push_back(
          GameLogic::OldData{values.top(), {QVector2D(0, 0), QVector2D(0, 0)}, {QVector2D(0, 0), QVector2D(0, 0)}});
      values.pop();
   }
   // >>> perform the actual test
   _ data = logic.getOldDataClosest(needle);
   return data.timestamp == needle;
}
#endif
