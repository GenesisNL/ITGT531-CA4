#include <iostream>

#include "MemoryPool.h"

namespace testing_memory_pool {
	struct Position {
		float x;
		float y;
		float z;
	};

	struct Item {
		size_t id;
	};

}

using namespace std;
using namespace simple_memory;
using namespace testing_memory_pool;

int main(int argc, const char* argv[]) {
	MemoryPoolManger pool(8);

	pool.print_memory_usage();

	cout << endl << endl << endl;
	cout << "Memory => Position (" << sizeof(Position) << "bytes)" << endl;
	Position* position = static_cast<Position*> (pool.take_memory(sizeof(Position)));
	pool.print_memory_usage();


	cout << endl << endl << endl;
	cout << "Memory => Item (" << sizeof(Item) << "bytes)" << endl;
	Item* item1 = static_cast<Item*> (pool.take_memory(sizeof(Item)));
	pool.print_memory_usage();

	cout << endl << endl << endl;
	cout << "Memory <= Position (" << sizeof(Position) << "bytes)" << endl;
	pool.return_memory(static_cast<void*>(position), sizeof(Position));
	pool.print_memory_usage();

	cout << endl << endl << endl;
	cout << "Memory => Item (" << sizeof(Item) << "bytes)" << endl;
	cout << "Memory => Item (" << sizeof(Item) << "bytes)" << endl;
	Item* item2 = static_cast<Item*> (pool.take_memory(sizeof(Item)));
	Item* item3 = static_cast<Item*> (pool.take_memory(sizeof(Item)));
	pool.print_memory_usage();

	item1->id = 1;
	item2->id = 2;
	item3->id = 3;

	cout << "Item1:" << item1->id << endl;
	cout << "Item2:" << item2->id << endl;
	cout << "Item3:" << item3->id << endl;

	return 0;
}
