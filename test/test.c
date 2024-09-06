#include <core.h>
#include <shs.h>

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

#include <stdio.h>
int snprintf(char* str, size_t size, const char *format, ...);

/* -- Test dataset -- */

#define TEST_FILES_COUNT 3
char* const test_files[TEST_FILES_COUNT] = {
	"shs_test_files/test_abcde", "shs_test_files/test_lorem", "shs_test_files/test_poem" };
SHS_Word32 test_exp_results[TEST_FILES_COUNT][5] = {
	{ 0xec113123, 0x86ad5616, 0x74f724b8, 0xcca7cf17, 0x96e26d1d },
	{ 0x0d7d0c40, 0x6f4b1152, 0x70cac19c, 0x2c67522a, 0x4a9d9478 },
	{ 0xd0b6dd32, 0x03ca6d7c, 0x0e442171, 0x3c34fe31, 0x8aa67ffa }
};

/* SHS tests
 * ------------------------------------------------------------ */

static void test_word_operations(void)
{
	core_test(SHS_Word32_ROTL(10, 5) == SHS_Word32_ROTR(10, 32 - 5),
	     "true", "false", "W32 Left/Right Rotation (SHS_Word32_ROTL, SHS_Word32_ROTR)"
	);

	/* core_test(SHS_Word64_ROTL(10, 5) == SHS_Word64_ROTR(10, 32 - 5), */
	/*      "true", "false", "W64 Left/Right Rotation (SHS_Word64_ROTL, SHS_Word64_ROTR)" */
	/* ); */
}

#if 0
static void print_block512_list(SHS_Block512_List blocks)
{
	n64 i;
	n8 j, k;
	for(i = 0; i < blocks.count; ++i)
	{
		SHS_Block512 block = blocks.items[i];

		printf("\n%02lu|", i + 1);
		for(j = 0; j < 8; ++j)
		{
			for(k = 0; k < 8; ++k)
			{
				char c = (char)(block.words[j * 2 + (k > 3)] >> (24 - 8 * k));
				if(c > 32 && c < 127) printf(" %c", c);
				else if(c == '\t') printf("\033[33m\\t\033[0m");
				else if(c == '\r') printf("\033[33m\\r\033[0m");
				else if(c == '\n') printf("\033[33m\\n\033[0m");
				else if(c ==  ' ') printf("\033[33m..\033[0m");
				else printf("\033[31m??\033[0m");

				if(k ==  3) putchar('_');
			}
			printf("|");
		}

		printf("\n%02lu|", i + 1);
		for(j = 0; j < 8; ++j)
		{
			printf("%08x_%08x|", block.words[j * 2], block.words[j * 2 + 1]);
		}
		printf("\n");
	}
}
#endif

static void test_block_creation(void)
{
	SHS_Block512_List blocks = {0};
	n64 i;

	for(i = 0; i < TEST_FILES_COUNT; ++i) {
		char* filename = test_files[i];

		FILE* file = fopen(filename, "r");
		if(file == NULL) {
			core_log(CORE_ERROR, "Can't open file '%s': %s",
					filename, strerror(errno));
			exit(failure);
		}

		blocks = SHS_block512_list_create_from_file(file);
		fclose(file);

		/* print_block512_list(blocks); */
		core_test(true, "", "",
			"Creating block512 list for file '%s'", filename
		);

		/* Check for 0B long files */
		/* Check for 23B files */
		/* Check for 64B files */
		/* Check for 75B files */

		SHS_block512_list_free(&blocks);
	}
}

static void test_digest_operations(void)
{
	#define BUFF_LEN 43
	char buff[BUFF_LEN] = {0};

	SHS_Word32 words[5] = { 0xda881919, 0x84932a60, 0xa59109ea, 0x2c276a19, 0x1380fb41 };
	char* exp_text = "0xda88191984932a60a59109ea2c276a191380fb41";

	SHS_digest160 digest = {0};

	SHS_digest_from_Word32(SHS_DS160, digest.byte, 5, words);

	assert(snprintf(buff, BUFF_LEN, "0x" SHS_D160_FMT, SHS_D160(digest)) < BUFF_LEN);
	core_test(!strncmp(exp_text, buff, BUFF_LEN), exp_text, buff,
		"Word32 array to digest (SHS_digest_from_Word32)"
	);


	/* TODO: add digest compare tests */
	/* core_test(SHS_digest_compare(SHS_DS160, digest.byte, hardig.byte, "SHA1 on file ''"), */
}

