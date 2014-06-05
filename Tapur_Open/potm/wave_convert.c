/*

Microsoft PCM Wave file => Rockwell ADPCM file
Rockwell ADPCM file => Microsoft PCM Wave file 

Created by Wenlin Zhao

*/ 

#include "wave_convert.h"
#include "debug.h"

static char errbuf[1024];


/* lib funciton */

rmd_header init_rmd_header = {"RMD1", "", 0x0000, 0, 0, {0x00, 0x00, 0x00,
 0x00, 0x00, 0x00, 0x00}};
state_t init_state = {0x0000, 0};

int bitmask[17] = {0x0000, 0x0001, 0x0003, 0x0007, 0x000f, 0x001f, 0x003f,
 0x007f, 0x00ff, 0x01ff, 0x03ff, 0x07ff, 0x0fff, 0x1fff, 0x3fff, 0x7fff,
 0xffff};

int read_bits (FILE *fd_in, state_t *state, int nbits)
     {
     static int data_new;

     while(state->nleft < nbits)
          {

          if ((data_new = getc(fd_in)) == EOF)
               return(EOF);

          state->data = (state->data << 8) | data_new;
          state->nleft += 8;
          }

     state->nleft -= nbits;
     return(state->data >> state->nleft) & bitmask[nbits];
     }

void write_bits (FILE *fd_out, state_t *state, int nbits, int data)
     {
     state->data = (state->data << nbits) | (data & bitmask[nbits]);
     state->nleft += nbits;

     while (state->nleft >= 8)
          {
          putc((state->data >> (state->nleft - 8)) & 0xff, fd_out);
          state->nleft -= 8;
          }

     }

int read_bits_reverse (FILE *fd_in, state_t *state, int nbits)
     {
     static int data;
     static int data_new;

     while(state->nleft < nbits)
          {

          if ((data_new = getc(fd_in)) != EOF)
               return(EOF);

          state->data |= (data_new << state->nleft);
          state->nleft += 8;
          }

     data = state->data & bitmask[nbits];
     state->nleft -= nbits;
     state->data >>= nbits;
     return(data);
     }

void write_bits_reverse (FILE *fd_out, state_t *state, int nbits, int data)
     {
     state->data |= ((data & bitmask[nbits]) << state->nleft);
     state->nleft += nbits;

     while(state->nleft >= 8)
          {
          putc(state->data & 0xff, fd_out);
          state->nleft -= 8;
          state->data >>= 8;
          }

     }

int read_rmd_header(FILE *fd_in, rmd_header *header_in)
     {
     *header_in = init_rmd_header;

     if (fread(header_in, sizeof(rmd_header), 1, fd_in) != 1)
          {
          LOG_ERR("Could not read rmd header");
          return(FAIL);
          };

     if (strncmp(header_in->magic, "RMD1", 4) != 0)
          {
          LOG_ERR("No rmd (raw modem data) header found");
          return(FAIL);
          };

     return(OK);
     }

int write_rmd_header(FILE *fd_out, rmd_header *header_out)
     {

     if (fwrite(header_out, sizeof(rmd_header), 1, fd_out) != 1)
          {
          LOG_ERR("Could not write rmd header");
          return(FAIL);
          };

     return(OK);
     }

static int read_pvf_data_8_ascii(FILE *fd_in)
     {
     static int data_new;

     if (fscanf(fd_in, "%d", &data_new) != 1)
          return(EOF);

     if (data_new < -0x80)
          data_new = -0x80;

     if (data_new > 0x7f)
          data_new = 0x7f;

     return(data_new << 16);
     }

static int read_pvf_data_16_ascii(FILE *fd_in)
     {
     static int data_new;

     if (fscanf(fd_in, "%d", &data_new) != 1)
          return(EOF);

     if (data_new < -0x8000)
          data_new = -0x8000;

     if (data_new > 0x7fff)
          data_new = 0x7fff;

     return(data_new << 8);
     }

static int read_pvf_data_32_ascii(FILE *fd_in)
     {
     static int data_new;

     if (fscanf(fd_in, "%d", &data_new) != 1)
          return(EOF);

     return(data_new);
     }

static int read_pvf_data_8(FILE *fd_in)
     {
     static signed char data_new;

     if (fread(&data_new, 1, 1, fd_in) != 1)
          return(EOF);

     return(data_new << 16);
     }

static int read_pvf_data_16(FILE *fd_in)
     {
     static signed short data_new;

     if (fread(&data_new, 2, 1, fd_in) != 1)
          return(EOF);

     data_new = ntohs(data_new);
     return(data_new << 8);
     }

static int read_pvf_data_32(FILE *fd_in)
     {
     static int data_new;

     if (fread(&data_new, 4, 1, fd_in) != 1)
          return(EOF);

     return(ntohl(data_new));
     }

static void write_pvf_data_8_ascii(FILE *fd_out, int data)
     {
     data >>= 16;

     if (data > 0x7f)
          data = 0x7f;

     if (data < -0x80)
          data = -0x80;

     fprintf(fd_out, "%d\n", data);
     }

static void write_pvf_data_16_ascii(FILE *fd_out, int data)
     {
     data >>= 8;

     if (data > 0x7fff)
          data = 0x7fff;

     if (data < -0x8000)
          data = -0x8000;

     fprintf(fd_out, "%d\n", data);
     }

static void write_pvf_data_32_ascii(FILE *fd_out, int data)
     {
     fprintf(fd_out, "%d\n", data);
     }

static void write_pvf_data_8(FILE *fd_out, int data)
     {
     data >>= 16;

     if (data > 0x7f)
          data = 0x7f;

     if (data < -0x80)
          data = -0x80;

     putc(data, fd_out);
     }

static void write_pvf_data_16(FILE *fd_out, int data)
     {
     static signed short out;

     data >>= 8;

     if (data > 0x7fff)
          data = 0x7fff;

     if (data < -0x8000)
          data = -0x8000;

     out = htons((short) data);
     fwrite(&out, 2, 1, fd_out);
     }

static void write_pvf_data_32(FILE *fd_out, int data)
     {
     static signed int out;

     out = htonl((long) data);
     fwrite(&out, 4, 1, fd_out);
     }

int read_pvf_header(FILE *fd_in, pvf_header *header_in)
     {
     char buffer[VOICE_BUF_LEN];
     int i;

     *header_in = init_pvf_header;

     if (fread(&buffer, 5, 1, fd_in) != 1)
          {
          LOG_ERR("Could not read pvf header");
          return(FAIL);
          };

     if (strncmp(buffer, "PVF1\n", 5) == 0)
          header_in->ascii = FALSE;
     else if (strncmp(buffer, "PVF2\n", 5) == 0)
          header_in->ascii = TRUE;
     else
          {
          LOG_ERR("No pvf (portable voice format) header found");
          return(FAIL);
          };

     for (i = 0; i < VOICE_BUF_LEN; i++)
          {

          if (fread(&buffer[i], 1, 1, fd_in) != 1)
               {
               LOG_ERR("Could not read pvf header");
               return(FAIL);
               };

          if (buffer[i] == '\n')
               break;

          };

     buffer[i] = 0x00;
     sscanf(buffer, "%d %d %d", &header_in->channels, &header_in->speed,
      &header_in->nbits);

     if ((header_in->channels < 1) || (32 < header_in->channels))
          {
          sprintf(errbuf, "Invalid number of channels (%d)", header_in->channels);
          LOG_ERR(errbuf);
          return(FAIL);
          };

     if ((header_in->speed < 0) || (50000 < header_in->speed))
          {
          sprintf(errbuf, "Invalid sample speed (%d)", header_in->speed);
          LOG_ERR(errbuf);
          return(FAIL);
          };

     if ((header_in->nbits != 8) && (header_in->nbits != 16) &&
      (header_in->nbits != 32))
          {
          sprintf(errbuf, "Invalid number of bits (%d) per sample", header_in->nbits);
          LOG_ERR(errbuf);
          return(FAIL);
          };

     if (header_in->ascii)
          {

          switch (header_in->nbits)
               {
               case 8:
                    header_in->read_pvf_data = &read_pvf_data_8_ascii;
                    break;
               case 16:
                    header_in->read_pvf_data = &read_pvf_data_16_ascii;
                    break;
               case 32:
                    header_in->read_pvf_data = &read_pvf_data_32_ascii;
                    break;
               default:
                    LOG_ERR("Illegal bit size for pvf input");
                    return(FAIL);
               };

          }
     else
          {

          switch (header_in->nbits)
               {
               case 8:
                    header_in->read_pvf_data = &read_pvf_data_8;
                    break;
               case 16:
                    header_in->read_pvf_data = &read_pvf_data_16;
                    break;
               case 32:
                    header_in->read_pvf_data = &read_pvf_data_32;
                    break;
               default:
                    LOG_ERR("Illegal bit size for pvf input");
                    return(FAIL);
               };

          };

     return(OK);
     }

