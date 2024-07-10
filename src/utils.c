#include "philo.h"
#include <sys/time.h>

time_t s_to_ms(time_t seconds) { return (seconds * 1e3); }

time_t us_to_ms(time_t useconds) { return (useconds / 1e3); }

time_t get_time_in_ms(void) {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return (s_to_ms(tv.tv_sec) + us_to_ms(tv.tv_usec));
}
