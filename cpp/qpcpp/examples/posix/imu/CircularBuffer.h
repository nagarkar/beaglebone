#pragma once
/*
template <class T> class CircularBuffer {
public:
	CircularBuffer(size_t size) :
		buf_(std::unique_ptr<T[]>(new T[size])),
		size_(size)
	{
		//empty constructor
	}

	void put(T item);
	T get(void);

	void reset(void)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		head_ = tail_;
	}

	bool empty(void);
	bool full(void);
	size_t size(void);

private:
	std::mutex mutex_;
	std::unique_ptr<T[]> buf_;
	size_t head_ = 0;
	size_t tail_ = 0;
	size_t size_;
};
*/
template <class T>
class CircularBuffer {
public:
	explicit CircularBuffer(size_t size) :
		buf_(std::unique_ptr<T[]>(new T[size])),
		size_(size)
	{
		//empty constructor
	}

	void put(T item)
	{
		std::lock_guard<std::mutex> lock(mutex_);

		buf_[head_] = item;
		head_ = (head_ + 1) % size_;

		if (head_ == tail_)
		{
			tail_ = (tail_ + 1) % size_;
		}
	}

	T get(void)
	{
		return access(false); // no peek
	}
	T peek(void)
	{
		return access(true); // peek
	}
	T access(bool peek)
	{
		std::lock_guard<std::mutex> lock(mutex_);

		if (empty())
		{
			return T();
		}

		//Read data and advance the tail (we now have a free space)
		auto val = buf_[tail_];
		if (!peek) {
			tail_ = (tail_ + 1) % size_;
		}
		return val;
	}

	void reset(void)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		head_ = tail_;
	}

	bool empty(void) const
	{
		//if head and tail are equal, we are empty
		return head_ == tail_;
	}

	bool full(void) const
	{
		//If tail is ahead the head by 1, we are full
		return ((head_ + 1) % size_) == tail_;
	}

	size_t size(void) const
	{
		return size_ - 1;
	}

	size_t currentSize(void) const
	{
		return ((head_ - tail_ + size_) % size_);
	}

private:
	std::mutex mutex_;
	std::unique_ptr<T[]> buf_;
	size_t head_ = 0;
	size_t tail_ = 0;
	size_t size_;
};