int write_pvf_header(FILE *fd_out, pvf_header *header_out)
     {
     char buffer[VOICE_BUF_LEN];

     if (header_out->ascii)
          {
          sprintf(buffer, "PVF2\n%d %d %d\n", header_out->channels,
           header_out->speed, header_out->nbits);

          switch (header_out->nbits)
               {
               case 8:
                    header_out->write_pvf_data = &write_pvf_data_8_ascii;
                    break;
               case 16:
                    header_out->write_pvf_data = &write_pvf_data_16_ascii;
                    break;
               case 32:
                    header_out->write_pvf_data = &write_pvf_data_32_ascii;
                    break;
               default:
                    LOG_ERR("Illegal bit size for pvf output");
                    return(FAIL);
               };

          }
     else
          {
          sprintf(buffer, "PVF1\n%d %d %d\n", header_out->channels,
           header_out->speed, header_out->nbits);

          switch (header_out->nbits)
               {
               case 8:
                    header_out->write_pvf_data = &write_pvf_data_8;
                    break;
               case 16:
                    header_out->write_pvf_data = &write_pvf_data_16;
                    break;
               case 32:
                    header_out->write_pvf_data = &write_pvf_data_32;
                    break;
               default:
                    LOG_ERR("Illegal bit size for pvf output");
                    return(FAIL);
               };

          };

     if (fwrite(&buffer, strlen(buffer), 1, fd_out) != 1)
          {
          LOG_ERR("Could not write pvf header");
          return(FAIL);
          };

     return(OK);
     }

pvf_header init_pvf_header = {FALSE, 1, 8000, 32, &read_pvf_data_32,
 &write_pvf_data_32};


/* Wave -> Adpcm */

char *sizes[] =
     {
     "NONSENSE!",
     "bytes",
     "shorts",
     "NONSENSE",
     "longs",
     "32-bit floats",
     "64-bit floats",
     "IEEE floats"
     };

/* Private data for .wav file */

typedef struct wavstuff
     {
     long samples;
     int  second_header; /* non-zero on second header write */
     } *wav_t;

/* wave file characteristics */

unsigned short wFormatTag;              /* data format */
unsigned short wChannels;               /* number of channels */
unsigned long  wSamplesPerSecond;       /* samples per second per channel */
unsigned long  wAvgBytesPerSec;    /* estimate of bytes per second needed */
unsigned short wBlockAlign;     /* byte alignment of a basic sample block */
unsigned short wBitsPerSample;          /* bits per sample */
unsigned long  data_length;             /* length of sound data in bytes */
unsigned long  bytespersample;          /* bytes per sample (per channel) */

static char *wav_format_str();

/* Read short, little-endian: little end first. VAX/386 style. */

unsigned short rlshort(ft_t ft)
     {
     unsigned char uc, uc2;
     uc  = getc(ft->fp);
     uc2 = getc(ft->fp);
     return (uc2 << 8) | uc;
     }

/* Read long, little-endian: little end first. VAX/386 style. */

unsigned long rllong(ft_t ft)
     {
     unsigned char uc, uc2, uc3, uc4;

/*   if (feof(ft->fp))
          fprintf(stderr,readerr);       No worky! */

     uc  = getc(ft->fp);
     uc2 = getc(ft->fp);
     uc3 = getc(ft->fp);
     uc4 = getc(ft->fp);
     return ((long)uc4 << 24) | ((long)uc3 << 16) | ((long)uc2 << 8) | (long)uc;
     }

/* Write long, little-endian: little end first. VAX/386 style. */

int wllong(ft_t ft, unsigned long ul)
     {
     int datum;

     datum = (ul) & 0xff;
     putc(datum, ft->fp);
     datum = (ul >> 8) & 0xff;
     putc(datum, ft->fp);
     datum = (ul >> 16) & 0xff;
     putc(datum, ft->fp);
     datum = (ul >> 24) & 0xff;
     putc(datum, ft->fp);

     if (ferror(ft->fp))
          return 1;

     return 0;
     }

/* Write short, little-endian: little end first. VAX/386 style. */

int wlshort(ft_t ft, unsigned short us)
     {
     putc(us, ft->fp);
     putc(us >> 8, ft->fp);

     if (ferror(ft->fp))
          return 1;

     return 0;
     }

