#include "phonebook.h"
#include "quicksort.h"
#include "return_codes.h"
#include <fstream>
#include <vector>

template< typename T >
void printer(ifstream &in, int size, vector< T > &a, ofstream &out)
{
	for (int i = 0; i < size; ++i)
	{
		out << a[i] << "\n";
	}
	a.clear();
	in.close();
	out.close();
}

using namespace std;
int main(int argc, char *argv[])
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
	string arg, modification;
	int size;
	in >> arg;
	in >> modification;
	in >> size;
	if (arg == "int")
	{
		vector< int > a;
		try
		{
			a.resize(size);
		} catch (bad_alloc &error)
		{
			cerr << "Not enough memory\n";
			a.clear();
			in.close();
			return ERROR_NOT_ENOUGH_MEMORY;
		}
		for (int i = 0; i < size; ++i)
		{
			int x;
			in >> x;
			a[i] = x;
		}
		if (modification == "descending")
		{
			quickSort< int, true >(a, 0, size - 1);
		}
		else if (modification == "ascending")
		{
			quickSort< int, false >(a, 0, size - 1);
		}

		ofstream out;
		out.open(argv[2]);
		if (!out)
		{
			cerr << "Can't open file\n";
			in.close();
			a.clear();
			return ERROR_FILE_NOT_FOUND;
		}
		printer(in, size, a, out);
		return ERROR_SUCCESS;
	}
	else if (arg == "float")
	{
		vector< float > a;
		try
		{
			a.resize(size);
		} catch (bad_alloc &error)
		{
			cerr << "Not enough memory\n";
			a.clear();
			in.close();
			return ERROR_NOT_ENOUGH_MEMORY;
		}
		for (int i = 0; i < size; ++i)
		{
			float x;
			in >> x;
			a[i] = x;
		}
		if (modification == "descending")
		{
			quickSort< float, true >(a, 0, size - 1);
		}
		else if (modification == "ascending")
		{
			quickSort< float, false >(a, 0, size - 1);
		}
		ofstream out;
		out.open(argv[2]);
		if (!out)
		{
			cerr << "Can't open file\n";
			in.close();
			a.clear();
			return ERROR_FILE_NOT_FOUND;
		}
		printer(in, size, a, out);
		return ERROR_SUCCESS;
	}
	else if (arg == "phonebook")
	{
		vector< Phonebook > a;
		try
		{
			a.resize(size);
		} catch (bad_alloc &error)
		{
			cerr << "Not enough memory\n";
			a.clear();
			in.close();
			return ERROR_NOT_ENOUGH_MEMORY;
		}
		for (int i = 0; i < size; ++i)
		{
			{
				Phonebook x;
				in >> x;
				a[i] = x;
			}
		}
		if (modification == "descending")
		{
			quickSort< Phonebook, true >(a, 0, size - 1);
		}
		else if (modification == "ascending")
		{
			quickSort< Phonebook, false >(a, 0, size - 1);
		}
		ofstream out;
		out.open(argv[2]);
		if (!out)
		{
			cerr << "Can't open file\n";
			in.close();
			a.clear();
			return ERROR_FILE_NOT_FOUND;
		}
		printer(in, size, a, out);
		return ERROR_SUCCESS;
	}
}
