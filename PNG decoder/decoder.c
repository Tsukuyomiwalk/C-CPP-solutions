#define _CRT_SECURE_NO_WARNINGS
#include "return_codes.h"
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#if defined(ZLIB)
	#include <zlib.h>
#elif defined(LIBDEFLATE)
	#include <libdeflate.h>
#elif defined(ISAL)
	#include <include/igzip_lib.h>
#else
	#error "Don't find libraries"
#endif

typedef unsigned char unchar;
typedef unsigned int uint;
unchar pngSign[] = "\x89\x50\x4e\x47\x0d\x0a\x1a\x0a";
unchar IhdrSign[] = "IHDR";
int len_Checker(FILE* in, int* check, unchar** lengthChank);
unsigned int sixt_int(const unchar* streamer, int length, int x)
{
	unsigned int count = 0;
	for (int i = 0 + x; i < length; i++)
	{
		count += streamer[i] << (8 * (length - i - 1));
	}
	return count;
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "illegal number of arguments");
		return ERROR_UNKNOWN;
	}

	FILE* in = fopen(argv[1], "rb");
	if (!in)
	{
		fprintf(stderr, "can't open file\n");
		return ERROR_UNKNOWN;
	}

	unchar* streamer = malloc(8 * sizeof(unchar));	  // 1
	if (!streamer)
	{
		fprintf(stderr, "not enough memory\n");
		fclose(in);
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	size_t s1;
	fread(streamer, 1, 8, in);
	if (memcmp(streamer, pngSign, 8) != 0)
	{
		fprintf(stderr, "It's not a png\n");
		free(streamer);
		fclose(in);
		return ERROR_INVALID_PARAMETER;
	}

	unchar* Chank4 = malloc(4 * sizeof(unchar));	// 2
	if (!Chank4)
	{
		fprintf(stderr, "not enough memory\n");
		free(streamer);
		fclose(in);
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	s1 = fread(Chank4, 1, 4, in);
	if (s1 != 4)
	{
		fprintf(stderr, "can't read it1 \n");
		free(streamer);
		free(Chank4);
		fclose(in);
		return ERROR_INVALID_DATA;
	}
	uint aL = sixt_int(Chank4, 4, 0);
	if (aL != 13)
	{
		fprintf(stderr, "File isn't correct\n");
		free(streamer);
		free(Chank4);
		fclose(in);
		return ERROR_INVALID_DATA;
	}

	s1 = fread(Chank4, 1, 4, in);
	if (s1 != 4)
	{
		fprintf(stderr, "can't read it2 \n");
		free(streamer);
		free(Chank4);
		fclose(in);
		return ERROR_INVALID_DATA;
	}
	if (memcmp(Chank4, IhdrSign, 4) != 0)
	{
		fprintf(stderr, "Png isn't correct\n");
		free(streamer);
		free(Chank4);
		fclose(in);
		return ERROR_INVALID_DATA;
	}

	unchar* MainInfo = malloc(aL * sizeof(unchar));	   // 3
	if (!MainInfo)
	{
		fprintf(stderr, "not enough memory\n");
		free(streamer);
		free(Chank4);
		fclose(in);
		return ERROR_NOT_ENOUGH_MEMORY;
	}
	s1 = fread(MainInfo, 1, aL, in);
	if (s1 != aL)
	{
		fprintf(stderr, "can't read it3 \n");
		free(streamer);
		free(Chank4);
		free(MainInfo);
		fclose(in);
		return ERROR_INVALID_DATA;
	}

	s1 = fread(Chank4, 1, 4, in);
	if (s1 != 4)
	{
		fprintf(stderr, "truncated crc \n");
		free(streamer);
		free(Chank4);
		free(MainInfo);
		fclose(in);
		return ERROR_INVALID_DATA;
	}

	uint w = sixt_int(MainInfo, 4, 0);
	uint h = sixt_int(MainInfo, 8, 4);

	if (w == 0 || h == 0)
	{
		fprintf(stderr, "can't convert png\n");
		free(streamer);
		free(Chank4);
		free(MainInfo);
		fclose(in);
		return ERROR_INVALID_DATA;
	}

	if (MainInfo[aL - 5] != 8)
	{
		fprintf(stderr, "This program can't support this\n");
		free(streamer);
		free(Chank4);
		free(MainInfo);
		fclose(in);
		return ERROR_INVALID_DATA;
	}

	uint type = MainInfo[9];

	for (int i = aL - 3; i < aL; i++)
	{
		if (MainInfo[i] != 0)
		{
			fprintf(stderr, "This program can't support this parametr\n");
			free(streamer);
			free(Chank4);
			free(MainInfo);
			fclose(in);
			return ERROR_INVALID_DATA;
		}
	}

	int f = 0;
	uint len = 0;
	int r = 1;
	int r1 = 0;
	unchar lengthChank[4];
	unchar Checker[4];

	while (f == 0)
	{
		s1 = fread(lengthChank, 1, 4, in);
		if (s1 != 4)
		{
			fprintf(stderr, "can't read it4 \n");
			free(streamer);
			free(Chank4);
			free(MainInfo);
			fclose(in);
			return ERROR_NOT_ENOUGH_MEMORY;
		}
		s1 = fread(Checker, 1, 4, in);
		if (s1 != 4)
		{
			fprintf(stderr, "can't read it5 \n");
			free(streamer);
			free(Chank4);
			free(MainInfo);
			fclose(in);
			return ERROR_NOT_ENOUGH_MEMORY;
		}
		if (memcmp(Checker, "IDAT", 4) == 0)
		{
			if (r == 0)
			{
				fprintf(stderr, "After idat - idat only\n");
				free(streamer);
				free(Chank4);
				free(MainInfo);
				fclose(in);
				return ERROR_INVALID_DATA;
			}
			len += sixt_int(lengthChank, 4, 0);
			fseek(in, sixt_int(lengthChank, 4, 0), SEEK_CUR);
			fseek(in, 4, SEEK_CUR);
			r = 1;
			r1 = 1;
		}
		else if (memcmp(Checker, "IEND", 4) == 0)
		{
			f = 1;
		}
		else
		{
			fseek(in, sixt_int(lengthChank, 4, 0) + 4, SEEK_CUR);
			if (r1 == 1)
			{
				r = 0;
			}
		}
		if (feof(in) && f == 0)
		{
			fprintf(stderr, "Your png hasn't got Iend chunk\n");
			free(streamer);
			free(Chank4);
			free(MainInfo);
			fclose(in);
			return ERROR_INVALID_DATA;
		}
	}

	unchar* Idatcur = malloc(len * sizeof(unchar));	   // 4
	if (!Idatcur)
	{
		fprintf(stderr, "Not enough memory\n");
		free(streamer);
		free(Chank4);
		free(Idatcur);
		free(MainInfo);
		fclose(in);
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	fseek(in, 0, SEEK_SET);
	fseek(in, 20 + aL, SEEK_CUR);
	uint index = 0;
	f = 0;
	while (f == 0)
	{
		s1 = fread(lengthChank, 1, 4, in);
		if (s1 != 4)
		{
			fprintf(stderr, "can't read it10 \n");
			free(streamer);
			free(Chank4);
			free(Idatcur);
			free(MainInfo);
			fclose(in);
			return ERROR_INVALID_DATA;
		}
		s1 = fread(Checker, 1, 4, in);
		if (s1 != 4)
		{
			fprintf(stderr, "can't read it11 \n");
			free(streamer);
			free(Chank4);
			free(Idatcur);
			free(MainInfo);
			fclose(in);
			return ERROR_INVALID_DATA;
		}

		if (memcmp(Checker, "IDAT", 4) == 0)
		{
			int curLen = sixt_int(lengthChank, 4, 0);
			unchar* Idatcur2 = malloc(curLen * sizeof(unchar));	   // 5
			if (!Idatcur2)
			{
				fprintf(stderr, "Not enough memory\n");
				free(streamer);
				free(Chank4);
				free(MainInfo);
				free(Idatcur);
				fclose(in);
				return ERROR_NOT_ENOUGH_MEMORY;
			}

			s1 = fread(Idatcur2, 1, curLen, in);
			if (s1 != curLen)
			{
				fprintf(stderr, "can't read it12 \n");
				free(streamer);
				free(Chank4);
				free(Idatcur2);
				free(Idatcur);
				free(MainInfo);
				fclose(in);
				return ERROR_INVALID_DATA;
			}
			for (uint i = 0; i < curLen; i++)
			{
				Idatcur[i + index] = Idatcur2[i];
			}
			s1 = fseek(in, 4, SEEK_CUR);
			index += curLen;
			free(Idatcur2);
		}
		else if (memcmp(Checker, "IEND", 4) == 0)
		{
			f = 1;
		}
		else
		{
			s1 = fseek(in, sixt_int(lengthChank, 4, 0) + 4, SEEK_CUR);
		}
	}
	int color;

	if (type == 0)
	{
		color = 1;
	}
	else if (type == 2)
	{
		color = 3;
	}
	else
	{
		fprintf(stderr, "Problems with colours\n");
		free(streamer);
		free(Chank4);
		free(Idatcur);
		free(MainInfo);
		fclose(in);
		return ERROR_INVALID_DATA;
	}

	unchar* UncompressData = malloc((h * (color * w + 1)) * sizeof(unchar));	// 6

	if (!UncompressData)
	{
		fprintf(stderr, "Not enough memory\n");
		free(streamer);
		free(Chank4);
		free(Idatcur);
		free(MainInfo);
		fclose(in);
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	unchar* PNMData = malloc((h * (color * w + 1)) * sizeof(unchar));	 // 7

	if (!PNMData)
	{
		fprintf(stderr, "Not enough memory\n");
		free(streamer);
		free(Chank4);
		free(Idatcur);
		free(MainInfo);
		free(UncompressData);
		fclose(in);
		return ERROR_NOT_ENOUGH_MEMORY;
	}

	unsigned long uncompresLen = (h * (color * w + 1));

#ifdef ZLIB

	uncompress(UncompressData, &uncompresLen, Idatcur, len);

	if (uncompress(UncompressData, &uncompresLen, Idatcur, len) != Z_OK)
	{
		fprintf(stderr, "impossible to Uncompress\n");
		free(streamer);
		free(Chank4);
		free(Idatcur);
		free(MainInfo);
		free(UncompressData);
		fclose(in);
		return ERROR_INVALID_DATA;
	}
#elif defined(LIBDEFLATE)

	unchar exeption = libdeflate_zlib_decompress(libdeflate_alloc_decompressor(), Idatcur, &len, UncompressData, uncompresLen, NULL);

	if (libdeflate_zlib_decompress(libdeflate_alloc_decompressor(), Idatcur, &len, UncompressData, uncompresLen, NULL) != 0)
	{
		fprintf(stderr, "impossible to Uncompress\n");
		free(streamer);
		free(Chank4);
		free(Idatcur);
		free(MainInfo);
		free(UncompressData);
		fclose(in);
		return ERROR_UNKNOWN;
	}
	libdeflate_free_decompressor(libdeflate_alloc_decompressor());
#elif defined(ISAL)
	struct inflate_state Uncompress;
	isal_inflate_init(&Uncompress);
	Uncompress.next_in = Idatcur;			 // массив всех идат
	Uncompress.avail_in = len;				 // длина всех идат
	Uncompress.next_out = UncompressData;	 //массив после анкомпресса
	Uncompress.avail_out = uncompresLen;	 //длина анкомпресс массива
	Uncompress.crc_flag = IGZIP_ZLIB;
	unchar flagg = isal_inflate_stateless(&Uncompress);

	if (flagg != ISAL_DECOMP_OK)
	{
		fprintf(stderr, "impossible to Uncompress\n");
		free(streamer);
		free(Chank4);
		free(Idatcur);
		free(MainInfo);
		free(UncompressData);
		fclose(in);
		return ERROR_UNKNOWN;
	}
#endif

	int shift;

	for (int i = 0; i < uncompresLen; i++)
	{
		if (UncompressData[i] == 1 && (((i + 1) % (w * color + 1)) == 1))
		{
			shift = 1;

			while ((shift + 1) <= (w * color + 1))
			{
				unchar v1;
				if (i + shift - color <= 0 || i + shift - color <= i)
				{
					v1 = 0;
				}
				else
				{
					v1 = UncompressData[(shift + i) - color];
				}
				UncompressData[shift + i] += v1;
				shift++;
			}
			i += shift - 1;
		}
		else if (UncompressData[i] == 2 && ((i + 1) % (w * color + 1) == 1))
		{
			shift = 1;

			while ((shift + 1) <= (w * color + 1))
			{
				unchar v2;
				if ((i + shift) - (w * color + 1) <= 0)
				{
					v2 = 0;
				}
				else
				{
					v2 = UncompressData[(i + shift) - (w * color + 1)];
				}
				UncompressData[shift + i] += v2;
				shift++;
			}
			i += shift - 1;
		}
		else if (UncompressData[i] == 0 && ((i + 1) % (w * (color) + 1) == 1))
		{
			shift = 1;

			while ((shift + 1) <= (w * color + 1))
			{
				shift++;
			}
			i += shift - 1;
		}
		else if (UncompressData[i] == 3 && ((i + 1) % (w * (color) + 1) == 1))
		{
			shift = 1;

			while ((shift + 1) <= (w * color + 1))
			{
				unchar a1;
				unchar a2;

				if (shift + i - (w * color + 1) <= 0)
				{
					a1 = 0;
				}
				else
				{
					a1 = UncompressData[shift + i - (w * color + 1)];
				}

				if ((shift + i) - color <= i || (shift + i) - color <= 0)
				{
					a2 = 0;
				}
				else
				{
					a2 = UncompressData[(shift + i) - color];
				}
				UncompressData[shift + i] += floor((a1 + a2) / 2);
				shift++;
			}
			i += shift - 1;
		}
		else if (UncompressData[i] == 4 && ((i + 1) % (w * (color) + 1) == 1))
		{
			shift = 1;

			while ((shift + 1) <= (w * color + 1))
			{
				int p;
				unchar a;
				unchar b;
				unchar c;

				if (shift + i - color <= 0 || shift + i - color <= i)
				{
					a = 0;
				}
				else
				{
					a = UncompressData[shift + i - color];
				}

				if (i + shift - (w * color + 1) <= 0)
				{
					b = 0;
				}
				else
				{
					b = UncompressData[i + shift - (w * color + 1)];
				}

				if ((i + shift - color <= 0 || i + shift - color <= i) || (i + shift) - (w * color + 1) <= 0)
				{
					c = 0;
				}
				else
				{
					c = UncompressData[(i + shift - color) - (w * color + 1)];
				}

				p = a + b - c;
				int pa = abs(p - a);
				int pb = abs(p - b);
				int pc = abs(p - c);
				int pr;

				if (pa <= pb && pa <= pc)
				{
					pr = a;
				}
				else if (pb <= pc)
				{
					pr = b;
				}
				else
				{
					pr = c;
				}

				UncompressData[i + shift] += pr;
				shift++;
			}
			i += shift - 1;
		}
	}

	int count1 = 0;

	for (int i = 0; i < uncompresLen; i++)
	{
		if (i % (color * w + 1) == 0)
		{
			count1++;
			continue;
		}

		PNMData[i - count1] = UncompressData[i];
	}
	FILE* out = fopen(argv[2], "wb");
	if (!out)
	{
		fprintf(stderr, "can't close file\n");
		fclose(in);
		return ERROR_UNKNOWN;
	}

	if (type == 0)
	{
		fprintf(out, "P5\n%i %i\n255\n", w, h);
	}
	else if (type == 2)
	{
		fprintf(out, "P6\n%i %i\n255\n", w, h);
	}
	else
	{
		fprintf(stderr, "Wrong type\n");
		free(streamer);
		free(Chank4);
		free(Idatcur);
		free(MainInfo);
		free(UncompressData);
		free(PNMData);
		fclose(in);
		fclose(out);
		return ERROR_INVALID_PARAMETER;
	}

	s1 = fwrite(PNMData, 1, (uncompresLen - count1), out);
	if (s1 != (uncompresLen - count1))
	{
		fprintf(stderr, "Can't write it \n");
		free(streamer);
		free(Chank4);
		free(Idatcur);
		free(MainInfo);
		free(PNMData);
		free(UncompressData);
		fclose(in);
		fclose(out);
		return ERROR_INVALID_DATA;
	}
	free(streamer);
	free(Chank4);
	free(Idatcur);
	free(PNMData);
	free(MainInfo);
	free(UncompressData);
	fclose(in);
	fclose(out);
	return ERROR_SUCCESS;
}