int wavstartread(ft_t ft)
     {
     wav_t     wav = (wav_t) ft->priv;
     char magic[4];
     unsigned int len;
     int  littlendian = 1;
     char *endptr;

     ft->info.rate      = 0;
     ft->info.size      = -1;
     ft->info.style     = -1;
     ft->info.channels  = -1;
     ft->comment   =  NULL;
     ft->swap      = 0;

     endptr = (char *) &littlendian;
     if   (!*endptr) ft->swap = 1;

     /* If you need to seek around the input file. */
     if   (0 && ! ft->seekable)
          fprintf(stderr, "Sorry, .wav input file must be a file, not a pipe");

     if   (   fread(magic, 1, 4, ft->fp) != 4
         || strncmp("RIFF", magic, 4))
          {
          fprintf(stderr, "Sorry, not a RIFF file");
          return ERROR;
          }

     len = rllong(ft);

     if   (   fread(magic, 1, 4, ft->fp) != 4
         || strncmp("WAVE", magic, 4))
          {
          fprintf(stderr, "Sorry, not a WAVE file");
          return ERROR;
          }

     /* Now look for the format chunk */
     for (;;)
          {
          if   ( fread(magic, 1, 4, ft->fp) != 4 )
               {
               fprintf(stderr, "Sorry, missing fmt spec");
               return ERROR;
               }
          len = rllong(ft);
          if   (strncmp("fmt ", magic, 4) == 0)
               break;    /* Found the format chunk */
          while (len > 0 && !feof(ft->fp))   /* skip to next chunk */
               {
               getc(ft->fp);
               len--;
               }
          }

     if   ( len < 16 )
          fprintf(stderr, "Sorry, fmt chunk is too short");

     wFormatTag = rlshort(ft);
     switch (wFormatTag)
          {
          case WAVE_FORMAT_UNKNOWN:
               fprintf(stderr, "Sorry, this WAV file is in Microsoft Official Unknown format.");
               return ERROR;
          case WAVE_FORMAT_PCM:    /* this one, at least, I can handle */
               break;
          case WAVE_FORMAT_ADPCM:
               fprintf(stderr, "Sorry, this WAV file is in Microsoft ADPCM format.");
               return ERROR;
          case WAVE_FORMAT_ALAW:   /* Think I can handle this */
               ft->info.style = ALAW;
               break;
          case WAVE_FORMAT_MULAW:  /* Think I can handle this */
               ft->info.style = ULAW;
               break;
          case WAVE_FORMAT_OKI_ADPCM:
               fprintf(stderr, "Sorry, this WAV file is in OKI ADPCM format.");
               return ERROR;
          case WAVE_FORMAT_DIGISTD:
               fprintf(stderr, "Sorry, this WAV file is in Digistd format.");
               return ERROR;
          case WAVE_FORMAT_DIGIFIX:
               fprintf(stderr, "Sorry, this WAV file is in Digifix format.");
               return ERROR;
          case IBM_FORMAT_MULAW:
               fprintf(stderr, "Sorry, this WAV file is in IBM U-law format.");
               return ERROR;
          case IBM_FORMAT_ALAW:
               fprintf(stderr, "Sorry, this WAV file is in IBM A-law format.");
               return ERROR;
          case IBM_FORMAT_ADPCM:
               fprintf(stderr, "Sorry, this WAV file is in IBM ADPCM format.");
               return ERROR;
          default:
               fprintf(stderr, "Sorry, don't understand format");
               return ERROR;
          }

     wChannels = rlshort(ft);
     ft->info.channels = wChannels;
     wSamplesPerSecond = rllong(ft);
     ft->info.rate = wSamplesPerSecond;
     wAvgBytesPerSec = rllong(ft); /* Average bytes/second */
     wBlockAlign = rlshort(ft);    /* Block align */
     wBitsPerSample =  rlshort(ft);     /* bits per sample per channel */
     bytespersample = (wBitsPerSample + 7)/8;
     switch (bytespersample)
     {
          case 1:
                    ft->info.size = BYTE;
               break;
          case 2:
                    ft->info.size = WORD;
               break;
          case 4:
                    ft->info.size = LONG;
               break;
          default:
               fprintf(stderr, "Sorry, don't understand .wav size");
               return ERROR;
     }
     len -= 16;
     while (len > 0 && !feof(ft->fp))
     {
          getc(ft->fp);
          len--;
     }

     /* Now look for the wave data chunk */
     for (;;)
     {
          if ( fread(magic, 1, 4, ft->fp) != 4 )
               {
               fprintf(stderr, "Sorry, missing data chunk");
               return ERROR;
               }
          len = rllong(ft);
          if (strncmp("data", magic, 4) == 0)
               break;    /* Found the data chunk */
          while (len > 0 && !feof(ft->fp)) /* skip to next chunk */
          {
               getc(ft->fp);
               len--;
          }
     }
     data_length = len;
     wav->samples = data_length/ft->info.size;    /* total samples */

     return OK;
     }

int wavwritehdr(ft_t ft,long data_size)
     {

     switch (ft->info.size)
          {
          case BYTE:
               wBitsPerSample = 8;
               if (ft->info.style == -1 || ft->info.style == UNSIGNED)
                    ft->info.style = UNSIGNED;
               else if (ft->info.style != ALAW && ft->info.style != ULAW)
                    fprintf(stderr, "User options overiding style written to .wav header");
               break;
          case WORD:
               wBitsPerSample = 16;
               if (ft->info.style == -1 || ft->info.style == SIGN2)
                    ft->info.style = SIGN2;
               break;
          case LONG:
               wBitsPerSample = 32;
               if (ft->info.style == -1 || ft->info.style == SIGN2)
                    ft->info.style = SIGN2;
               break;
          default:
               wBitsPerSample = 32;
               if (ft->info.style == -1)
                    ft->info.style = SIGN2;
               break;
          }

     switch (ft->info.style)
          {
          case UNSIGNED:
               wFormatTag = WAVE_FORMAT_PCM;
               if (wBitsPerSample != 8 )
                    fprintf(stderr, "Warning - writing bad .wav file using unsigned data and %d bits/sample",wBitsPerSample);
               break;
          case SIGN2:
               wFormatTag = WAVE_FORMAT_PCM;
               if (wBitsPerSample == 8 )
                    fprintf(stderr, "Warning - writing bad .wav file using signed data and %d bits/sample",wBitsPerSample);
               break;
          case ALAW:
               wFormatTag = WAVE_FORMAT_ALAW;
               if (wBitsPerSample != 8 )
                    fprintf(stderr, "Warning - writing bad .wav file using A-law data and %d bits/sample",wBitsPerSample);
               break;
          case ULAW:
               wFormatTag = WAVE_FORMAT_MULAW;
               if (wBitsPerSample != 8 )
                    fprintf(stderr, "Warning - writing bad .wav file using U-law data and %d bits/sample",wBitsPerSample);
               break;
          }

     wSamplesPerSecond = ft->info.rate;
     bytespersample = (wBitsPerSample + 7)/8;
     wAvgBytesPerSec = ft->info.rate * ft->info.channels * bytespersample;
     wChannels = ft->info.channels;
     wBlockAlign = ft->info.channels * bytespersample;
          data_length = data_size;

     /* figured out header info, so write it */
     fputs("RIFF", ft->fp);
     wllong(ft, data_length + 8+16+12+1);    /* Waveform chunk size: FIXUP(4) */
                                    /* die 1 ist von mir karlo */
     fputs("WAVE", ft->fp);
     fputs("fmt ", ft->fp);
     wllong(ft, (long)16);         /* fmt chunk size */
     wlshort(ft, wFormatTag);
     wlshort(ft, wChannels);
     wllong(ft, wSamplesPerSecond);
     wllong(ft, wAvgBytesPerSec);
     wlshort(ft, wBlockAlign);
     wlshort(ft, wBitsPerSample);

     fputs("data", ft->fp);
     wllong(ft, data_length);  /* data chunk size: FIXUP(40) */

     return OK;
     }

int wavstartwrite(ft_t ft,long data_size)
     {
     wav_t     wav = (wav_t) ft->priv;
     int  littlendian = 1;
     char *endptr;

     endptr = (char *) &littlendian;
     if (!*endptr) ft->swap = 1;

     wav->samples = 0;
     wav->second_header = 0;

     if   (wavwritehdr(ft,data_size) != OK)
          return ERROR;
     return OK;
     }

/*
 * Return a string corresponding to the wave format type.
 */
static char *wav_format_str(unsigned wFormatTag)
     {

     switch (wFormatTag)
          {
          case WAVE_FORMAT_UNKNOWN:
               return "Microsoft Official Unknown";
          case WAVE_FORMAT_PCM:
               return "Microsoft PCM";
          case WAVE_FORMAT_ADPCM:
               return "Microsoft ADPCM";
          case WAVE_FORMAT_ALAW:
               return "Microsoft A-law";
          case WAVE_FORMAT_MULAW:
               return "Microsoft U-law";
          case WAVE_FORMAT_OKI_ADPCM:
               return "OKI ADPCM format.";
          case WAVE_FORMAT_DIGISTD:
               return "Digistd format.";
          case WAVE_FORMAT_DIGIFIX:
               return "Digifix format.";
          case IBM_FORMAT_MULAW:
               return "IBM U-law format.";
          case IBM_FORMAT_ALAW:
               return "IBM A-law";
          case IBM_FORMAT_ADPCM:
               return "IBM ADPCM";
          default:
               return "Unknown";
          }

     }

