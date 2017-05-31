#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <iostream>
using namespace std;

#include "leaktrace.h"
#undef new

bool traceFlag = true;
bool activeFlag = false;

namespace {
	typedef struct MemInfo
	{
		struct      MemInfo *next, *prev;  //List
		void        *ptr;  //malloc memory
		size_t 	    size;
		size_t      line;
		char       *file;
	} MemInfo;

	class _leak_detector
	{
	private:
		void LeakDetector() noexcept;
	public:
		_leak_detector() noexcept
		{
			std::cout << "Create _leak_detector target\n";
		}
		~_leak_detector() noexcept
		{
			std::cout << "Destroy _leak_detector target\n";
			LeakDetector();
		}
	};

	static MemInfo _root = { &_root, &_root, NULL, 0, 0, NULL };  
	static size_t _memory_allocated = 0;


	void _leak_detector::LeakDetector(void) noexcept
	{
		unsigned int count = 0;
		MemInfo *ptr = _root.next;
		while (ptr && ptr != &_root)
		{
			cout << "leak out:   ";
			std::cout << ptr << " size = " << ptr->size;
			if (ptr->file)
				std::cout << " (in " << ptr->file << " at " << ptr->line << " )";
			else
				std::cout << " (no file info.)";
			std::cout << std::endl;

			++count;
			ptr = ptr->next;
		}
		if (count)
			std::cout << "Exist " << count << " times memory leak out, the total size is " << _memory_allocated << " byte." << std::endl;
	}

	_leak_detector leak_detector;  //Destruct function called sequence is reversed as Construct function
} //anonymous namespace

// Overload scalar operator new
void* operator new(size_t _size, const char *_file, long _line)
{
	void *p = malloc(_size);

	if (activeFlag)
	{
		MemInfo *newElem = (MemInfo *)malloc(sizeof(MemInfo));
		newElem->next = _root.next;  //insert after root
		newElem->prev = &_root;
		newElem->ptr = p;
		newElem->size = _size;
		newElem->line = _line;
		newElem->file = NULL;
		if (_file)
		{
			newElem->file = (char *)malloc(strlen(_file) + 1);  //
			strcpy(newElem->file, _file);
		}
		_root.next->prev = newElem;
		_root.next = newElem;
		_memory_allocated += _size;
		printf("Allocated List node address %p, and ", newElem);
	}
	if (traceFlag)
	{
		printf("Allocated %u bytes at address %p ", _size, p);
		printf("(file: %s, line: %ld)\n", _file, _line);
	}
	return p;
}

void* operator new(size_t _size) 
{
	return operator new(_size, NULL, 0);
}

// Overload array operator new
void* operator new[](size_t siz, const char* file, long line)
{
	return operator new(siz, file, line);
}

// Override scalar operator delete
void operator delete(void* p)
{
	MemInfo *MemInfoPtr = _root.next;
	for (; MemInfoPtr != &_root; MemInfoPtr = MemInfoPtr->next)
	{
		if (MemInfoPtr->ptr == p)
			break;
	}
	if (MemInfoPtr->ptr == p)
	{
		free(MemInfoPtr->ptr);
		MemInfoPtr->prev->next = MemInfoPtr->next;
		MemInfoPtr->next->prev = MemInfoPtr->prev;
		_memory_allocated -= MemInfoPtr->size;
		if (MemInfoPtr->file)
			free(MemInfoPtr->file);
		free(MemInfoPtr);
		if (traceFlag)
			printf("Deleted ListNode at address %p, Deleted memory at address %p\n", MemInfoPtr, p);
	}
	else
	{
		if(traceFlag)
			printf("Error: ListNode at address %p, memory pointer %p, Failed to delete.\n", MemInfoPtr, p);
	}
}
// Override array operator delete
void operator delete[](void* p)
{
	operator delete(p);
}
