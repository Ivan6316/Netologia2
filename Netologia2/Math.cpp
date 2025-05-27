#include <iostream>
#include "Math.h"

void myMath::Counter::add(){ ++__number; }
void myMath::Counter::deduct() { --__number; }
void myMath::Counter::result() { std::cout << __number << std::endl; }
void myMath::Counter::initialValue(int value) { __number = value; }

myMath::Counter::Counter(){ __number = 1; }
myMath::Counter::Counter(int initialValue){ __number = initialValue; }