int pvftowav (FILE *fd_in, FILE *fd_out, pvf_header *header_in, int wav_bits)
     {
     int bytespersample;
     long data_size = 0;
     int buffer_size = 0;
     int *buffer = NULL;
     int data,*ptr;
     int voice_samples = 0;
     struct soundstream s;

     bytespersample = (wav_bits + 7) / 8;

     switch (bytespersample)
          {
          case 1:
               s.info.size = BYTE;
               break;
          case 2:
               s.info.size = WORD;
               break;
          case 4:
               s.info.size = LONG;
               break;
          default:
               fprintf(stderr, "sorry, don't understand .wav size");
               return(ERROR);
          }

     s.info.rate     = header_in->speed;
     s.info.style    = -1;
     s.info.channels = header_in->channels;
     s.comment      = NULL;
     s.swap         = 0;
     s.filetype     = (char *) 0;
     s.fp                = fd_out;
     s.seekable          = 0;

     while(!feof(fd_in))
          {
          data = header_in->read_pvf_data(fd_in);

          if (voice_samples >= buffer_size)
               {
               buffer_size += BLOCK_SIZE;
               buffer = (int *) realloc(buffer, buffer_size * sizeof(int));

               if (buffer == NULL)
                    {
                    LOG_ERR("out of memory in pvftowav");
                    free(buffer);
                    return(ERROR);
                    };

               }

          buffer[voice_samples++] = data;
          data_size++;
          }

     if   (wavstartwrite(&s,data_size) != OK)
          {
          free(buffer);
          return ERROR;
          }

     ptr = buffer;

     switch (s.info.size)
          {
          case BYTE:

               while (data_size--)
                    {
                    *ptr >>=16;

                    if   (*ptr > 0x7f)
                         *ptr = 0x7f;

                    if   (*ptr < -0x80)
                         *ptr = -0x80;

                    putc(*ptr+0x80,fd_out);
                    ptr++;
                    };

               break;
          case WORD:

               while (data_size--)
                    {
                    *ptr >>=8;

                    if   (*ptr > 0x7fff)
                         *ptr = 0x7fff;

                    if   (*ptr < -0x8000)
                         *ptr = -0x8000;

                    putc(*ptr, fd_out);
                    putc(*ptr++ >> 8, fd_out);
                    };

               break;
          case LONG:

               while (data_size--)
                    {
                    putc(*ptr, fd_out);
                    putc(*ptr >> 8, fd_out);
                    putc(*ptr >> 16, fd_out);
                    putc(*ptr++ >> 24, fd_out);
                    }

               break;
          }

     free(buffer);
     return(OK);
     }

int wavtopvf (FILE *fd_in, FILE *fd_out, pvf_header *header_out)
     {
     struct soundstream s;
     int d;

     s.fp = fd_in;

     if   (wavstartread(&s) != OK)
          return ERROR;

     header_out->channels = (int) wChannels;
     header_out->speed = (int) wSamplesPerSecond;
     write_pvf_header(fd_out, header_out);

     while (data_length--)
          {

          if (feof(fd_in))
               return(OK);

          switch (wBitsPerSample)
               {
               case 8:
                    d = getc(fd_in) - 0x80;
                    d <<= 16;
                    break;
               case 16:
                    d = getc(fd_in) & 0xFF;
                    d += (getc(fd_in) << 8);

                    if (d & 0x8000)
                     d |= 0xFFFF0000;

                    d <<= 8;
                    break;
               default:
                    sprintf(errbuf, "unsupported number of bits per sample (%d)", wBitsPerSample);
                    LOG_ERR(errbuf);
                    return(ERROR);
               };

          header_out->write_pvf_data(fd_out, d);
          };

     return(OK);
     }


#define RV_HONOUR_SILENCE_CODEWORDS

#define RV_PNT98 32113        /* 0.98 */
#define RV_PNT012 393         /* 0.012 */
#define RV_PNT006 197         /* 0.006 */
#define RV_QDLMN 0x1F         /* QDLMN = IQDLMN = 2.87 mV. */
#define RV_DEMPCF 0x3333 /* 0.4 */
#define RV_PDEMPCF 0x3333     /* 0.4 */
#define RV_QORDER 8      /*  Total delay line length for the pole and zero delay lines */

static vgetty_s_int16 RV_pzTable[8];     /* Coefficient Table for the pole and zero linear predictor. */
                    /*     a1 a2 b1 b2 b3 b4 b5 b6      */
static vgetty_s_int16 RV_QdataIndex = 0; /*  Delay line pointer to the coefficient a1. */
static vgetty_s_int16 RV_Qdata[RV_QORDER];    /*  Delay line. */

#ifdef POSTFILTER        /*        DON'T USE THIS          */
     /*
     The POSTFILTER code is in Rockwell's original D.ASM, too.
     They too, don't use it in their distributed executables
     though. I have no idea under what circumstances it might be
     useful, I just left the code in here as I went through the
     effort of writing it before I realised that it appears to be
     useless here.
     */
static vgetty_s_int16 RV_QPdata[RV_QORDER];
static vgetty_s_int16 RV_QPPdata[RV_QORDER];
#endif

static vgetty_s_int16 RV_LastNu = 0;     /*  Last Nu value. */
static vgetty_s_int16 RV_Dempz = 0; /*  De-emphasis filter delay line (one element). */
static vgetty_s_int16 RV_NewQdata = 0;   /*  Adaptive quantizer output. */
static vgetty_s_int16 RV_NewAppData = 0; /* Temporay data storage */

/* ML2bps, ML3bps, and ML4bps are combined in mul[][], just like Torsten suggested */

static vgetty_s_int16 RV_mul[3][16] =
{              /*  Multiplier table to calculate new Nu for 2/3/4 BPS. */
  {0x3333, 0x199A, 0x199A, 0x3333},
  {0x3800, 0x2800, 0x1CCD, 0x1CCD, 0x1CCD, 0x1CCD, 0x2800, 0x3800},
  {0x4CCD, 0x4000, 0x3333, 0x2666, 0x1CCD, 0x1CCD, 0x1CCD, 0x1CCD, 0x1CCD, 0x1CCD, 0x1CCD, 0x1CCD, 0x2666, 0x3333, 0x4000, 0x4CCD}
};

/* Zeta2bps, Zeta3bps, and Zeta4bps are combined in Zeta[][],
   just like Torsten suggested */

static vgetty_u_int16 RV_Zeta[3][16] =
{                   /*  Multiplier table for 2/3/4 BPS to calculate inverse */
                    /*  quantizer output.  This number, index by the code */
                    /*  word, times Nu is the inverse quantizer output. */
  {0xCFAE, 0xF183, 0x0E7D, 0x3052},
  {0xBB23, 0xD4FE, 0xE7CF, 0xF828, 0x07D8, 0x1831, 0x2B02, 0x44DD},
  {0xA88B, 0xBDCB, 0xCC29, 0xD7CF, 0xE1D8, 0xEAFB, 0xF395, 0xFBE4, 0x041C, 0x0C6B, 0x1505, 0x1E28, 0x2831, 0x33C7, 0x4235, 0x5775}
};

static vgetty_s_int16 *RV_mul_p;
static vgetty_s_int16 *RV_Zeta_p;
static vgetty_u_int16 RV_silence_words[3] = {0x13ec, 0x23de, 0xc11c};
static vgetty_u_int16 RV_silence_word;

static vgetty_s_int16 RV_QDelayMX = 0;
static vgetty_s_int16 RV_QDelayTable[3] = {0x54C4,0x3B7A,0x2ED5}; /* 2.01V, 1.41V, 1.11V */

  /*
     Macro definitions used in the decompression, interpolation, and
     compression functions.
   */

static vgetty_s_int32 RV_max_local_int16,RV_min_local_int16;
static vgetty_s_int64 RV_max_local_int32,RV_min_local_int32;

#define RV_clip_16(a) ((a)<RV_min_local_int16?RV_min_local_int16:(a)>RV_max_local_int16?RV_max_local_int16:(a))
#define RV_clip_32(a) ((a)<RV_min_local_int32?RV_min_local_int32:(a)>RV_max_local_int32?RV_max_local_int32:(a))

