#ifndef COUNTER_H
#define COUNTER_H

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

		// ����������� � ����������
		Counter(int initialValue);
	};
}


#endif