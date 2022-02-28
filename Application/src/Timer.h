#pragma once

// Time conversion macros
#define PFO_NanosecToMicrosec(nanoseconds)		(nanoseconds / 1000.0)
#define PFO_NanosecToMillisec(nanoseconds)		(nanoseconds / 1000000.0)
#define PFO_NanosecToSeconds(nanoseconds)		(nanoseconds / 1000000000.0)
#define PFO_NanosecToMinutes(nanoseconds)		(nanoseconds / 60000000000.0)
#define PFO_NanosecToHours(nanoseconds)			(nanoseconds / 3600000000000.0)

#define PFO_MicrosecToNanosec(microseconds)		(microseconds * 1000.0)
#define PFO_MicrosecToMillisec(microseconds)	(microseconds / 1000.0)
#define PFO_MicrosecToSeconds(microseconds)		(microseconds / 1000000.0)
#define PFO_MicrosecToMinutes(microseconds)		(microseconds / 60000000.0)
#define PFO_MicrosecToHours(microseconds)		(microseconds / 3600000000.0)

#define PFO_MillisecToNanosec(milliseconds)		(milliseconds * 1000000.0)
#define PFO_MillisecToMicrosec(milliseconds)	(milliseconds * 1000.0)
#define PFO_MillisecToSeconds(milliseconds)		(milliseconds / 1000.0)
#define PFO_MillisecToMinutes(milliseconds)		(milliseconds / 60000.0)
#define PFO_MillisecToHours(milliseconds)		(milliseconds / 3600000.0)

#define PFO_SecondsToNanosec(seconds)			(seconds * 1000000000.0)
#define PFO_SecondsToMicrosec(seconds)			(seconds * 1000000.0)
#define PFO_SecondsToMillisec(seconds)			(seconds * 1000.0)
#define PFO_SecondsToMinutes(seconds)			(seconds / 60.0)
#define PFO_SecondsToHours(seconds)				(seconds / 3600.0)

#define PFO_MinutesToNanosec(minutes)			(minutes * 60000000000.0)
#define PFO_MinutesToMicrosec(minutes)			(minutes * 60000000.0)
#define PFO_MinutesToMillisec(minutes)			(minutes * 60000.0)
#define PFO_MinutesToSeconds(minutes)			(minutes * 60.0)
#define PFO_MinutesToHours(minutes)				(minutes / 60.0)

#define PFO_HoursToNanosec(hours)				(hours * 3600000000000.0)
#define PFO_HoursToMicrosec(hours)				(hours * 3600000000.0)
#define PFO_HoursToMillisec(hours)				(hours * 3600000.0)
#define PFO_HoursToSeconds(hours)				(hours * 3600.0)
#define PFO_HoursToMinutes(hours)				(hours * 60.0)


class Timer
{
public:
	int test = 0;
	using ull = unsigned long long;
	enum Time : char
	{
		//hours, microseconds, milliseconds, minutes, nanoseconds, seconds
		NANOSECONDS, MICROSECONDS, MILLISECONDS, SECONDS, MINUTES, HOURS
	};
private:
	ull m_TimeStamp = 0;
	ull m_StartTime = 0;
	ull m_StopTime = 0;
	ull m_ExecutionTime = 0;
	ull m_FrameTimeStart = 0;

	unsigned int m_FPS = 0;
	unsigned int m_OldFPS = 0;
private:
	ull getTime(const char& time = Time::MILLISECONDS) const;
public:
	Timer() = default;
	~Timer() = default;
	ull getCurrentTime(const char& time = Time::MILLISECONDS) const;

	void setTimeStamp(const char& time = Time::MILLISECONDS);
	ull getTimeStamp() const;

	void startFrameCounter();
	unsigned int measureFPS();
	void startFrameTimeCounter(const char& time = Time::MICROSECONDS);
	ull measureFrameTime(const char& time = Time::MICROSECONDS);

	void startFrameTime_FPSCounter();
	void measureFrameTime_FPS();

	void start(const char& time = Time::MILLISECONDS);
	void stop(const char& time = Time::MILLISECONDS);
	ull executionTime();
	ull stopExec(const char& time = Time::MILLISECONDS);

	void sleep(const ull& sleep_for, const char& time = Time::SECONDS) const;
	void wait(const ull& sleep_for, const char& time = Time::SECONDS) const;
};