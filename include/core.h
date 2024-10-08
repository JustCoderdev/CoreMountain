/*
 * JustCoderdev's Core library v9
 */

#ifndef CORE_H_
#define CORE_H_

#define CORE_VER 8

/* #define SGR_ENABLE 1 */

/* #define DEBUG_ENABLE 1 */
/* #define DEBUG_STRING_ENABLE 1 */
/* #define DEBUG_MEMDEB_ENABLE 1 */

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>


/* Types
 * ------------------------------------------------------------ */

typedef unsigned char uchar;
typedef unsigned int  uint;

typedef unsigned char     n8;
typedef unsigned short    n16;
typedef unsigned int      n32;
typedef unsigned long int n64;

typedef signed char     i8;
typedef signed short    i16;
typedef signed int      i32;
typedef signed long int i64;

/* Nuke Raylib bool implementation */
#define bool bool

typedef enum bool {
	true = (1 == 1),
	false = (1 != 1)
} bool;

typedef enum error {
	success = false,
	failure = true
} error;

typedef int Errno;


/* String module v6
 * ------------------------------------------------------------ */
#define CORE_STR_VER 6

/* #if CORE_STR_SHA != 0xdced4c9c69dbe5a3 */
/* #error Core module STR is out of date, bump the version and update the shas */
/* #endif */

typedef const char* CString;

typedef struct {
	char* chars;
	n32 count;
} FString;

typedef struct {
	FString data;
	n32 capacity;
} DString;

#define STR_FMT "%.*s"
#define FSTR(STRING) (STRING).count, (STRING).chars
#define DSTR(STRING) (STRING).data.count, (STRING).data.chars
#define CSTR(STRING) (n32)strlen((STRING)), (STRING)

#define dstring_new(DSTRING, CAPACITY) dstring_new_(DSTRING, CAPACITY, __FILE__, __LINE__)
#define dstring_new_from(DSTRING, TEXT_LEN, TEXT) dstring_new_from_(DSTRING, TEXT_LEN, TEXT, __FILE__, __LINE__)
#define dstring_from(DSTRING, TEXT_LEN, TEXT) dstring_from_(DSTRING, TEXT_LEN, TEXT, __FILE__, __LINE__)
#define dstring_nterm(DSTRING) dstring_nterm_(DSTRING, __FILE__, __LINE__)
#define dstring_free(DSTRING) dstring_free_(DSTRING, __FILE__, __LINE__)

#define fstring_new(FSTRING, CAPACITY) fstring_new_(FSTRING, CAPACITY, __FILE__, __LINE__)
#define fstring_new_from(FSTRING, TEXT_LEN, TEXT) fstring_new_from_(FSTRING, TEXT_LEN, TEXT, __FILE__, __LINE__)
#define fstring_from(FSTRING, TEXT_LEN, TEXT) fstring_from_(FSTRING, TEXT_LEN, TEXT, __FILE__, __LINE__)
#define fstring_free(FSTRING) fstring_free_(FSTRING, __FILE__, __LINE__)

#define dstring_as_CStr(DSTRING) dstring_as_CStr_(DSTRING, __FILE__, __LINE__)
#define fstring_as_CStr(FSTRING) fstring_as_CStr_(FSTRING, __FILE__, __LINE__)

#define dstring_append(DSTRING, CHR) dstring_append_(DSTRING, CHR, __FILE__, __LINE__)

extern void dstring_new_(DString* string, n32 capacity, char* file, int line);
extern void dstring_new_from_(DString* string, n32 text_len, char* text, char* file, int line);
extern void dstring_from_(DString* string, n32 text_len, char* text, char* file, int line);
extern void dstring_nterm_(DString* string, char* file, int line); /* Null terminate a string */
extern void dstring_free_(DString* string, char* file, int line);

extern void fstring_new_(FString* string, n32 capacity, char* file, int line);
extern void fstring_new_from_(FString* string, n32 text_len, char* text, char* file, int line);
extern void fstring_from_(FString* string, n32 text_len, char* text, char* file, int line);
extern void fstring_free_(FString* string, char* file, int line);

extern CString dstring_as_CStr_(DString* string, char* file, int line);
extern CString fstring_as_CStr_(FString* string, char* file, int line);

extern void dstring_append_(DString* string, char chr, char* file, int line);
extern void dstring_clear(DString* string);
extern void dstring_fmt(DString* string, CString format, ...);
extern void dstring_remove(DString* string, n32 count);

extern bool fstring_equals(FString strA, n32 strB_len, const char* strB);
extern bool fstring_equals_CStr(FString strA, CString strB);


/* Macros
 * ------------------------------------------------------------ */

