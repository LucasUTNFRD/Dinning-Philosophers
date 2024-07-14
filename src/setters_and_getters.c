#include "philo.h"

// GETTERS

bool check_stop_condition_safely(pthread_mutex_t *mutex, bool *stop_flag) {
  bool after_check;

  pthread_mutex_lock(mutex);
  after_check = *stop_flag;
  pthread_mutex_unlock(mutex);
  return (after_check);
}

unsigned int check_times_eaten_safely(pthread_mutex_t *mutex,
                                      unsigned int *times_eaten) {
  unsigned int after_check;

  pthread_mutex_lock(mutex);
  after_check = *times_eaten;
  pthread_mutex_unlock(mutex);
  return (after_check);
}

time_t check_last_meal_time_safely(pthread_mutex_t *mutex,
                                   time_t *last_meal_time) {
  time_t after_check;

  pthread_mutex_lock(mutex);
  after_check = *last_meal_time;
  pthread_mutex_unlock(mutex);
  return (after_check);
}

// SETTERS

void update_stop_condition_safely(pthread_mutex_t *mutex, bool *end_flag,
                                  bool new_value) {
  pthread_mutex_lock(mutex);
  *end_flag = new_value;
  pthread_mutex_unlock(mutex);
}

void update_times_eaten_safely(pthread_mutex_t *mutex,
                               unsigned int *times_eaten,
                               unsigned int new_value) {
  pthread_mutex_lock(mutex);
  *times_eaten = new_value;
  pthread_mutex_unlock(mutex);
}

void update_last_meal_time_safely(pthread_mutex_t *mutex,
                                  time_t *last_meal_time, time_t new_value) {
  pthread_mutex_lock(mutex);
  *last_meal_time = new_value;
  pthread_mutex_unlock(mutex);
}
