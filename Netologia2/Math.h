#ifndef MATH_H
#define MATH_H

namespace myMath
{
	class Counter
	{
	private:
		int __number{};
	public:
		void add();
		void deduct();
		void result();
		void initialValue(int value);

		Counter();

		// Конструктор с параметром
		Counter(int initialValue);
	};
}


#endif