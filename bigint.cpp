#include "bigint.h"
#include <iostream>
#include <string>
#include <climits>
#include <cmath>
using namespace std;

//******************************************************************
//START OF THE CONSTRUCTORS SECTION
//******************************************************************

/*
// Create a default BigInt with base 10.
*/
BigInt::BigInt(){
  base = 10;
  isPositive = true;
}

/*
// Create a BigInt with a specified base.
*/
BigInt::BigInt(int setbase){
    if(setbase < 2 || setbase > 36){
        throw InvalidBaseException();
    }
    base = setbase;
    isPositive = true;
}

/*
// Destructure
*/
BigInt::~BigInt(){}

/*
//  Create a BigInt from string.
*/
BigInt::BigInt(const string &s, int setbase){
    if(setbase < 2 || setbase > 36){
        throw InvalidBaseException();
    }
    base = setbase;
    for (unsigned int i = s.length(); i > 0; i--) {
        if (s[i - 1] == '-') {
            isPositive = false;
            continue;
        }
        int num = (int)(s[i - 1]);
        if (num < 65) {
            vec.push_back(num - 48);
        }
        else {
            vec.push_back(num - 55);
        }
    }
}


/*
//  Create a BigInt from int (base 10).
//  - Convert the int (base 10) input into a specified base (setbase).
*/
BigInt::BigInt(int input,int setbase){
    if(setbase < 2 || setbase > 36){
        throw InvalidBaseException();
    }
    base = setbase;
    if (input < 0) {
        isPositive = false;
        input *= -1;
    }
    while (input > 0) {
        int val = input % setbase;
        vec.push_back(val);
        input /= setbase;
    }

}

/*
//  Create a BigInt from another BigInt.
//  This is a copy constructor.
*/
BigInt::BigInt(const BigInt &b){

    (*this) = b;

}

/*
//  Assignment operator. (i.e., BigInt a = b;)
//  OVERRIDING a's data with BigInt b's data
//       - vec
//       - isPositive
//       - base
*/
BigInt & BigInt::operator = (const BigInt &b){

    vec.resize(b.vec.size());
    for (unsigned int i = 0; i < b.vec.size(); i++) {
        vec[i] = b.vec[i];
    }
    if (isPositive != b.isPositive) {
        isPositive = !isPositive;
    }
    base = b.base;


    return *this; 
}


//******************************************************************
//END OF THE CONSTRUCTORS SECTION
//******************************************************************

//******************************************************************
//START OF THE DISPLAYS SECTION
//******************************************************************

string BigInt::to_string(){

    string word = "";
    if (!isPositive) {
        word += '-';
    }
    char c;
    for (unsigned int i = vec.size(); i > 0; i--) {
        int n = vec[i - 1];
        if (n > 9) {
            c = (char)(vec[i - 1] + 55);
        }
        else {
            c = (char)(vec[i - 1] + 48);
        }
        word += c;
    }
    if (vec.size() == 0) {
      word += "0";
    }
    return word;
}

int BigInt::to_int() const{

    long long sum = 0;
    for (unsigned int i = 0; i < vec.size(); i++) {
      if (isPositive) {
        sum += pow(base, i) * vec[i];
        if (sum >= (long long)(INT_MAX)) {
          return INT_MAX;
         }
      }
      else {
        sum -= pow(base, i) * vec[i];
        if (sum < (long long)(INT_MIN)) {
          return INT_MIN;
        }
      }
    }
  return (int)(sum);
}

//******************************************************************
//END OF THE DISPLAYS SECTION
//******************************************************************

//******************************************************************
//START OF THE EQUALITY CHECK OPERATORS SECTION
//******************************************************************

