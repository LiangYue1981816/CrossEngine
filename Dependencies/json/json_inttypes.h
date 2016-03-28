
#ifndef _json_inttypes_h_
#define _json_inttypes_h_

#include <stdint.h>

#ifndef WIN32
#include <inttypes.h>
#endif

#ifndef PRId64
#define PRId64     "I64d"
#endif

#ifndef SCNd64
#define SCNd64     "I64d"
#endif

#ifndef INT64_MIN
#define INT64_MIN  (-0x7fffffffffffffff - 1)
#endif

#ifndef INT64_MAX
#define INT64_MAX  0x7fffffffffffffff
#endif

#ifndef INT32_MIN
#define INT32_MIN  (-0x7fffffff - 1)
#endif

#ifndef INT32_MAX
#define INT32_MAX  0x7fffffff
#endif

#endif
