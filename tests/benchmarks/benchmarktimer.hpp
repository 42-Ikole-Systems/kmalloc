#pragma once

#include <string>
#include <map>
#include <chrono>
#include <iostream>

#define COLOR_LBLUE	 "\033[38;5;14m"
#define COLOR_BLUE	 "\033[38;5;12m"
#define COLOR_RED	 "\033[31m"
#define COLOR_GREEN	 "\033[32m"
#define COLOR_YELLOW "\033[38;5;11m"
#define COLOR_RESET	 "\033[0m"


/*
 * @brief not thread safe.
*/
class BenchmarkTimer
{
	static inline std::map<std::string, size_t> kmTimeMap; // map<topic, topicTimeInMiliSeconds>
	static inline std::map<std::string, size_t> ogTimeMap; // map<topic, topicTimeInMiliSeconds>

	const std::string topic;
	const bool isOriginal;
	const std::chrono::system_clock::time_point startTime;
	bool stopped;

public:

	BenchmarkTimer(const std::string& topic_, const bool isOriginal_)
		: topic(topic_)
		, isOriginal(isOriginal_)
		, startTime(std::chrono::system_clock::now())
		, stopped(false)
	{
	}
 
	~BenchmarkTimer() {
		StopTimer();
	}
	
	void StopTimer() {
		if (!stopped) {
			stopped = true;
			const auto stopTime = std::chrono::system_clock::now();
			const size_t duration = std::chrono::duration_cast<std::chrono::microseconds>(stopTime - startTime).count();
			if (isOriginal) {
				ogTimeMap[topic] += duration;
			}
			else  {
				kmTimeMap[topic] += duration;
			}
		}
	}

	static void Print()
	{
		for (const auto& [topic, kmDuration] : kmTimeMap) {
			std::cout << "[" COLOR_LBLUE << topic << COLOR_RESET "] took: \n"
				"\t[" COLOR_BLUE "km" COLOR_RESET "]: " << kmDuration << "ms\n"
				"\t[" COLOR_RED "og" COLOR_RESET "]: " << ogTimeMap.at(topic) << "ms\n\n";
		}
	}

};