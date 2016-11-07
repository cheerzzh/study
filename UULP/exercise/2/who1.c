// who1.c - a firest version of the who program
//          open, read UTMP file, and show results

#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

#define SHOWHOST

extern void show_info(struct utmp *utmp_record);

int main(int argc, char *argv[]) {
  int utmp_fd;
  struct utmp utmp_record;
  int utmp_size = sizeof(utmp_record);

  if ((utmp_fd = open(UTMP_FILE, O_RDONLY)) == -1) {
    perror(UTMP_FILE);
    exit(EXIT_FAILURE);
  }

  while (read(utmp_fd, &utmp_record, utmp_size) == utmp_size) {
    show_info(&utmp_record);
  }
  close(utmp_fd);

  return EXIT_SUCCESS;
}

extern void show_info(struct utmp *utmp_record) {
  printf("%-8.8s", utmp_record->ut_user);
  printf(" ");
  printf("%-8.8s", utmp_record->ut_line);
  printf(" ");
  printf("%10ld", utmp_record->ut_time);
  printf(" ");
#ifdef SHOWHOST
  printf("(%s)", utmp_record->ut_host);
#endif
  printf("\n");
}