#ifndef min /* clamp(0, x, y) = min(x, y) if x is unsigned */
#define min(A, B) ((A) > (B) ? (B) : (A))
#endif

#ifndef max
#define max(A, B) ((A) > (B) ? (A) : (B))
#endif

#ifndef clamp
#define clamp(MIN, VAL, MAX) ((VAL) <= (MIN) ? (MIN) : ((VAL) >= (MAX) ? (MAX) : (VAL)))
#endif

#ifndef pclamp /* positive clamp */
#define pclamp(VAL, MAX) ((VAL) > (MAX) ? (MAX) : (VAL))
#endif

#ifndef oclamp /* overflow clamp */
#define oclamp(MIN, VAL, MAX) ((VAL) < (MIN) ? (MAX) : ((VAL) > (MAX) ? (MIN) : (VAL)))
#endif

#ifndef range_out
#define range_out(LB, VAL, HB) ((VAL) < (LB) || (VAL) > (HB))
#endif

#ifndef range_in
#define range_in(LB, VAL, HB) ((VAL) > (LB) && (VAL) < (HB))
#endif


/* Dinamic arrays implementation (WIP)
 * ------------------------------------------------------------ */

/* typedef struct VoidArray { */
/* 	char* items; */
/* 	char item_size; */
/* 	n64 count; */
/* 	n64 capacity; */
/* } VoidArray; */

#define arr_new(arr, cap) \
	do { \
		if((arr)->capacity < (cap)) { \
			(arr)->items = drealloc((arr)->items, (cap) * sizeof(*(arr)->items)); \
			if((arr)->items == NULL) { \
				core_log(CORE_ERROR, "Could not resize array, message: %s", \
						strerror(errno)); \
				exit(failure); \
			} \
			(arr)->capacity = (cap); \
		} \
		(arr)->count = 0; \
	} while(0)

#define arr_from(arr, src, src_len) do { \
	if((arr)->capacity < (src_len)) { \
		(arr)->items = drealloc((arr)->items, (src_len) * sizeof(*(arr)->items)); \
		if((arr)->items == NULL) { \
			core_log(CORE_ERROR, "Could not resize array, message: %s", \
					strerror(errno)); \
			exit(failure); \
		} \
		(arr)->capacity = (src_len); \
	} \
	(void)memcpy((arr)->items, (src)->items, (src_len)); \
	(arr)->count = (src_len); \
} while(0)

#define arr_at_end(arr, off) ((arr)->items[(arr)->count - 1 - (off)])

#define arr_append(arr, item) do { \
	if((arr)->capacity < (arr)->count + 1) { \
		(arr)->items = drealloc((arr)->items, ((arr)->count + 1) * 2 * sizeof(*(arr)->items)); \
		if((arr)->items == NULL) { \
			core_log(CORE_ERROR, "Could not resize array, message: %s", \
					strerror(errno)); \
			exit(failure); \
		} \
		(arr)->capacity = ((arr)->count + 1) * 2; \
	} \
	(arr)->items[(arr)->count] = (item); \
	(arr)->count++; \
} while(0)


/* Bit v3
 * ------------------------------------------------------------ */
#define CORE_BIT_VER 3

/* #if CORE_BIT_SHA != 0xfac875ad08307edd */
/* #error Core module BIT is out of date, bump the version and update the shas */
/* #endif */

extern void printc(char c);
extern void printb(n8 byte);
extern void printw(n32 word);


/* Buffer v2
 * ------------------------------------------------------------ */
#define CORE_BUF_VER 2

/* #if CORE_BUF_SHA != 0x75a0b7a96790fa97 */
/* #error Core module BUF is out of date, bump the version and update the shas */
/* #endif */


#define buffer_write_tofd(FD, BUFF_LEN, BUFFER) \
	buffer_write_tofd_((FD), (BUFF_LEN), (BUFFER), __FILE__, __LINE__)


extern void buffer_write_tofd_(int fd, n32 buff_len, char *buffer, char* file, int line);


extern n32 buffer_fmt(n32 len, char* buffer, CString format, ...);

/* Return the index of the chr or -1 */
extern i64 buffer_find_chr(char chr, char *buffer, n64 len);

/* Return the index of the first char that is not a delimiter or -1 */
extern i64 buffer_skip_str(char *del, n64 del_len, char *buffer, n64 buff_len);

/* Copy from src to dest buffers until the delimiter is reached */
extern n64 buffer_copy_until_chr(char delimiter,
            char *src_buffer, n64 src_len,
            char *dest_buffer, n64 dest_len);

/* Copy from src to dest buffers until the delimiter is reached */
extern n64 buffer_copy_until_str(char *delimiter, n64 del_len,
            char *src_buffer, n64 src_len,
            char *dest_buffer, n64 dest_len);