#define HIWORD(x) (((vgetty_u_int32)x) >> 16)
#define LOWORD(x) ((vgetty_u_int32)(((unsigned int)x) & 0xffff))
#define LOBYTE(x) ((unsigned int)(((unsigned int)x) & 0xff))
#define RV_round_32_into_16(x) (vgetty_s_int16)((((vgetty_u_int32)x)>>16)+((((vgetty_u_int32)x)>>15)&0x0001))

 /* In order to stay as close as possible to the original assembler
    (a kludge, I know), we simulate the system's register(s) below    */

static vgetty_s_int16 RV_di;

  /*  Utilities.
     Routines that both the decompressor and the compressor use. */

  /*
     pzPred
     Linear predictor coefficient update routine.  Local to this module.
     Inputs:
     CX = Q(n), i.e. WORD PTR NewQData
     Output:   DI points to (QDataIndex+7)%8.
   */

static void RV_pzPred(vgetty_s_int16 cx){

  static vgetty_s_int32 x,y;        /* local accumulator(s) */
  static vgetty_s_int16 di;
  static int i;

  di = RV_QdataIndex;

  /*  Calculate coefficients a1 a2 b1 b2 b3 b4 b5 b6 . */

  for (i = 0; i < 8; i++)
    {

      x = RV_pzTable[i] * ((vgetty_s_int32) RV_PNT98);
      x <<= 1; /*
             Rockwell-adjust for decimal point shift, then round off
             lower 16 bits to obtain a 16 bit representation of the
             result.
           */
      x = RV_round_32_into_16 (x);
                         /* cx contains the NewQdata=Q(n) */
      y = ((vgetty_s_int32) cx) * ((vgetty_s_int32) RV_Qdata[di]);
      y <<= 1;           /* Rockwell-adjust for decimal point shift. */
      y = RV_round_32_into_16 (y);
      x += (y < 0 ? -1 : 1) * (i < 2 ? RV_PNT012 : RV_PNT006);
      /* i<2 ? The a's get RV_PNT012. All b's get RV_PNT006. */
      /*
         The result of a multiplication needs adjusting & rounding.
         The result of an addition/subtraction needs clipping.
       */
      RV_pzTable[i] = RV_clip_16 (x);
      di++;
      di %= 8;
    }
}

static vgetty_s_int32 RV_XpzCalc(vgetty_s_int16 cx){
  /*
     Linear pole and zero predictor calculate.  CX,BX register pair is the
     32 bit accumulator.  Local to this module.
     Input:   CX = Initial Value.  BX set to zero.
     Output:   CX,BX contains the result of the sum of products.
     Also, DI points to (QDataIndex+7)%8.
   */
  static vgetty_s_int32 x;          /* local accumulator */
  static vgetty_s_int64 sum;
  int i;

  RV_di = RV_QdataIndex;
  sum = ((vgetty_s_int32) cx) << 16;

  for (i = 0; i < 8; i++)
    {
      x = ((vgetty_s_int32) RV_pzTable[i]) * ((vgetty_s_int32) RV_Qdata[RV_di]);
      x <<= 1;           /* Rockwell-adjust for decimal point shift. */
      sum += x;
      sum = RV_clip_32 (sum);
      RV_di++;
      RV_di %= 8;
    }
  RV_di = (RV_QdataIndex + 7) % 8;
  return (vgetty_s_int32) sum;
}

static void RV_Reset(int bps){
  int i;
  vgetty_u_int16 tmp_int16 = 0;
  vgetty_u_int32 tmp_int32 = 0;

  tmp_int16 = ~tmp_int16;
  tmp_int16 >>= 1;
  RV_max_local_int16 =  tmp_int16;
  RV_min_local_int16 = tmp_int16;
  RV_min_local_int16 = -RV_min_local_int16;
  RV_min_local_int16--;
  tmp_int32 = ~tmp_int32;
  tmp_int32 >>= 1;
  RV_max_local_int32 = tmp_int32;
  RV_min_local_int32 = tmp_int32;
  RV_min_local_int32 = -RV_min_local_int32;
  RV_min_local_int32--;

  RV_QdataIndex = 0;
  for (i = 0; i < RV_QORDER; i++)
    {
      RV_Qdata[i] = 0;
#ifdef POSTFILTER
      RV_QPdata[i] = 0;
      RV_QPPdata[i] = 0;
#endif
    }
  RV_Dempz = 0;
  RV_NewQdata = 0;
  RV_NewAppData = 0;
  RV_LastNu = RV_QDLMN;
  RV_QDelayMX = RV_QDelayTable[bps-2];
  RV_silence_word = RV_silence_words[bps-2];
  RV_mul_p = RV_mul[bps-2];
  RV_Zeta_p = (vgetty_s_int16 *)RV_Zeta[bps-2];
}

#ifdef POSTFILTER

static vgetty_s_int16 RV_P8Table[6] =    /* Adaptive post filter number 1 coefficients */
{0x6666, 0x51eb, 0x4189, 0x346d, 0x29f1, 0x218e};
static vgetty_s_int16 RV_PM5Table[6] =   /* Adaptive post filter number 2 coefficients */
{0xc000, 0xe000, 0xf000, 0xf800, 0xfc00, 0xfe00};

static vgetty_s_int32 RV_App1Calc(vgetty_s_int16 cx){
  /*  Adaptive post filter number 1  */
  /*
     Load pointers to the predictor table and the pointer
     to the coefficient a1.
   */

  static vgetty_s_int32 x;          /* local accumulator */
  static vgetty_s_int64 sum;
  static vgetty_s_int16 di;
  int i;

  di = RV_QdataIndex;
  RV_NewAppData = cx;
  sum = 0;

  for (i = 0; i < 8; i++)
    {
      x = ((vgetty_s_int32) RV_pzTable[i]) * ((vgetty_s_int32) RV_P8Table[i]);
      x <<= 1; /*
             Rockwell-adjust for decimal point shift, then round off
             lower 16 bits to obtain a 16 bit representation of the
             result.
            */
      x = ((vgetty_s_int32)
        (RV_round_32_into_16 (x))) * ((vgetty_s_int32) RV_QPdata[di]);
      x <<= 1;           /* Rockwell-adjust for decimal point shift. */
      sum += x;
      sum = RV_clip_32 (sum);
      di++;
      di %= 8;
    }
  cx = HIWORD(sum);
  di = (RV_QdataIndex + 7) % 8;
  RV_QPdata[di] = cx;         /* drop b6, now a1 Qdata */
  di += 2;
  di %= 8;
  RV_QPdata[di] = RV_NewAppData;   /* drop a2, now b1 Qdata */
  return (vgetty_s_int32) sum;
}

static vgetty_s_int32 RV_App2Calc(vgetty_s_int16 cx){

  /*  Adaptive post filter number 2  */
  /*
     Load pointers to the predictor table and the pointer
     to the coefficient a1.
   */

  static vgetty_s_int32 x;          /* local accumulator */
  static vgetty_s_int64 sum;
  static vgetty_s_int16 di;
  int i;

  di = RV_QdataIndex;
  RV_NewAppData = cx;
  sum = 0;

  for (i = 0; i < 8; i++)
    {
      x = ((vgetty_s_int32) RV_pzTable[i]) * ((vgetty_s_int32) RV_PM5Table[i]);
      x <<= 1; /*
             Rockwell-adjust for decimal point shift, then round off
             lower 16 bits to obtain a 16 bit representation of the
             result.
           */
      x = ((vgetty_s_int32) RV_round_32_into_16 (x)) * ((vgetty_s_int32) RV_QPPdata[di]);
      x <<= 1;           /* Rockwell-adjust for decimal point shift. */
      sum += x;
      sum = RV_clip_32 (sum);
      di++;
      di %= 8;
    }
  cx = HIWORD(sum);
  di = (RV_QdataIndex + 7) % 8;
  RV_QPPdata[di] = cx;   /* drop b6, now a1 Qdata */
  di += 2;
  di %= 8;
  RV_QPPdata[di] = RV_NewAppData;  /* drop a2, now b1 Qdata */
  return (vgetty_s_int32) sum;
}

