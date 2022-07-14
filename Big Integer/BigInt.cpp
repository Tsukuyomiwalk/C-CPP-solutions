#include "BigInt.h"

#include <string_view>

#include <cmath>
#include <cstring>
#include <iomanip>
using namespace std;

BigInt::BigInt(const char* str)
{
	if (!memcmp(str, "NaN", 3))
	{
		Nan = true;
	}
	size_t len = 0;
	for (const char* ch = str; *ch; ch++)
	{
		len++;
	}
	if (len == 0)
	{
		return;
	}
	if (str[0] == '-')
	{
		negate = true;
	}
	if ((str[0] == '0' && len == 1) || (memcmp(str, "-0", 2) == 0 && len == 2))
	{
		zero = true;
		return;
	}

	long long x = 0;
	skipO(str, x);

	if (len > 0)
	{
		for (long long i = len - 1; i >= x; i -= 9)
		{
			char temp[10];
			if (i < 9)
			{
				long long shift = i - x + 1;
				memcpy(temp, str + x, shift);
				temp[shift] = '\0';
				try
				{
					digits.push_back(atoll(temp));
				} catch (bad_alloc&)
				{
					cerr << "Not enough memory\n";
					digits.clear();
					throw errc::not_enough_memory;
				}
			}
			else
			{
				memcpy(temp, str + (i - 8), 9);
				temp[9] = '\0';
				try
				{
					digits.push_back(atoll(temp));
				} catch (bad_alloc&)
				{
					cerr << "Not enough memory\n";
					digits.clear();
					throw errc::not_enough_memory;
				}
			}
		}
	}
}
BigInt::BigInt(int i)
{
	zero = i == 0;
	negate = i < 0;
	if (negate)
	{
		try
		{
			digits.push_back(abs(i) % BigInt::BAZA);
		} catch (bad_alloc& error)
		{
			cerr << "Not enough memory\n";
			digits.clear();
			throw errc::not_enough_memory;
		}
	}
	else
	{
		digits.push_back(i % BigInt::BAZA);
	}
	i /= BigInt::BAZA;
	if (abs(i) != 0)
	{
		if (!negate)
		{
			try
			{
				digits.push_back(i);
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				digits.clear();
				throw errc::not_enough_memory;
			}
		}
		else
		{
			try
			{
				digits.push_back(abs(i));
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				digits.clear();
				throw errc::not_enough_memory;
			}
		}
	}
}
BigInt::BigInt(string_view str)
{
	if (str == "NaN")
	{
		Nan = true;
		return;
	}

	if (str[0] == '-')
	{
		negate = true;
	}
	if (str.length() == 0)
	{
		return;
	}
	if (str[0] == '-')
	{
		negate = true;
	}
	if ((str == "0" && str.length() == 1) || (str == "-0" && str.length() == 2))
	{
		zero = true;
		return;
	}

	long long x = 0;
	skipO(str.data(), x);
	if (str.length() > 0)
	{
		for (long long i = str.length() - 1; i >= x; i -= 9)
		{
			char temp[10];
			if (i < 9)
			{
				long long shift = i - x + 1;
				memcpy(temp, &str[x], shift);
				temp[shift] = '\0';
				try
				{
					digits.push_back(atoll(temp));
				} catch (bad_alloc&)
				{
					cerr << "Not enough memory\n";
					digits.clear();
					throw errc::not_enough_memory;
				}
			}
			else
			{
				memcpy(temp, &str[i - 8], 9);
				temp[9] = 0;
				try
				{
					digits.push_back(atoll(temp));
				} catch (bad_alloc&)
				{
					cerr << "Not enough memory\n";
					digits.clear();
					throw errc::not_enough_memory;
				}
			}
		}
	}
}

void BigInt::cut()
{
	while (digits.size() > 1 && digits.back() == 0)
	{
		digits.pop_back();
	}

	if (digits.size() == 1 && digits[0] == 0)
	{
		negate = false;
		zero = true;
	}
}

bool BigInt::operator==(const BigInt& a) const
{
	if (a.Nan || this->Nan)
	{
		return 0;
	}
	else
	{
		return comparator(a) == 0;
	}
}

BigInt BigInt::operator-() const
{
	BigInt a(*this);

	if (a.Nan)
	{
		return { "NaN" };
	}
	else if (a == BigInt(0))
	{
		return a;
	}
	else
	{
		a.negate = !a.negate;
		return a;
	}
}

