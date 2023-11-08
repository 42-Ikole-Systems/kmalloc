
#include "kmalloc/kmalloc.h"
#include "benchmarktimer.hpp"
#include <functional>

static void TestAllocationSameSizeImpl(const std::string& topic, bool isOriginal, size_t allocationSize, size_t repeatAmount, std::function<void*(size_t size)> allocator)
{
	BenchmarkTimer timer(topic, isOriginal);
	for (size_t i = 0; i < repeatAmount; i++) {
		(void)allocator(allocationSize);
	}

}
static void TestAllocationSameSize(const std::string& topic, size_t allocationSize, size_t repeatAmount)
{
	TestAllocationSameSizeImpl(topic, true, allocationSize, repeatAmount, malloc);
	TestAllocationSameSizeImpl(topic, false, allocationSize, repeatAmount, km_malloc);

	// TestAllocationSameSizeImpl(topic, false, allocationSize, repeatAmount, km_malloc);
	// TestAllocationSameSizeImpl(topic, true, allocationSize, repeatAmount, malloc);
}

int main()
{
	TestAllocationSameSize("Allocating 32bytes 1048576 times", 32, 4096);

	BenchmarkTimer::Print();
}
