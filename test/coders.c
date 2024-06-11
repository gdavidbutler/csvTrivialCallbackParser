#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "csv.h"

int
main(
  void
){
  static const unsigned char enc1[] = "This is a test - how did it do?";
  static const unsigned char dnc1[] = "This is a test - how did it do?";
  static const unsigned char enc2[] = "This is a test of ,\" how did it do?";
  static const unsigned char dnc2[] = "\"This is a test of ,\"\" how did it do?\"";
  static const unsigned char dnc3[] = "This is a test of \",\"\"\" how did it do?";
  int sz;
  unsigned char *bf;
  unsigned char *tbf;

  if (!(bf = malloc(BUFSIZ)))
    return (2);

  if ((sz = csvEncodeValue(bf, BUFSIZ, enc1, sizeof (enc1) - 1)) > BUFSIZ)
    return (2);
  printf("csvEncodeValue(%s)->%.*s\n", enc1, sz, bf);
  putchar('\n');
  if (!(tbf = malloc(BUFSIZ)))
    return (2);
  if ((sz = csvDecodeValue(tbf, BUFSIZ, bf, sz)) > BUFSIZ)
    return (2);
  if (sz != sizeof (enc1) - 1 || memcmp(tbf, enc1, sizeof (enc1) - 1))
    return (2);
  free(tbf);
  if ((sz = csvDecodeValue(bf, BUFSIZ, dnc1, sizeof (dnc1) - 1)) > BUFSIZ)
    return (2);
  printf("csvDecodeValue(%s)->%.*s\n", dnc1, sz, bf);
  putchar('\n');

  if ((sz = csvEncodeValue(bf, BUFSIZ, enc2, sizeof (enc2) - 1)) > BUFSIZ)
    return (2);
  printf("csvEncodeValue(%s)->%.*s\n", enc2, sz, bf);
  putchar('\n');
  if (!(tbf = malloc(BUFSIZ)))
    return (2);
  if ((sz = csvDecodeValue(tbf, BUFSIZ, bf, sz)) > BUFSIZ)
    return (2);
  if (sz != sizeof (enc2) - 1 || memcmp(tbf, enc2, sizeof (enc2) - 1))
    return (2);
  free(tbf);
  if ((sz = csvDecodeValue(bf, BUFSIZ, dnc2, sizeof (dnc2) - 1)) > BUFSIZ)
    return (2);
  printf("csvDecodeValue(%s)->%.*s\n", dnc2, sz, bf);
  putchar('\n');


  if ((sz = csvDecodeValue(bf, BUFSIZ, dnc3, sizeof (dnc3) - 1)) > BUFSIZ)
    return (2);
  printf("csvDecodeValue(%s)->%.*s\n", dnc3, sz, bf);
  putchar('\n');

  free(bf);
  return (0);
}
