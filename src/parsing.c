#include "philo.h"
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

static bool check_str_to_int(const char *str) {
  char *endptr;
  long num = strtol(str, &endptr, 10);

  if (*endptr != '\0') {
    return false; // Conversion failed
  } else {
    return true; // Conversion successful
  }
}

bool can_prepare_dinner(t_dinner *dinner, int argc, char **argv) {
  if (argc == 5 || argc == 6) {
    // check if args can be converted to nubmer
    int i = 0;
    while (++i < argc) {
      if (!check_str_to_int(argv[i])) {
        return false;
      }
      return true;
    }
  } else {
    return false;
  }
}
