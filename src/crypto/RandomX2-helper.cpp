//Extra simple rx2 hashing api based off src/tests/api-example2.cpp

#include "RandomX2/src/randomx.h" //Or where your submodule is (if you even use it)
#include <iostream>
#include <iomanip>
#include <thread>

void CalculateRandomX2Hash(const void *blockData, size_t blockLength, char *blockHash) {

	const char myKey[] = "RandomX example key";
	//const char myInput[] = "RandomX example input";
	//char hash[RANDOMX_HASH_SIZE];

	randomx_flags flags = randomx_get_flags();
	//flags |= RANDOMX_FLAG_LARGE_PAGES; //This will break your cache if you don't set a big vm pagefile (vm.nr_hugepages=1500 or more)
	flags |= RANDOMX_FLAG_FULL_MEM;
	randomx_cache *myCache = randomx_alloc_cache(flags);
	if (myCache == nullptr) {
		std::cout << "Cache allocation failed" << std::endl;
		return;
	}
	randomx_init_cache(myCache, myKey, sizeof myKey);

	randomx_dataset *myDataset = randomx_alloc_dataset(flags);
	if (myDataset == nullptr) {
		std::cout << "Dataset allocation failed" << std::endl;
		return;
	}

	auto datasetItemCount = randomx_dataset_item_count();
	std::thread t1(&randomx_init_dataset, myDataset, myCache, 0, datasetItemCount / 2);
	std::thread t2(&randomx_init_dataset, myDataset, myCache, datasetItemCount / 2, datasetItemCount - datasetItemCount / 2);
	t1.join();
	t2.join();
	randomx_release_cache(myCache);

	randomx_vm *myMachine = randomx_create_vm(flags, nullptr, myDataset);
	if (myMachine == nullptr) {
		std::cout << "Failed to create a virtual machine" << std::endl;
		return;
	}

	randomx_calculate_hash(myMachine, blockData, blockLength, blockHash);

	for (unsigned i = 0; i < RANDOMX_HASH_SIZE; ++i)
		std::cout << std::hex << std::setw(2) << std::setfill('0') << ((int)blockHash[i] & 0xff);

	std::cout << std::endl;

	randomx_destroy_vm(myMachine);
	randomx_release_dataset(myDataset);

	return;
}