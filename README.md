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
4. num1 is bigger and negative, num2 is smaller and negative. (num1+num2)(ans = -ve)
5. num1 is smaller and positive, num2 is bigger and negative. (num2-num1)(ans = -ve)
```C++
  inf_int bigint3("100000000000");
	inf_int bigint4("-100000000005");
	bigint3.add(bigint4);
	bigint3.print();
```
6. num1 is smaller and negative, num2 is bigger and positive. (num2-num1)(ans = +ve)
7. num1 is smaller and positive, num2 is bigger and positive. (num2+num1)(ans = +ve)
8. num1 is smaller and negative, num2 is bigger and negative. (num2+num1)(ans = -ve)