static void test_sha1(void)
{
	#define BUFF_LEN 43
	char buffEXP[BUFF_LEN] = {0},
		 buffDIG[BUFF_LEN] = {0};

	SHS_Block512_List blocks = {0};
	SHS_digest160 digest;
	n64 i;

	for(i = 0; i < TEST_FILES_COUNT; ++i) {
		char* filename = test_files[i];
		SHS_Word32* exp_result = test_exp_results[i];

		FILE* file = fopen(filename, "r");
		blocks = SHS_block512_list_create_from_file(file);
		fclose(file);

		digest = SHS_SHA1_digest_generate(blocks);
		SHS_block512_list_free(&blocks);

		assert(snprintf(buffDIG, BUFF_LEN, "0x" SHS_D160_FMT, SHS_D160(digest)) < BUFF_LEN);
		SHS_digest_from_Word32(SHS_DS160, digest.byte, 5, exp_result);
		assert(snprintf(buffEXP, BUFF_LEN, "0x" SHS_D160_FMT, SHS_D160(digest)) < BUFF_LEN);

		core_test(!strncmp(buffDIG, buffEXP, BUFF_LEN), buffEXP, buffDIG,
			"SHA1 on file '%s'", filename
		);
	}
}

/* Core Types tests
 * ------------------------------------------------------------ */

static void check_type_sizes(void)
{
	String buff = {0};
	string_new(&buff, 8);

#define test_sizeof(TYPE, EXP) do { \
	string_fmt(&buff, "%dB", sizeof(TYPE)); \
	core_test(sizeof(TYPE) == EXP, #EXP "B", string_as_CStr(&buff), \
			"sizeof(" #TYPE ")\t== " #EXP "B"); \
} while(0)

	test_sizeof(uchar, 1);
	test_sizeof(uint, 4);

	test_sizeof(n8 , 1);
	test_sizeof(n16, 2);
	test_sizeof(n32, 4);
	test_sizeof(n64, 8);

	test_sizeof(i8 , 1);
	test_sizeof(i16, 2);
	test_sizeof(i32, 4);
	test_sizeof(i64, 8);

#undef test_sizeof

	string_free(&buff);
}

int main(void)
{
	printf("CoreMountain Validation test suite\n");

	printf("\n==== Core v%d ====\n", CORE_VER);

	printf("\n Type sizes:\n");
	check_type_sizes();

	printf("\n Utility Macros: " CSI FG_RED M "MISSING" CSI RESET M "\n");
	printf("\n Dinamic Arrays: " CSI FG_RED M "MISSING" CSI RESET M "\n");
	printf("\n SGR rendering: " CSI FG_RED M "MISSING" CSI RESET M "\n");
	printf("\n Target Info: " CSI FG_RED M "MISSING" CSI RESET M "\n");

	printf("\n==== String v%d ====\n", CORE_STR_VER);
	printf("\n==== Bit v%d ====\n", CORE_BIT_VER);
	printf("\n==== Buffer v%d ====\n", CORE_BUF_VER);
	printf("\n==== Logger v%d ====\n", CORE_LOG_VER);
	printf("\n==== Memdeb v%d ====\n", CORE_MEM_VER);
	printf("\n==== Net v%d ====\n", CORE_NET_VER);
	printf("\n==== Stuff v%d ====\n", CORE_STF_VER);

	printf("\n==== SHS v%d ====\n", SHS_VER);

	printf("\n SHS_Word32 Operations:\n");
	test_word_operations();

	printf("\n Block creation:\n");
	test_block_creation();

	printf("\n Digest operations:\n");
	test_digest_operations();

	printf("\n SHA-1:\n");
	test_sha1();

	/* from different seeds,
	 * generate blobs of random data, size
	 * and compare them with known results
	 * */

	return success;
}
