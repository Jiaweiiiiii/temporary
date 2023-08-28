#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include "lzmadec.h"
#include "lzma_private.h"
#include "lzma_hw_api.h"

#define CHUNKSIZE_IN 4096
#define CHUNKSIZE_OUT (1024*512)

/*************************************************************************
 *************************************************************************/
int main(int argc, char **argv)
{
  int ret = NULL;
#ifdef EYER_SIM_ENV
  eyer_system_ini(0);// eyer environment initialize.
  eyer_reg_segv();   // simulation error used
  eyer_reg_ctrlc();  // simulation error used
#elif CSE_SIM_ENV
    lzma_mem_init();
    if ((ret = __aie_mmap(0x4000000)) == NULL) { //64MB
        printf("nna box_base virtual generate failed !!\n");
        return -1;
    }
#endif

    //int ret;
        lzmadec_stream strm;
        size_t write_size;
        FILE *file_in;// = stdin;
        FILE *file_out = stdout;
        unsigned char *buffer_in = malloc (CHUNKSIZE_IN);
        unsigned char *buffer_out = malloc (CHUNKSIZE_OUT);

#ifdef WIN32

        setmode(fileno(stdout), O_BINARY);
        setmode(fileno(stdin), O_BINARY);
#endif

        if (buffer_in == NULL || buffer_out == NULL) {
                fprintf (stderr, "%s: Not enough memory.\n", argv[0]);
#ifdef EYER_SIM_ENV
  eyer_stop();
#endif
                return 5;
        }
        /* Other initializations */
        strm.lzma_alloc = NULL;
        strm.lzma_free = NULL;
        strm.opaque = NULL;
        strm.avail_in = 0;
        strm.next_in = NULL;

        if (lzmadec_init (&strm) != LZMADEC_OK) {
                fprintf (stderr, "Not enough memory.\n");
#ifdef EYER_SIM_ENV
  eyer_stop();
#endif
                return 2;
        }

    /*************************************/
        LZMA_CFG_S cfg_inst;
        LZMA_CFG_S *cfg = &cfg_inst;
    memset(cfg, 0, sizeof(LZMA_CFG_S));
    //SIZE
    if ((file_in = fopen(argv[1],"rb")) == NULL)
      printf("error: open %s fail\n", argv[1]);
    fseek(file_in, 0L, SEEK_END);
    cfg->bs_size = ftell(file_in);
    fseek(file_in, 0, SEEK_SET);
    if ((cfg->bs_base = (uint32_t)lzma_malloc(4, cfg->bs_size)) == NULL)
      fprintf(stderr, "error: malloc cfg->bs_base failed\n");
    else {
      uint32_t real_bs_size = fread (cfg->bs_base, sizeof (unsigned char),
                     cfg->bs_size, file_in);
      if (real_bs_size != cfg->bs_size)
        fprintf(stderr, "error: bs_size get fail(0x%0x -> 0x%0x)\n", cfg->bs_size, real_bs_size);
    }
    if ((cfg->leftbyte_base = (uint16_t*)lzma_malloc(64, 4096*4096*4096)) == NULL )
      fprintf(stderr,"error: malloc cfg->leftbyte_base failed\n");
    if ((cfg->ctx_base = (uint32_t*)lzma_malloc(64, 4096*4096*4096)) == NULL)
      fprintf(stderr,"error: malloc cfg->ctx_base failed\n");
        /*************************************/

    uint32_t crc_index = 0;
    uint32_t crc_num = 0x9debae;
    uint16_t *crc_ref = malloc(crc_num*2);
    if (crc_ref == NULL)
      fprintf(stderr, "error: malloc crc_ref failed!\n");
    crc_ref[0] = 0xffff;
    uint16_t mdl_crc = 0xffff;
        /* Decode */
        while (1) {
                if (strm.avail_in == 0) {
                        strm.next_in = buffer_in;
            fseek(file_in, CHUNKSIZE_IN*cfg->buf_idx, SEEK_SET);
                        strm.avail_in = fread (buffer_in, sizeof (unsigned char),
                                        CHUNKSIZE_IN, file_in);
                        //printf("strm.avail_in[0x%0x] = 0x%0x\n", cfg->buf_idx, strm.avail_in);
            cfg->buf_idx++;
                }
                strm.next_out = buffer_out;

                strm.avail_out = CHUNKSIZE_OUT;
                ret = lzmadec_decode (&strm, strm.avail_in == 0, cfg);
                if (ret != LZMADEC_OK && ret != LZMADEC_STREAM_END) {
                  fprintf(stderr, "return %d;\n", ret);
#ifdef EYER_SIM_ENV
  eyer_stop();
#endif
                        return 1;
                }
                write_size = CHUNKSIZE_OUT - strm.avail_out;
        { //add by cbzhang
          cfg->dst_size += write_size;
          //printf("cfg->dst_size = 0x%0x\n", cfg->dst_size);
        }
        if (1){
          uint8_t i;
          size_t crc_i;
          for (crc_i = 0; crc_i < write_size; crc_i++)
            {
              mdl_crc ^= buffer_out[crc_i];
              for(i = 0; i < 8; i++)
            {
              if(mdl_crc & 0x01)
                mdl_crc = (mdl_crc >> 1) ^ 0xa001;
              else
                mdl_crc = (mdl_crc >> 1);
            }
              crc_index ++;
              if (crc_index > crc_num) {
            fprintf(stderr, "error: the space for store crc_ref is so small(0x%0x). You should modify it\n", crc_num);
              }
              crc_ref[crc_index] = mdl_crc;
            }
        }
#if 0
                if (write_size != (fwrite (buffer_out, sizeof (unsigned char),
                                           write_size, file_out))) {
                  fprintf(stderr, "return 2;\n");
#ifdef EYER_SIM_ENV
  eyer_stop();
#endif
                        return 2;
                }
#endif
                if (ret == LZMADEC_STREAM_END) {
                        lzmadec_end (&strm);
            { //add by cbzhang
              if ((cfg->dst_base = (uint8_t*)lzma_malloc(64, cfg->dst_size + 8)) == NULL)
                fprintf(stderr,"error: malloc cfg->dst_base failed\n");
              //printf("crc_index = 0x%0x\n", crc_index);
              lzma_cfg(cfg);
            }
            uint16_t crc_imp_j = 0;
            uint16_t crc_imp_i_pre = 0;
            while (lzma_read_reg(LZMA_CTRL, 0) & 0x1 != 0) {
              uint32_t dst_crc = lzma_read_reg(LZMA_OCRC, 0);
              uint16_t crc_imp_i_cur = (dst_crc >> 16) & 0xffff;
              uint16_t crc_imp = dst_crc & 0xffff;
              if (crc_imp_i_pre > crc_imp_i_cur)
                crc_imp_j ++;
              crc_imp_i_pre = crc_imp_i_cur;
              uint32_t crc_ref_i = crc_imp_j*65536 + crc_imp_i_cur;
              if (crc_ref[crc_ref_i] != crc_imp) {
                fprintf(stderr, "error: crc[0x%0x] = 0x%x -> 0x%x\n", crc_ref_i, crc_ref[crc_ref_i], crc_imp);
                break;
              }
            };
            {//crc check
              uint16_t dst_crc = lzma_read_reg(LZMA_OCRC, 0);
              if(dst_crc != mdl_crc)
                fprintf(stderr, "error: crc = 0x%x -> 0x%x\n", mdl_crc, dst_crc);
            }
#ifdef EYER_SIM_ENV
  eyer_stop();
#endif
                        return 0;
                }
        }
}