extern void buffer_put_to_file(FILE *file, n32 buff_len, char *buffer);
extern bool buffer_equals(n64 buffA_len, n8* buffA, n64 buffB_len, n8* buffB);


/* Logger v3
 * ------------------------------------------------------------ */
#define CORE_LOG_VER 3

/* #if CORE_LOG_SHA != 0xc4238c8855ffa914 */
/* #error Core module LOG is out of date, bump the version and update the shas */
/* #endif */

#ifndef CORE_LOG_MODULE
#define CORE_LOG_MODULE NULL
#endif

#ifndef CORE_LOG_STREAM
#define CORE_LOG_STREAM stdout
#endif

typedef enum {
	LL_INFO = 0, LL_DEBUG, LL_WARN, LL_ERROR, LL_COUNT
} LogLevel;

#define CORE_INFO   LL_INFO , CORE_LOG_MODULE, __FILE__, __LINE__
#define CORE_DEBUG  LL_DEBUG, CORE_LOG_MODULE, __FILE__, __LINE__
#define CORE_WARN   LL_WARN , CORE_LOG_MODULE, __FILE__, __LINE__
#define CORE_ERROR  LL_ERROR, CORE_LOG_MODULE, __FILE__, __LINE__

#define CORE_INFO_   LL_INFO , CORE_LOG_MODULE, file, line
#define CORE_DEBUG_  LL_DEBUG, CORE_LOG_MODULE, file, line
#define CORE_WARN_   LL_WARN , CORE_LOG_MODULE, file, line
#define CORE_ERROR_  LL_ERROR, CORE_LOG_MODULE, file, line

extern void core_log(LogLevel level, CString module,
		CString file, int line, CString format, ...);
extern void core_test(bool pass, CString expected,
		CString got, CString format, ...);


/* Memdeb v1
 * ------------------------------------------------------------ */
#define CORE_MEM_VER 1

/* #if CORE_MEM_SHA != 0xfdd83e2b940504d1 */
/* #error Core module MEM is out of date, bump the version and update the shas */
/* #endif */

#define dmalloc(SIZE)        malloc_(SIZE, __FILE__, __LINE__)
#define dfree(PTR)           free_(PTR, __FILE__, __LINE__)
#define dcalloc(NMEMB, SIZE) calloc_(NMEMB, SIZE, __FILE__, __LINE__)
#define drealloc(PTR, SIZE)  realloc_(PTR, SIZE, __FILE__, __LINE__)

extern void *malloc_(size_t size, char* file, int line);
extern void free_(void* ptr, char* file, int line);
extern void *calloc_(size_t nmemb, size_t size, char* file, int line);
extern void *realloc_(void* ptr, size_t size, char* file, int line);


/* Net v1
 * ------------------------------------------------------------ */
#define CORE_NET_VER 1

/* #if CORE_NET_SHA != 0x2c0848b091d04575 */
/* #error Core module NET is out of date, bump the version and update the shas */
/* #endif */

typedef union ip4_addr {
	struct in_addr addr;
	struct { n8 D, C, B, A; } part;
	n8 bytes_rev[4];
} in_addr;


extern struct in_addr addr_to_bytes(n8 A, n8 B, n8 C, n8 D);

/* Buffer must be at least 000.000.000.000 */
/* extern void addr_to_str(struct in_addr addr, char* buffer); */

extern error hostname_resolve(const char *hostname, struct in_addr *address);


/* Stuff v2
 * ------------------------------------------------------------ */
#define CORE_STF_VER 2

/* #if CORE_STF_SHA != 0x7170ff389f686ff1 */
/* #error Core module STF is out of date, bump the version and update the shas */
/* #endif */

extern char* shift(int* argc, char*** argv);
extern long file_size_get(FILE* file, CString file_name);


/* Select Graphic Rendition (SGR)
 * ------------------------------------------------------------ */
#if SGR_ENABLE

/* TODO: if SGR_ENABLE is undefined then replace with empty strings */

#define CSI    "\033["
#define M      "m"
#define Z      ";"
#define RESET  "0"

/* Text styles */
#define BOLD       "1"
#define DIM        "2"
#define ITALIC     "3"
#define UNDERLINE  "4"
#define SLW_BLINK  "5"
#define FST_BLINK  "6"
#define SWAP       "7"
#define CONCEAL    "8"
#define CROSS      "9"

/* Text style disabler */
#define NO_BOLD       "21"
#define NO_DIM        "22"
#define NO_ITALIC     "23"
#define NO_UNDERLINE  "24"
#define NO_BLINK      "25"

#define NO_SWAP     "27"
#define NO_CONCEAL  "28"
#define NO_CROSS    "29"

