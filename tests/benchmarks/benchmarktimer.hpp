#pragma once

#include <string>
#include <map>
#include <chrono>
#include <iostream>

/*
 * @brief not thread safe.
*/
class BenchmarkTimer
{
	static inline std::map<std::string, size_t> kmTimeMap; // map<topic, topicTimeInMiliSeconds>
	static inline std::map<std::string, size_t> ogTimeMap; // map<topic, topicTimeInMiliSeconds>

	const std::string topic;
	const bool isOriginal;
	const std::__1::chrono::system_clock::time_point startTime;
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
			std::cout << "[" << topic << "] took: \n"
				"\t[km_malloc]" << kmDuration << "ms\n"
				"\t[og_malloc]" << ogTimeMap.at(topic) << "ms\n\n";
		}
	}

};