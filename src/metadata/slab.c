
#include "kmalloc/metadata/slab.h"

slab_metadata get_small_slab_metadata()
{
	slab_metadata slabMetadata;

	slabMetadata.minAllocationSize = KMALLOC_SMALL_SLAB_ALLOCATION_MIN_SIZE;
	slabMetadata.maxAllocaitonSize = KMALLOC_SMALL_SLAB_ALLOCATION_MAX_SIZE;
	slabMetadata.sizeInBytes = KMALLOC_SMALL_SLAB_SIZE;

	return slabMetadata;
}

slab_metadata get_large_slab_metadata()
{
	slab_metadata slabMetadata;

	slabMetadata.minAllocationSize = KMALLOC_LARGE_SLAB_ALLOCATION_MIN_SIZE;
	slabMetadata.maxAllocaitonSize = KMALLOC_LARGE_SLAB_ALLOCATION_MAX_SIZE;
	slabMetadata.sizeInBytes = KMALLOC_LARGE_SLAB_SIZE;

	return slabMetadata;
}

slab_header** get_slab_head_from_zone(zone_header* zone, size_t sizeWithHeader)
{
	if (sizeWithHeader <= KMALLOC_SMALL_SLAB_ALLOCATION_MAX_SIZE) {
		return &(zone->smallSlabs);
	}
	else if (sizeWithHeader < KMALLOC_LARGE_SLAB_ALLOCATION_MAX_SIZE) {
		return &(zone->largeSlabs);
	}
	else {
		return NULL; // should not happen
	}
}

slab_header* create_slab(zone_header* zoneHead, slab_metadata metaData)
{
	assert(zoneHead != NULL);

	zone_header* zone = zoneHead;
	zone_header* prev = NULL;
	while (zone)
	{
		// TODO: if smaller than page division results in 0
		if (zone->freePages <= metaData.sizeInBytes / KMALLOC_PAGE_SIZE) {
			break ;
		}
		prev = zone;
		zone = zone->nextZone;
	}

	slab_header** slabHead = get_slab_head_from_zone(zone, metaData.maxAllocaitonSize);
	if (slabHead == NULL) {
		return NULL;
	}
	slab_header* prev;
	for (slab_header* slab = slabHead; slab; slab = slab->nextSlab)
	{
		prev = slab;
	}
	if (prev == NULL)
	{
		zone->nextFreePage // aaahhh
	}

}

void* get_region_in_slab(const slab_header* slab, size_t sizeWithHeader, slab_metadata slabMetadata)
{
	const size_t consecutiveRegionsRequired = getSizeInRegions(sizeWithHeader, slabMetadata);
	size_t consecutiveFreeRegions = 0;
	const uint32_t bitsInBitmask = 32;

	// we loop over all bits
	for (uint32_t i = 0; i < sizeof(slab->allocations) * bitsInBitmask; i++)
	{
		// can be optimised by checking whole bytes or 4bytes at a time if they are full.
		const uint32_t bitToCheck = 1 << (i % bitsInBitmask);
		if (slab->allocations[i / bitsInBitmask] & bitToCheck) {
			consecutiveFreeRegions++;
		}
		else {
			consecutiveFreeRegions = 0;
		}
		
		if (consecutiveFreeRegions == consecutiveRegionsRequired) {
			return (void*)slab + ((i - consecutiveRegionsRequired) * (slabMetadata.minAllocationSize));
		}
	}
	return NULL;
}

size_t get_true_allocation_size(size_t sizeWithHeader, slab_metadata slabMetadata)
{
	return ((size_t)km_ceil((double)sizeWithHeader / slabMetadata.minAllocationSize)) * slabMetadata.minAllocationSize;
}

void* slab_allocate(slab_header* slab, size_t sizeWithHeader)
{
	void* region = get_region_in_slab(slab, sizeWithHeader);
}