/* Foreground colors 3-bit */
#define FG_BLACK         "30"
#define FG_RED           "31"
#define FG_GREEN         "32"
#define FG_YELLOW        "33"
#define FG_BLUE          "34"
#define FG_MAGENTA       "35"
#define FG_CYAN          "36"
#define FG_WHITE         "37"
#define FG_PAL(CODE)     "38;5;" #CODE
#define FG_RGB(R, G, B)  "38;2;" #R";"#G";"#B
#define FG_DEFAULT       "39"

/* Background colors 3-bit */
#define BG_BLACK         "40"
#define BG_RED           "41"
#define BG_GREEN         "42"
#define BG_YELLOW        "43"
#define BG_BLUE          "44"
#define BG_MAGENTA       "45"
#define BG_CYAN          "46"
#define BG_WHITE         "47"
#define BG_PAL(CODE)     "48;5;" #CODE
#define BG_RGB(R, G, B)  "48;2;" #R";"#G";"#B
#define BG_DEFAULT       "49"

/* Foreground bright colors 3-bit */
#define FG_BR_BLACK  "90"
#define FG_BR_RED    "91"
#define FG_BR_GREEN  "92"
#define FG_BR_YELLO  "93"
#define FG_BR_BLUE   "94"
#define FG_BR_MAGEN  "95"
#define FG_BR_CYAN   "96"
#define FG_BR_WHITE  "97"

/* Background colors 3-bit */
#define BG_BR_BLACK  "100"
#define BG_BR_RED    "101"
#define BG_BR_GREEN  "102"
#define BG_BR_YELLO  "103"
#define BG_BR_BLUE   "104"
#define BG_BR_MAGEN  "105"
#define BG_BR_CYAN   "106"
#define BG_BR_WHITE  "107"

#endif /* SGR_ENABLE */


/* Target information
 * ------------------------------------------------------------ */

/* Checked for compilers: Clang, gcc, MinGW, MSVC
 * source: <https://github.com/cpredef/predef/tree/master> */

#define TENV_UNKNOWN  0
#define TENV_SHELL    1
#define TENV_WINDOW   2
#define TENV_ARDUINO  3

#define TOS_UNKOWN  0
#define TOS_NONE    1
#define TOS_LINUX   2
#define TOS_MACOS   3
#define TOS_WINDOWS 4

#define TARCH_UNKNOWN 0
#define TARCH_i386    1
#define TARCH_x32     2
#define TARCH_x86_64  3
#define TARCH_arm     4
#define TARCH_arm64   5
#define TARCH_avr     6

/*
 * Set environment
 */

/* Ideally the environment should be set by the dev... */
#ifndef TARGET_ENV
#	define TARGET_ENV TENV_UNKNOWN
#endif

/*
 * Find architecture
 */

/* is i386 */
#ifndef TARGET_ARCH
#	if defined(__i386__) || defined(_M_I86) || defined(_M_IX86) || defined(_X86_)
#		define TARGET_ARCH TARCH_i386
#	endif
#endif

/* is x32 */
#ifndef TARGET_ARCH
#	if defined(__ILP32__)
#		define TARGET_ARCH TARCH_x32
#	endif
#endif

/* is x86_64 */
#ifndef TARGET_ARCH
#	if defined(__amd64__) || defined(__x86_64__) || defined(_M_AMD64)
#		define TARGET_ARCH TARCH_x86_64
#	endif
#endif

/* is arm */
#ifndef TARGET_ARCH
#	if defined(__arm__) || defined(_M_ARM)
#		define TARGET_ARCH TARCH_arm
#	endif
#endif

/* is arm64 */
#ifndef TARGET_ARCH
#	if defined(__aarch64__) || defined(_M_ARM64)
#		define TARGET_ARCH TARCH_x86_64
#	endif
#endif

/* Fallback */
#ifndef TARGET_ARCH
#	define TARGET_ARCH TARCH_UNKNOWN
#	error No target architecture has been found
#endif

/*
 * Find Operating system
 */

/* is linux */
#ifndef TARGET_OS
#	if defined(__gnu_linux__)
#		define TARGET_OS TOS_LINUX
#	endif
#endif

/* is macos */
#ifndef TARGET_OS
#	if defined(__APPLE__)
#		define TARGET_OS TOS_MACOS
#	endif
#endif

/* is windows */
#ifndef TARGET_OS
#	if defined(_WIN32)
#		define TARGET_OS TOS_WINDOWS
#	endif
#endif

/* Fallback */
#ifndef TARGET_OS
#	define TARGET_OS TOS_UNKOWN
#	error No target os has been found
#endif


#endif /* CORE_ */
