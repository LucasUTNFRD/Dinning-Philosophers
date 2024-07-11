#include "philo.h"
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/*
 * we make all threads start at the same time. by getting the current time
 * and wating until the current thread reaches the start_time
 */
void wait_all_threads(time_t start_time) {
  while (get_time_in_ms() < start_time) {
    continue;
  }
}

bool check_stop_condition(pthread_mutex_t *mutex, bool *stop) {
  bool result;
  pthread_mutex_lock(mutex);   // Lock the mutex to ensure thread safety
  result = *stop;              // Read the stop condition
  pthread_mutex_unlock(mutex); // Unlock the mutex
  return result;               // Return the stop condition
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

void take_left_fork(t_philosopher *philo) {
  pthread_mutex_lock(&philo->left_fork->fork);
  print_philo_status(philo, TAKING_FORK);
}

void take_right_fork(t_philosopher *philo) {
  pthread_mutex_lock(&philo->right_fork->fork);
  print_philo_status(philo, TAKING_FORK);
}

void take_forks(t_philosopher *philo) {
  take_left_fork(philo);
  take_right_fork(philo);
}

void release_forks(t_philosopher *philo) {
  pthread_mutex_unlock(&philo->right_fork->fork);
  pthread_mutex_unlock(&philo->left_fork->fork);
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

void eat(t_philosopher *philo) {
  take_forks(philo);
  print_philo_status(philo, EATING);
  // TODO: Update times eaten and last meal time safely
  update_times_eaten_safely(&philo->meal_counter_mutex, &philo->meal_counter,
                            philo->meal_counter++);
  update_last_meal_time_safely(&philo->last_meal_time_mutex,
                               &philo->last_meal_time, get_time_in_ms());
  release_forks(philo);
}

void rest(t_philosopher *philo) { print_philo_status(philo, SLEEPING); }
void think(t_philosopher *philo) { print_philo_status(philo, THINKING); }

void *dinner_simulation(void *data) {
  t_philosopher *philo = (t_philosopher *)data;
  t_dinner *dinner = philo->dinner;

  wait_all_threads(dinner->rules->start_time);

  // set last meal time
  while (
      !check_stop_condition(&philo->dinner->stop_mutex, &philo->dinner->stop)) {
    eat(philo);
    rest(philo);
    think(philo);
  }

  return (NULL);
}

static void update_stop_condition_safely(pthread_mutex_t *mutex, bool *stop,
                                         bool new_value) {
  pthread_mutex_lock(mutex);
  *stop = new_value;
  pthread_mutex_unlock(mutex);
}

static bool died_from_starvation(t_philosopher *philo) {
  if (get_time_in_ms() - philo->last_meal_time >=
      philo->dinner->rules->lifespan) {
    print_philo_status(philo, DEAD);
    update_stop_condition_safely(&philo->dinner->stop_mutex,
                                 &philo->dinner->stop, true);
    pthread_mutex_unlock(&philo->dinner->stop_mutex);
    return true;
  }
  return false;
}

static bool is_full(t_philosopher *philo) {
  if (philo->dinner->rules->min_meals != MIN_MEAL_NOT_SET) {

    if (check_times_eaten_safely(&philo->meal_counter_mutex,
                                 &philo->meal_counter) >
        philo->dinner->rules->min_meals) {
      update_stop_condition_safely(&philo->dinner->stop_mutex,
                                   &philo->dinner->stop, true);
      pthread_mutex_unlock(&philo->last_meal_time_mutex);
      return true;
    }
  }
  return false;
}

void *monitor_routine(void *data) {
  int i;
  t_dinner *dinner = (t_dinner *)data;
  t_philosopher *philo;
  wait_all_threads(dinner->rules->start_time);
  while (true) {
    i = -1;
    while (++i < dinner->rules->philo_count) {
      philo = dinner->philo + i;
      pthread_mutex_lock(&philo->last_meal_time_mutex);
      if (died_from_starvation(philo))
        return (NULL);
      if (is_full(philo))
        return (NULL);
      pthread_mutex_unlock(&philo->last_meal_time_mutex);
    }
  }
  return (NULL);
}

void dinner_start_simulation(t_dinner *dinner) {
  int i;
  dinner->rules->start_time = get_time_in_ms();
  // create philosophers threads
  for (i = 0; i < dinner->rules->philo_count; i++) {
    pthread_create(&dinner->philo[i].thread, NULL, dinner_simulation,
                   &dinner->philo[i]);
  }
  if (dinner->rules->philo_count > 1) {
    pthread_create(&dinner->supervisor, NULL, monitor_routine, &dinner);
  }
}

void dinner_end_simulation(t_dinner *dinner) {
  int i;
  // join philo threads
  for (i = 0; i < dinner->rules->philo_count; i++) {
    pthread_join(dinner->philo[i].thread, NULL);
  }

  // Join monitor thread
  if (dinner->rules->philo_count > 1) {
    pthread_join(dinner->supervisor, NULL);
  }
}
