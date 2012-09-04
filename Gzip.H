/*
 * ** Copyright (C) 2012 Shinichi Ishida, All Rights Reserved.
 * **
 * ** Please see the LICENSE file distributed with this source
 * ** code archive for information covering the modification and
 * ** redistribution of this file and binaries built from it.
 * */

/*
 * $Id: Gzip.H,v 5.2 2011-11-16 05:59:23 sin Exp $
*/

#ifndef Gzip_H
#define Gzip_H


#define FILEBUFSIZ   1024 * 1024         /* 入力バッファサイズ（任意） */
#define INBUFSIZ   1024          /* 入力バッファサイズ（任意） */

#define TMPBUFSIZ 200

/* gzip flag byte */
#define GZ_MAGIC_0	0x1f
#define GZ_MAGIC_1	0x8b
#define ASCII_FLAG   0x01 /* bit 0 set: file probably ascii text */
#define HEAD_CRC     0x02 /* bit 1 set: header CRC present */
#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present */
#define ORIG_NAME    0x08 /* bit 3 set: original file name present */
#define COMMENT      0x10 /* bit 4 set: file comment present */
#define RESERVED     0xE0 /* bits 5..7: reserved */

class Gzip{
	public:
		u_char outbuffer[SAVE_MAX_SIZE];
		Gzip();
		~Gzip();
		int dec_init(z_stream *z);
		int dec_gzip(u_char* outbuf, u_char* inbuf, int len, z_stream *z);
		int dec_zlib(u_char* outbuf, u_char* inbuf, int len, z_stream *z);
		int dec_deflate(u_char* outbuf, u_char* inbuf, int size, z_stream *z);

	private:

};

#endif
