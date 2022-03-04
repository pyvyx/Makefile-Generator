#include <chrono>
#include <thread>
#include <iostream>

#include "Timer.h"



Timer::ull Timer::getTime(const char& time) const
{
	switch (time)
	{
	case(Time::NANOSECONDS):
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	case(Time::MICROSECONDS):
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	case(Time::MILLISECONDS):
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	case(Time::SECONDS):
		return std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
	case(Time::MINUTES):
		return std::chrono::duration_cast<std::chrono::minutes>(std::chrono::system_clock::now().time_since_epoch()).count();
	case(Time::HOURS):
		return std::chrono::duration_cast<std::chrono::hours>(std::chrono::system_clock::now().time_since_epoch()).count();
	default:
		return 0;
	}
}


Timer::ull Timer::getCurrentTime(const char& time) const { return getTime(time); }
void Timer::setTimeStamp(const char& time) { m_TimeStamp = getTime(time); }
Timer::ull Timer::getTimeStamp() const { return m_TimeStamp; }


void Timer::startFrameCounter() { setTimeStamp(); }
unsigned int Timer::measureFPS()
{
	++m_FPS;
	if ((getTime() - getTimeStamp()) > 999)
	{
		m_OldFPS = m_FPS;
		m_FPS = 0;
		setTimeStamp();
	}
	return m_OldFPS;
}

void Timer::startFrameTimeCounter(const char& time)
{
	m_FrameTimeStart = getTime(time);
}

Timer::ull Timer::measureFrameTime(const char& time)
{
	Timer::ull current_time = getTime(time);
	Timer::ull frame_time = current_time - m_FrameTimeStart;
	m_FrameTimeStart = current_time;
	return frame_time;
}


void Timer::startFrameTime_FPSCounter()
{
	startFrameCounter();
	startFrameTimeCounter();
}

void Timer::measureFrameTime_FPS()
{
	std::cout << "Frames: " << measureFPS() << " | Frame Time: " << measureFrameTime() / 1000000.0 << "\r";
}


void Timer::start(const char& time) { m_StartTime = getTime(time); }
void Timer::stop(const char& time) { m_StopTime = getTime(time); }

Timer::ull Timer::executionTime() { return m_ExecutionTime = m_StopTime - m_StartTime; }
Timer::ull Timer::stopExec(const char& time) { stop(time); return m_ExecutionTime = m_StopTime - m_StartTime; }


void Timer::sleep(const Timer::ull& sleep_for, const char& time) const
{
	switch (time)
	{
	case(Time::NANOSECONDS):
		//std::this_thread::sleep_until(std::chrono::time_point(sleep_for);
		std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_for));
		break;
	case(Time::MICROSECONDS):
		std::this_thread::sleep_for(std::chrono::microseconds(sleep_for));
		break;
	case(Time::MILLISECONDS):
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep_for));
		break;
	case(Time::SECONDS):
		std::this_thread::sleep_for(std::chrono::seconds(sleep_for));
		break;
	case(Time::MINUTES):
		std::this_thread::sleep_for(std::chrono::minutes(sleep_for));
		break;
	case(Time::HOURS):
		std::this_thread::sleep_for(std::chrono::hours(sleep_for));
		break;
	default:
		break;
	}
}


void Timer::wait(const Timer::ull& sleep_for, const char& time) const
{
	Timer::ull start_time = getTime(time);
	while (getTime(time) - start_time < sleep_for);
}