#endif

static vgetty_s_int16 RV_DecomOne(vgetty_s_int16 ax, vgetty_s_int16 bx){
/*
   RVDecomOne

   Decode a code word.  Local to this module.

   Inputs:
   AX = ML, adaptive multiplier for Nu.
   BX = Zeta, base inverse quantizer value, modified by Nu.
   Also, updates QdataIndex to (QdataIndex+7)%8 .
 */
  static vgetty_s_int16 si;
  static vgetty_s_int32 LastNu_bak;
  static vgetty_s_int32 x;          /* local accumulator */
  static vgetty_s_int64 sum;

  LastNu_bak = RV_LastNu;
  x = ((vgetty_s_int32) ax) * ((vgetty_s_int32) RV_LastNu);
  x <<= 1;               /* Rockwell-adjust for decimal point shift. */
                                        /* Round and Multiply by 4 */
  x = (RV_round_32_into_16 (x) * ((vgetty_s_int32)4));
  RV_LastNu = RV_clip_16 (x);
  if (RV_LastNu < RV_QDLMN)
    RV_LastNu = RV_QDLMN;
  else if (RV_LastNu > RV_QDelayMX)
    RV_LastNu = RV_QDelayMX;

  x = bx * LastNu_bak;   /* Zeta * LastNu */
  x <<= 1;               /* Rockwell-adjust for decimal point shift. */
  x = (RV_round_32_into_16 (x) * ((vgetty_s_int32)4));
  RV_NewQdata = RV_clip_16 (x);
  sum = RV_XpzCalc (RV_NewQdata);  /*  Compute (Xp+z)(n) + Q(n)  */
  si = HIWORD(sum);      /*  Y(n) done, save in SI for later */
#ifdef POSTFILTER
  sum = RV_App1Calc ((vgetty_s_int16)(HIWORD(sum)));
  sum = RV_App2Calc ((vgetty_s_int16)(HIWORD(sum)));
#endif
  /*  Use a de-emphasis filter on Y(n) to remove the effects */
  /*  of the emphasis filter used during compression. */
  x = RV_DEMPCF * RV_Dempz;
  x <<= 1;               /* Rockwell-adjust for decimal point shift. */
  sum += x;
  sum = RV_clip_32 (sum);
  RV_Dempz = HIWORD(sum);
      /*  Update predictor filter coefficients. */
  RV_pzPred (RV_NewQdata);
  RV_Qdata[RV_di] = si;  /*  drop b6, now a1 Qdata */
  /*  Update delay line at the a1(n) table entry position. */
  RV_QdataIndex = RV_di;
  RV_di += 2;
  RV_di %= 8;
  RV_Qdata[RV_di] = RV_NewQdata;   /*  drop a2, now b1 Qdata */
 return RV_Dempz;
}

                 /*          Compression!!         */

/* cd2x, cd3x, and cd4x are combined in cd[][], just like Torsten suggested */

static vgetty_s_int16 RV_cdx[3][16] =
{
  {0x1F69},
  {0x1005, 0x219A, 0x37F0},
  {0x0843, 0x10B8, 0x1996, 0x232B, 0x2DFC, 0x3B02, 0x4CD6}
};
static vgetty_s_int16 RV_cdx_length[3] = { 1,3,7 };
static vgetty_s_int16 *RV_cd;
static vgetty_s_int16 RV_cd_length;

static vgetty_s_int32 RV_ComOne(vgetty_s_int16 ax)
{
/*
   RVComOne

   Code a word into a bps bit codeword.  Local to this module.

   Inputs:
   AX = X(n)

 */
 int i;
 static vgetty_s_int16 adc16z1 = 0; /*         X(n-1)                 */
 static vgetty_s_int16 NewXpz = 0;  /*         Xp+z(n)                */
 static vgetty_s_int16 bx,cx,dx;
 static vgetty_s_int32 x, y;        /* local accumulator */
 static vgetty_s_int64 sum;

 sum = RV_XpzCalc ((vgetty_s_int16)0);   /*  Compute Xp+z(n)  */
 NewXpz = HIWORD(sum);
 x = ((vgetty_s_int32) adc16z1) * RV_PDEMPCF;
 x <<= 1;                  /* Rockwell-adjust for decimal point shift. */
 sum += x;
 sum = RV_clip_32 (sum);
 cx = HIWORD(sum);
               /* Optimise any of this at your own peril ! */
 x = cx;
 x -= ax;
 cx = RV_clip_16 (x);
 bx = cx;
 if (bx<0) bx = -bx;
 cx = -cx;
 adc16z1 = ax;
 y = RV_LastNu;          /*
                    Optimise any of this at your own peril !
                    Ideally, the important variables
                    here should all be declared volatile.
                    If you change any of this, your
                    optimiser might break it !
               */
 for (i=0;i<RV_cd_length;i++){
  x = ((vgetty_s_int32)RV_cd[i]) * y;
  x <<= 1;               /* Rockwell-adjust for decimal point shift. */
                                        /* Round and Multiply by 4 */
  x = (RV_round_32_into_16 (x) * ((vgetty_s_int32)4));
  dx = RV_clip_16 (x);
  if (bx<dx) break;
 }
 i++;
 if (cx<0){
  i -= RV_cd_length;
  i--;
  i = -i;
 } else i+= RV_cd_length;
 ax = i;
 x = ((vgetty_s_int32)RV_mul_p[i]) * y;
 x <<= 1;           /* Rockwell-adjust for decimal point shift. */
                                        /* Round and Multiply by 4 */
 x = (RV_round_32_into_16 (x) * ((vgetty_s_int32)4));
 RV_LastNu = RV_clip_16 (x);
 if (RV_LastNu < RV_QDLMN)
   RV_LastNu = RV_QDLMN;
 else if (RV_LastNu > RV_QDelayMX)
   RV_LastNu = RV_QDelayMX;
 /*            Make a new inverse quantizer value.          */
 x = ((vgetty_s_int32)RV_Zeta_p[i]) * y;
 x <<= 1;           /* Rockwell-adjust for decimal point shift. */
                                        /* Round and Multiply by 4 */
 x = (RV_round_32_into_16 (x) * ((vgetty_s_int32)4));
 cx = RV_clip_16 (x);
               /* Update predictor filter coefficients. */
 RV_pzPred (cx);
               /* Update delay line at the a1(n) table entry position. */
 RV_QdataIndex += 7;
 RV_QdataIndex %= 8;
 x = NewXpz;
 x += cx;
 RV_Qdata[RV_QdataIndex] = RV_clip_16 (x);   /* drop b6, now a1 Qdata. */
 RV_Qdata[(RV_QdataIndex+2)%8] = cx;         /* drop a2, now b1 Qdata. */
 return (vgetty_s_int32) ax;
}

#ifdef RV_HONOUR_SILENCE_CODEWORDS       /* Honour silence codewords and insert the requested silence */

static int put_silence(vgetty_s_int32 num_samples, FILE * out)
{
  /*      Write num_samples 16 bit ints of value 0    */
  num_samples *= 2;

  while (num_samples && (putc (0, out) != EOF))
    num_samples--;

  if (num_samples)
    {
	  LOG_ERR("write silence");
      return (ERROR);
    }
	return (OK);
}

static int getcodeword(FILE * in, vgetty_s_int32 *codeword){
 /*
    Rockwell modems always pass on 16bit ints in little-endian format.
    Therefore, we have to read the data the same way if we don't want
    to miss a silence codeword.
 */
  static int c;
  if ((c = getc (in)) == EOF) return 0;
  *codeword = c;
  if ((c = getc (in)) == EOF) return 8;
  *codeword |= (c<<8);
  return 16;
}

