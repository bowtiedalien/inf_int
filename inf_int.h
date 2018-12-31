#pragma once
#include <iostream>
#include <string>
#include <math.h>
#include <limits.h>
#include <vector>
#define max_digits 9
using namespace std;

class inf_int
{
private:
	char *digit = nullptr; //stores the digits in an array
	int length = 0;
	bool sign = 1; //positive by default
	static int objcount;
	int id = 0;

public:

	int computeLengthofInt(int a) //to find the length of a number passed as an integer
	{
		int count = 0;
		if (a < 0)
		{
			a = a * -1;
			while (a != 0)
			{
				a = a / 10;
				count++;
			}
			return count + 1;
		}
		else
		{
			while (a != 0)
			{
				a = a / 10;
				count++;
			}
			return count;
		}
	}

	inf_int()
	{
		objcount++;
		id = objcount;

		length = 1;
		digit = new char[1];//array is initialised with size zero 
		digit[0] = '0';

	}

	inf_int(int num) //parameterised constructor with int param
	{
		objcount++;
		id = objcount;

		string numb;

		if (num<0) sign = 0; //negative
		else sign = 1; //positive

		numb = to_string(num); //convert the incoming integer into a string 

		digit = new char[numb.length()+1];
		length = numb.length();
		int i = 0;
		while (numb[i] != '\0')
		{
			digit[i] = numb[i];
			i++;
		}
		digit[length] = '\0';

		//.........to avoid headaches:
		if (digit[0] == '+')
		{
			deleteFirstDigit();
		}
	}

	void deleteFirstDigit()
	{
		string a(digit);
		a.erase(0, 1);
		string_copy(digit, a);
		length--;
		digit[length] = '\0';
	}

	inf_int(string num)
	{
		if (num[0] == '-')
			sign = 0; 
		else
			sign = 1; //positive

		objcount++;
		id = objcount;

		digit = new char[num.length()+1]; //+1 : a place for null char
		length = num.length();

		int i = 0;
		string_copy(digit, num);

		if (digit[0] == '+')
		{
			num.erase(0, 1);
			length = length - 1;
		}

	}

	inf_int(const inf_int& other) //copy constructor
	{
		objcount++;
		id = objcount;
		length = other.length; //update its length to the length of the object it is being assigned to
		digit = new char[length+1];
		sign = other.sign;
		string s(other.digit);

		string_copy(digit, s);

		//strcpy(digit, s.c_str()); //EXCEPTION: access violation. 
		//potential problems:
		//1)I'm trying to write to read-only memory
		//(apparently a pointer is placed in read-only memory because it is a constant, so it can't be written on.)
		//2)I'm passing a NULL for dest in strcpy() - the dest in this case is digit[]
		//3)my dest doesn't have enough memory allocated for src
	}

	void string_copy(char *a, string s) //mock-up of strcpy()
	{
		for (int i = 0; i < length; i++)
		{
			digit[i] = s[i];
		}
		digit[length] = '\0';
		
	}

	
	void setValue(int v)
	{
		string value;
		value = to_string(v);
		delete digit; //delete old value of this object, if it had one.
		length = 0; //reinitialise length
		int i = 0;
		if (value[0] == '-')
			sign = 0; //reinitialise sign
		else if (value[0] == '+')
			sign = 1; //reinitialise sign
		else
			sign = 1;

		digit = new char[value.length()];
		while (value[i] != '\0')
		{
			digit[i] = value[i];
			i++;
			length++; //increment the length instance variable
		}
	}

	void reassign_this(int len) //to help with the repeated reassignment I do in Specialadd() and SpecialSubtract()
	{
		//NOT TO BE USED WITH OBJECT	S OTHER THAN *THIS
		length = len;
		delete[] digit; //again, error if I try to use this function in cleanup() because of delete[] digit here
		digit = new char[length];
	}