int BigInt::compare(const BigInt &b) const{
    if(base != b.base){
        throw DiffBaseException();
    }

    //If ALL digits are the same, then they MUST be equal!!
    if (isPositive != b.isPositive) {
        if (isPositive) {
            return 1;
        }
        return -1;
    }

    if (vec.size() > b.vec.size()) {
        if (isPositive) return 1;
        return -1;
    }
    else if (b.vec.size() > vec.size()) {
        if (isPositive) return -1;
        return 1;
    }

    for (unsigned int i = vec.size(); i > 0; i--) {
        if (vec[i - 1] > b.vec[i - 1]) {
            if (isPositive) return 1;
            return -1;
        }
        else if (b.vec[i - 1] > vec[i - 1]) {
            if (isPositive) return -1;
            return 1;
        }
    }
    return 0;
}


//Call the compare function above to check if a == b
bool operator == (const BigInt &a, const BigInt &b){
  
  return (a.compare(b) == 0);
  
}

//Call the compare function above to check if a != b
bool operator != (const BigInt &a, const BigInt &b){
  
  if (a.compare(b) == 0) {
    return false;
  }
  return true;
}

//Call the compare function above to check if a <= b
bool operator <= (const BigInt &a, const BigInt &b){
  
  return (a.compare(b) == -1 || a.compare(b) == 0);
}

//Call the compare function above to check if a >= b
bool operator >= (const BigInt &a, const BigInt &b){
  return (a.compare(b) == 1 || a.compare(b) == 0);
}

//Call the compare function above to check if a > b
bool operator > (const BigInt &a, const BigInt &b){
  
  return (a.compare(b) == 1);
}

//Call the compare function above to check if a < b
bool operator < (const BigInt &a, const BigInt &b){
  
  return (a.compare(b) == -1);
}

//******************************************************************
//END OF THE EQUALITY CHECK OPERATORS SECTION
//******************************************************************

//******************************************************************
//START OF THE ARITHMETIC OPERATORS SECTION
//******************************************************************

/*
//======================
// NON-MEMBER function
//======================
// 1. Create a copy of a
// 2. Call += operator on the copy and b
// 3. Return the copy
*/
BigInt operator + (const  BigInt &a, const BigInt & b){

  BigInt c = a;
  c += b;
  return c;
}

/*
//==================
// MEMBER function
//==================
//  Addition assignment operator.
//    i.e., a += b
*/
const BigInt & BigInt::operator += (const BigInt &b){
    if(base != b.base){
        throw DiffBaseException();
    }
    BigInt copy;
    if (isPositive && !b.isPositive) {
      copy = b;
      copy.isPositive = true;
      (*this) -= copy;
      return *this;
    }
    else if (!isPositive && b.isPositive) {
      copy = b;
      isPositive = true;
      copy -= *this;
      *this = copy;
      return *this;
    }

    int val_a = 0, val_b = 0, result;
    for (unsigned int i = 0; i < max(vec.size(), b.vec.size()); i++) {
      if (i < vec.size()) {
        val_a = vec[i];
      }
      else {
        val_a = 0;
      }

      if (i < b.vec.size()) {
        val_b = b.vec[i];
      }
      else {
        val_b = 0;
      }

      result = val_a + val_b;
      if (i < vec.size()) {
        vec[i] = result % base;
      }
      else {
        vec.push_back(result % base);
      }
      
      result /= base;
      if (i + 1 == max(vec.size(), b.vec.size()) && result > 0) {
        vec.push_back(result);
        break;
      }
      else if (i + 1 != max(vec.size(), b.vec.size())) {
        if (i + 1< vec.size()) {
        vec[i+1] += result;
      }
        else {
          vec.push_back(result);
        }
      }
    }

  return *this;
}

/*
//======================
// NON-MEMBER function
//======================
// 1. Create a copy of a
// 2. Call -= operator on the copy and b
// 3. Return the copy
*/
BigInt operator - (const  BigInt &a, const BigInt & b){

  BigInt c;
  c = a;
  c -= b;
  return c;

}


