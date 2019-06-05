
#ifndef MemoryPoolhpp
#define MemoryPoolhpp

#include <stdio.h>
#include <vector>

namespace simple_memory {

	class MemoryPool final {

	public:

		static size_t number_of_block_from_bytes(size_t t_number_of_byte);

		MemoryPool();
		MemoryPool(size_t t_memory_size);
		~MemoryPool();

		void* take_memory(size_t t_number_of_byte);
		void return_memory(void* t_pointer, size_t t_number_of_byte);
		bool is_owned(void* t_pointer);
		void print_memory_usage() const;

	private:

		void allocate_memory(size_t t_number_of_byte);
		int find_avaliable_location(size_t t_number_of_block);

		// Copy Constructor is not supported
		MemoryPool(const MemoryPool& other);
		// Copy assignment operator is not supported
		MemoryPool& operator=(MemoryPool& other);

		unsigned char* m_memory;
		size_t m_allocated_memory;
		bool* m_memory_used;

	};


	class MemoryPoolManger final {

	public:
		MemoryPoolManger();
		MemoryPoolManger(size_t t_memory_size);
		~MemoryPoolManger();

		void* take_memory(size_t t_number_of_byte);
		void return_memory(void* t_pointer, size_t t_number_of_byte);
		void print_memory_usage() const;

	private:
		// Copy Constructor is not supported
		MemoryPoolManger(const MemoryPoolManger& other);
		// Copy assignment operator is not supported
		MemoryPoolManger& operator=(MemoryPoolManger& other);

		std::vector<MemoryPool*> m_memory_pool;
		size_t m_memory_size;
	};


}

#endif /* simple_memory_pool_hpp */
