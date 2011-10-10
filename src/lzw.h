#ifndef __LZW_H__

#define DICT_SIZE	(1 << 20)
#define CODE_NULL	DICT_SIZE
#define HASH_SIZE	(DICT_SIZE/8)

#define LZW_ERR_DICT_IS_FULL	-1
#define LZW_ERR_INPUT_BUF		-2
#define LZW_ERR_WRONG_CODE		-3

typedef struct _bitbuffer
{
	unsigned buf;
	unsigned n;
}
bitbuffer_t;

// LZW encoder node, represents a string
typedef struct _node_enc
{
	int           prev;		// prefix code
	int           first;	// firts child code
	int           next;		// next child code
	unsigned char ch;		// last symbol
}
node_enc_t;

// LZW decoder node, represents a string
typedef struct _node_dec
{
	int           prev;		// prefix code
	unsigned char ch;		// last symbol
}
node_dec_t;

// LZW encoder context
typedef struct _lzw_enc
{
	node_enc_t    dict[DICT_SIZE];	// code dictionary
	int           hash[HASH_SIZE];
	unsigned      hash_hit;
	int           code;				// current code
	unsigned      max;				// maximal code
	unsigned      codesize;			// number of bits in code
	bitbuffer_t   bb;				// bit-buffer struct
	void          *stream;			// pointer to the stream object
	unsigned      lzwn;				// buffer byte counter
	unsigned      lzwm;				// buffer size (decoder only)
	unsigned char buff[256];		// output code buffer
}
lzw_enc_t;

// LZW decoder context
typedef struct _lzw_dec
{
	node_dec_t    dict[DICT_SIZE];	// code dictionary
	int           code;				// current code
	unsigned      max;				// maximal code
	unsigned      codesize;			// number of bits in code
	bitbuffer_t   bb;				// bit-buffer struct
	void          *stream;			// pointer to the stream object
	unsigned      lzwn;				// input buffer byte counter
	unsigned      lzwm;				// input buffer size
	unsigned char *inbuff;		    // input code buffer
	unsigned char c;				// first char of the code
	unsigned char buff[DICT_SIZE];	// output string buffer
}
lzw_dec_t;

void lzw_enc_init(lzw_enc_t *ctx, void *stream);
int  lzw_encode  (lzw_enc_t *ctx, char buf[], unsigned size);
void lzw_enc_end (lzw_enc_t *ctx);

void lzw_dec_init(lzw_dec_t *ctx, void *stream);
int  lzw_decode  (lzw_dec_t *ctx, char buf[], unsigned size);

// Application defined stream callbacks
void     lzw_writebuf(void *stream, char *buf, unsigned size);
unsigned lzw_readbuf (void *stream, char *buf, unsigned size);

#endif //__LZW_H__
