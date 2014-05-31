//============================================================================
// Name        : TimerTest.cpp
// Author      : dieter.niklaus@gmx.net
// Version     :
// Copyright   : GPL
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Timer.h"
#include "UptimeInfo.h"

#include <iostream>
using namespace std;

#define DELAY(delayTime) \
  Timer delayTimer(0, Timer::IS_NON_RECURRING, (delayTime)); while (!delayTimer.isTimerExpired())

int main()
{
	cout << "Hello from Timer Test." << endl; // prints Hello from Timer Test.

	class TestTimerAdapter : public TimerAdapter
	{
	private:
	  unsigned int m_startTimeMillis;
	  unsigned int m_intervalMillis;
	  unsigned int m_nextExpectedTimeMillis;
	  const char* m_name;

	public:
	  TestTimerAdapter(const char* name, unsigned int intervalMillis)
	  : m_startTimeMillis(UptimeInfo::tMillis())
	  , m_intervalMillis(intervalMillis)
	  , m_nextExpectedTimeMillis(m_startTimeMillis + intervalMillis)
	  , m_name(name)
	  {	}

	  void timeExpired()
	  {
	    unsigned int millis = UptimeInfo::tMillis();
	    cout << "TestTimerAdapter::timeExpired(), name = " << m_name << "; millis = " << millis << " [ms]; expected = " << m_nextExpectedTimeMillis << " [ms]" << ((millis == m_nextExpectedTimeMillis) ? "; OK" : "; ERROR!!") << endl;
	    m_nextExpectedTimeMillis += m_intervalMillis;
	  }
	};

  new Timer(new TestTimerAdapter("Timer_1s", 1000), Timer::IS_RECURRING, 1000);
  unsigned int timeMillis1 = UptimeInfo::tMillis();
  DELAY(333);
  unsigned int delayTimeMillis = UptimeInfo::tMillis() - timeMillis1;
  if (333 == delayTimeMillis)
  {
    cout << "Timer_100ms start delayed by 333ms after Timer_1s start, OK" << endl;
  }
  new Timer(new TestTimerAdapter("Timer_100ms", 100), Timer::IS_RECURRING, 100);

	while (1)
	{
	  scheduleTimers();
	}

	return 0;
}
