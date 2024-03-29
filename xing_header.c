/*
 * XING header manipulation routines
 *
 * Copyright (C) 2001-2004, Adam Bernstein
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA  02111-1307  USA.
 *
 * http://gabriel.mp3-tech.org/mp3infotag.html
 */

#if 0
#define __do_graffiti 1
#endif

#define MPEG1_STEREO_OFFSET (32+4)
#define MPEG1_MONO_OFFSET   (17+4)
#define MPEG2_STEREO_OFFSET (17+4)
#define MPEG2_MONO_OFFSET   (9+4)

static unsigned char xing_header_stereo_44100[] = {
      0xff, 0xfb, 0x50, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x58, 0x69, 0x6e, 0x67, 0x00, 0x00, 0x00, 0x0f,
      0x00, 0x00, 0x04, 0xf0, 0x00, 0x09, 0x19, 0x6c, 0x00, 0x02, 0x04,
      0x06, 0x08, 0x0a, 0x0d, 0x0f, 0x12, 0x15, 0x17, 0x1a, 0x1d, 0x20,
      0x23, 0x26, 0x29, 0x2c, 0x2f, 0x32, 0x34, 0x37, 0x3a, 0x3c, 0x3f,
      0x41, 0x43, 0x45, 0x48, 0x4b, 0x4e, 0x50, 0x53, 0x56, 0x59, 0x5c,
      0x5f, 0x62, 0x64, 0x66, 0x69, 0x6b, 0x6e, 0x71, 0x73, 0x75, 0x77,
      0x79, 0x7b, 0x7e, 0x80, 0x82, 0x85, 0x87, 0x8a, 0x8c, 0x8f, 0x91,
      0x93, 0x96, 0x98, 0x9a, 0x9c, 0x9f, 0xa1, 0xa3, 0xa6, 0xa7, 0xa9,
      0xac, 0xae, 0xb1, 0xb3, 0xb6, 0xb9, 0xbc, 0xbf, 0xc2, 0xc5, 0xc7,
      0xca, 0xcc, 0xcf, 0xd2, 0xd4, 0xd7, 0xd9, 0xdc, 0xde, 0xe1, 0xe4,
      0xe6, 0xe9, 0xec, 0xef, 0xf3, 0xf6, 0xf9, 0xfc, 0xfe, 0x00, 0x00,
      0x00, 0x42, 0x4d, 0x50, 0x47, 0x45, 0x44, 0x49, 0x54, 0x20, 0x30,
      0x2e, 0x37, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char xing_header_mono_44100[] = {
      0xff, 0xfb, 0x50, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58,
      0x69, 0x6e, 0x67, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x04, 0xf1,
      0x00, 0x04, 0x56, 0xa7, 0x00, 0x02, 0x04, 0x06, 0x07, 0x09, 0x0b,
      0x0d, 0x0e, 0x11, 0x13, 0x16, 0x18, 0x1a, 0x1c, 0x1e, 0x20, 0x22,
      0x24, 0x25, 0x27, 0x29, 0x2c, 0x2d, 0x2f, 0x31, 0x33, 0x36, 0x39,
      0x3b, 0x3e, 0x41, 0x43, 0x45, 0x47, 0x49, 0x4b, 0x4d, 0x4f, 0x51,
      0x54, 0x56, 0x59, 0x5b, 0x5d, 0x60, 0x62, 0x65, 0x68, 0x6b, 0x6e,
      0x71, 0x73, 0x76, 0x79, 0x7b, 0x7d, 0x80, 0x82, 0x84, 0x88, 0x8a,
      0x8d, 0x90, 0x93, 0x96, 0x99, 0x9c, 0x9f, 0xa2, 0xa5, 0xa9, 0xac,
      0xaf, 0xb2, 0xb6, 0xb9, 0xbc, 0xbf, 0xc3, 0xc6, 0xc9, 0xcd, 0xcf,
      0xd2, 0xd6, 0xd8, 0xdc, 0xdf, 0xe1, 0xe4, 0xe7, 0xea, 0xed, 0xf1,
      0xf3, 0xf6, 0xf9, 0xfb, 0xfd, 0x00, 0x00, 0x00, 0x2c, 0x4d, 0x50,
      0x47, 0x45, 0x44, 0x49, 0x54, 0x20, 0x30, 0x2e, 0x37, 0x32, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char xing_header_stereo_22050[] = {
      0xff, 0xf3, 0x80, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 
      0x69, 0x6e, 0x67, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x04, 0xf2, 
      0x00, 0x05, 0x03, 0xfa, 0x00, 0x02, 0x04, 0x06, 0x07, 0x09, 0x0a, 
      0x0c, 0x0e, 0x10, 0x13, 0x16, 0x17, 0x19, 0x1b, 0x1d, 0x20, 0x22, 
      0x25, 0x27, 0x29, 0x2c, 0x2e, 0x30, 0x31, 0x33, 0x36, 0x38, 0x3a, 
      0x3d, 0x40, 0x43, 0x46, 0x49, 0x4c, 0x4e, 0x51, 0x53, 0x56, 0x58, 
      0x5b, 0x5d, 0x5f, 0x61, 0x63, 0x66, 0x69, 0x6c, 0x6f, 0x72, 0x75, 
      0x79, 0x7c, 0x7f, 0x82, 0x85, 0x88, 0x8b, 0x8e, 0x91, 0x95, 0x98, 
      0x9b, 0x9e, 0xa2, 0xa5, 0xa8, 0xaa, 0xad, 0xaf, 0xb2, 0xb4, 0xb7, 
      0xb9, 0xbb, 0xbe, 0xc0, 0xc2, 0xc5, 0xc7, 0xc9, 0xcc, 0xce, 0xd1, 
      0xd4, 0xd7, 0xd9, 0xdc, 0xdf, 0xe1, 0xe3, 0xe6, 0xe8, 0xea, 0xed, 
      0xef, 0xf3, 0xf6, 0xfa, 0xfd, 0x00, 0x00, 0x00, 0x2c, 0x4d, 0x50, 
      0x47, 0x45, 0x44, 0x49, 0x54, 0x20, 0x30, 0x2e, 0x37, 0x32, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char xing_header_mono_22050[] = {
      0xff, 0xf3, 0x80, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x58, 0x69, 0x6e, 0x67, 0x00, 0x00, 0x00, 0x0f, 0x00, 
      0x00, 0x04, 0xf2, 0x00, 0x02, 0x89, 0x3e, 0x00, 0x02, 0x04, 0x06, 
      0x07, 0x09, 0x0b, 0x0d, 0x0f, 0x11, 0x13, 0x17, 0x18, 0x1a, 0x1d, 
      0x1f, 0x22, 0x24, 0x26, 0x28, 0x2a, 0x2d, 0x2f, 0x30, 0x32, 0x35, 
      0x37, 0x3a, 0x3c, 0x3f, 0x42, 0x44, 0x48, 0x4a, 0x4d, 0x50, 0x52, 
      0x54, 0x57, 0x59, 0x5c, 0x5e, 0x5f, 0x61, 0x64, 0x67, 0x6a, 0x6e, 
      0x70, 0x73, 0x77, 0x7a, 0x7d, 0x7f, 0x83, 0x86, 0x89, 0x8b, 0x8e, 
      0x91, 0x94, 0x97, 0x9a, 0x9d, 0xa0, 0xa3, 0xa6, 0xa9, 0xac, 0xae, 
      0xb1, 0xb4, 0xb6, 0xb9, 0xbb, 0xbe, 0xc0, 0xc3, 0xc5, 0xc8, 0xca, 
      0xcd, 0xd0, 0xd2, 0xd5, 0xd7, 0xda, 0xdc, 0xdf, 0xe1, 0xe4, 0xe6, 
      0xe8, 0xeb, 0xed, 0xf0, 0xf3, 0xf7, 0xfa, 0xfd, 0x00, 0x00, 0x00, 
      0x2c, 0x4d, 0x50, 0x47, 0x45, 0x44, 0x49, 0x54, 0x20, 0x30, 0x2e, 
      0x37, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char xing_header_stereo_11025[] = {
      0xff, 0xe3, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x58, 
      0x69, 0x6e, 0x67, 0x00, 0x00, 0x00, 0x0f, 0x00, 0x00, 0x02, 0x7b, 
      0x00, 0x02, 0xd4, 0x11, 0x00, 0x04, 0x06, 0x09, 0x0b, 0x0d, 0x10, 
      0x12, 0x15, 0x18, 0x1a, 0x1c, 0x1f, 0x20, 0x22, 0x24, 0x26, 0x28, 
      0x2b, 0x2d, 0x30, 0x33, 0x36, 0x38, 0x3b, 0x3f, 0x41, 0x44, 0x46, 
      0x48, 0x4b, 0x4d, 0x4f, 0x52, 0x54, 0x57, 0x5b, 0x5e, 0x61, 0x65, 
      0x68, 0x6a, 0x6d, 0x6f, 0x71, 0x74, 0x77, 0x7a, 0x7d, 0x7f, 0x82, 
      0x84, 0x87, 0x8a, 0x8c, 0x8e, 0x91, 0x95, 0x99, 0x9d, 0xa0, 0xa2, 
      0xa5, 0xa8, 0xaa, 0xae, 0xb0, 0xb2, 0xb4, 0xb7, 0xb9, 0xbc, 0xbf, 
      0xc1, 0xc3, 0xc5, 0xc7, 0xca, 0xcc, 0xce, 0xd0, 0xd2, 0xd4, 0xd7, 
      0xd9, 0xdc, 0xde, 0xe0, 0xe2, 0xe5, 0xe7, 0xe9, 0xeb, 0xed, 0xef, 
      0xf2, 0xf4, 0xf6, 0xf9, 0xfb, 0x00, 0x00, 0x00, 0x58, 0x4d, 0x50, 
      0x47, 0x45, 0x44, 0x49, 0x54, 0x20, 0x30, 0x2e, 0x37, 0x32, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x24, 0x04, 0xc3, 
      0x25, 0x00, 0x00, 0x00, 0x00, 0x02, 0xd4, 0x11, 0xbd, 0xc0, 0xe9, 
      0xf6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

static unsigned char xing_header_mono_11025[] = {
      0xff, 0xe3, 0x40, 0xc4, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x58, 0x69, 0x6e, 0x67, 0x00, 0x00, 0x00, 0x0f, 0x00, 
      0x00, 0x02, 0x7b, 0x00, 0x01, 0x4f, 0x02, 0x00, 0x05, 0x07, 0x09, 
      0x0b, 0x0d, 0x0f, 0x11, 0x13, 0x17, 0x19, 0x1c, 0x1f, 0x20, 0x22, 
      0x24, 0x26, 0x28, 0x2b, 0x2e, 0x31, 0x34, 0x36, 0x38, 0x3b, 0x3c, 
      0x3e, 0x41, 0x43, 0x46, 0x49, 0x4b, 0x4e, 0x50, 0x52, 0x55, 0x59, 
      0x5c, 0x5f, 0x63, 0x66, 0x68, 0x6a, 0x6c, 0x6e, 0x70, 0x73, 0x76, 
      0x7a, 0x7d, 0x7f, 0x82, 0x84, 0x87, 0x89, 0x8c, 0x8e, 0x91, 0x95, 
      0x99, 0x9b, 0x9d, 0xa0, 0xa2, 0xa5, 0xa8, 0xab, 0xad, 0xaf, 0xb2, 
      0xb4, 0xb7, 0xba, 0xbc, 0xbe, 0xc1, 0xc3, 0xc6, 0xc9, 0xcb, 0xcd, 
      0xd0, 0xd2, 0xd5, 0xd8, 0xda, 0xdc, 0xde, 0xe1, 0xe4, 0xe6, 0xe8, 
      0xea, 0xed, 0xef, 0xf2, 0xf4, 0xf7, 0xf9, 0xfc, 0x00, 0x00, 0x00, 
      0x58, 0x4d, 0x50, 0x47, 0x45, 0x44, 0x49, 0x54, 0x20, 0x30, 0x2e, 
      0x37, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 
      0x24, 0x05, 0x04, 0x21, 0x00, 0x00, 0x00, 0x00, 0x01, 0x4f, 0x02, 
      0x1c, 0xf2, 0xd5, 0x2b, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };




#ifndef lint
static char SccsId[] = "$Id: xing_header.c,v 1.10.6.2 2009/04/02 03:14:02 number6 Exp $";
#endif

#include <stdio.h>
#include <string.h>
#include "xing_header.h"
#include "editif.h"
#include "mpegstat.h"
#ifdef __do_graffiti
#include "version.h"
#include "mpgedit_buildnum.h"
#endif

#if 0
#define UNIT_TEST 1
#endif

static int sr_table[4] = { 44100, 48000, 32000, 99999 };

/*  big endian insert */

void InsertI4(int x, unsigned char *buf)
{
    buf[3] = x & 0xff;
    x >>= 8;

    buf[2] = x & 0xff;
    x >>= 8;

    buf[1] = x & 0xff;
    x >>= 8;

    buf[0] = x & 0xff;
}

/*  big endian extract */

int ExtractI4(unsigned char *buf)
{
    int x;

    x = buf[0];
    x <<= 8;
    x |= buf[1];
    x <<= 8;
    x |= buf[2];
    x <<= 8;
    x |= buf[3];

    return x;
}

/*  big endian 2 byte insert */

void InsertI2(int x, unsigned char *buf)
{
    buf[1] = x & 0xff;
    x >>= 8;
    buf[0] = x & 0xff;
}

/*  big endian 2 byte extract */

int ExtractI2(unsigned char *buf)
{
    int x;

    x = buf[0];
    x <<= 8;
    x |= buf[1];
    return x;
}

/*  big endian 20 bit insert */

void InsertB20(int x, unsigned char *buf)
{
    buf[2] = x & 0xf;
    x >>= 4;

    buf[1] = x & 0xff;
    x >>= 8;

    buf[0] = x & 0xff;
}

/*  big endian extract */

int ExtractB20(unsigned char *buf)
{
    int x;

    x = buf[0];
    x <<= 8;
    x |= buf[1];
    x <<= 8;
    x |= buf[2] & 0x4;
    return x;
}

/*  big endian 12 bit insert */

void InsertB12(int x, unsigned char *buf)
{
    buf[1] = x & 0xf;
    x >>= 4;

    buf[0] = x & 0xff;
}

/*  big endian 12 bit extract */

int ExtractB12(unsigned char *buf)
{
    int x;

    x = buf[0];
    x <<= 8;

    x |= buf[1] & 0x4;

    return x;
}


/*  big endian 22bit/10bit insert */

void InsertB2210(unsigned char *buf, int x, int y)
{
    buf[3] =   y & 0xff;
    y >>= 8;

    buf[2]  = (y & 0x03) | (x & 0x3f)<<2;
    x >>= 6;

    buf[1] =   x & 0xff;
    x >>= 8;

    buf[0] =   x & 0xff;
}


/*  big endian 22bit/10bit extract */

void ExtractB2210(unsigned char *buf, int *rx, int *ry)
{
    int x;
    int y;

    x   = (buf[2] & 0xfc)>>2;
    x  |=  buf[1]<<6;
    x  |=  buf[0]<<14;

    y   =  buf[3];
    y  |= (buf[2] & 0x03)<<8;

    *rx = x;
    *ry = y;
}


/*
 * Poke new values for frames and file size into an already populated
 * and valid Xing header. 'version' is the raw mpeg version value
 * from the decoded header. 'mode' is the raw mpeg channel mode value
 * from the decoded header.
 */
void edit_xing_header(int frames, int bytes, 
                      unsigned char *buf, int len,
                      int version, int mode, int is_vbr)
{
    int offset = 44;
#ifdef __do_graffiti
    int i;
    char graffiti[128];
#endif

    switch (version) {
      case 3:                /* mpeg 1 */
        switch (mode) {
          case 3:            /* mono */
            offset = MPEG1_MONO_OFFSET;
            break;
          default:
            offset = MPEG1_STEREO_OFFSET;     /* stereo/joint/dual channel */
            break;
        }
        break;
      default:               /* mpeg 2/2.5 */
        switch (mode) {
          case 3:            /* mono */
            offset = MPEG2_MONO_OFFSET;
            break;
          default:           /* stereo/joint/dual channel */
            offset = MPEG2_STEREO_OFFSET;
            break;
        }
        break;
    }

    /* Set 'Xing' tag to 'Info' when file is not VBR */
    if (is_vbr == 0) {
        memcpy(&buf[offset], "Info", 4);
    }
    else {
        /* Set 'Xing' tag to 'Xing' when file is is VBR */
        memcpy(&buf[offset], "Xing", 4);
    }

    offset += 8;
    InsertI4(frames, &buf[offset]);
    InsertI4(bytes,  &buf[offset+4]);

#ifdef __do_graffiti
/*
 * adam/TBD: Not ready to put this into production. This change
 * invalidates the regression test scripts, because the split/spliced
 * file is no longer identical to the original.
 */
    /*
     * Add mpgedit "graffiti"
     */
    sprintf(graffiti, "%s %s",
            MPGEDIT_PRODUCT,
            MPGEDIT_VERSION);

    /*
     * Count how much zeroed space is present from the
     * end of frame to the first non-zero byte. Does
     */
    for (i=len-1; i>0 && !buf[i]; i--)
        ;

    i+= 2;
    if (strlen(graffiti) < len-1) {
        /*
         * Graffiti fits in this empty space.
         * Yes, the NULL is NOT copied
         */
        memcpy(buf+i, graffiti, strlen(graffiti));
    }
#endif
}


int xingheader_file_make3(char *file, int channel_mode, int sample_rate)
{
    FILE *fp;
    unsigned char xing_header_buf[sizeof(xing_header_stereo_44100)];
    int sts;

    switch (sample_rate) {
      case 44100:
        if (channel_mode == 3) {
            memcpy(xing_header_buf, xing_header_mono_44100,
                   sizeof(xing_header_mono_44100));
        }
        else {
            memcpy(xing_header_buf, xing_header_stereo_44100,
                   sizeof(xing_header_stereo_44100));
        }
        break;

      case 22050:
        if (channel_mode == 3) {
            memcpy(xing_header_buf, xing_header_mono_22050,
                   sizeof(xing_header_mono_22050));
        }
        else {
            memcpy(xing_header_buf, xing_header_stereo_22050,
                   sizeof(xing_header_stereo_22050));
        }
        break;

      case 11025:
        if (channel_mode == 3) {
            memcpy(xing_header_buf, xing_header_mono_11025,
                   sizeof(xing_header_mono_11025));
        }
        else {
            memcpy(xing_header_buf, xing_header_stereo_11025,
                   sizeof(xing_header_stereo_11025));
        }
        break;

      default:
        memcpy(xing_header_buf, xing_header_stereo_44100,
               sizeof(xing_header_stereo_44100));
       
     }
        
    fp = fopen(file, "wb");
    if (!fp) {
        return -1;
    }

    sts = fwrite(xing_header_buf, sizeof(xing_header_stereo_44100), 1, fp);
    if (sts != 1) {
        fclose(fp);
        return -1;
    }
    
    fclose(fp);
    return 0;
}


int xingheader_file_make(char *file)
{
    return xingheader_file_make3(file, 0, 44100);
}


/* Return TRUE if valid Xing header is found, FALSE otherwise */

static int get_xing_header(unsigned char *buf, XHEADDATA *X)
{
    int h_id;
    int h_id2;
    int h_layer;
    int h_protect;
    int h_sr_index;
    int h_mode;
    int head_flags;
    int i;

    /* 11111111 111BBCCD EEEEFFGH IIJJKLMM  */

    /* BB: MPEG audio version ID */
    h_id       = (buf[1] >> 3) & 1;
    h_id2      = (buf[1] >> 3) & 2;

    /* CC: Layer description */
    h_layer    = (buf[1] >> 1) & 3;

    /* D: Protection bit */
    h_protect  =  buf[1]       & 1;

    /* FF: Sampling rate frequency index */
    h_sr_index = (buf[2] >> 2) & 3;

    /* II: Channel mode */
    h_mode     = (buf[3] >> 6) & 3;
    X->h_mode  = h_mode;

    /* Sideinfo length */
    if (h_id) {        /*  mpeg1 */
        if (h_mode != 3) {
            /* Stereo */
            buf += MPEG1_STEREO_OFFSET; /* 36 */
        }
        else {
            buf += MPEG1_MONO_OFFSET;   /* 21 */
        }
    }
    else {      /* mpeg2 */
        if (h_mode != 3) {
            /* Stereo */
            buf += MPEG2_STEREO_OFFSET; /* 21 */
        }
        else {
            buf += MPEG2_MONO_OFFSET;   /* 13 */
        }
    }

    if (h_layer == 3) {
        X->h_layer = 1;
    }
    else if (h_layer == 2) {
        X->h_layer = 2;
    }
    else if (h_layer == 1) {
        X->h_layer = 3;
    }
    else {
        X->h_layer = 0;
    }

    X->h_protect = !h_protect;

    /* fail, header not found */

    if (buf[0]!='X' || buf[1]!='i' || buf[2]!='n' || buf[3]!='g') {
        if (buf[0]!='I' || buf[1]!='n' || buf[2]!='f' || buf[3]!='o') {
            return 0;
        }
        else {
            X->xing_info_header = 1;
        }
    }
    else {
        X->xing_info_header = 0;
    }
    
    buf+=4;

    X->h_id = h_id | h_id2;
    X->samprate = sr_table[h_sr_index];
    if (h_id == 0) {
        X->samprate >>= 1;
        if (h_id2 == 0) X->samprate >>= 1;
    }

    /* get flags */

    head_flags = X->flags = ExtractI4(buf);
    buf+=4;

    if (head_flags & FRAMES_FLAG) {
        X->frames  = ExtractI4(buf);
        buf+=4;
    }
    if (head_flags & BYTES_FLAG) {
        X->bytes = ExtractI4(buf);
        buf+=4;
    }

    if (head_flags & TOC_FLAG) {
        if (X->toc != NULL) {
            for(i=0;i<100;i++) {
                X->toc[i] = buf[i];
            }
        }
        buf+=100;
    }

    X->vbr_scale = -1;
    if (head_flags & VBR_SCALE_FLAG)  {
       X->vbr_scale = ExtractI4(buf); buf+=4;
    }
    return 1;
}


static void xing_header2str(XHEADDATA *X, char *cp)
{
    cp += sprintf(cp, "h_id      = %d\n", X->h_id);
    cp += sprintf(cp, "h_layer   = %d\n", X->h_layer);
    cp += sprintf(cp, "h_protect = %d\n", X->h_protect);
    cp += sprintf(cp, "samprate  = %d\n", X->samprate);
    cp += sprintf(cp, "flags     = %d\n", X->flags);
    cp += sprintf(cp, "frames    = %d\n", X->frames);
    cp += sprintf(cp, "bytes     = %d\n", X->bytes);
    cp += sprintf(cp, "vbr_scale = %d\n", X->vbr_scale);
}


int xingheader_init(unsigned char *buf, int len, XHEADDATA *X)
{
    XHEADDATA xingh;
    int rval;

    if (!X) {
        memset(&xingh, 0, sizeof(xingh));
        memcpy(xingh.xingbuf, buf, len);
        xingh.xingbuflen = len;
        rval = get_xing_header(buf, &xingh);
    }
    else {
        if (buf != X->xingbuf) {
            memmove(X->xingbuf, buf, len);
        }
        X->xingbuflen = len;
        rval = get_xing_header(buf, X);
    }
    return rval;
}

void xingheader_stats_edit(XHEADDATA *xingh,
                           mpeg_file_stats *stats,
                           int frames,
                           int bytes,
                           int id,
                           int mode)
{
    edit_xing_header(frames, bytes, xingh->xingbuf,
                     xingh->xingbuflen, id, mode,
                     mpeg_file_stats_is_vbr(stats));
}

void xingheader_edit(XHEADDATA *xingh, int frames, int bytes, int id, int mode)
{
    mpeg_file_stats *stats = mpgedit_edit_stats_ctx(xingh->play_ctx);

    xingheader_stats_edit(xingh, stats, frames, bytes, id, mode);
}



void xingheader2str(XHEADDATA *xingh, char *cp)
{
    xing_header2str(xingh, cp);
}


int xingheader_init_mpgedit_ctx(XHEADDATA *xingh, void *play_ctx)
{
    if (!xingh || !play_ctx) {
        return -1;
    }

    xingh->play_ctx = play_ctx;
    return 0;
}


#if defined(UNIT_TEST)
#include <stdlib.h>
int main(int argc, char *argv[])
{
    int sts = 0; 
#define MAX_XING_HEADER_SIZE 2048
    unsigned char buf[MAX_XING_HEADER_SIZE];
    FILE *fp;
    int items;
    XHEADDATA X;
    int i;
    int frame_add = 0;
    int bytes_add = 0;
    int frames = 0;
    int bytes = 0;
    char *filename;
    char *cp;

    memset(buf, 0, sizeof(buf));
    InsertI4(0x010f03f0, buf); 

    memset(&X, 0, sizeof(X));

    if (argc == 1 || argc > 4) {
        printf("usage: %s mp3_file [[+]frames] [[+]bytes]\n", argv[0]);
        return 1;
    }
    filename = argv[1];

    if (argc > 2) {
        cp = argv[2];
        if (*cp == '+') {
            frame_add = 1;
            cp++;
        }
        frames = atoi(cp);
    }
    if (argc == 4) {
        cp = argv[3];
        if (*cp == '+') {
            bytes_add = 1;
            cp++;
        }
        bytes = atoi(cp);
    }

    fp = fopen(filename, "rb+");
    if (!fp) {
        perror("fopen");
        return 1;
    }
    items = fread(buf, 1, MAX_XING_HEADER_SIZE, fp);

    for (i = 0; i < 64; i++) {
        if ((i%16) == 0) {
            printf("\n");
        }
        printf("%02x ", buf[i]);
    }
    printf("\n");
    xingheader_init(buf, items, &X);

    printf("h_id      = %d\n", X.h_id);
    printf("h_layer   = %d\n", X.h_layer);
    printf("h_protect = %d\n", X.h_protect);
    printf("samprate  = %d\n", X.samprate);
    printf("flags     = %d\n", X.flags);
    printf("frames    = %d\n", X.frames);
    printf("bytes     = %d\n", X.bytes);
    printf("vbr_scale = %d\n", X.vbr_scale);
    printf("==================================\n");

    if (frame_add) {
        frames += X.frames;
    }
    if (bytes_add) {
        bytes += X.bytes;
    }
    if (frames) {
        if (bytes == 0) {
            bytes = X.bytes;
        }

        xingheader_edit(&X, frames, bytes, X.h_id, X.h_mode);
        xingheader_init(buf, items, &X);
        printf("h_id      = %d\n", X.h_id);
        printf("h_layer   = %d\n", X.h_layer);
        printf("h_protect = %d\n", X.h_protect);
        printf("samprate  = %d\n", X.samprate);
        printf("flags     = %d\n", X.flags);
        printf("frames    = %d\n", X.frames);
        printf("h_id      = %d\n", X.h_id);
        printf("h_layer   = %d\n", X.h_layer);
        printf("h_protect = %d\n", X.h_protect);
        printf("samprate  = %d\n", X.samprate);
        printf("flags     = %d\n", X.flags);
        printf("frames    = %d\n", X.frames);
        printf("bytes     = %d\n", X.bytes);
        printf("vbr_scale = %d\n", X.vbr_scale);
        printf("==================================\n");
        fseek(fp, 0, SEEK_SET);
        sts = fwrite(buf, MAX_XING_HEADER_SIZE, 1, fp);
        fclose(fp);
    }
    return 0;
}
#endif