/*
//==================
// MEMBER function
//==================
//  Subtraction assignment operator.
//    i.e., a -= b
*/
const BigInt & BigInt::operator -= (const BigInt &b){
    if(base != b.base){
        throw DiffBaseException();
    }
    bool flag = true;
    BigInt copy;
    BigInt small;
    if (isPositive && b.isPositive) {
      if (*this < b) {
        copy = b;
        small = *this;
        flag = false;
      }
      else if (b < *this){
        copy = *this;
        small = b;
      }
      else {
        vec[0] = 0;
        vec.resize(1);
        return *this;
      }
    }
    else if (!isPositive && !b.isPositive) {
      if (*this < b) {
        copy = *this;
        small = b;
        flag = false;
      }
      else {
        copy = b;
        small = *this;
      }
    }
    else if (isPositive && !b.isPositive) {
      copy = b;
      copy.isPositive = true;
      *this += copy;
      return *this;
    }
    else if (!isPositive && b.isPositive) {
      copy = b;
      copy.isPositive = false;
      *this += copy;
      return *this;
    }

    int val_a = 0, val_b = 0, result;
    for (unsigned int i = 0; i < max(copy.vec.size(), small.vec.size()); i++) {
      if (i < copy.vec.size()) {
        val_a = copy.vec[i];
      }
      else {
        val_a = 0;
      }

      if (i < small.vec.size()) {
        val_b = small.vec[i];
      }
      else {
        val_b = 0;
      }

      result = val_a - val_b;
      if (result < 0) {
        if (i+1 < copy.vec.size()) {
          copy.vec[i+1] -= 1;
          copy.vec[i] += base;
        }
        val_a = copy.vec[i];
        result = val_a - val_b;
      }
      copy.vec[i] = result;
    }
  int var = copy.vec.size() - 1;
  while (copy.vec[var] == 0 && var > 0) {
    copy.vec.resize(copy.vec.size() - 1);
    var -= 1;
  }

  if (flag) {
    copy.isPositive = true;
  }
  else {
    copy.isPositive = false;
  }
  *this = copy;
  return *this;
}

/*
//======================
// NON-MEMBER function
//======================
// 1. Create a copy of a
// 2. Call *= operator on the copy and b
// 3. Return the copy
*/
BigInt operator * (const  BigInt &a, const BigInt & b){

  BigInt copy;
  copy = a;
  copy *= b;
  return copy;
}



/*
//==================
// MEMBER function
//==================
//  Multiplication assignment operator.
//    i.e., a *= b
//  Implement Long Multiplication
//  
*/
const BigInt & BigInt::operator *= (const BigInt &b){
    if(base != b.base){
        throw DiffBaseException();
    }
    bool flag;
    if (isPositive != b.isPositive) {
      flag = false;
    }
    else {
      flag = true;
    }

    if (vec.size() == 1) {
      if (vec[0] == 0) {
        vec.resize(1);
        vec[0] = 0;
        isPositive = true;
        return *this;
      }
      else if (vec[0] == 1) {
        *this = b;
        if (flag) {
          isPositive = true;
        }
        else {
          isPositive = false;
        }
        return *this;
      }
    }
    if (b.vec.size() == 1) {
      if (b.vec[0] == 0) {
        vec.resize(1);
        vec[0] = 0;
        isPositive = true;
        return *this;
      }
      else if (b.vec[0] == 1) {
        if (flag) {
          isPositive = true;
        }
        else {
          isPositive = false;
        }
        return *this;
      }
    }
    BigInt total("0", base);
    int carry = 0;
    for (unsigned int i = 0; i < b.vec.size(); i++) {
      BigInt temp(base);
      for (unsigned int k = 0; k < i; k++) {
        temp.vec.push_back(0);
      }
      for (unsigned int j = 0; j < vec.size(); j++) {
        int product = vec[j] * b.vec[i];
        product += carry;
        temp.vec.push_back(product % base);
        product /= base;
        carry = product;
      }
      if (carry > 0) {
        temp.vec.push_back(carry);
      }
      carry = 0;
      total += temp;
    }

    *this = total;
    if (flag) {
      isPositive = true;
    }
    else {
      isPositive = false;
    }
    return *this;
}

