/*
 * csvTrivialCallbackParser - a CSV callback parser in C
 * Copyright (C) 2023 G. David Butler <gdb@dbSystems.com>
 *
 * This file is part of csvTrivialCallbackParser
 *
 * csvTrivialCallbackParser is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * csvTrivialCallbackParser is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "csv.h"

int
csvParse(
  csvCb_t c
 ,const unsigned char *s
 ,unsigned int l
,void *v
){
  const unsigned char *sb;
  csvSt_t vl;
  unsigned int rw;
  unsigned int cl;

  if (!(sb = s))
    return (-1);
  rw = 0;

  goto bgn;

err:
  ++l, --s;
  goto rtn;

val:
  vl.s = s;
  for (; l--;) switch (*s++) {
  case '\n':
    vl.l = s - vl.s - (*(s - 2) == '\r' ? 2 : 1);
    if (c && c(csvTp_Cv, rw, cl, &vl, v))
      goto rtn;
    vl.s = s - 1, vl.l = 0;
    if (c && c(csvTp_Ce, rw, cl, &vl, v))
      goto rtn;
    ++rw;
    goto bgn;

  case '\r':
    break;

  case '"':
    for (; l; --l, ++s)
      if (*s == '"') {
        --l, ++s;
        if (!l || *s != '"')
          break;
      }
    break;

  case ',':
    vl.l = s - vl.s - 1;
    if (c && c(csvTp_Cv, rw, cl, &vl, v))
      goto rtn;
    ++cl;
    goto val;

  default:
    break;
  }
  goto err;

bgn:
  cl = 0;
  for (; l--;) switch (*s++) {
  case '\n':
    vl.s = s, vl.l = 0;
    if (c && c(csvTp_Cb, rw, cl, &vl, v))
      goto rtn;
    if (c && c(csvTp_Ce, rw, cl, &vl, v))
      goto rtn;
    ++rw;
    break;

  case '\r':
    break;

  default:
    ++l, --s;
    vl.s = s, vl.l = 0;
    if (c && c(csvTp_Cb, rw, cl, &vl, v))
      goto rtn;
    goto val;
  }

rtn:
  return (s - sb);
}

int
csvDecodeValue(
  unsigned char *out
 ,unsigned int olen
 ,const unsigned char *in
 ,unsigned int ilen
){
  int len;

  len = 0;
  for (; ilen--;) switch (*in) {
  case '"':
    for (;;) {
      if (!(++in, ilen--))
        goto err;
      if (*in == '"') {
        if (!ilen || *++in != '"')
          break;
        --ilen;
      }
      if (olen > 0) {
        *out++ = *in;
        --olen;
      }
      ++len;
    }
    break;

  default:
    if (olen > 0) {
      *out++ = *in++;
      --olen;
    } else
      ++in;
    ++len;
    break;
  }
  return (len);
err:
  return (-1);
}

int
csvEncodeValue(
  unsigned char *out
 ,unsigned int olen
 ,const unsigned char *in
 ,unsigned int ilen
){
  unsigned char *outS;
  const unsigned char *inS;
  unsigned int olenS;
  unsigned int ilenS;
  int len;

  /* This parser accepts embedded quoted values. Most can't. Play nice. */
  outS = out;
  olenS = olen;
  inS = in;
  ilenS = ilen;
  len = 0;
  for (; ilen; --ilen, ++in) switch (*in) {
  case '\r': case '\n': case '"':  case ',':
    goto quote;

  default:
    if (olen > 0) {
      *out++ = *in;
      --olen;
    }
    ++len;
    break;
  }
  return (len);
quote:
  out = outS;
  olen = olenS;
  in = inS;
  ilen = ilenS;
  len = 0;
  if (olen > 0) {
    *out++ = '"';
    --olen;
  }
  ++len;
  for (; ilen; --ilen, ++in) {
    if (*in == '"') {
      if (olen > 0) {
        *out++ = *in;
        --olen;
      }
      ++len;
    }
    if (olen > 0) {
      *out++ = *in;
      --olen;
    }
    ++len;
  }
  if (olen > 0) {
    *out++ = '"';
    --olen;
  }
  ++len;
  return (len);
}