/******************************************************************************
                            dec_main
******************************************************************************/

#ifndef UINT64_MAX
#define UINT64_MAX (~(uint64_t)0)
#endif

/* Cleaner way to refer to strm->state */
#define STATE ((lzmadec_state*)(strm->state))

static void *lzmadec_alloc (void *opaque, size_t nmemb, size_t size);
static void lzmadec_free (void *opaque, void *addr);
static int_fast8_t lzmadec_internal_init (lzmadec_stream *strm);
static inline int_fast8_t lzmadec_decode_main (
                lzmadec_stream *strm,
                const int_fast8_t finish_decoding,
                           LZMA_CFG_S *cfg);
static int_fast8_t lzmadec_header_properties (
                uint_fast8_t *pb,
                uint_fast8_t *lp,
                uint_fast8_t *lc,
                const uint8_t c);
static int_fast8_t lzmadec_header_dictionary (
                uint_fast32_t *size,
                const uint8_t *buffer);
static void lzmadec_header_uncompressed (
                uint_fast64_t *size,
                uint_fast8_t *is_streamed,
                const uint8_t *buffer);

/******************
  extern functions
 ******************/

/* This function doesn't do much but it's here to be as close to zlib
   as possible. See lzmadec_internal_init for actual initialization. */
extern int_fast8_t
lzmadec_init (lzmadec_stream *strm)
{
        /* Set the functions */
        if (strm->lzma_alloc == NULL)
                strm->lzma_alloc = lzmadec_alloc;
        if (strm->lzma_free == NULL)
                strm->lzma_free = lzmadec_free;
        strm->total_in = 0;
        strm->total_out = 0;
        /* Allocate memory for internal state structure */
        strm->state = (lzmadec_state*)((strm->lzma_alloc)(strm->opaque, 1,
                        sizeof (lzmadec_state)));

        if (strm->state == NULL)
                return LZMADEC_MEM_ERROR;
        /* We will allocate memory and put the pointers in probs and
           dictionary later. Before that, make it clear that they contain
           no valid pointer yet. */
        STATE->probs = NULL;
        STATE->dictionary = NULL;
        /* Mark that the decoding engine is not yet initialized. */
        STATE->status = LZMADEC_STATUS_UNINITIALIZED;

        /* Initialize the internal data if there is enough input available */
        if (strm->avail_in >= LZMA_MINIMUM_COMPRESSED_FILE_SIZE) {
                return (lzmadec_internal_init (strm));
        }
    //printf("uncompressed_size = 0x%0x\n", strm->state.uncompressed_size);
        return LZMADEC_OK;
}

