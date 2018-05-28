#include <cstdio>
#include <memory>
#include <mutex>
#include "CircularBuffer.h"

/**
* Important Usage Note: This library reserves one spare entry for queue-full detection
* Otherwise, corner cases and detecting difference between full/empty is hard.
* You are not seeing an accidental off-by-one.
*/
/*
template<class T> 
void CircularBuffer<T>::put(T item)
{
	std::lock_guard<std::mutex> lock(mutex_);

	buf_[head_] = item;
	head_ = (head_ + 1) % size_;

	if (head_ == tail_)
	{
		tail_ = (tail_ + 1) % size_;
	}
}

template<class T>
T CircularBuffer<T>::get(void)
{
	std::lock_guard<std::mutex> lock(mutex_);

	if (empty())
	{
		return T();
	}

	//Read data and advance the tail (we now have a free space)
	auto val = buf_[tail_];
	tail_ = (tail_ + 1) % size_;

	return val;
}

template<class T>
bool CircularBuffer<T>::empty(void)
{
	//if head and tail are equal, we are empty
	return head_ == tail_;
}

template<class T>
bool CircularBuffer<T>::full(void)
{
	//If tail is ahead the head by 1, we are full
	return ((head_ + 1) % size_) == tail_;
}

template<class T>
size_t CircularBuffer<T>::size(void)
{
	return size_ - 1;
}
*/