bool BigInt::operator<(const BigInt& a) const
{
	if (a.Nan || this->Nan)
	{
		return 0;
	}
	else
	{
		return comparator(a) == -1;
	}
}

bool BigInt::operator!=(const BigInt& a) const
{
	if (a.Nan || this->Nan)
	{
		return 1;
	}
	else
	{
		return comparator(a) != 0;
	}
}

bool BigInt::operator<=(const BigInt& a) const
{
	if (a.Nan || this->Nan)
	{
		return 0;
	}
	else
	{
		return comparator(a) <= 0;
	}
}

bool BigInt::operator>(const BigInt& a) const
{
	if (a.Nan || this->Nan)
	{
		return 0;
	}
	else
	{
		return comparator(a) == 1;
	}
}

bool BigInt::operator>=(const BigInt& a) const
{
	if (a.Nan || this->Nan)
	{
		return 0;
	}
	else
	{
		return comparator(a) >= 0;
	}
}

BigInt operator+(const BigInt& a, const BigInt& b)
{
	if (a.Nan || b.Nan)
	{
		return "NaN";
	}
	if (a.zero || a == 0)
	{
		return b;
	}
	if (b.zero || b == 0)
	{
		return a;
	}

	if (a.negate)
	{
		if (b.negate)
		{
			return -(-a + (-b));
		}
		else
		{
			return b - (-a);
		}
	}
	if (b.negate)
	{
		return a - (-b);
	}
	vector< long long > res;
	try
	{
		res.resize(std::max(a.digits.size(), b.digits.size()), 0);
	} catch (bad_alloc& error)
	{
		cerr << "Not enough memory\n";
		res.clear();
		throw errc::not_enough_memory;
	}
	for (size_t i = 0; i < std::min(a.digits.size(), b.digits.size()); i++)
	{
		res[i] += a.digits[i] + b.digits[i];
		if (res[i] >= BigInt::BAZA)
		{
			res[i + 1] = res[i] / BigInt::BAZA;
			res[i] = res[i] % BigInt::BAZA;
		}
	}
	for (size_t i = std::min(a.digits.size(), b.digits.size()); i < std::max(a.digits.size(), b.digits.size()); i++)
	{
		if (a.digits.size() < b.digits.size())
		{
			res[i] += b.digits[i];
		}
		else
			res[i] += a.digits[i];
		if (res[i] >= BigInt::BAZA)
		{
			res[i + 1] = res[i] / BigInt::BAZA;
			res[i] = res[i] % BigInt::BAZA;
		}
	}
	BigInt c;
	try
	{
		c.digits = res;
	} catch (bad_alloc& error)
	{
		cerr << "Not enough memory\n";
		c.digits.clear();
		res.clear();
		throw errc::not_enough_memory;
	}
	return c;
}

BigInt& BigInt::operator+=(const BigInt& value)
{
	return *this = *this + value;
}

BigInt operator-(const BigInt& a, const BigInt& b)
{
	if (a.Nan || b.Nan)
	{
		return { "NaN" };
	}
	if (a.zero)
	{
		return -b;
	}
	if (b.zero)
	{
		return a;
	}
	if (a < b)
	{
		return -(b - a);
	}
	if (b.negate)
	{
		return a + (-b);
	}
	if (a.negate)
	{
		return -(-a + b);
	}
	if (a == b)
	{
		return BigInt("0");
	}
	BigInt c;
	try
	{
		c = a;
	} catch (bad_alloc& error)
	{
		cerr << "Not enough memory\n";
		c.digits.clear();
		throw errc::not_enough_memory;
	}
	int carry = 0;
	for (size_t i = 0; i < b.digits.size() || carry; ++i)
	{
		if (i < b.digits.size())
		{
			c.digits[i] -= b.digits[i] + carry;
		}
		else
		{
			c.digits[i] -= carry;
		}
		carry = c.digits[i] < 0;
		if (carry != 0)
			c.digits[i] += BigInt::BAZA;
	}
	c.cut();
	return c;
}

BigInt& BigInt::operator-=(const BigInt& value)
{
	return *this = *this - value;
}

BigInt& BigInt::operator*=(const BigInt& value)
{
	return *this = *this * value;
}
BigInt& BigInt::operator%=(const BigInt& value)
{
	return *this = *this % value;
}

