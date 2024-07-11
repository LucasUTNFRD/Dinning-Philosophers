#include "philo.h"
#include <stdio.h>
#include <sys/time.h>

// Displaying time functions

time_t s_to_ms(time_t seconds) { return (seconds * 1e3); }

time_t us_to_ms(time_t useconds) { return (useconds / 1e3); }

time_t get_time_in_ms(void) {
  struct timeval tv;

  gettimeofday(&tv, NULL);
  return (s_to_ms(tv.tv_sec) + us_to_ms(tv.tv_usec));
}

// Displaying and returning errors

void report_and_set_error(t_dinner *dinner, int code, char *msg) {
  pthread_mutex_lock(&dinner->print_mutex);
  printf("philo: %s\n", msg);
  dinner->exit_status = code;
  pthread_mutex_unlock(&dinner->print_mutex);
}

static void print_in_required_format(t_philosopher *philo, char *action) {
  pthread_mutex_lock(&philo->dinner->print_mutex);
  printf("%ld %d %s\n", get_time_in_ms() - philo->dinner->rules->start_time,
         philo->id, action);
  pthread_mutex_unlock(&philo->dinner->print_mutex);
}

void print_philo_status(t_philosopher *philo, int status) {
  // if (check_stop_condition_safely(&philo->dinner->stop_mutex,
  // &philo->dinner->stop))
  // return;
  switch (status) {
  case DEAD:
    print_in_required_format(philo, MSG_DEAD);
    break;
  case EATING:
    print_in_required_format(philo, MSG_EATING);
    break;
  case SLEEPING:
    print_in_required_format(philo, MSG_SLEEPING);
    break;
  case THINKING:
    print_in_required_format(philo, MSG_THINKING);
    break;
  case TAKING_FORK:
    print_in_required_format(philo, MSG_TAKING_FORK);
    break;
  }
}
