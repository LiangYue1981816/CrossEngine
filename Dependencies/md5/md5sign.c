#include "md5.h"
#include <string.h>


typedef struct {
	unsigned int i[2];                        /* number of _bits_ handled mod 2^64 */
	unsigned int buf[4];                      /* scratch buffer */
	unsigned char in[64];                     /* input buffer */
	unsigned char digest[16];                 /* actual digest after md5_final call */
	unsigned char md5[33];                    /* actual md5 from digest (note eof) */
} MD5_CTX;

static MD5_CTX ctx;


static void transform ();

static unsigned char PADDING[64] = {
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* F, G and H are basic MD5 functions: selection, majority, parity */
#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z))) 

/* ROTATE_LEFT rotates x left n bits */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/* FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4 */
/* Rotation is separate from addition to prevent recomputation */
#define FF(a, b, c, d, x, s, ac) { \
	(a) += F ((b), (c), (d)) + (x) + (unsigned int)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define GG(a, b, c, d, x, s, ac) { \
	(a) += G ((b), (c), (d)) + (x) + (unsigned int)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define HH(a, b, c, d, x, s, ac) { \
	(a) += H ((b), (c), (d)) + (x) + (unsigned int)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}
#define II(a, b, c, d, x, s, ac) { \
	(a) += I ((b), (c), (d)) + (x) + (unsigned int)(ac); \
	(a) = ROTATE_LEFT ((a), (s)); \
	(a) += (b); \
}

void md5_init ()
{
	ctx.i[0] = ctx.i[1] = ctx.md5[0] = (unsigned int)0;

	/* Load magic initialization constants.
	*/
	ctx.buf[0] = (unsigned int)0x67452301;
	ctx.buf[1] = (unsigned int)0xefcdab89;
	ctx.buf[2] = (unsigned int)0x98badcfe;
	ctx.buf[3] = (unsigned int)0x10325476;
}

void md5_update (unsigned char *inbuf, unsigned int inlen)
{
	unsigned int in[16];
	int mdi;
	unsigned int i, ii;

	/* compute number of bytes mod 64 */
	mdi = (int)((ctx.i[0] >> 3) & 0x3F);

	/* update number of bits */
	if (ctx.i[0] + ((unsigned int)inlen << 3) < ctx.i[0]) {
		ctx.i[1]++;
	}
	ctx.i[0] += ((unsigned int)inlen << 3);
	ctx.i[1] += ((unsigned int)inlen >> 29);

	while (inlen--) {
		/* add new character to buffer, increment mdi */
		ctx.in[mdi++] = *inbuf++;

		/* transform if necessary */
		if (mdi == 0x40) {
			for (i = 0, ii = 0; i < 16; i++, ii += 4) {
				in[i] = (((unsigned int)ctx.in[ii+3]) << 24) | (((unsigned int)ctx.in[ii+2]) << 16) | (((unsigned int)ctx.in[ii+1]) << 8) | ((unsigned int)ctx.in[ii]);
			}
			transform (ctx.buf, in);
			mdi = 0;
		}
	}
}

void md5_final (unsigned char *sign)
{
	unsigned int in[16];
	int mdi;
	unsigned int i, ii;
	unsigned int padlen;

	/* save number of bits */
	in[14] = ctx.i[0];
	in[15] = ctx.i[1];

	/* compute number of bytes mod 64 */
	mdi = (int)((ctx.i[0] >> 3) & 0x3F);

	/* pad out to 56 mod 64 */
	padlen = (mdi < 56) ? (56 - mdi) : (120 - mdi);
	md5_update (PADDING, padlen);

	/* append length in bits and transform */
	for (i = 0, ii = 0; i < 14; i++, ii += 4) {
		in[i] = (((unsigned int)ctx.in[ii+3]) << 24) | (((unsigned int)ctx.in[ii+2]) << 16) | (((unsigned int)ctx.in[ii+1]) << 8) | ((unsigned int)ctx.in[ii]);
	}
	transform (ctx.buf, in);

	/* store buffer in digest */
	for (i = 0, ii = 0; i < 4; i++, ii += 4) {
		ctx.digest[ii+0] = (unsigned char)(ctx.buf[i] & 0xFF);
		ctx.digest[ii+1] = (unsigned char)((ctx.buf[i] >> 8) & 0xFF);
		ctx.digest[ii+2] = (unsigned char)((ctx.buf[i] >> 16) & 0xFF);
		ctx.digest[ii+3] = (unsigned char)((ctx.buf[i] >> 24) & 0xFF);
	}

	/* convert digest to md5 */
	for (i = 0; i < 16; i++) {
		sprintf(ctx.md5, "%s%02x", ctx.md5, ctx.digest[i]);
	}

	strcpy((char *)sign, ctx.md5);
}

/* Basic MD5 step. transform buf based on in.
*/
static void transform (unsigned int *buf, unsigned int *in)
{
	unsigned int a = buf[0], b = buf[1], c = buf[2], d = buf[3];

	/* Round 1 */
	#define S11 7
	#define S12 12
	#define S13 17
	#define S14 22
	FF ( a, b, c, d, in[ 0], S11, 3614090360UL); /* 1 */
	FF ( d, a, b, c, in[ 1], S12, 3905402710UL); /* 2 */
	FF ( c, d, a, b, in[ 2], S13,  606105819UL); /* 3 */
	FF ( b, c, d, a, in[ 3], S14, 3250441966UL); /* 4 */
	FF ( a, b, c, d, in[ 4], S11, 4118548399UL); /* 5 */
	FF ( d, a, b, c, in[ 5], S12, 1200080426UL); /* 6 */
	FF ( c, d, a, b, in[ 6], S13, 2821735955UL); /* 7 */
	FF ( b, c, d, a, in[ 7], S14, 4249261313UL); /* 8 */
	FF ( a, b, c, d, in[ 8], S11, 1770035416UL); /* 9 */
	FF ( d, a, b, c, in[ 9], S12, 2336552879UL); /* 10 */
	FF ( c, d, a, b, in[10], S13, 4294925233UL); /* 11 */
	FF ( b, c, d, a, in[11], S14, 2304563134UL); /* 12 */
	FF ( a, b, c, d, in[12], S11, 1804603682UL); /* 13 */
	FF ( d, a, b, c, in[13], S12, 4254626195UL); /* 14 */
	FF ( c, d, a, b, in[14], S13, 2792965006UL); /* 15 */
	FF ( b, c, d, a, in[15], S14, 1236535329UL); /* 16 */

	/* Round 2 */
	#define S21 5
	#define S22 9
	#define S23 14
	#define S24 20
	GG ( a, b, c, d, in[ 1], S21, 4129170786UL); /* 17 */
	GG ( d, a, b, c, in[ 6], S22, 3225465664UL); /* 18 */
	GG ( c, d, a, b, in[11], S23,  643717713UL); /* 19 */
	GG ( b, c, d, a, in[ 0], S24, 3921069994UL); /* 20 */
	GG ( a, b, c, d, in[ 5], S21, 3593408605UL); /* 21 */
	GG ( d, a, b, c, in[10], S22,   38016083UL); /* 22 */
	GG ( c, d, a, b, in[15], S23, 3634488961UL); /* 23 */
	GG ( b, c, d, a, in[ 4], S24, 3889429448UL); /* 24 */
	GG ( a, b, c, d, in[ 9], S21,  568446438UL); /* 25 */
	GG ( d, a, b, c, in[14], S22, 3275163606UL); /* 26 */
	GG ( c, d, a, b, in[ 3], S23, 4107603335UL); /* 27 */
	GG ( b, c, d, a, in[ 8], S24, 1163531501UL); /* 28 */
	GG ( a, b, c, d, in[13], S21, 2850285829UL); /* 29 */
	GG ( d, a, b, c, in[ 2], S22, 4243563512UL); /* 30 */
	GG ( c, d, a, b, in[ 7], S23, 1735328473UL); /* 31 */
	GG ( b, c, d, a, in[12], S24, 2368359562UL); /* 32 */

	/* Round 3 */
	#define S31 4
	#define S32 11
	#define S33 16
	#define S34 23
	HH ( a, b, c, d, in[ 5], S31, 4294588738UL); /* 33 */
	HH ( d, a, b, c, in[ 8], S32, 2272392833UL); /* 34 */
	HH ( c, d, a, b, in[11], S33, 1839030562UL); /* 35 */
	HH ( b, c, d, a, in[14], S34, 4259657740UL); /* 36 */
	HH ( a, b, c, d, in[ 1], S31, 2763975236UL); /* 37 */
	HH ( d, a, b, c, in[ 4], S32, 1272893353UL); /* 38 */
	HH ( c, d, a, b, in[ 7], S33, 4139469664UL); /* 39 */
	HH ( b, c, d, a, in[10], S34, 3200236656UL); /* 40 */
	HH ( a, b, c, d, in[13], S31,  681279174UL); /* 41 */
	HH ( d, a, b, c, in[ 0], S32, 3936430074UL); /* 42 */
	HH ( c, d, a, b, in[ 3], S33, 3572445317UL); /* 43 */
	HH ( b, c, d, a, in[ 6], S34,   76029189UL); /* 44 */
	HH ( a, b, c, d, in[ 9], S31, 3654602809UL); /* 45 */
	HH ( d, a, b, c, in[12], S32, 3873151461UL); /* 46 */
	HH ( c, d, a, b, in[15], S33,  530742520UL); /* 47 */
	HH ( b, c, d, a, in[ 2], S34, 3299628645UL); /* 48 */

	/* Round 4 */
	#define S41 6
	#define S42 10
	#define S43 15
	#define S44 21
	II ( a, b, c, d, in[ 0], S41, 4096336452UL); /* 49 */
	II ( d, a, b, c, in[ 7], S42, 1126891415UL); /* 50 */
	II ( c, d, a, b, in[14], S43, 2878612391UL); /* 51 */
	II ( b, c, d, a, in[ 5], S44, 4237533241UL); /* 52 */
	II ( a, b, c, d, in[12], S41, 1700485571UL); /* 53 */
	II ( d, a, b, c, in[ 3], S42, 2399980690UL); /* 54 */
	II ( c, d, a, b, in[10], S43, 4293915773UL); /* 55 */
	II ( b, c, d, a, in[ 1], S44, 2240044497UL); /* 56 */
	II ( a, b, c, d, in[ 8], S41, 1873313359UL); /* 57 */
	II ( d, a, b, c, in[15], S42, 4264355552UL); /* 58 */
	II ( c, d, a, b, in[ 6], S43, 2734768916UL); /* 59 */
	II ( b, c, d, a, in[13], S44, 1309151649UL); /* 60 */
	II ( a, b, c, d, in[ 4], S41, 4149444226UL); /* 61 */
	II ( d, a, b, c, in[11], S42, 3174756917UL); /* 62 */
	II ( c, d, a, b, in[ 2], S43,  718787259UL); /* 63 */
	II ( b, c, d, a, in[ 9], S44, 3951481745UL); /* 64 */

	buf[0] += a;
	buf[1] += b;
	buf[2] += c;
	buf[3] += d;
}

void md5_sign (unsigned char *inbuf, unsigned int inlen, unsigned char *sign)
{
	md5_init();
	md5_update(inbuf, inlen);
	md5_final(sign);
}

//void main()
//{
//	MD5_CTX ctx;
//	char code[] = "Hello World!";
//	md5_init(&ctx);
//	md5_update(&ctx, (unsigned char *)code, strlen(code));
//	md5_final(&ctx);
//	printf("%s", ctx.md5);
//}
