# inf_int
What is this? A class that enables storing of infinitely big integers.

Test Cases of SpecialAdd() and SpecialSubtract():
1. num1 is bigger and positive, num2 is smaller and negative. (num1-num2)(ans = +ve) 
  ```C++
  inf_int b1("1000000000");
  inf_int b2("-100000000");
  b1.SpecialAdd(b2);
  ```
2. num1 is bigger and negative, num2 is smaller and positive. (num1-num2)(ans = -ve)
```C++
  inf_int bigint3("-100000000005");
	inf_int bigint4("100000000000");
	bigint3.add(bigint4);
	bigint3.print();
```
3. num1 is bigger and positive, num2 is smaller and positive. (num1+num2)(ans = +ve)
```C++
	inf_int bigint3("100000000005");
	inf_int bigint4("100000000000");
	bigint3.add(bigint4);
	bigint3.print();
```
4. num1 is bigger and negative, num2 is smaller and negative. (num1+num2)(ans = -ve)
```C++
	inf_int bigint3("-100000000005");
	inf_int bigint4("-100000000000");
	bigint3.add(bigint4);
	bigint3.print();
```
5. num1 is smaller and positive, num2 is bigger and negative. (num2-num1)(ans = -ve)
```C++
  inf_int bigint3("100000000000");
	inf_int bigint4("-100000000005");
	bigint3.add(bigint4);
	bigint3.print();
```
6. num1 is smaller and negative, num2 is bigger and positive. (num2-num1)(ans = +ve)
```C++
  inf_int bigint3("-100000000000");
	inf_int bigint4("100000000005");
	bigint3.add(bigint4);
	bigint3.print();
```
7. num1 is smaller and positive, num2 is bigger and positive. (num2+num1)(ans = +ve)
```C++
	inf_int bigint3("100000000003");
	inf_int bigint4("100000000009");
	bigint3.add(bigint4);
	bigint3.print();
```
8. num1 is smaller and negative, num2 is bigger and negative. (num2+num1)(ans = -ve)
```C++
	inf_int bigint3("-100000000003");
	inf_int bigint4("-100000000009");
	bigint3.add(bigint4);
	bigint3.print();
```
9. addition of two numbers with same length that results in result of length+1
10. subtraction that results in 0
