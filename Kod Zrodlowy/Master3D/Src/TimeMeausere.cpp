#include "stdafx.h"
#include <TimeMeasure.h>

#define NANO_PER_SECOND 1000000000.0

TimeMeasure TimeMeasure::start()
{
	return TimeMeasure();
}


double TimeMeasure::reset()
{
	double elapsedSec = measure();
	mStartHigh = std::chrono::high_resolution_clock::now();
	return elapsedSec;
}

double TimeMeasure::measure() const
{
	auto finish = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(finish - mStartHigh).count() / NANO_PER_SECOND;
	 
}

TimeMeasure::TimeMeasure()
{
	mStart = clock();
	mStartHigh = std::chrono::high_resolution_clock::now();
}


#pragma once
