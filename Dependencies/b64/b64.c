#include "b64.h"

/*
** Translation Table as described in RFC1113
*/
static const char cb64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/*
** Translation Table to decode (created by author)
*/
static const char cd64[] = "|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

/*
** encode_block
**
** encode 3 8-bit binary bytes as 4 '6-bit' characters
*/
static void encode_block(unsigned char *in, unsigned char *out, int len)
{
	out[0] = (unsigned char)cb64[(int)(in[0] >> 2)];
	out[1] = (unsigned char)cb64[(int)(((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4))];
	out[2] = (unsigned char)(len > 1 ? cb64[(int)(((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6))] : '=');
	out[3] = (unsigned char)(len > 2 ? cb64[(int)(in[2] & 0x3f)] : '=');
}

/*
** encode
*/
int base64_encode(const char *incode, int insize, char *outcode, int outsize)
{
	int retcode = 0;
	int i, len;
	unsigned char in[3] = {0};
	unsigned char out[4] = {0};
	while (insize) {
		len = 0;
		for (i = 0; i < 3; i++) {
			if (insize) {
				insize--;
				len++;
				in[i] = *incode++;
			}
			else {
				in[i] = 0;
			}
		}
		if (len > 0) {
			encode_block(in, out, len);
			for (i = 0; i < 4; i++) {
				if (outsize) {
					outsize--;
					retcode++;
					*outcode++ = out[i];
					*outcode = 0;
				}
			}
		}
	}
	return retcode;
}

/*
** decode_block
**
** decode 4 '6-bit' characters into 3 8-bit binary bytes
*/
static void decode_block(unsigned char *in, unsigned char *out)
{   
	out[ 0 ] = (unsigned char)(in[0] << 2 | in[1] >> 4);
	out[ 1 ] = (unsigned char)(in[1] << 4 | in[2] >> 2);
	out[ 2 ] = (unsigned char)(((in[2] << 6) & 0xc0) | in[3]);
}

/*
** decode
*/
int base64_decode(const char *incode, int insize, char *outcode, int outsize)
{
	int retcode = 0;
	int i, v, len;
	unsigned char in[5] = {0};
	unsigned char out[3] = {0};
	while (insize) {
		for (len = 0, i = 0; i < 4 && insize; i++) {
			v = 0;
			while (insize && v == 0) {
				insize--;
				v = *incode++;
				v = ((v < 43 || v > 122) ? 0 : (int) cd64[v - 43]);
				v = (v != 0) ? ((v == (int)'$') ? 0 : v - 61) : v;
			}
			len++;
			if (v != 0) {
				in[i] = v - 1;
				in[i + 1] = 0;
			}
		}
		if (len > 0) {
			decode_block(in, out);
			for (i = 0; i < len - 1; i++) {
				if (outsize) {
					outsize--;
					retcode++;
					*outcode++ = out[i];
					*outcode = 0;
				}
			}
		}
	}
	return retcode;
}

/*
** main
**
** parse and validate arguments and call b64 engine or help
*/
//int main( int argc, char **argv )
//{
//	int elen = 0;
//	int dlen = 0;
//	char code[] = "Hello world! ÕæÅ£±Æ";
//	char ecode[256];
//	char dcode[256];
//	memset(ecode, 0, sizeof(ecode));
//	memset(dcode, 0, sizeof(dcode));
//	elen = base64_encode(code, strlen(code), ecode, sizeof(ecode));
//	dlen = base64_decode(ecode, strlen(ecode), dcode, sizeof(dcode));
//	return( 0 );
//}