	void add(const inf_int& obj)
	{
		if (length > max_digits || obj.length > max_digits)
		{
			if(sign!=obj.sign)
			{
				SpecialSubtract(obj);
				cleanup();
			}
			else
				SpecialAdd(obj);
		}
		else {
			int num1 = stoi(digit);
			int num2 = stoi(obj.digit);

			int result = num1 + num2;
			string sresult = to_string(result);
			this->reassign_this(sresult.length());
			string_copy(digit, sresult);
		}

	}
	
	string subtract(inf_int& obj1, inf_int& obj2)
	{
		string firstnum(obj1.digit);
		string secondnum(obj2.digit);

		int num1;
		int num2;
		int res;
		string result = "";

		bool bringin = false;

		for (int i = (obj1.length - 1); i >= 0; i--) //working backwards, do subtraction
		{
			num1 = to_int(firstnum[i]);
			num2 = to_int(secondnum[i]);

			if (bringin)
			{
				num1 = num1 - 1;
			}

			if (num2 > num1)
			{
				bringin = true;
				num1 = num1 + 10; 
			}

			res = num1 - num2;
			result = to_string(res) + result; //building up towards the left, starting from the right
		}
		return result;
		
	}

	//--------------------if number exceeds INT_MAX, these functions are used


	void SpecialSubtract(const inf_int& obj)
	{
		inf_int temp = obj;
		string result; //what subtract() returns

		if (sign == 0 && temp.sign == 1) //if the calling object is negative and incoming object is positive
		{
			{
				deleteFirstDigit();
			}
			int difference;

			enum oneOfTwo { ob1, ob2 }; //meant to decrease overhead for repeatedly calling Bigger() in following conditional statements
			oneOfTwo bigger = (Bigger(temp) == 0) ? ob1 : ob2;
		
			if (length == temp.length && digit[0] == temp.digit[0]) //they're the same length and same first number
			{
				if (checkequality(*this, temp) == true) {
					//they evaluate to zero

					this->reassign_this(1);
					digit[0] = '0';
				}
				else { 
				//we only call for Bigger when we have already tested for the
				//possibility that the numbers are equal. Because Bigger() can not discover equality. It only discovers who's bigger
					
					if (Bigger(temp) == 0)
						bigger = ob1;
					else
						bigger = ob2;
				
				}
			}

			if (bigger==ob1)  //if first number is bigger
			{
				//we will do: calling object - incoming object
				//result is negative
				//expand temp

				if (length > temp.length) //if the length is different, expand the second object
				{
					difference = length - temp.length;
					temp.expand(difference);
				}

				result = subtract(*this, temp);

				this->reassign_this(result.length() + 1); 
				result = '-' + result;
				strcpy(digit, result.c_str());
			}

			else //second object is bigger
			{
				//incoming - calling
				//result is positive

				if (temp.length > length) { //if length is different, expand
					difference = temp.length - length;
					expand(difference);
				}

				result = subtract(temp, *this);

				this->reassign_this(result.length());
				strcpy(digit, result.c_str());
			}
			
		}
		
		else //if calling object is positive and incoming object is negative
			{
				temp.deleteFirstDigit();
				int difference;

				enum oneOfTwo { ob1, ob2 }; 
				/*oneOfTwo bigger;
				if (Bigger(temp) == 0)
					bigger = ob1;
				else
					bigger = ob2;*/
				oneOfTwo bigger = Bigger(temp) == 0 ? ob1 : ob2;

				if (temp.length == length && digit[0] == temp.digit[0]) //same length and same starting digit, ignore Bigger()'s results.
				{
					if (checkequality(*this,temp) == true) { 
						//they are the same number
						//their subtraction evaluates to zero

						this->reassign_this(1); //reassign digit[] to length=1
						digit[0] = '0';
					}
					else
					{
						if (Bigger(temp) == 0)
						{
							bigger = ob1;
						}
						else
							bigger = ob2;
					}
				}

				if (bigger == ob2) //if negative incoming is bigger: incoming - calling
				{
					//result is negative

					if (temp.length > length) //if incoming is bigger IN LENGTH
					{
						difference = temp.length - length;
						expand(difference);
					}

					string result = subtract(temp, *this); //take the string that subtract() returns

					this->reassign_this(result.length() + 1);
					result = '-' + result; //append the minus sign
					strcpy(digit, result.c_str());
				}

				else //if positive calling object is bigger: calling - incoming
				{
					if (length > temp.length) //if *this is bigger IN LENGTH
					{
						difference = length - temp.length;
						temp.expand(difference);
					}

					string result = subtract(*this, temp);
					string_copy(digit, result);

				}
		}

	}