void BigInt::shift_right()
{
	if (digits.empty())
	{
		try
		{
			digits.push_back(0);
		} catch (bad_alloc& error)
		{
			cerr << "Not enough memory\n";
			digits.clear();
			throw errc::not_enough_memory;
		}
	}
	else
	{
		*this = *this * BigInt::BAZA;
	}
}
BigInt::BigInt(long long l)
{
	if (l == 0ll)
	{
		zero = true;
	}
	negate = l < 0;
	if (!negate)
	{
		try
		{
			digits.push_back(l % BigInt::BAZA);
		} catch (bad_alloc& error)
		{
			cerr << "Not enough memory\n";
			digits.clear();
			throw errc::not_enough_memory;
		}
	}
	else
	{
		try
		{
			digits.push_back(abs(l) % BigInt::BAZA);
		} catch (bad_alloc& error)
		{
			cerr << "Not enough memory\n";
			digits.clear();
			throw errc::not_enough_memory;
		}
	}
	l /= BigInt::BAZA;
	if (abs(l) != 0)
		try
		{
			digits.push_back(negate ? abs(l) : l);
		} catch (bad_alloc& error)
		{
			cerr << "Not enough memory\n";
			digits.clear();
			throw errc::not_enough_memory;
		}
}
BigInt operator/(const BigInt& a, const BigInt& b)
{
	if ((b.zero && a.zero) || (a == 0 && b == 0))
	{
		return { "NaN" };
	}
	if (b.zero || b == 0)
	{
		return "NaN";
	}
	if (a.Nan || b.Nan)
	{
		return { "NaN" };
	}
	if (a.zero)
	{
		return BigInt("0");
	}
	if (b == a)
	{
		return { "1" };
	}
	BigInt db;
	try
	{
		db = b;
	} catch (bad_alloc& error)
	{
		cerr << "Not enough memory\n";
		db.digits.clear();
		throw errc::not_enough_memory;
	}
	BigInt da;
	try
	{
		da = a;
	} catch (bad_alloc& error)
	{
		cerr << "Not enough memory\n";
		da.digits.clear();
		throw errc::not_enough_memory;
	}
	da.negate = db.negate = false;
	BigInt c, c1;
	if (da < db)
	{
		return BigInt("0");
	}
	c1.zero = c.zero = false;
	c1.negate = a.negate != b.negate;
	try
	{
		c1.digits.resize(a.digits.size());
	} catch (bad_alloc& error)
	{
		cerr << "Not enough memory\n";
		c1.digits.clear();
		throw errc::not_enough_memory;
	}
	for (long long i = a.digits.size() - 1; i >= 0; --i)
	{
		c.shift_right();
		c.digits[0] = a.digits[i];
		c.zero = false;
		c.cut();
		long long x = 0;
		long long l = 0;
		long long r = BigInt::BAZA;
		while (l <= r)
		{
			long long m = (l + r) / 2;
			BigInt t = db * m;
			if (t <= c)
			{
				x = m;
				l = m + 1;
			}
			else
			{
				r = m - 1;
			}
		}
		c1.digits[i] = x;
		c -= db * x;
	}
	c1.cut();
	return c1;
}
BigInt operator%(const BigInt& a, const BigInt& b)
{
	if (a.Nan || b.Nan || b.zero)
	{
		return { "NaN" };
	}
	if (b == 1 && !a.negate && a != b)
	{
		return a;
	}
	if (b == 1 && a.negate)
	{
		return BigInt("0");
	}
	if (a.zero)
	{
		return { "0" };
	}
	BigInt res = a - ((a / b) * b);
	res.cut();
	return res;
}

BigInt& BigInt::operator/=(const BigInt& value)
{
	return *this = *this / value;
}

BigInt operator*(const BigInt& a, const BigInt& b)
{
	if (a.Nan || b.Nan)
	{
		return "NaN";
	}
	if (a.zero || b.zero || a == 0 || b == 0)
	{
		return BigInt("0");
	}
	if (a == 1)
	{
		return b;
	}
	if (b == 1)
	{
		return a;
	}

	BigInt c;
	c.zero = false;
	try
	{
		c.digits.resize(a.digits.size() + b.digits.size() + 1, 0);
	} catch (bad_alloc& error)
	{
		cerr << "Not enough memory\n";
		c.digits.clear();
		throw errc::not_enough_memory;
	}
	c.negate = a.negate != b.negate;
	for (long long i = 0; i < a.digits.size(); ++i)
	{
		long long carry = 0;
		for (long long j = 0; carry ? j < b.digits.size() || 1 : j < b.digits.size(); ++j)
		{
			long long cur;
			if (j < b.digits.size())
			{
				cur = c.digits[i + j] + (a.digits[i] * b.digits[j]) + carry;
			}
			else
			{
				cur = c.digits[i + j] + carry;
			}
			c.digits[i + j] = cur % BigInt::BAZA;
			carry = cur / BigInt::BAZA;
		}
	}
	c.cut();
	return c;
}

