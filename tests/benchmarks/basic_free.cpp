
#include "kmalloc/kmalloc.h"
#include "benchmarktimer.hpp"
#include <functional>
#include <vector>

static void TestAllocationSameSizeImpl(const std::string& topic, bool isOriginal, size_t allocationSize, size_t repeatAmount, std::function<void*(size_t size)> allocator, std::function<void(void*)> deallocator)
{
	std::vector<void*> allocations;
	for (size_t i = 0; i < repeatAmount; i++) {
		allocations.push_back(allocator(allocationSize));
	}
	BenchmarkTimer timer(topic, isOriginal);
	for (auto allocation : allocations) {
		deallocator(allocation);
	}
}
static void TestAllocationSameSize(const std::string& topic, size_t allocationSize, size_t repeatAmount)
{
	TestAllocationSameSizeImpl(topic, true, allocationSize, repeatAmount, malloc, free);
	TestAllocationSameSizeImpl(topic, false, allocationSize, repeatAmount, km_malloc, km_free);

	// TestAllocationSameSizeImpl(topic, false, allocationSize, repeatAmount, km_malloc);
	// TestAllocationSameSizeImpl(topic, true, allocationSize, repeatAmount, malloc);
}

int main()
{
	TestAllocationSameSize("freeing 32bytes 1048576 times", 32, 99999);

	BenchmarkTimer::Print();
}
