#ifndef BENCHMARKING_H_
#define BENCHMARKING_H_

#include <chrono>

class Timer {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start_timepoint;
public:
	Timer();
	~Timer();
	void stop();
};


#endif // BENCHMARKING_H_