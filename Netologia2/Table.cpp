#include "Table.h"

template<typename T>
table<T>::table(int x, int y) : _size_(x * y), _x_(x)
{
	_table_ = new T * [x];
	for (size_t index{}; index < x; ++index)
	{
		_table_[index] = new T[y]{};
	}
}

template<typename T>
int table<T>::Size() const { return _size_; }

template<typename T>
T* table<T>::operator[](size_t index) const { return _table_[index]; }

template<typename T>
T* table<T>::operator[](size_t index) { return _table_[index]; }

template<typename T>
table<T>::~table()
{
	for (size_t index{}; index < _x_; ++index)
	{
		delete[] _table_[index];
	}

	delete[] _table_;
}