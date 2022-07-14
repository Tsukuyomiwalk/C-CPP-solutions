#include <fstream>
#include <iostream>
#include <vector>
using namespace std;
template< typename T, bool descending >
void quickSort(vector< T > &array, int left, int right)
{
	while (left < right)
	{
		T pivot = array[(left + right) / 2];
		int i = left;
		int j = right;
		if (!descending)
		{
			while (i <= j)
			{
				while (array[i] < pivot)
				{
					i++;
				}
				while (array[j] > pivot)
				{
					j--;
				}
				if (i >= j)
				{
					break;
				}
				swap(array[i++], array[j--]);
			}
		}
		else
		{
			while (i <= j)
			{
				while (array[i] > pivot)
				{
					i++;
				}
				while (array[j] < pivot)
				{
					j--;
				}
				if (i >= j)
				{
					break;
				}
				swap(array[i++], array[j--]);
			}
		}
		if (j - left < right - j)
		{
			quickSort< T, descending >(array, left, j);
			left = j + 1;
		}
		else
		{
			quickSort< T, descending >(array, j, right);
			right = j - 1;
		}
	}
}
