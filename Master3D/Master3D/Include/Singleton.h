#pragma once
#include <cassert>

#ifndef __SINGLETON_H__
#define __SINGLETON_H__

template <typename T> class Singleton {
protected:
	static T* ms_singleton;
public:
	Singleton(void)
	{
		assert(!ms_singleton);
		int offset = (int)(T*)1 - (int)(Singleton<T>*)(T*)1;
		ms_singleton = (T*)((int)this + offset);
	}
	~Singleton(void)
	{
		assert(ms_singleton);
		ms_singleton = 0;
	}
	static T& GetSingleton(void)
	{
		assert(ms_singleton);
		return (*ms_singleton);
	}
	static T* GetSingletonPtr(void)
	{
		return ms_singleton;
	}
};
template<typename T> T* Singleton<T>::ms_singleton = 0;

#endif