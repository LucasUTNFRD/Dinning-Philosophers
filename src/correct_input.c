#include "philo.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

bool is_digit(int c) { return ('0' <= c && c <= '9'); }

bool incorrect_num_of_args(int argc, t_dinner *dinner) {
  bool time_to_die_provided;
  bool time_to_die_not_provided;

  time_to_die_provided = (argc < 5);
  time_to_die_not_provided = (argc > 6);
  if (time_to_die_provided || time_to_die_not_provided) {
    report_and_set_error(dinner, ERR_NUM_ARGS, MSG_NUM_ARGS);
    return (true);
  } else
    return (false);
}

bool can_convert_str_to_int(t_dinner *dinner, const char *str, int *res) {
  long long n;

  n = 0;
  while (*str != '\0') {
    n = n * 10 + (*str - '0');
    if (n > INT_MAX) {
      report_and_set_error(dinner, ERR_ATOI, MSG_ATOI);
      return (false);
    }
    str++;
  }
  *res = n;
  return (true);
}

bool not_only_digits(char *str, t_dinner *dinner) {
  int i;

  i = 0;
  while (str[i] != '\0') {
    if (!is_digit(str[i])) {
      report_and_set_error(dinner, ERR_NOT_ONLY_DIGITS, MSG_NOT_ONLY_DIGITS);
      return (true);
    }
    i++;
  }
  return (false);
}

bool wrong_num(int i, int curr_arg, t_dinner *dinner) {
  if (i == 1 && !(0 < curr_arg && curr_arg <= PHILO_MAX)) {
    report_and_set_error(dinner, ERR_NUM_PHILOS, MSG_NUM_PHILOS);
    return (true);
  } else if ((2 <= i && i <= 5) && curr_arg < 1) {
    report_and_set_error(dinner, ERR_NUM, MSG_NUM);
    return (true);
  }
  return (false);
}

bool can_prepare_dinner(t_dinner *dinner, int argc, char **argv) {
  int i;
  int curr_arg;

  if (incorrect_num_of_args(argc, dinner))
    return (false);
  i = 1;
  while (i < argc) {
    if (not_only_digits(argv[i], dinner))
      return (false);
    if (!can_convert_str_to_int(dinner, argv[i], &curr_arg))
      return (false);
    if (wrong_num(i, curr_arg, dinner))
      return (false);
    i++;
  }
  return (true);
}