extern int_fast8_t
lzmadec_decode (lzmadec_stream *strm, const int_fast8_t finish_decoding, LZMA_CFG_S *cfg)
{
  //printf("lzmadec_decode...\n");
  if (strm == NULL || STATE == NULL) {
    fprintf(stderr, "error: LZMADEC_SEQUENCE_ERROR\n");
    return LZMADEC_SEQUENCE_ERROR;
  }
        switch (STATE->status) {
                case LZMADEC_STATUS_UNINITIALIZED:
          if (strm->avail_in < LZMA_MINIMUM_COMPRESSED_FILE_SIZE) {
            fprintf(stderr, "error: LZMADEC_BUF_ERROR: strm->avail_in = 0x%0x < 0x%0x\n",
                strm->avail_in, LZMA_MINIMUM_COMPRESSED_FILE_SIZE);
            return LZMADEC_BUF_ERROR;
          }
          if (lzmadec_internal_init (strm) != LZMADEC_OK) {
            fprintf(stderr, "error: LZMADEC_HEADER_ERROR\n");
            return LZMADEC_HEADER_ERROR;
          }
                        /* Fall through */
                case LZMADEC_STATUS_RUNNING:
                        /* */
                        if (strm->total_out < STATE->uncompressed_size)
                                break;
                        if (strm->total_out > STATE->uncompressed_size) {
              fprintf(stderr, "error: LZMADEC_STATUS_RUNNING, LZMADEC_DATA_ERROR\n");
              return LZMADEC_DATA_ERROR;
            }
                        STATE->status = LZMADEC_STATUS_STREAM_END;
                        /* Fall through */
                case LZMADEC_STATUS_FINISHING:
                        /* Sanity check */
          if (!finish_decoding) {
            fprintf(stderr, "error: LZMADEC_SEQUENCE_ERROR\n");
            return LZMADEC_SEQUENCE_ERROR;
          }
          if (strm->total_out > STATE->uncompressed_size) {
            fprintf(stderr, "error: LZMADEC_STATUS_FINISHING, LZMADEC_DATA_ERROR\n");
            return LZMADEC_DATA_ERROR;
          }
                        if (strm->total_out <  STATE->uncompressed_size)
                                break;
                        /* Fall through */
                case LZMADEC_STATUS_STREAM_END:

                        return LZMADEC_STREAM_END;
                case LZMADEC_STATUS_ERROR:
                default:
                        return LZMADEC_SEQUENCE_ERROR;
        }
        /* Let's decode! */
        return (lzmadec_decode_main(strm, finish_decoding, cfg));
}

extern int_fast8_t
lzmadec_end (lzmadec_stream *strm)
{
        if (strm == NULL || STATE == NULL)
                return LZMADEC_SEQUENCE_ERROR;

        (strm->lzma_free)(strm->opaque, STATE->dictionary);
        STATE->dictionary = NULL;
        (strm->lzma_free)(strm->opaque, STATE->probs);
        STATE->probs = NULL;
        (strm->lzma_free)(strm->opaque, strm->state);
        strm->state = NULL;
        return LZMADEC_OK;
}

extern int_fast8_t
lzmadec_buffer_info (lzmadec_info *info, const uint8_t *buffer,
        const size_t len)
{
        /* LZMA header is 13 bytes long. */
        if (len < 13)
                return LZMADEC_BUF_ERROR;
        if (lzmadec_header_properties (&info->pb, &info->lp, &info->lc,
                        buffer[0]) != LZMADEC_OK)
                return LZMADEC_HEADER_ERROR;
        if (LZMADEC_OK != lzmadec_header_dictionary (
                        &info->dictionary_size, buffer + 1))
                return LZMADEC_HEADER_ERROR;
        lzmadec_header_uncompressed (&info->uncompressed_size,
                        &info->is_streamed, buffer + 5);
        return LZMADEC_OK;
}

/*******************
  Memory allocation
 *******************/