int rockwelltopvf (FILE *fd_in, FILE *fd_out, int nbits, pvf_header *header_out)
{
 vgetty_s_int32 w;        /* packed compressed codewords */
 int c;                  /* single compressed codeword  */
 vgetty_s_int32 mask=(1<<nbits)-1;    /* bitmask for the decompression */
 vgetty_s_int32 a = 0;         /* local accumulator */
 int valbits = 0;        /* number of bits valid in accumulator */

 /* The pvf header should have been written by now, start with the decompression */

 /* Reset the coefficient table for the pole and zero linear predictor. */
 for (w = 0; w < 8; w++) RV_pzTable[w] = 0;
 RV_Reset (nbits);
 /*
          The algorithm below (copied from Torsten Duwe's code)
          takes care of bit concatenation.
 */
 while ((c=getcodeword(fd_in,&w)))
  /*
      Not using the pvf library generic read_bits interface because
      Rockwell modems always pass on 16bit ints in little-endian format.
      Therefore, we have to read the data the same way if we don't want
      to miss a silence codeword.
  */
    {
      if (w == RV_silence_word)
     {
       getcodeword(fd_in,&w);
       put_silence (w, fd_out);
       RV_Reset (nbits);
       valbits = 0;
       continue;
     }
      a |= w<<valbits;
      valbits += c;
      while (valbits >= nbits)
     {
       c = a & mask;
       w = RV_DecomOne(RV_mul_p[c],RV_Zeta_p[c]);
       w <<= 8; /* The pvf routines expect a 24bit int */
       header_out->write_pvf_data(fd_out, w);
       a >>= nbits;
       valbits -= nbits;
     }
    }

  return(OK);
}

#else    /*  Silence codewords are just gobbled up as data */

int rockwelltopvf (FILE *fd_in, FILE *fd_out, int nbits, pvf_header *header_out)
{
 state_t s = init_state;
 int c;                  /* single compressed codeword or EOF */

 /* The pvf header should have been written by now, start with the decompression */

 /* Reset the coefficient table for the pole and zero linear predictor. */
 for (c = 0; c < 8; c++) RV_pzTable[c] = 0;
 RV_Reset (nbits);

 while ((c = read_bits_reverse(fd_in,&s,nbits)) != EOF)
  header_out->write_pvf_data(fd_out,  /* The pvf routines expect a 24bit int */
                    ((vgetty_s_int32)RV_DecomOne(RV_mul_p[c],RV_Zeta_p[c]))<<8 );

  return(OK);
}

#endif

int pvftorockwell (FILE *fd_in, FILE *fd_out, int nbits, pvf_header *header_in)
{
 state_t s = init_state;
 vgetty_s_int32 w;       /* uncompressed audio word */

 /* Reset the coefficient table for the pole and zero linear predictor. */
 for (w = 0; w < 8; w++) RV_pzTable[w] = 0;
 RV_Reset (nbits);
 RV_cd = RV_cdx[nbits-2];
 RV_cd_length = RV_cdx_length[nbits-2];

 /* The rmd header should have been written by now. Do the compression. */

 while (1)
  {
  w = header_in->read_pvf_data(fd_in);
  if (feof(fd_in))
    break;
  /* The pvf routines work on 24bit ints */
  write_bits_reverse(fd_out, &s, nbits, (int)RV_ComOne((vgetty_s_int16)(w>>8)) );
  }

 if (s.nleft > 0) write_bits_reverse(fd_out, &s, 8 - s.nleft, 0x00);

  return(OK);
}

/*
 * I borrowed this code from wav.c - it assumes the pvf is a certain size and
 * I'm not sure this is a good assumption -- Bill Nugent <whn@topelo.lopi.com>
 */

int rockwellpcmtopvf (FILE *fd_in, FILE *fd_out, int nbits, pvf_header *header_out)
{
  int d;
  /* 8 bit PCM */
  while ((d = getc(fd_in)) != EOF)
   {
    d -= 0x80;
    d <<= 16;
    header_out->write_pvf_data(fd_out, d);
  }
  return(OK);
}

int pvftorockwellpcm (FILE *fd_in, FILE *fd_out, int nbits, pvf_header *header_in)
{
  int data;
  /* 8 bit PCM */
          
   while((data = header_in->read_pvf_data(fd_in)) != EOF)
    {
      data >>=16;

      if   (data > 0x7f)
	data = 0x7f;

      if   (data < -0x80)
	data = -0x80;
      putc(data+0x80,fd_out);
  };

  return(OK);
}

void check_system (void)
     {

     /*
      * Check, that right bit shift works properly
      */

     volatile signed int a = -1024;
     volatile signed int b;
     volatile signed int c = 1024;
     volatile signed int d = 10;

     b = a / c;
     c = a >> d;
     assert(b == c);

     /*
      * Check, that the bitsizes are ok
      */

     assert(sizeof(vgetty_s_int16) == 2);
     assert(sizeof(vgetty_u_int16) == 2);
     assert(sizeof(vgetty_s_int32) == 4);
     assert(sizeof(vgetty_u_int32) == 4);
     assert(sizeof(vgetty_s_int64) == 8);
     assert(sizeof(vgetty_u_int64) == 8);

     /*
      * Check, that int is at least 32 bits wide
      */

     assert(sizeof(int) >= 4);
     }


/* Change Speed of voice */
/* Any speed of voice change to 
 */

int pvfspeed(FILE *fd_in)
 {
   FILE *fd_tmp;
   char *tmp_file = VTMP3;
   pvf_header header_in  = init_pvf_header,
              header_out = init_pvf_header;

   double fexponent = 1.0;
   int exponent;
   int speed;
   int srate;

   /* fixed header */
   header_out.ascii = FALSE;
   header_out.nbits = 32;
   header_out.speed = 7200;

   printf("Pass here \n"); 
   if (read_pvf_header(fd_in, &header_in) != OK)
     return(ERROR);

   if (header_in.speed == 0)
     {
       fprintf(stderr, "Pvfspeed: Sample speed is 0!!!\n");
       return(ERROR);
     }

   if((fd_tmp = fopen(tmp_file, "w"))== NULL)
     {
       fclose(fd_tmp);
       fprintf(stderr,"Couldn't Creat temp file %s on disk\n",tmp_file);
       return(ERROR);
     }

   if (write_pvf_header(fd_tmp, &header_out) != OK)
     {
       fclose(fd_tmp);
       
       if (fd_tmp != stdout)
	 unlink(tmp_file);
       
       return(ERROR);
     }

   speed = (int) ((vgetty_s_int64) ONE * header_in.speed /
		  header_out.speed);
   exponent = (int) ((vgetty_s_int64) ONE * fexponent);
   srate = header_in.speed / 10;

   if (speed <= ONE)
     {

       /*
	* slow down the sample, create new values using
	* simple linear interpolation
	*/
       
       int a = 0;
       int b;
       int t;
       int i = 0;

       a = header_in.read_pvf_data(fd_in);
       if (!feof(fd_in))
	 header_out.write_pvf_data(fd_tmp, a);
       
       t = speed;
       
       while (1)
	 {
	   
	   b = header_in.read_pvf_data(fd_in);
	   if (feof(fd_in))
	     break;
	   
	   while (t <= ONE)
	     {
	       header_out.write_pvf_data(fd_tmp, (int) (a +
	       ((((vgetty_s_int64) b - a) * t) >> SHIFT)));
	       t += speed;
	     }
	   
	   t -= ONE;
	   a = b;
	   
	   if (++i == srate)
	     {
	       i -= srate;
	       speed = (speed * exponent) >> SHIFT;
	     }
	   
	 }
       
     }
   else
     {
       
       /*
	* speed up the sample, use averaging
	*/
       
       int t = 0;
       int sum = 0;
       int i = 0;
       int b;
       
       while (1)
	 {
	   b = header_in.read_pvf_data(fd_in);
	   if (feof(fd_in))
	     break;
	   t += ONE;
	   
	   if (t >= speed)
	     {
	       t -= speed;
	       sum += b * ((vgetty_s_int64) ONE - t) >> SHIFT;
	       header_out.write_pvf_data(fd_tmp, (vgetty_s_int64) sum *
			  ONE / speed);
	       sum = ((vgetty_s_int64) b * t) >> SHIFT;
	     }
	   else
	     sum += b;
	   
	   if (++i == srate)
	     {
	       i -= srate;
	       speed = (speed * exponent) >> SHIFT;
	     }
	   
	 }
       
     }
   fclose(fd_tmp);
   return(OK);
   
 }