int BigInt::comparator(const BigInt& a) const
{
	if (a.Nan && Nan)
	{
		return 1;
	}

	if (a.Nan && !Nan)
	{
		return -1;
	}
	bool check;
	check = !(!negate || !a.negate);
	if (a.zero && zero)
	{
		return 0;
	}
	if (!a.negate && negate)
	{
		return -1;
	}

	if (a.negate && !negate)
	{
		return 1;
	}

	if (digits.size() > a.digits.size())
	{
		if (check)
		{
			return -1;
		}
		else
		{
			return 1;
		}
	}

	if (digits.size() < a.digits.size())
	{
		if (check)
		{
			return 1;
		}
		else
		{
			return -1;
		}
	}

	for (long long i = digits.size() - 1; i >= 0; i--)
	{
		if (digits[i] > a.digits[i])
		{
			if (check)
			{
				return -1;
			}
			else
			{
				return 1;
			}
		}
		if (digits[i] < a.digits[i])
		{
			if (check)
			{
				return 1;
			}
			else
			{
				return -1;
			}
		}
	}
	return 0;
}
BigInt::BigInt()
{
	negate = false;
	Nan = false;
}
ostream& operator<<(ostream& os, const BigInt& l)
{
	long long size = l.digits.size();
	if (l.Nan)
	{
		os << "NaN";
		return os;
	}
	else
	{
		if (l.digits.empty())
		{
			os << 0;
			return os;
		}
		else
		{
			if (l.negate)
			{
				os << '-';
			}
			os << l.digits.back();
			os.fill('0');
			for (long long i = size - 2; i >= 0; --i)
			{
				os << setw(9) << l.digits[i];
			}
		}
	}
	return os;
}

void BigInt::skipO(const char* str, long long& x)
{
	while (str[x] == '0' || str[x] == '-')
	{
		x++;
	}
}

BigInt operator~(const BigInt& a)
{
	if (a.Nan)
	{
		return { "NaN" };
	}
	if (a.negate && !a.zero)
	{
		return { "NaN" };
	}
	if (a == BigInt("1"))
	{
		return { a };
	}
	if (a.zero || a == 0)
	{
		return { "0" };
	}
	BigInt root = BigInt("0");
	BigInt r = a;
	BigInt n = a;
	BigInt l = 0;
	while (l <= r)
	{
		BigInt m = (l + r) / BigInt("2");
		if (m * m <= n)
		{
			root = m;
			l = m + BigInt("1");
		}
		else
		{
			r = m - BigInt("1");
		}
	}
	return root;
}

bool BigInt::Bool() const
{
	return !zero;
}

BigInt ::BigInt(BigInt const & toA)
{
	if (this != &toA)
	{
		negate = toA.negate;
		zero = toA.zero;
		Nan = toA.Nan;
		digits = toA.digits;
	}
}
BigInt::BigInt(BigInt&& toA) noexcept
{
	if (this != &toA)
	{
		negate = toA.negate;
		zero = toA.zero;
		Nan = toA.Nan;
		digits = toA.digits;
		toA.digits.clear();
	}
}
BigInt& BigInt::operator=(BigInt&& toA) noexcept
{
	if (this != &toA)
	{
		negate = toA.negate;
		zero = toA.zero;
		Nan = toA.Nan;
		digits = toA.digits;
		toA.digits.clear();
	}
	return *this;
}
BigInt& BigInt::operator=(const BigInt& toA)
{
	if (this != &toA)
	{
		negate = toA.negate;
		zero = toA.zero;
		Nan = toA.Nan;
		digits = toA.digits;
	}
	return *this;
}
BigInt operator""_BigInt(const char* str)
{
	return BigInt(str);
}
long long BigInt::toLongLong()
{
	if (digits.size() >= 3)
	{
		throw invalid_argument("this number can't be converted to long long");
	}
	else if (Nan)
	{
		throw invalid_argument("this number it's NaN and can't be converted to long long");
	}
	else if (zero)
	{
		return 0ll;
	}
	else
	{
		long long sum = 0;
		for (long long i = 0; i < digits.size(); ++i)
		{
			sum += digits[i] * pow(BigInt::BAZA, i);
		}
		return negate ? -sum : sum;
	}
}