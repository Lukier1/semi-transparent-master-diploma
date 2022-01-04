#pragma once

#include <ctime>

class TimeMeasure {
public:
	static TimeMeasure start();
	double reset();
	double measure() const;
private:
	TimeMeasure();
	bool started = false;
	clock_t mStart;
	std::chrono::time_point<std::chrono::high_resolution_clock> mStartHigh;
};