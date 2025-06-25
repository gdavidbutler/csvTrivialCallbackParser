#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include "csv.h"

static int
cb(
  csvTp_t t
 ,unsigned int r
 ,unsigned int c
 ,const csvSt_t *l
 ,void *v
){
  int i;

  (void)v;
  switch (t) {
  case csvTp_Cb:
    printf("B %u @%p\n", r, (void *)l->s);
    break;
  case csvTp_Ce:
    printf("E %u @%p\n", r, (void *)l->s);
    break;
  case csvTp_Cv:
    printf("V %u/%u", r, c);
    {
      unsigned char *d;

      i = -1;
      if (!(d = malloc(l->l))
       || (i = csvDecodeValue(d, l->l, l->s, l->l)) < 0
       || i > (int)l->l)
        printf(":%.*s:%d:%u\n", l->l, l->s, i, l->l);
      else
        printf(":%.*s~%.*s\n", l->l, l->s, i, d);
      free(d);
    }
    break;
  }
  return (0);
}

int
main(
  int argc
 ,char *argv[]
){
  int fd;
  int sz;
  unsigned char *bf;

  if (argc != 3) {
    fprintf(stderr, "Usage: %s 0|1 file\n", argv[0]);
    return (1);
  }
  if ((fd = open(argv[2], O_RDONLY)) < 0) {
    fprintf(stderr, "%s: Can't open %s\n", argv[0], argv[2]);
    return (1);
  }
  sz = lseek(fd, 0, SEEK_END);
  lseek(fd, 0, SEEK_SET);
  bf = malloc(sz);
  if (read(fd, bf, sz) != sz) {
    fprintf(stderr, "%s: read fail on %s\n", argv[0], argv[2]);
    return (1);
  }
  close(fd);
  printf("%d %d\n", sz, csvParse(atoi(argv[1]) ? cb : 0, bf, sz, 0));
  free(bf);
  return (0);
}
