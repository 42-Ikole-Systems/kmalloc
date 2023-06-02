
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

slab_metadata get_slab_metadata(size_t sizeWithHeader)
{
    if (sizeWithHeader <= KMALLOC_SMALL_SLAB_ALLOCATION_MAX_SIZE) {
        return get_small_slab_metadata();
    }
    else {
        return get_large_slab_metadata();
    }
}

size_t get_true_allocation_size(size_t sizeWithHeader)
{
	const slab_metadata slabMetadata = get_slab_metadata(sizeWithHeader);
	return ((size_t)km_ceil((double)sizeWithHeader / slabMetadata.minAllocationSize)) * slabMetadata.minAllocationSize;
}
