#include "return_codes.h"

#include <malloc.h>
#include <math.h>
#include <stdio.h>
int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Unknown error!");
		return ERROR_UNKNOWN;
	}
	FILE *in = fopen(argv[1], "r");

	if (!in)
	{
		fprintf(stderr, "Can't open a file 'in' ");
		return ERROR_FILE_NOT_FOUND;
	}

	int N;
	int countSwap = 0;
	float *rowCol, *rowSwap;
	float rowColB, rowSwapB;
	float EPS = pow(100000000, -1);
	fscanf(in, "%i", &N);

	if (!N)
	{
		fprintf(stderr, "You have an Invalid parameter");
		fclose(in);
		return ERROR_INVALID_PARAMETER;
	}
	float *B = malloc(N * sizeof(float));

	if (!B)
	{
		fprintf(stderr, "You haven't got enough memory");
		fclose(in);
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	float **matrix = malloc(N * sizeof(float));

	if (!matrix)
	{
		fprintf(stderr, "You haven't got enough memory");
		free(B);
		fclose(in);
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	for (int i = 0; i < N; ++i)
	{
		matrix[i] = malloc((N + 1) * sizeof(float));
		if (!matrix[i])
		{
			fprintf(stderr, "You haven't got enough memory");
			free(B);
			for (int j = 0; j < i; ++j)
			{
				free(matrix[j]);
			}
			free(matrix);
			fclose(in);
			return ERROR_NOT_ENOUGH_MEMORY;
		}
		for (int j = 0; j < N + 1; ++j)
		{
			fscanf(in, "%f", &matrix[i][j]);
		}
		B[i] = matrix[i][N];
	}

	for (int col = 0; col < N - 1; ++col)
	{
		int swapPotential = col;
		int check = swapPotential;
		for (int i = col + 1; i < N; ++i)
		{
			if (matrix[i][col] > matrix[swapPotential][col])
			{
				swapPotential = i;
			}
		}

		rowColB = B[col];
		rowSwapB = B[swapPotential];
		rowCol = matrix[col];
		rowSwap = matrix[swapPotential];

		if (check != swapPotential)
		{
			float *temp1 = rowCol;
			rowCol = rowSwap;
			rowSwap = temp1;
			float temp = rowColB;
			rowColB = rowSwapB;
			rowSwapB = temp;
			countSwap++;
		}
		B[col] = rowColB;
		B[swapPotential] = rowSwapB;
		matrix[col] = rowCol;
		matrix[swapPotential] = rowSwap;

		for (int i = col + 1; i < N; ++i)
		{
			if (fabsf(matrix[col][col]) < EPS)
			{
				continue;
			}
			else
			{
				double coeff = matrix[i][col] / matrix[col][col];
				for (int j = col; j < N; ++j)
				{
					matrix[i][j] -= coeff * matrix[col][j];
				}
				B[i] -= coeff * B[col];
			}
		}
	}
	float *result = malloc(N * sizeof(float));	  // 4
	if (!result)
	{
		fprintf(stderr, "You haven't got enough memory");
		free(B);
		for (int j = 0; j < N; ++j)
		{
			free(matrix[j]);
		}
		free(matrix);
		fclose(in);
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	for (int i = 0; i < N; ++i)
	{
		result[i] = EPS;
	}
	for (int i = N - 1; i >= 0; --i)
	{
		for (int j = i; j < N; ++j)
		{
			B[i] -= matrix[i][j] * result[j];
		}
		if (fabsf(matrix[i][i]) < EPS)
		{
			continue;
		}
		float res = B[i] / matrix[i][i];
		result[i] = res;
	}
	int rankA = 0;
	int rankB = 0;
	int flag = 0;
	for (int i = 0; i < N; ++i)
	{
		int counter = 0;
		int counter2 = 0;
		for (int j = 0; j < N; ++j)
		{
			if (fabsf(matrix[i][j]) > EPS)
			{
				counter++;
			}
			if (fabsf(matrix[j][i]) > EPS)
			{
				counter2++;
			}
		}
		if (counter2 == 0)
		{
			rankA--;
		}
		if (counter2 == 0 && fabsf(B[i]) > EPS && i == 2)
		{
			flag = 1;
		}
		if (counter > 0 && fabsf(B[i]) < EPS)
		{
			rankA++;
			rankB++;
		}
		else if (fabsf(B[i]) > EPS && counter == 0)
		{
			rankB++;
			rankA--;
		}
		else if (fabsf(B[i]) > EPS && counter > 0)
		{
			rankB++;
			rankA++;
		}
		else if (counter == 0 && fabsf(B[i]) < EPS)
		{
			rankB--;
			rankA--;
		}
	}

	FILE *out = fopen(argv[2], "w");

	if (!out)
	{
		fprintf(stderr, "Can't open the file 'out' ");
		free(B);
		for (int j = 0; j < N; ++j)
		{
			free(matrix[j]);
		}
		free(result);
		free(matrix);
		fclose(in);
		return ERROR_FILE_NOT_FOUND;
	}

	if (flag == 1)
	{
		fprintf(out, "no solution");
		for (int i = 0; i < N; ++i)
			free(matrix[i]);
		free(B);
		free(matrix);
		free(result);
		fclose(in);
		fclose(out);
		return ERROR_SUCCESS;
	}
	else if (rankA == -2 * N)
	{
		fprintf(out, "many solutions");
		for (int i = 0; i < N; ++i)
			free(matrix[i]);
		free(B);
		free(matrix);
		free(result);
		fclose(in);
		fclose(out);
		return ERROR_SUCCESS;
	}
	else if (rankB == rankA)
	{
		if (rankB < N)
		{
			fprintf(out, "many solutions");
			for (int i = 0; i < N; ++i)
				free(matrix[i]);
			free(B);
			free(matrix);
			free(result);
			fclose(in);
			fclose(out);
			return ERROR_SUCCESS;
		}
		else if (rankB == N)
		{
			for (int i = 0; i < N; ++i)
			{
				fprintf(out, "%g \n", result[i]);
			}
		}
		for (int i = 0; i < N; ++i)
			free(matrix[i]);
		free(B);
		free(matrix);
		free(result);
		fclose(in);
		fclose(out);
		return ERROR_SUCCESS;
	}
	else
	{
		fprintf(out, "no solution");
		for (int i = 0; i < N; ++i)
			free(matrix[i]);
		free(B);
		free(matrix);
		free(result);
		fclose(in);
		fclose(out);
		return ERROR_SUCCESS;
	}
}