/* Default function for allocating memory */
static void *
lzmadec_alloc (void *opaque, size_t nmemb, size_t size)
{
        return (malloc (nmemb * size)); /* No need to zero the memory. */
}

/* Default function for freeing memory */
static void
lzmadec_free (void *opaque, void *addr)
{
        free (addr);
}

/****************
  Header parsing
 ****************/

/* Parse the properties byte */
static int_fast8_t
lzmadec_header_properties (
        uint_fast8_t *pb, uint_fast8_t *lp, uint_fast8_t *lc, const uint8_t c)
{
        /* pb, lp and lc are encoded into a single byte. */
        if (c > (9 * 5 * 5))
                return LZMADEC_HEADER_ERROR;
        *pb = c / (9 * 5);        /* 0 <= pb <= 4 */
        *lp = (c % (9 * 5)) / 9;  /* 0 <= lp <= 4 */
        *lc = c % 9;              /* 0 <= lc <= 8 */
        assert (*pb < 5 && *lp < 5 && *lc < 9);
        return LZMADEC_OK;
}

/* Parse the dictionary size (4 bytes, little endian) */
static int_fast8_t
lzmadec_header_dictionary (uint_fast32_t *size, const uint8_t *buffer)
{
        uint_fast32_t i;
        *size = 0;
        for (i = 0; i < 4; i++)
                *size += (uint_fast32_t)(*buffer++) << (i * 8);
        /* The dictionary size is limited to 256 MiB (checked from
           LZMA SDK 4.30) */
        if (*size > (1 << 28))
                return LZMADEC_HEADER_ERROR;
        return LZMADEC_OK;
}

/* Parse the uncompressed size field (8 bytes, little endian) */
static void
lzmadec_header_uncompressed (uint_fast64_t *size, uint_fast8_t *is_streamed,
        const uint8_t *buffer)
{
        uint_fast32_t i;

        /* Streamed files have all 64 bits set in the size field.
           We don't know the uncompressed size beforehand. */
        *is_streamed = 1; /* Assume streamed. */
        *size = 0;
        for (i = 0; i < 8; i++) {
                *size += (uint_fast64_t)buffer[i] << (i * 8);
                if (buffer[i] != 255)
                        *is_streamed = 0;

        }
        assert ((*is_streamed == 1 && *size == UINT64_MAX)
                        || (*is_streamed == 0 && *size < UINT64_MAX));
    //printf("lzmadec_header_uncompressed: 0x%0x\n", *size);
}

/* Because the LZMA decoder cannot be initialized in practice by
   lzmadec_decode_init(), lzmadec_internal_init()
   is run when lzmadec_decompress() is called the first time.
   lzmadec_decompress() provides the FIXME FIXME FIXME
   is because initialization needs to know how much to allocate memory.
   This function reads the first 18 (LZMA_MINIMUM_COMPRESSED_FILE_SIZE)
   bytes of an LZMA stream, parses it, allocates the required memory and
   initializes the internal variables to a good values. 18 bytes is also
   the size of the smallest possible LZMA encoded stream. */
