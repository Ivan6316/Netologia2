#ifndef TABLE_H
#define TABLE_H

#include <iostream>

template<typename T>
class table
{
private:
	T** _table_;
	int _size_{};
	int _x_{};

public:
	table(int x, int y);

	int Size() const;

	T* operator[](size_t index) const;
	T* operator[](size_t index);

	~table();
};


#endif // !TABLE_H