/*
//======================
// NON-MEMBER function
//======================
// 1. Create a copy of a
// 2. Call /= operator on the copy and b
// 3. Return the copy
*/
BigInt operator / (const  BigInt &a, const BigInt & b){

    BigInt copy;
    copy = a;
    copy /= b;

    return copy;
}


/*
//==================
// MEMBER function
//==================
//  Division assignment operator.
//    - Call 'divisionMain' to get the quotient;
*/
const BigInt & BigInt::operator /= (const BigInt &b){
    if(base != b.base){
        throw DiffBaseException();
    }
    if(b.vec.size() == 1 && b.vec[0] == 0){
        throw DivByZeroException();
    }
    BigInt quotient(base);
    BigInt remainder(base);
    divisionMain(b, quotient, remainder);
    *this = quotient;
    if (vec.size() == 0) {
      isPositive = true;
    } 

    return *this;
}

/*
//======================
// NON-MEMBER function
//======================
// 1. Create a copy of a
// 2. Call %= operator on the copy and b
// 3. Return the copy
*/
BigInt operator % (const  BigInt &a, const BigInt & b){

  
    BigInt copy;
    copy = a;
    copy %= b;
    return copy;
}


/*
//==================
// MEMBER function
//==================
//  Modulus assignment operator.
//    - Call 'divisionMain' to get the remainder;
//    - Note: remainder takes the sign of the dividend.
*/
const BigInt & BigInt::operator %= (const BigInt &b){
    if(base != b.base){
        throw DiffBaseException();
    }
    if(b.vec.size() == 1 && b.vec[0] == 0){
        throw DivByZeroException();//divide by zero.
    }

    BigInt quotient(base);
    BigInt remainder(base);
    divisionMain(b, quotient, remainder);
    *this = remainder;
    return *this;
}

/*
//  Main function for the Division (/=) and Modulus (%=) operators.
//     - Compute (q)uotient and (r)emainder
//     - Implement Long Division
*/
void BigInt::divisionMain(const BigInt &b, BigInt &quotient, BigInt &remainder){

    bool flag1, flag2;
    if (isPositive) {
      flag1 = true;
    }
    else {
      flag1 = false;
    }
    if (b.isPositive) {
      flag2 = true;
    }
    else {
      flag2 = false;
    }

    BigInt dividend;
    BigInt divisor;
    dividend = *this;
    divisor = b;
    dividend.isPositive = true;
    divisor.isPositive = true;

    if (dividend == divisor) {
      quotient.vec.push_back(1);
      if (flag1 == flag2) {
        quotient.isPositive = true;
      }
      else {
        quotient.isPositive = false;
      }
      return;
    }

    int pos = dividend.vec.size() - 1;
    BigInt dividend_seq(base);
    dividend_seq.vec.push_back(dividend.vec[pos]);
    while (dividend_seq < divisor) {
      pos -= 1;
      if (pos >= 0) {
        dividend_seq.vec.insert(dividend_seq.vec.begin(), dividend.vec[pos]);
      }
      else {
        break;
      }
    }

    while (pos >= 0) {
      int times = 0;
      while (dividend_seq >= divisor) {
        dividend_seq -= divisor;
        times += 1;
      }

      if (quotient.vec.size() == 0) {
        quotient.vec.push_back(times);
      }
      else {
        quotient.vec.insert(quotient.vec.begin(), times);
      }
      
      if (dividend_seq.vec.size() == 1) {
        if (dividend_seq.vec[0] == 0) {
          dividend_seq.vec.resize(0);
        }
      }

      if (pos > 0) {
        dividend_seq.vec.insert(dividend_seq.vec.begin(), dividend.vec[pos - 1]);
      }
      pos -= 1;
    }
    remainder = dividend_seq;
    if (remainder.vec.size() == 0 || flag1) {
      remainder.isPositive = true;
    }
    else {
      remainder.isPositive = false;
    }

    if (flag1 == flag2) {
      quotient.isPositive = true;
    }
    else {
      quotient.isPositive = false;
    }

}