int waveToAdpcm(FILE *fd_in, FILE *fd_out)
{
  
  FILE *fd_tmp;
  char *tmp_file = VTMP1;

  int compression = 4;

  pvf_header header = init_pvf_header;
  /* header is set as fixed volue */
  header.ascii = FALSE;
  header.nbits = 32;
  header.speed = 7200;

  check_system();

  if((fd_tmp = fopen(tmp_file, "w"))== NULL)
    {
      fclose(fd_tmp);
      fprintf(stderr,"Couldn't Creat temp file %s on disk\n",tmp_file);
      return(ERROR);
    }


  /* wav file -> pvf file */
  if (wavtopvf(fd_in, fd_tmp, &header) != OK)
    {
      fclose(fd_tmp);
      
      if (fd_tmp != stdout)
	unlink(tmp_file);
      
      return(ERROR);
    }

  /* reset file point to zero for next step */
  /*
  if(fseek(fd_tmp,0,0)!=OK)
      fprintf(stderr,"Error at fseek file\n");
  */
  freopen(tmp_file,"r",fd_tmp);

  /* Change voice speed(pvf file) to 7200 */

  if(header.speed != 7200)
    {
      if(pvfspeed(fd_tmp) != OK)
	{
	  fclose(fd_tmp);
	  unlink(tmp_file);
	  fprintf(stderr, "Pvf speed convertion error\n");
	  return(ERROR);
	}
      else
	{
	  fclose(fd_tmp);
	  unlink(tmp_file);

	  if((fd_in = fopen(VTMP3, "r"))==NULL)
	    {
	      fprintf(stderr, "IO error! Couldn't open %s file\n", VTMP3);
	      return(ERROR);
	    }
	}
      
    }

  if (read_pvf_header(fd_tmp, &header) != OK)
    return(ERROR);

  /* pvf file -> rmd file */

  if (header.speed != 7200)
    {
      fprintf(stderr, "In Wav2rmd  Unsupported sample speed (%d)\n",
	      header.speed);
      fclose(fd_tmp);
      if (fd_tmp != stdout)
	unlink(tmp_file);

      return(ERROR);
    };

  if (pvftorockwell(fd_tmp, fd_out, compression, &header) != OK)
    {
      fclose(fd_tmp);
      if (fd_tmp != stdout)
	unlink(tmp_file);

      return(ERROR);
    }

  fclose(fd_tmp);
  if (fd_tmp != stdout)
    unlink(tmp_file);

  return(OK);

}


int adpcmToWave(FILE *fd_in, FILE *fd_out)
{

  FILE *fd_tmp;
  char *tmp_file  = VTMP2;
  int nbits = 4;
  int wav_bits = 8;

  pvf_header header_out = init_pvf_header;

  
  if((fd_tmp = fopen(tmp_file, "w"))== NULL)
    {
      fclose(fd_tmp);
      fprintf(stderr,"Couldn't Creat temp file %s on disk\n",tmp_file);
      return(ERROR);
    }


  /* fixed pvf header_out */
  header_out.ascii = FALSE;
  header_out.nbits = 8;
  header_out.speed = 7200;


  /* Creating a pvf header */
  if (write_pvf_header(fd_tmp, &header_out) != OK)
    {
      fclose(fd_tmp);
      
      if (fd_tmp != stdout)
	unlink(tmp_file);
      
      return(ERROR);
    }
    
  if (rockwelltopvf(fd_in, fd_tmp, nbits, &header_out) != OK)
    {
      fprintf(stderr,"Coundn't convert voice from raw data to pvf file\n");
      fclose(fd_tmp);
      
      if (fd_tmp != stdout)
	unlink(tmp_file);
      
      return(ERROR);
    }

  if(freopen(tmp_file, "r", fd_tmp) == NULL)
    {
      fprintf(stderr,"Coundn't reopen temp voice file\n");
      fclose(fd_tmp);
      
      if (fd_tmp != stdout)
	unlink(tmp_file);
      
      return(ERROR);
    }

  if (read_pvf_header(fd_tmp, &header_out) != OK)
    return(ERROR);


  /* pvf file -> wav file  */

  if (pvftowav(fd_tmp, fd_out, &header_out, wav_bits) != OK)
    {
      fprintf(stderr,"Coundn't convert voice from pvf data to wav file\n");

      fclose(fd_tmp);
      if (fd_tmp != stdout)
	unlink(tmp_file);
      
      return(ERROR);
    }      

  fclose(fd_tmp);
  if (fd_tmp != stdout)
    unlink(tmp_file);

  return(OK);

}



/*
	convert WAVE file to ADPCM file
	return -> 0:ok -1:error
*/
int Wave_Adpcm(char *wave_filename, char *adpcm_filename)
{
	FILE *fp_in, *fp_out;
	int ret=0;

	// open input file
	fp_in = fopen(wave_filename, "rb");
	if (fp_in == NULL) {
		sprintf(errbuf, "Can't open %s", wave_filename);
		LOG_ERR(errbuf);
		return -1;
	}
	flock(fileno(fp_in), LOCK_SH);	// lock

	// open output file
	fp_out = fopen(adpcm_filename, "wb");
	if (fp_out == NULL) {
		sprintf(errbuf, "Can't open %s", adpcm_filename);
		LOG_ERR(errbuf);
		flock(fileno(fp_in), LOCK_UN);
		return -1;
	}
	flock(fileno(fp_out), LOCK_EX);	// lock

	// convert
	ret = waveToAdpcm(fp_in, fp_out);

	// close
	flock(fileno(fp_out), LOCK_UN);
	fclose(fp_out);
	flock(fileno(fp_in), LOCK_UN);
	fclose(fp_in);

	return(ret);
}


/*
	convert ADPCM file to WAVE file
	return -> 0:ok -1:error
*/
int Adpcm_Wave(char *adpcm_filename, char *wave_filename)
{
	FILE *fp_in, *fp_out;
	int ret=0;

	// open input file
	fp_in = fopen(adpcm_filename, "rb");
	if (fp_in == NULL) {
		sprintf(errbuf, "Can't open %s", adpcm_filename);
		LOG_ERR(errbuf);
		return -1;
	}
	flock(fileno(fp_in), LOCK_SH);	// lock

	// open output file
	fp_out = fopen(wave_filename, "wb");
	if (fp_out == NULL) {
		sprintf(errbuf, "Can't open %s", wave_filename);
		LOG_ERR(errbuf);
		flock(fileno(fp_in), LOCK_UN);
		return -1;
	}
	flock(fileno(fp_out), LOCK_EX);	// lock

	// convert
	ret = adpcmToWave(fp_in, fp_out);

	// close
	flock(fileno(fp_out), LOCK_UN);
	fclose(fp_out);
	flock(fileno(fp_in), LOCK_UN);
	fclose(fp_in);

	return(ret);
}








