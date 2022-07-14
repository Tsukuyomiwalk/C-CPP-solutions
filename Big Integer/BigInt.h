#include <iostream>
#include <string>
#include <vector>
using namespace std;

class BigInt
{
  private:
	void shift_right();
	static void skipO(const char* str, long long& x);
	int comparator(const BigInt& a) const;
	void cut();

  public:
	BigInt(int i);
	BigInt(long long l);
	BigInt();
	BigInt(const char* str);
	BigInt(string_view str);
	BigInt(BigInt const & toA);
	BigInt(BigInt&& toA) noexcept;
	static const int BAZA = 1000000000;
	vector< long long > digits;
	bool negate{};
	bool zero{};
	bool Nan{};
	bool Bool() const;

	long long toLongLong();

	bool operator<(const BigInt& a) const;
	bool operator<=(const BigInt& a) const;
	bool operator>(const BigInt& a) const;
	bool operator>=(const BigInt& a) const;
	bool operator==(const BigInt& a) const;
	bool operator!=(const BigInt& a) const;

	BigInt operator-() const;

	friend BigInt operator+(const BigInt& a, const BigInt& b);
	friend BigInt operator*(const BigInt& a, const BigInt& b);
	friend BigInt operator-(const BigInt& a, const BigInt& b);
	friend BigInt operator/(const BigInt& a, const BigInt& b);
	friend BigInt operator%(const BigInt& a, const BigInt& b);
	friend BigInt operator~(const BigInt& a);

	BigInt& operator+=(const BigInt& value);
	BigInt& operator-=(const BigInt& value);
	BigInt& operator*=(const BigInt& value);
	BigInt& operator/=(const BigInt& value);
	BigInt& operator%=(const BigInt& value);
	BigInt& operator=(BigInt&& toA) noexcept;
	BigInt& operator=(const BigInt& toA);
	friend BigInt operator""_BigInt(const char* a);
};
ostream& operator<<(std::ostream& os, const BigInt& l);