static int_fast8_t
lzmadec_internal_init (lzmadec_stream *strm)
{
        uint_fast32_t i;
        uint32_t num_probs;
        size_t lzmadec_num_probs;

        /* Make sure we have been called sanely */
        if (STATE->probs != NULL || STATE->dictionary != NULL
                        || STATE->status != LZMADEC_STATUS_UNINITIALIZED)
                return LZMADEC_SEQUENCE_ERROR;

        /* Check that we have enough input */
        if (strm->avail_in < LZMA_MINIMUM_COMPRESSED_FILE_SIZE)
                return LZMADEC_BUF_ERROR;

        /* Parse the header (13 bytes) */
        /* - Properties (the first byte) */

        if (lzmadec_header_properties (&STATE->pb, &STATE->lp, &STATE->lc,
                        *strm->next_in) != LZMADEC_OK)
                return LZMADEC_HEADER_ERROR;

        strm->next_in++;
        strm->avail_in--;

        /* - Calculate these right away: */
        STATE->pos_state_mask = (1 << STATE->pb) - 1;
        STATE->literal_pos_mask = (1 << STATE->lp) - 1;

        /* - Dictionary size */
        lzmadec_header_dictionary (&STATE->dictionary_size, strm->next_in);
        strm->next_in += 4;
        strm->avail_in -= 4;

        /* - Uncompressed size */
        lzmadec_header_uncompressed (&STATE->uncompressed_size,
                        &STATE->streamed, strm->next_in);
    //printf("STATE->uncompressed_size = 0x%0x\n", STATE->uncompressed_size);
        strm->next_in += 8;
        strm->avail_in -= 8;
        /* Allocate memory for internal data */
        lzmadec_num_probs = (LZMA_BASE_SIZE
                        + (LZMA_LIT_SIZE << (STATE->lc + STATE->lp)));
        STATE->probs = (CProb *)((strm->lzma_alloc)(strm->opaque, 1,
                        lzmadec_num_probs * sizeof(CProb)));
        if (STATE->probs == NULL)
                return LZMADEC_MEM_ERROR;

        /* When dictionary_size == 0, it must be set to 1. */
        if (STATE->dictionary_size == 0)
                STATE->dictionary_size = 1;
        /* Allocate dictionary */
        STATE->dictionary = (unsigned char*)((strm->lzma_alloc)(
                        strm->opaque, 1, STATE->dictionary_size));
        if (STATE->dictionary == NULL) {
                /* First free() the memory allocated for internal data */
                (strm->lzma_free)(strm->opaque, STATE->probs);
                return LZMADEC_MEM_ERROR;
        }
        /* Initialize the internal data */
        num_probs = LZMA_BASE_SIZE
                        + ((CProb)LZMA_LIT_SIZE << (STATE->lc + STATE->lp));
        //printf("------------------------------lc = %d, lp = %d, pb = %d, \n", STATE->lc, STATE->lp, STATE->pb);

        for (i = 0; i < num_probs; i++)
                STATE->probs[i] = 1024; /* LZMA_BIT_MODEL_TOTAL >> 1; */
        /* Read the first five bytes of data and initialize STATE->code */
        STATE->code = 0;
        for (i = 0; i < 5; i++)
          {
            STATE->code = (STATE->code << 8) | (uint32_t)(*strm->next_in++);

          }
        strm->avail_in -= 5;

        /* Zero the buffer[] */
        memset (STATE->buffer, 0,
                        LZMA_IN_BUFFER_SIZE + LZMA_REQUIRED_IN_BUFFER_SIZE);

        /* Set the initial static values */
        STATE->rep0 = 1;
        STATE->rep1 = 1;
        STATE->rep2 = 1;
        STATE->rep3 = 1;
        STATE->state = 0;
        strm->total_out = 0;
        STATE->distance_limit = 0;
        STATE->dictionary_position = 0;
        STATE->dictionary[STATE->dictionary_size - 1] = 0;
        STATE->buffer_size = 0;
        STATE->buffer_position = STATE->buffer;
        STATE->len = 0;
        STATE->range = 0xFFFFFFFF;

        /* Mark that initialization has been done */
        STATE->status = LZMADEC_STATUS_RUNNING;

        return LZMADEC_OK;
}

/*********************
  LZMA decoder engine
 *********************/

/* Have a nice day! */

#define RC_NORMALIZE \
                if (range < LZMA_TOP_VALUE/*1<<24)*/) { \
                        range <<= 8; \
                        code = (code << 8) | *buffer++; \
                }

#define IfBit0(p) \
                RC_NORMALIZE; \
                bound = (range >> LZMA_NUM_BIT_MODEL_TOTAL_BITS) * *(p); \
                if (code < bound)

#define UpdateBit0(p) \
                range = bound; \
                *(p) += (LZMA_BIT_MODEL_TOTAL - *(p)) >> LZMA_NUM_MOVE_BITS;\

#define UpdateBit1(p) \
                range -= bound; \
                code -= bound; \
                *(p) -= (*(p)) >> LZMA_NUM_MOVE_BITS; \

#define RC_GET_BIT2(p, mi, A0, A1) \
                IfBit0(p) { \
                        UpdateBit0(p); \
                        mi <<= 1; \
                        A0; \
                } else { \
                        UpdateBit1(p); \
                        mi = (mi + mi) + 1; \
                        A1; \
                }

#define RC_GET_BIT(p, mi) RC_GET_BIT2(p, mi, ; , ;)

#define RangeDecoderBitTreeDecode(probs, numLevels, res) \
                { \
                        int i_ = numLevels; \
                        res = 1; \
                        do { \
                                CProb *p_ = probs + res; \
                                RC_GET_BIT(p_, res) \
                        } while(--i_ != 0); \
                        res -= (1 << numLevels); \
                }