	int SpecialAdd(const inf_int& obj) //add an incoming object to a calling object 
		//TESTED
	{
		if (sign != obj.sign) {
			SpecialSubtract(obj);
			return 0; //do not resume the rest of the function
		}
#if 0
		inf_int temp;
		temp = obj;

		if (temp.sign == 0 && sign == 0) //if they're negative, remove signs to deal with raw numbers
		{
			string a(this->digit); //why I can do this: string class has a constructor that takes a NULL-terminated c-string
			string b(temp.digit);
			a.erase(0, 1);
			b.erase(0, 1);
		}
		//then resume addition

		if (length != temp.length) //if they're not equal in length, we have to make them equal.
		{
			int difference = abs(length - temp.length);

			if (length > temp.length)
				temp.expand(difference); //function that adds extra places and fills them with zeros
			else
				this->expand(difference); //expand operation is performed on this object
		}
		string result(length,'0'); //create a string that is as long as the length initialised to all zeros
		int carry = 0;
		int add = 0; //to hold the result of the integer addition
		int num1;
		int num2;
		string numb1(this->digit);
		string numb2(temp.digit);
		reverse(numb1.begin(),numb1.end()); //flip the numbers
		reverse(numb2.begin(),numb2.end());

		for (int i = 0; i<length; i++) 
		{
			num1 = to_int(numb1[i]); //save current digits as ints
			num2 = to_int(numb2[i]);

			add = num1 + num2 + carry; //MAIN OPERATION

			if (add > 9)
			{
				if (i == (length-1)) //if it's the last digit
				{
					result[i] = to_char(add % 10); //take the second digit
					add = add / 10; //the first digit (the overflow)
					result = result + to_char(add); //concatenate the carry to the end of the string result
				}
				else
				{
					result[i] = to_char(add % 10); //take the second digit
					add = add / 10;
					carry = add; //save the overflow as carry
				}
			}
			else {
				result[i] = to_char(add); //save the addition as char to get ready for moving into digit[] array
				carry = 0;
			}
		}
		if (sign == 1) { //if the result was supposed to be positive
			numb1 = result; //copy the result into the first object's string
			reverse(numb1.begin(), numb1.end()); //reverse it back to original
			this->reassign_this(result.length());
			strcpy(digit, numb1.c_str());
		}
		else { //if the numbers were both negative, the result will be negative too.
			numb1 = result;
			reverse(numb1.begin(), numb1.end()); //flip to original
			numb1 = '-' + numb1; //negative result

			this->reassign_this(numb1.length());
			strcpy(digit, numb1.c_str());
		}
		return 1; //it worked correctly
#endif
	}

	//-------------------otherwise, they are ignored.
	
	void cleanup() //removes zeros at the MSBs after subtraction
	{//CHECKED
		string holder(digit); //copy the char array into a string

		int end = holder.length();

		for (int i = 0; i < end; i++)
		{
			if (holder[i] == '-')
			{
				continue; //skip the minus sign
			}
			else if (holder[i] == '0')
			{
				holder.erase(i, 1);
				end = end - 1;
				i = i - 1; 
			}
			else //once we hit a number!=0, we don't need to clean up any more
			{
				break;
			}
		}

		length = end;
		/*this->reassign_this(length);*/ 
		delete[] digit; //won't let me.
		digit = new char[length];
		strcpy(digit, holder.c_str());

	}