/*
//======================
// NON-MEMBER function
//======================

*/
BigInt pow(const  BigInt &a, const BigInt & b){

    BigInt copy;
    copy = a;
    copy.exponentiation(b);
    return copy;
}

/*
//==================
// MEMBER function
//==================
*/
const BigInt & BigInt::exponentiation(const BigInt &b){
    if(base != b.base){
        throw DiffBaseException();
    }
    if(!b.isPositive){
        throw ExpByNegativeException();
    }
    
    if (b.vec.size() == 1 && b.vec[0] == 0) {
      vec.resize(1);
      vec[0] = 1;
      isPositive = true;
      return *this;
    }
    BigInt comp("1", base);
    BigInt comp2("2", base);
    BigInt placeholder("1", base);
    BigInt copy;
    copy = b;
    copy.isPositive = true;
    bool flag, flag2 = false;
    if (isPositive) {
      flag = true;
    }
    else {
      flag = false;
    }
    isPositive = true;
    while (copy > comp) {
      if (copy.vec[0] % 2 == 0) {
        (*this) *= (*this);
        copy /= comp2;
      }
      else {
        flag2 = true;
        placeholder *= (*this);
        placeholder.isPositive = true;
        (*this) *= (*this);
        copy -= comp;
        copy /= comp2;
      }
    }
    if (flag2) {
      (*this) *= placeholder;
    }

    if (flag) {
      isPositive = true;
    }
    else {
      if (b.vec[0] % 2 == 0) {
        isPositive = true;
      }
      else {
        isPositive = false;
      }
    }
    return *this;
}

/*
//======================
// NON-MEMBER function
//======================
*/
BigInt modPow(const BigInt &a, const BigInt &b, const BigInt &m){

    /************* You complete *************/
    BigInt copy;
    copy = a;
    copy.modulusExp(b, m);
    return copy;
}


/*
//==================
// MEMBER function
//==================
//  Modulus Exponentiation assignment function.
//     - i.e., a.modulusExp(b)
// Note: 1. implement Modulus Exponentiation (see the writeup)
//       2. b should be treated as BigInt, not int or other data type.
// Hint: same implementation as exponentiation, but take modulus 
//          after every call to the *= operator.
*/
const BigInt & BigInt::modulusExp(const BigInt &b, const BigInt &m){
    if(base != b.base || base != m.base){
        throw DiffBaseException();
    }
    if(!b.isPositive){
        throw ExpByNegativeException();
    }
    
    if (b.vec.size() == 1 && b.vec[0] == 0) {
      vec.resize(1);
      vec[0] = 1;
      isPositive = true;
      return *this;
    }
    BigInt comp("1", base);
    BigInt comp2("2", base);
    BigInt placeholder("1", base);
    BigInt copy;
    copy = b;
    copy.isPositive = true;
    bool flag, flag2 = false;
    if (isPositive) {
      flag = true;
    }
    else {
      flag = false;
    }
    isPositive = true;
    while (copy > comp) {
      if (copy.vec[0] % 2 == 0) {
        (*this) *= (*this);
        (*this) %= m;
        copy /= comp2;
      }
      else {
        flag2 = true;
        placeholder *= (*this);
        placeholder.isPositive = true;
        (*this) *= (*this);
        (*this) %= m;
        copy -= comp;
        copy /= comp2;
      }
    }
    if (flag2) {
      (*this) *= placeholder;
      (*this) %= m;
    }

    if (flag) {
      isPositive = true;
    }
    else {
      if (b.vec[0] % 2 == 0) {
        isPositive = true;
      }
      else {
        isPositive = false;
      }
    }
    return *this;
}


void BigInt::printVecItems(){
  cout<<"[";
  for(size_t i = 0;i<vec.size();i++){
    cout<<vec[i];
    if(i < vec.size()-1){
      cout<<",";
    }
  }
  cout<<"]"<<endl;
}

bool BigInt::getSign(){
	return isPositive;
}

int BigInt::getBase(){
	return base;
}