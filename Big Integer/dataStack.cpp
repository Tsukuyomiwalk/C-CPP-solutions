#include "BigInt.h"
#include "return_codes.h"

#include <cstring>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
using namespace std;
int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		cerr << "wrong amount of arguments\n";
		return ERROR_UNKNOWN;
	}
	ifstream in;
	in.open(argv[1]);
	if (!in)
	{
		cerr << "Can't open file\n";
		return ERROR_FILE_NOT_FOUND;
	}

	stack< BigInt > stack;
	string s;
	for (string str; in >> s;)
	{
		if ((s[0] >= '0' && s[0] <= '9') || (s[0] == '-' && s[1] >= '0' && s[1] <= '9') || s == "NaN")
		{
			try
			{
				stack.push(BigInt(s));
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				size_t sz = stack.size();
				in.close();
				for (long long i = 0; i < sz; ++i)
				{
					stack.pop();
				}
				throw errc::not_enough_memory;
			}
			continue;
		}
		if (s == "+")
		{
			BigInt arg1 = stack.top();
			stack.pop();
			BigInt arg2 = stack.top();
			stack.pop();
			BigInt w = arg2 + arg1;
			try
			{
				stack.push(w);
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				size_t sz = stack.size();
				in.close();
				for (long long i = 0; i < sz; ++i)
				{
					stack.pop();
				}
				throw errc::not_enough_memory;
			}
			continue;
		}
		if (s == "~")
		{
			BigInt arg1 = stack.top();
			stack.pop();
			BigInt w = (~arg1);
			try
			{
				stack.push(w);
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				size_t sz = stack.size();
				in.close();
				for (long long i = 0; i < sz; ++i)
				{
					stack.pop();
				}
				throw errc::not_enough_memory;
			}
			continue;
		}
		if (s == "%")
		{
			BigInt arg1 = stack.top();
			stack.pop();
			BigInt arg2 = stack.top();
			stack.pop();
			BigInt w = arg2 % arg1;
			try
			{
				stack.push(w);
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				size_t sz = stack.size();
				in.close();
				for (long long i = 0; i < sz; ++i)
				{
					stack.pop();
				}
				throw errc::not_enough_memory;
			}
			continue;
		}
		if (s == "*")
		{
			BigInt arg1 = stack.top();
			stack.pop();
			BigInt arg2 = stack.top();
			stack.pop();
			BigInt w = arg2 * arg1;
			try
			{
				stack.push(w);
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				size_t sz = stack.size();
				in.close();
				for (long long i = 0; i < sz; ++i)
				{
					stack.pop();
				}
				throw errc::not_enough_memory;
			}
			continue;
		}
		if (s == "-")
		{
			BigInt arg1 = stack.top();
			stack.pop();
			BigInt arg2 = stack.top();
			stack.pop();
			BigInt w = arg2 - arg1;
			try
			{
				stack.push(w);
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				size_t sz = stack.size();
				in.close();
				for (long long i = 0; i < sz; ++i)
				{
					stack.pop();
				}
				throw errc::not_enough_memory;
			}
			continue;
		}
		if (s == "/")
		{
			BigInt arg1 = stack.top();
			stack.pop();
			BigInt arg2 = stack.top();
			stack.pop();
			BigInt w = arg2 / arg1;
			try
			{
				stack.push(w);
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				size_t sz = stack.size();
				in.close();
				for (long long i = 0; i < sz; ++i)
				{
					stack.pop();
				}
				throw errc::not_enough_memory;
			}
			continue;
		}
		if (s == "<")
		{
			BigInt arg1 = stack.top();
			stack.pop();
			BigInt arg2 = stack.top();
			stack.pop();
			BigInt m = arg2 < arg1;
			try
			{
				stack.push(m);
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				size_t sz = stack.size();
				in.close();
				for (long long i = 0; i < sz; ++i)
				{
					stack.pop();
				}
				throw errc::not_enough_memory;
			}
			continue;
		}
		if (s == ">")
		{
			BigInt arg1 = stack.top();
			stack.pop();
			BigInt arg2 = stack.top();
			stack.pop();
			stack.push(BigInt(arg2 > arg1));
			continue;
		}
		if (s == "<=")
		{
			BigInt arg1 = stack.top();
			stack.pop();
			BigInt arg2 = stack.top();
			stack.pop();
			try
			{
				stack.push(BigInt(arg2 <= arg1));
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				size_t sz = stack.size();
				in.close();
				for (long long i = 0; i < sz; ++i)
				{
					stack.pop();
				}
				throw errc::not_enough_memory;
			}
			continue;
		}
		if (s == ">=")
		{
			BigInt arg1 = stack.top();
			stack.pop();
			BigInt arg2 = stack.top();
			stack.pop();
			try
			{
				stack.push(BigInt(arg2 >= arg1));
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				size_t sz = stack.size();
				in.close();
				for (long long i = 0; i < sz; ++i)
				{
					stack.pop();
				}
				throw errc::not_enough_memory;
			}
			continue;
		}
		if (s == "==")
		{
			BigInt arg1 = stack.top();
			stack.pop();
			BigInt arg2 = stack.top();
			stack.pop();
			try
			{
				stack.push(BigInt(arg2 == arg1));
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				size_t sz = stack.size();
				in.close();
				for (long long i = 0; i < sz; ++i)
				{
					stack.pop();
				}
				throw errc::not_enough_memory;
			}
			continue;
		}
		if (s == "!=")
		{
			BigInt arg1 = stack.top();
			stack.pop();
			BigInt arg2 = stack.top();
			stack.pop();
			try
			{
				stack.push(BigInt(arg2 != arg1));
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				size_t sz = stack.size();
				in.close();
				for (long long i = 0; i < sz; ++i)
				{
					stack.pop();
				}
				throw errc::not_enough_memory;
			}
			continue;
		}
		if (s == "_")
		{
			BigInt arg1 = stack.top();
			stack.pop();
			arg1 = -arg1;
			try
			{
				stack.push(arg1);
			} catch (bad_alloc& error)
			{
				cerr << "Not enough memory\n";
				size_t sz = stack.size();
				in.close();
				for (long long i = 0; i < sz; ++i)
				{
					stack.pop();
				}
				throw errc::not_enough_memory;
			}
			continue;
		}
	}
	ofstream out;
	out.open(argv[2]);
	if (!out)
	{
		cerr << "Can't open file\n";
		in.close();
		return ERROR_FILE_NOT_FOUND;
	}
	size_t len = stack.size();
	for (int i = 0; i < len; ++i)
	{
		BigInt out1 = stack.top();
		stack.pop();
		out << out1 << "\n";
	}
	in.close();
	out.close();
	return ERROR_SUCCESS;
}
