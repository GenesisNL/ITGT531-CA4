#include "MemoryPool.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace simple_memory {

	const size_t DEFAULT_ALLOCATE_SIZE = 20;
	const size_t DEFAULT_MEMORY_BLOCK_SIZE = 4;

	size_t MemoryPool::number_of_block_from_bytes(size_t t_number_of_byte) {
		return (t_number_of_byte / DEFAULT_MEMORY_BLOCK_SIZE) +
			((t_number_of_byte % DEFAULT_MEMORY_BLOCK_SIZE == 0) ? 0 : 1);
	}

	MemoryPool::MemoryPool() : MemoryPool(DEFAULT_ALLOCATE_SIZE) {}

	MemoryPool::MemoryPool(size_t t_memory_size) : m_allocated_memory(t_memory_size) {
		allocate_memory(t_memory_size * DEFAULT_MEMORY_BLOCK_SIZE);
		m_memory_used = new bool[t_memory_size];
		memset(m_memory_used, 0, t_memory_size);
	}


	MemoryPool::~MemoryPool() {
		free(m_memory);
		delete[] m_memory_used;
	}

	void* MemoryPool::take_memory(size_t t_number_of_byte) {
		size_t number_of_block = number_of_block_from_bytes(t_number_of_byte);

		int location = find_avaliable_location(number_of_block);
		if (location < 0) { return nullptr; }

		for (size_t i = 0; i < number_of_block; i++) {
			m_memory_used[location + i] = true;
		}

		return static_cast<void*>(m_memory + location * DEFAULT_MEMORY_BLOCK_SIZE);
	}

	void MemoryPool::return_memory(void* t_pointer, size_t t_number_of_byte) {
		size_t number_of_block = number_of_block_from_bytes(t_number_of_byte);
		size_t location = (static_cast<unsigned char*>(t_pointer) - m_memory) / DEFAULT_MEMORY_BLOCK_SIZE;

		for (size_t i = 0; i < number_of_block; i++) {
			m_memory_used[location + i] = false;
		}
	}

	bool MemoryPool::is_owned(void* t_pointer) {
		return t_pointer >= m_memory && t_pointer <= (m_memory + m_allocated_memory * DEFAULT_MEMORY_BLOCK_SIZE);
	}


	void MemoryPool::print_memory_usage() const {

		static const size_t DISPLAY_COLUMN = 10;

		printf("     Memory     ");
		for (size_t i = 0; i < m_allocated_memory; i++) {
			if (i % DISPLAY_COLUMN == 0) {
				printf("\n\n");
			}
			printf((m_memory_used[i]) ? "|X" : "| ");
		}
		printf("|\n\n");

	}

	int MemoryPool::find_avaliable_location(size_t t_number_of_block) {

		for (int i = 0; i < m_allocated_memory - (t_number_of_block - 1); i++) {
			bool used = false;

			for (size_t block = 0; block < t_number_of_block; block++) {
				used = used || m_memory_used[i + block];
			}

			if (!used) {
				return i;
			}
		}
		return -1;
	}

	void MemoryPool::allocate_memory(size_t t_number_of_byte) {
		m_memory = static_cast<unsigned char*>(malloc(t_number_of_byte));

		if (m_memory == nullptr) {
			throw "cannot allocate memory";
		}
	}


	MemoryPool::MemoryPool(const MemoryPool & other) {
		throw "Copy Constructor is not supported";
	}

	MemoryPool& MemoryPool::operator=(MemoryPool & other) {
		throw "Copy assignment operator is not supported";
	}




	// MemoryPoolManager

	MemoryPoolManger::MemoryPoolManger() :MemoryPoolManger(DEFAULT_ALLOCATE_SIZE) {}

	MemoryPoolManger::MemoryPoolManger(size_t t_memory_size) : m_memory_size(t_memory_size) {
		m_memory_pool.push_back(new MemoryPool(t_memory_size));
	}

	MemoryPoolManger::~MemoryPoolManger() {
		for (size_t i = 0; i < m_memory_pool.size(); i++) {
			delete m_memory_pool[i];
		}
	};

	void* MemoryPoolManger::take_memory(size_t t_number_of_byte) {
		for (size_t i = 0; i < m_memory_pool.size(); i++) {
			void* pointer = m_memory_pool[i]->take_memory(t_number_of_byte);
			if (pointer != nullptr) { return pointer; }
		}

		MemoryPool* new_pool = new MemoryPool(m_memory_size);
		m_memory_pool.push_back(new_pool);
		return new_pool->take_memory(t_number_of_byte);
	}

	void MemoryPoolManger::return_memory(void* t_pointer, size_t t_number_of_byte) {

		for (size_t i = 0; i < m_memory_pool.size(); i++) {
			if (m_memory_pool[i]->is_owned(t_pointer)) {
				m_memory_pool[i]->return_memory(t_pointer, t_number_of_byte);
				return;
			}
		}

		throw "Something went wrong";
	}

	void MemoryPoolManger::print_memory_usage() const {
		printf("     Manager     \n");
		for (size_t i = 0; i < m_memory_pool.size(); i++) {
			m_memory_pool[i]->print_memory_usage();
		}
	}



	MemoryPoolManger::MemoryPoolManger(const MemoryPoolManger & other) {
		throw "Copy Constructor is not supported";
	}

	MemoryPoolManger& MemoryPoolManger::operator=(MemoryPoolManger & other) {
		throw "Copy assignment operator is not supported";
	}
}