	bool checkequality(const inf_int& obj1, const inf_int& obj2) //checks if two objects are equal (does not consider signs)
	{//RECHECKED
		bool areEqual = true;

		for (int i = 0; i < obj1.length; i++)
		{
			if (obj1.digit[i] != obj2.digit[i])
			{
				areEqual = false;
			}

		}

		return areEqual;
	}

	void expand(int diff)
	{//RECHECKED
		string st;
		for (int i = 0; i < length; i++)
		{
			st = st + digit[i];
		}
		for (int i = 0; i < diff; i++)
		{
			st = '0' + st; //concatenate zeros to the beginning of the string array
		}
		strcpy(digit, st.c_str());
		length = length + diff;
	}

	//cute little functions:
	char to_char(int a) 
	{
		return a+48;
	}
	int to_int(char a)
	{
		return a - 48;
	}
	//----------

	void changeSign() //RECHECKED
	{
		if (sign == 0) //negative
		{
			sign = 1; //make positive
			string a(digit); //check whether the digit array that gets sent here is null-terminated.
			a.erase(0, 1);
		}
		else
		{
			sign = 0;
			length = length + 1; //to make room for negative sign now
			char* temp = new char[length];
			temp[0] = '-';

			int j = 0;
			for (int i = 1; i < length; i++)
			{
				temp[i] = digit[j];
				j++;
			}
			this->reassign_this(length);

			for (int i = 0; i < length; i++) //deep copy for digit[] array
			{
				digit[i] = temp[i];
			}
			delete[] temp;
		}
	}
	void print()
	{
		cout << "Info of object " << id << endl;
		cout << "Length is: " << length << endl;
		cout << "Number is: ";

		for (int i = 0; i < length; i++)
		{
			cout << digit[i];
		}
		cout << endl;
	}

	void concatenate(const inf_int& obj)
	{
		string temp; //to temporarily store the resulting string of numbers

		for (int i = 0; i < length; i++)
		{
			temp = temp + digit[i]; //temp takes every digit in this object
		}

		for (int j = 0; j < obj.length; j++)
		{
			temp = temp + obj.digit[j];
		}

		delete[] digit; //get rid of old object

		length = length + obj.length; //set new length 

		digit = new char[length]; 

		int i = 0;
		while (temp[i] != '\0')
		{
			digit[i] = temp[i];
			i++;
		}
		
	}

	bool Bigger(const inf_int& obj2) //return bool
	{
		string a(digit);
		string b(obj2.digit);
			/*inf_int temp1 = *this;
			inf_int temp2 = obj2;*/
		int dif;
		if (/*temp1.length*/a.length() > /*temp2.length*/ b.length()) //if temp1 is bigger in length, it wins
		{
			return 0; //temp1
		}
#if 0
		else if (temp2.length > temp1.length) //if temp2 is bigger in length, it wins
		{
			return 1; //temp2
		}
		else if (temp1.digit[0]>temp2.digit[0]) //if they reach here, they must be equal in length. if temp1's first digit is bigger, it wins
		{
			return 0; //temp1
		}
		else if(temp2.digit[0]>temp1.digit[0]) //otherwise if temp2's first digit is bigger, it wins
		{
			return 1; //temp2
		}
		else //otherwise we need to check every digit until we find the biggest of the two
		{
			int biggerlen;
			biggerlen = (temp1.length>temp2.length)?temp1.length:temp2.length; //it takes the value of the bigger length
			for (int i = 0; i < biggerlen; i++)
			{
				if (temp1.digit[i] > temp2.digit[i])
				{
					return 0;
				}
				else if (temp1.digit[i] == temp2.digit[i])
					continue;
				else
					return 1; 
			}
		}
#endif
	}

