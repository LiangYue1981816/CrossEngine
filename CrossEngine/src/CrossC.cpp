/****************************************************************************
Copyright (c) 2015 LiangYue.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sub license, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "_CrossEngine.h"


//
// ʱ�ӵδ�
// ˵��: ��ȷ��1΢��, 1s = 1000ms = 1000000��
//
_CrossExport unsigned int tick(void)
{
#if defined _PLATFORM_WINDOWS_

	LARGE_INTEGER freq;
	LARGE_INTEGER count;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&count);

	return (unsigned int)(((double)count.QuadPart / freq.QuadPart) * 1000000);

#else

	struct timeval curtime;
	gettimeofday(&curtime, 0);
	return (unsigned int)(curtime.tv_sec * 1000000 + curtime.tv_usec);

#endif
}

//
// �ָ��ļ���
//
_CrossExport void splitfilename(const char *name, char *fname, char *ext)
{
	ASSERT(name);
	ASSERT(fname);
	ASSERT(ext);

	const char *p = NULL;
	const char *c = NULL;
	const char *base = name;

	for (p = base; *p; p++) {
		if (*p == '/' || *p == '\\') {
			do {
				p++;
			} while (*p == '/' || *p == '\\');

			base = p;
		}
	}

	size_t len = strlen(base);
	for (p = base + len; p != base && *p != '.'; p--);
	if (p == base && *p != '.') p = base + len;

	if (fname) {
		for (c = base; c < p; c++) {
			fname[c - base] = *c;
		}

		fname[c - base] = 0;
	}

	if (ext) {
		for (c = p; c < base + len; c++) {
			ext[c - p] = *c;
		}

		ext[c - p] = 0;
	}
}

//
// �ļ���С
//
_CrossExport size_t fsize(FILE *stream)
{
	ASSERT(stream);

	long pos;
	size_t size;

	pos = ftell(stream);
	{
		fseek(stream, 0, SEEK_END);
		size = ftell(stream);
	}
	fseek(stream, pos, SEEK_SET);

	return size;
}

//
// �����ļ���
//
_CrossExport size_t freadline(char *buffer, size_t size, FILE *stream)
{
	ASSERT(buffer);
	ASSERT(stream);
	ASSERT(size > 0);

	char c;
	size_t count = 0;

	while (feof(stream) == FALSE && count < size - 1) {
		c = fgetc(stream);

		if (c == '\r') {
			c = fgetc(stream);
		}

		if (c == '\n') {
			break;
		}

		if (c == -1) {
			break;
		}

		*buffer = c;

		buffer++;
		count++;
	}

	*buffer = 0;

	return count;
}

//
// �����ַ���
//
_CrossExport size_t freadstring(char *buffer, size_t size, FILE *stream)
{
	ASSERT(buffer);
	ASSERT(stream);
	ASSERT(size > 0);

	size_t len = 0;
	size_t reads = 0;

	reads += fread(&len, sizeof(len), 1, stream);
	reads += fread(buffer, sizeof(*buffer), min(len, size), stream);
	buffer[min(len, size)] = 0;

	return reads;
}

//
// �����ַ���
//
_CrossExport size_t fwritestring(const char *buffer, size_t size, FILE *stream)
{
	ASSERT(buffer);
	ASSERT(stream);
	ASSERT(size > 0);

	size_t len = 0;
	size_t writes = 0;

	len = min(strlen(buffer), size);
	writes += fwrite(&len, sizeof(len), 1, stream);
	writes += fwrite(buffer, sizeof(*buffer), len, stream);

	return writes;
}

//
// ���ӽ��ַ���ת��Ϊ����
//
_CrossExport int wcstoi(const wchar_t *wsz)
{
	ASSERT(wsz);

	char sz[_MAX_STRING];
	wcstombs(sz, wsz, _MAX_STRING);
	return atoi(sz);
}

//
// ���ӽ��ַ���ת��Ϊ������
//
_CrossExport float wcstof(const wchar_t *wsz)
{
	ASSERT(wsz);

	char sz[_MAX_STRING];
	wcstombs(sz, wsz, _MAX_STRING);
	return (float)atof(sz);
}

#if !defined _PLATFORM_WINDOWS_

#if defined _PLATFORM_IOS_

//
// ���ֽ��ַ���ת��Ϊ���ӽ��ַ���
//
_CrossExport size_t mbstowcs(wchar_t *wsz, const char *sz, size_t count)
{
	ASSERT(sz);
	ASSERT(wsz);
	ASSERT(count > 0);

	wchar_t *wc = wsz;

	while (*sz && count--) {
		*(wc++) = (wchar_t)(*(sz++));
	}

	*(wc) = 0;

	return (size_t)(wc - wsz);
}

//
// ���ӽ��ַ���ת��Ϊ���ֽ��ַ���
//
_CrossExport size_t wcstombs(char *sz, const wchar_t *wsz, size_t count)
{
	ASSERT(sz);
	ASSERT(wsz);
	ASSERT(count > 0);

	char *c = sz;

	while (*wsz && count--) {
		*(c++) = (char)(*(wsz++));
	}

	*(c) = 0;

	return (size_t)(c - sz);
}

#endif

//
// �ַ����Ƚ�
//
_CrossExport int stricmp(const char *src, const char *dst)
{
	ASSERT(src);
	ASSERT(dst);

	int f, l;

	do {
		f = (unsigned char)(*(dst++));
		l = (unsigned char)(*(src++));

		if ((f >= 'A') && (f <= 'Z')) f -= 'A' - 'a';
		if ((l >= 'A') && (l <= 'Z')) l -= 'A' - 'a';
	} while (f && (f == l));

	return (f - l);
}

//
// �ַ����Ƚ�
//
_CrossExport int strnicmp(const char *src, const char *dst, int count)
{
	ASSERT(src);
	ASSERT(dst);
	ASSERT(count > 0);

	int f, l;

	do {
		f = (unsigned char)(*(dst++));
		l = (unsigned char)(*(src++));

		if ((f >= 'A') && (f <= 'Z')) f -= 'A' - 'a';
		if ((l >= 'A') && (l <= 'Z')) l -= 'A' - 'a';
	} while (f && (f == l) && --count);

	return (f - l);
}

//
// dirent��finddataת��
//
static void _dirent2finddata(struct dirent *d, struct _finddata_t *data)
{
	ASSERT(d);
	ASSERT(data);

	data->attrib = 0;
	strcpy(data->name, d->d_name);

	switch (d->d_type) {
	case DT_FIFO:break;                           // �ܵ�
	case DT_CHR: break;                           // �ַ��豸�ļ�
	case DT_DIR: data->attrib = _A_SUBDIR; break; // Ŀ¼
	case DT_BLK: break;                           // ���豸�ļ�
	case DT_REG: data->attrib = _A_NORMAL; break; // ��ͨ�ļ�
	case DT_LNK: break;                           // �����ļ�
	case DT_SOCK:break;                           // �׽���
	case DT_WHT: break;
	}
}

//
// �����׸�ƥ����ļ�
//
_CrossExport long _findfirst(const char *pattern, struct _finddata_t *data)
{
	ASSERT(pattern);
	ASSERT(data);

	DIR *id = opendir(pattern);
	if (id == NULL) return -1;

	struct dirent *d = readdir((DIR *)id);
	if (d == NULL) return -1;

	_dirent2finddata(d, data);
	return (long)id;
}

//
// ������һ��ƥ����ļ�
//
_CrossExport long _findnext(long id, struct _finddata_t *data)
{
	ASSERT(id);
	ASSERT(data);

	struct dirent *d = readdir((DIR *)id);
	if (d == NULL) return -1;

	_dirent2finddata(d, data);
	return 0;
}

//
// ��������
//
_CrossExport long _findclose(long id)
{
	ASSERT(id);

	closedir((DIR *)id);
	return 0;
}

#endif
