/*
 * csvTrivialCallbackParser - a CSV callback parser in C
 * Copyright (C) 2023 G. David Butler <gdb@dbSystems.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/* csv "string" (fragment) is not \0 terminated */
typedef struct {
  const unsigned char *s;
  unsigned int l;
} csvSt_t;

/* callback event types */
typedef enum {
  csvTp_Cb /* begin */
 ,csvTp_Cv /* value */
 ,csvTp_Ce /* end */
} csvTp_t;

/* prototype of a callback function */
/* returns 0 on success else aborts parse */
typedef int (*csvCb_t)(
  csvTp_t
 ,unsigned int row /* zero based */
 ,unsigned int column /* zero based */
 ,const csvSt_t *value
 ,void *userContext
);

/* return -1 on error else offset of last char parsed */
/* provide a tag buffer large enough for the deepest level of nesting supported */
int csvParse(
  csvCb_t
 ,const unsigned char *csv
 ,unsigned int jlen
 ,void *userContext
);

/* return -1 on error else the length of out */
/* if length returned is more than length provided, allocate needed memory and retry */
int csvDecodeValue(
  unsigned char *out
 ,unsigned int olen
 ,const unsigned char *in
 ,unsigned int ilen
);
            
/* return -1 on error else the length of out */
/* if length returned is more than length provided, allocate needed memory and retry */
int csvEncodeValue(
  unsigned char *out
 ,unsigned int olen
 ,const unsigned char *in
 ,unsigned int ilen
);