	bool operator==(const inf_int& obj)
	{
		if (this->length == obj.length && this->sign == obj.sign) //check the length and sign
		{
			for (int i = 0; i < length; i++) //check each digit
			{
				if (this->digit[i] != obj.digit[i])
				{
					return false;
				}
			}
			return true;
		}
		else
			return false;
	}

	inf_int operator=(const inf_int& other) 
	{
		delete[] digit; //delete current value of the calling object
		length = other.length; //update its length to the length of the object it is being assigned to
	
		sign = other.sign; 
		digit = new char[length]; 
		string stemp;
		into_string(other.digit, stemp, other.length);
		string_copy(digit, stemp);

		return *this;
	}
	
	inf_int operator++(int) //post increment operator
	{
		inf_int tmp = *this;
		string str = "";
		for (int i = 0; i < length; i++)
		{
			str = str + digit[i];
		}
		if (str.length() >= max_digits)
		{
			post_increment();
			return tmp;
		}
		else {
			int num;
			num = stoi(str);
			num = num + 1; //incremented
			length = computeLengthofInt(num);
			delete[] digit;
			digit = new char[length];

			str = to_string(num);

			strcpy(digit, str.c_str()); //strcpy cleans up the garbage characters at the end of my digit[] array. Wow!
		return tmp; //return old value 
		}	
	}

	inf_int post_increment()
	{
		inf_int tmp;
		tmp = *this; //returned in the end
		string s = "";
		into_string(digit, s, length); //copy values of digit into the string s
		reverse(s.begin(), s.end()); 

		int increm = to_int(s[0]) + 1; //I'm adding 1 to the very last digit (i.e. first digit now)

		int i = 0; //current digit
		if (increm > 9)
		{
			while (increm > 9) {
				s[i] = '0';
				increm = to_int(s[i + 1]) + 1;
				i++;
				if (i == length)
				{
					s = s + '0'; //add an extra place
					increm = 1;
				}
			};
		}
			s[i] = to_char(increm); 

		this->reassign_this(s.length());
		reverse(s.begin(), s.end()); //re-reverse to origin
		strcpy(digit, s.c_str());
		return tmp;
		//temp.digit is destroyed by the end of this function. I don't have to do it manually.
	}

	void into_string(char* ar, string& s, int len) //to convert digit[] into a single string. Instead of using string's constructor,
		//which would copy the garbage values present in my digit[] array. Using this function, no garbage values are copied. And I can
		//maintain a clean digit[] array.
	{
		for (int i = 0; i < len; i++)
		{
			s = s + ar[i];
		}
	}
	inf_int operator+(const inf_int& x) 
	{
		inf_int obj; //the sum of them
		int num1 = stoi(digit);
		int num2 = stoi(x.digit);
		int val = num1 + num2;
		string sval = to_string(val);
		obj.length = sval.length();
		cout << "the length is: " << obj.length << endl;
		cout << "the value is: " << val << endl;
		obj.digit = new char[obj.length];
		for (int i = 0; i < obj.length; i++)
		{
			obj.digit[i] = sval[i];
		}
		if (obj.digit[0] == '-')
			obj.sign = 0;
		else
			obj.sign = 1;

		return obj;
	}

	~inf_int() //destructor
	{
		if (digit != nullptr) {
			delete[] digit;
		digit = nullptr;
		}
		objcount--;
		id = objcount;
	}

};

//remark:
//storing a number in a character array as a string is actually so much easier and gives a better output
//easy because: splitting the numbers is done simply by going through the string using array notation 
//better because: the resulting digits are automatically arranged from left to right
//on the other hand, splitting an integer's digits requires a long process of dividing by 10 and taking the remainder, and doing
//so through a loop whose end condition we need to calculate as well (the number of digits)
//and eventually, the digits that result are not even ordered and they need another algorithm to rearrange them!