static inline int_fast8_t
lzmadec_decode_main (lzmadec_stream *strm, const int_fast8_t finish_decoding, LZMA_CFG_S *cfg) {
    /* Split the *strm structure to separate _local_ variables.
       This improves readability a little. The major reason to do
       this is performance; at least with GCC 3.4.4 this makes
       the code about 30% faster! */
    /* strm-> */
    unsigned char *next_out = strm->next_out;
    unsigned char *next_in = strm->next_in;
    size_t avail_in = strm->avail_in;
    uint64_t total_out = strm->total_out;
    /* strm->state-> */
    const int_fast8_t lc = STATE->lc;
    const uint32_t pos_state_mask = STATE->pos_state_mask;
    const uint32_t literal_pos_mask = STATE->literal_pos_mask;
    const uint32_t dictionary_size = STATE->dictionary_size;
    unsigned char *dictionary = STATE->dictionary;
/*    int_fast8_t streamed;*/ /* boolean */
    CProb *p = STATE->probs;
     uint32_t range = STATE->range;
     uint32_t code = STATE->code;
    uint32_t dictionary_position = STATE->dictionary_position;
    uint32_t distance_limit = STATE->distance_limit;
    uint32_t rep0 = STATE->rep0;
    uint32_t rep1 = STATE->rep1;
    uint32_t rep2 = STATE->rep2;
    uint32_t rep3 = STATE->rep3;
    int state = STATE->state;
    int len = STATE->len;
    unsigned char *buffer_start = STATE->buffer;
    size_t buffer_size = STATE->buffer_size;
    /* Other variable initializations */
    int_fast8_t i; /* Temporary variable for loop indexing */
    unsigned char *next_out_end = next_out + strm->avail_out;
    unsigned char *buffer = STATE->buffer_position;

    /* This should have been verified in lzmadec_decode() already: */
    assert (STATE->uncompressed_size > total_out);
    /* With non-streamed LZMA stream the output has to be limited. */
    if (STATE->uncompressed_size - total_out < strm->avail_out) {
        next_out_end = next_out + (STATE->uncompressed_size - total_out);
    }

    /* The main loop */
    while (1) {
assert (len >= 0);
assert (state >= 0);
        /* Copy uncompressed data to next_out: */
        {
          cfg->debug_en = 0;//(cfg->ctx_size < 0x6f125) && (cfg->ctx_size > 0x6f110);
          uint8_t match_flag = (len != 0 && next_out != next_out_end);
          if (match_flag) {
            //cfg->ctx_base[2*cfg->ctx_size + 0] = rep0;
            //cfg->ctx_base[2*cfg->ctx_size + 1] = ((1<<31) | len);
            if (cfg->debug_en)
              printf("ctx_base[0x%0x] = 0x%0x, %0x\n", cfg->ctx_size, ((1<<31) | len), rep0);
          }
                unsigned char *foo = next_out;
            while (len != 0 && next_out != next_out_end) {
                uint32_t pos = dictionary_position - rep0;
                if (pos >= dictionary_size) {
                  //printf("pos(0x%0x) >= dictionary_size(0x%0x)\n", pos, dictionary_size);
                    pos += dictionary_size;
                }
                *next_out++ = dictionary[dictionary_position] = dictionary[pos];
                if (++dictionary_position == dictionary_size) {
                  //printf("dictionary_position == dictionary_size(0x%0x)\n", dictionary_size);
                    dictionary_position = 0;
                }
                len--;
            }
            total_out += next_out - foo;
            if (match_flag) {
              uint32_t match_pos = dictionary_position - rep0;
              if (match_pos >= dictionary_size)
                match_pos += dictionary_size;
              uint32_t left_pos = dictionary_position - rep0 - 1;
              if (left_pos >= dictionary_size)
                left_pos += dictionary_size;
              //cfg->leftbyte_base[cfg->leftbyte_size] = ((dictionary[match_pos] << 8) | dictionary[left_pos]);
              if (cfg->debug_en)
                printf("leftbyte_base[0x%0x] = 0x%0x\n",
                   cfg->leftbyte_size, ((dictionary[match_pos] << 8) | dictionary[left_pos]));
              cfg->leftbyte_size++;
              cfg->ctx_size++;
            }
        }

        /* Fill the internal input buffer: */
        {
            size_t avail_buf;
            /* Check for overflow (invalid input) */
            if (buffer > buffer_start + LZMA_IN_BUFFER_SIZE)
                return LZMADEC_DATA_ERROR;
            /* Calculate how much data is unread in the buffer: */
            avail_buf = buffer_size - (buffer - buffer_start);

            /* Copy more data to the buffer if needed: */
            if (avail_buf < LZMA_REQUIRED_IN_BUFFER_SIZE) {
                const size_t copy_size = MIN (avail_in,
                        LZMA_IN_BUFFER_SIZE - avail_buf);
                if (avail_buf > 0)
                    memmove (buffer_start, buffer, avail_buf);
                memcpy (buffer_start + avail_buf,
                        next_in, copy_size);
                buffer = buffer_start;
                next_in += copy_size;
                avail_in -= copy_size;
                buffer_size = avail_buf + copy_size;
            }
        }

        /* Decoder cannot continue if there is
           - no output space available
           - less data in the input buffer than a single decoder pass
             could consume; decoding is still continued if the callee
             has marked that all available input data has been given. */
        if ((next_out == next_out_end)
                || (!finish_decoding
                && buffer_size < LZMA_REQUIRED_IN_BUFFER_SIZE))
            break;

        assert (STATE->status != LZMADEC_STATUS_FINISHING);

        /* The rest of the main loop can at maximum
           - read at maximum of LZMA_REQUIRED_IN_BUFFER_SIZE bytes
             from the buffer[]
           - write one byte to next_out. */
        {
            CProb *prob;
            uint32_t bound;
            int_fast32_t posState = (int_fast32_t)(total_out & pos_state_mask);
            prob = p + LZMA_IS_MATCH + (state << LZMA_NUM_POS_BITS_MAX) + posState;
            IfBit0(prob) {
                int_fast32_t symbol = 1;
                UpdateBit0(prob)
                prob = p + LZMA_LITERAL + (LZMA_LIT_SIZE *
                    (((total_out & literal_pos_mask) << lc)
                    + ((dictionary_position != 0
                    ? dictionary[dictionary_position - 1]
                    : dictionary[dictionary_size - 1])
                    >> (8 - lc))));
                if (state >= LZMA_NUM_LIT_STATES) {
                    int_fast32_t matchByte;
                    uint32_t pos = dictionary_position - rep0;
                    if (pos >= dictionary_size)
                        pos += dictionary_size;
                    matchByte = dictionary[pos];
                    if (cfg->debug_en)
                      printf("matchByte = dictionary[0x%0x] = 0x%0x\n", pos, matchByte);
                    do {
                        int_fast32_t bit;
                        CProb *probLit;
                        matchByte <<= 1;
                        bit = (matchByte & 0x100);
                        probLit = prob + 0x100 + bit + symbol;
                        RC_GET_BIT2(probLit, symbol,
                            if (bit != 0) break,
                            if (bit == 0) break)
                    } while (symbol < 0x100);
                }
                while (symbol < 0x100) {
                    CProb *probLit = prob + symbol;
                    RC_GET_BIT(probLit, symbol)
                }

                if (distance_limit < dictionary_size)
                    distance_limit++;

                /* Eliminate? */
                *next_out++ = dictionary[dictionary_position]
                        = (char)symbol;
                {
                  //cfg->ctx_base[2*cfg->ctx_size + 0] = symbol - 256;/*literal*/
                  //cfg->ctx_base[2*cfg->ctx_size + 1] = 0;
                  if (cfg->debug_en) {
                    printf("ctx_base[0x%0x][0x%0x] = 0x%0x;\n", cfg->ctx_size, dictionary_position, symbol - 256);
                  }
                  cfg->ctx_size++;
                }
                if (++dictionary_position == dictionary_size)
                    dictionary_position = 0;
                total_out++;

                if (state < 4)
                    state = 0;
                else if (state < 10)
                    state -= 3;
                else
                    state -= 6;

                continue;
            }

            UpdateBit1(prob);
            prob = p + LZMA_IS_REP + state;
            IfBit0(prob) {
                UpdateBit0(prob);
                rep3 = rep2;
                rep2 = rep1;
                rep1 = rep0;
                state = state < LZMA_NUM_LIT_STATES ? 0 : 3;
                prob = p + LZMA_LEN_CODER;
            } else {
                UpdateBit1(prob);
                prob = p + LZMA_IS_REP_G0 + state;
                IfBit0(prob) {
                    UpdateBit0(prob);
                    prob = p + LZMA_IS_REP0_LONG + (state
                            << LZMA_NUM_POS_BITS_MAX)
                            + posState;
                    IfBit0(prob) {
                        UpdateBit0(prob);
                        if (distance_limit == 0)
                            return LZMADEC_DATA_ERROR;
                        if (distance_limit < dictionary_size)
                            distance_limit++;
                        state = state < LZMA_NUM_LIT_STATES ? 9 : 11;
                        len++;
                        continue;
                    } else {
                        UpdateBit1(prob);
                    }
                } else {
                    uint32_t distance;
                    UpdateBit1(prob);
                    prob = p + LZMA_IS_REP_G1 + state;
                    IfBit0(prob) {
                        UpdateBit0(prob);
                        distance = rep1;
                    } else {
                        UpdateBit1(prob);
                        prob = p + LZMA_IS_REP_G2 + state;
                        IfBit0(prob) {
                            UpdateBit0(prob);
                            distance = rep2;
                        } else {
                            UpdateBit1(prob);
                            distance = rep3;
                            rep3 = rep2;
                        }
                        rep2 = rep1;
                    }
                    rep1 = rep0;
                    rep0 = distance;
                }
                state = state < LZMA_NUM_LIT_STATES ? 8 : 11;
                prob = p + LZMA_REP_LEN_CODER;
            }

            {
                int_fast32_t numBits, offset;
                CProb *probLen = prob + LZMA_LEN_CHOICE;
                IfBit0(probLen) {
                    UpdateBit0(probLen);
                    probLen = prob + LZMA_LEN_LOW
                            + (posState
                            << LZMA_LEN_NUM_LOW_BITS);
                    offset = 0;
                    numBits = LZMA_LEN_NUM_LOW_BITS;
                } else {
                    UpdateBit1(probLen);
                    probLen = prob + LZMA_LEN_CHOICE2;
                    IfBit0(probLen) {
                        UpdateBit0(probLen);
                        probLen = prob + LZMA_LEN_MID
                            + (posState
                            << LZMA_LEN_NUM_MID_BITS);
                        offset = LZMA_LEN_NUM_LOW_SYMBOLS;
                        numBits = LZMA_LEN_NUM_MID_BITS;
                    } else {
                        UpdateBit1(probLen);
                        probLen = prob + LZMA_LEN_HIGH;
                        offset = LZMA_LEN_NUM_LOW_SYMBOLS
                            + LZMA_LEN_NUM_MID_SYMBOLS;
                        numBits = LZMA_LEN_NUM_HIGH_BITS;
                    }
                }
                RangeDecoderBitTreeDecode(probLen, numBits, len);
                len += offset;
            }

            if (state < 4) {
                int_fast32_t posSlot;
                state += LZMA_NUM_LIT_STATES;
                prob = p + LZMA_POS_SLOT + (MIN (len,
                        LZMA_NUM_LEN_TO_POS_STATES - 1)
                        << LZMA_NUM_POS_SLOT_BITS);
                RangeDecoderBitTreeDecode(prob, LZMA_NUM_POS_SLOT_BITS, posSlot);
                if (posSlot >= LZMA_START_POS_MODEL_INDEX) {
                    int_fast32_t numDirectBits = ((posSlot >> 1) - 1);
                    rep0 = (2 | ((uint32_t)posSlot & 1));
                    if (posSlot < LZMA_END_POS_MODEL_INDEX) {
                        rep0 <<= numDirectBits;
                        prob = p + LZMA_SPEC_POS + rep0 - posSlot - 1;
                    } else {
                        numDirectBits -= LZMA_NUM_ALIGN_BITS;
                        do {
                            RC_NORMALIZE
                            range >>= 1;
                            rep0 <<= 1;
                            if (code >= range) {
                                code -= range;
                                rep0 |= 1;
                            }
                        } while (--numDirectBits != 0);
                        prob = p + LZMA_ALIGN;
                        rep0 <<= LZMA_NUM_ALIGN_BITS;
                        numDirectBits = LZMA_NUM_ALIGN_BITS;
                    }
                    {
                        int_fast32_t mi = 1;
                        i = 1;
                        do {
                            CProb *prob3 = prob + mi;
                            RC_GET_BIT2(prob3, mi, ; , rep0 |= i);
                            i <<= 1;
                        } while(--numDirectBits != 0);
                    }
                } else {
                    rep0 = posSlot;
                }
                if (++rep0 == (uint32_t)(0)) {
                    /* End of stream marker detected */
                    STATE->status = LZMADEC_STATUS_STREAM_END;
                    break;
                }
            }

            if (rep0 > distance_limit)
                return LZMADEC_DATA_ERROR;

            len += LZMA_MATCH_MIN_LEN;
            if (dictionary_size - distance_limit > (uint32_t)(len))
                distance_limit += len;
            else
                distance_limit = dictionary_size;
        }
    }
    RC_NORMALIZE;

    if (STATE->uncompressed_size < total_out) {
        STATE->status = LZMADEC_STATUS_ERROR;
        return LZMADEC_DATA_ERROR;
    }

    /* Store the saved values back to the lzmadec_stream structure. */
    strm->total_in += (strm->avail_in - avail_in);
    strm->total_out = total_out;
    strm->avail_in = avail_in;
    strm->avail_out -= (next_out - strm->next_out);
    strm->next_in = next_in;
    strm->next_out = next_out;
    STATE->range = range;
    STATE->code = code;
    STATE->rep0 = rep0;
    STATE->rep1 = rep1;
    STATE->rep2 = rep2;
    STATE->rep3 = rep3;
    STATE->state = state;
    STATE->len = len;
    STATE->dictionary_position = dictionary_position;
    STATE->distance_limit = distance_limit;
    STATE->buffer_size = buffer_size;
    STATE->buffer_position = buffer;

    if (STATE->status == LZMADEC_STATUS_STREAM_END
            || STATE->uncompressed_size == total_out) {
        STATE->status = LZMADEC_STATUS_STREAM_END;
        if (len == 0)
            return LZMADEC_STREAM_END;
    }
    return LZMADEC_OK;
}
