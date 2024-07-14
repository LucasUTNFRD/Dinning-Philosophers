#include "philo.h"
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/*
 * we make all threads start at the same time. by getting the current time
 * and wating until the current thread reaches the start_time
 */

// Create safe threads
bool can_create_thread(pthread_t *thread, void *(*routine)(void *), void *arg) {
  if (pthread_create(thread, NULL, routine, arg) != SUCCESS)
    return (false);
  return (true);
}

void align_start_times(time_t start_time) {
  while (get_time_in_ms() < start_time) {
    continue;
  }
}

bool died_from_starvation(t_philosopher *philo) {
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

void life_check_and_wait(t_philosopher *philo, time_t duration) {
  bool should_stop;
  time_t time_to_wait_for;

  time_to_wait_for = get_time_in_ms() + duration;
  while (get_time_in_ms() < time_to_wait_for) {
    should_stop = (check_stop_condition_safely(&philo->dinner->stop_mutex,
                                               &philo->dinner->stop) == true);
    if (should_stop)
      return;
    usleep(100);
  }
}

void take_left_fork(t_philosopher *philo) {
  pthread_mutex_lock(&philo->dinner->forks[philo->forks[0]]);
  print_philo_status(philo, TAKING_FORK);
}

void take_right_fork(t_philosopher *philo) {
  pthread_mutex_lock(&philo->dinner->forks[philo->forks[1]]);
  print_philo_status(philo, TAKING_FORK);
}

void take_forks(t_philosopher *philo) {
  take_left_fork(philo);
  take_right_fork(philo);
}

void release_forks(t_philosopher *philo) {
  pthread_mutex_unlock(&philo->dinner->forks[philo->forks[0]]);
  pthread_mutex_unlock(&philo->dinner->forks[philo->forks[1]]);
}

void eat(t_philosopher *philo) {
  take_forks(philo);
  print_philo_status(philo, EATING);
  life_check_and_wait(philo, philo->dinner->rules->dining_duration);
  update_times_eaten_safely(&philo->meal_counter_mutex, &philo->meal_counter,
                            philo->meal_counter + 1);
  update_last_meal_time_safely(&philo->last_meal_time_mutex,
                               &philo->last_meal_time, get_time_in_ms());
  release_forks(philo);
}

void rest(t_philosopher *philo) {
  print_philo_status(philo, SLEEPING);
  life_check_and_wait(philo, philo->dinner->rules->rest_duration);
}

static time_t calculate_thinking_duration(t_philosopher *philo) {
  time_t lifespan;
  time_t thinking_duration;
  time_t fasting_duration;
  time_t dining_duration;

  lifespan = philo->dinner->rules->lifespan;
  fasting_duration = get_time_in_ms() -
                     check_last_meal_time_safely(&philo->last_meal_time_mutex,
                                                 &philo->last_meal_time);
  dining_duration = philo->dinner->rules->dining_duration;
  thinking_duration = (lifespan - fasting_duration - dining_duration) / 2;
  if (thinking_duration <= 0)
    thinking_duration = 1;
  if (thinking_duration > 600)
    thinking_duration = 200;
  return (thinking_duration);
}

void think(t_philosopher *philo) {
  print_philo_status(philo, THINKING);
  life_check_and_wait(philo, calculate_thinking_duration(philo));
}

void *philo_routine(void *data) {
  t_philosopher *philo;

  philo = (t_philosopher *)data;
  align_start_times(philo->dinner->rules->start_time);
  // if (philo->dinner->rules.philo_count == 1)
  //   return (lonely_philosopher(philo));
  while (!check_stop_condition_safely(&philo->dinner->stop_mutex,
                                      &philo->dinner->stop)) {
    eat(philo);
    rest(philo);
    think(philo);
  }
  return (NULL);
}

bool is_full(t_philosopher *philo) {
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
  t_dinner *dinner;

  dinner = (t_dinner *)data;
  t_philosopher *philo;
  align_start_times(dinner->rules->start_time);
  while (true) {
    i = 0;
    philo = dinner->philos + 1;
    while (i < dinner->rules->philo_count) {
      pthread_mutex_lock(&philo->last_meal_time_mutex);
      if (died_from_starvation(philo))
        return (NULL);
      if (is_full(philo))
        return (NULL);
      pthread_mutex_unlock(&philo->last_meal_time_mutex);
      i++;
    }
  }
  return (NULL);
}

void dinner_start_simulation(t_dinner *dinner) {
  // int i;
  // dinner->rules->start_time = get_time_in_ms();
  // // create philosophers threads
  // for (i = 0; i < dinner->rules->philo_count; i++) {
  //   pthread_create(&dinner->philo[i].thread, NULL, philo_routine,
  //                  &dinner->philo[i]);
  // }
  // if (dinner->rules->philo_count > 1) {
  //   pthread_create(&dinner->supervisor, NULL, monitor_routine, &dinner);
  // }
  int i;

  i = 0;
  dinner->rules->start_time = get_time_in_ms();
  while (i < dinner->rules->philo_count) {
    if (!can_create_thread(&dinner->philos[i].thread, philo_routine,
                           &dinner->philos[i])) {
      report_and_set_error(dinner, ERR_THREAD_CREATE, MSG_THREAD_CREATE);
      return;
    }
    i++;
  }
  if (dinner->rules->philo_count > 1) {
    if (!can_create_thread(&dinner->supervisor, monitor_routine, dinner)) {
      report_and_set_error(dinner, ERR_THREAD_CREATE, MSG_THREAD_CREATE);
      return;
    }
  }
}

void dinner_end_simulation(t_dinner *dinner) {
  int i;
  for (i = 0; i < dinner->rules->philo_count; i++) {
    pthread_join(dinner->philos[i].thread, NULL);
  }
  if (dinner->rules->philo_count > 1) {
    pthread_join(dinner->supervisor, NULL);
  }
  for (i = 0; i < dinner->rules->philo_count; i++) {
    pthread_mutex_destroy(&dinner->philos[i].meal_counter_mutex);
    pthread_mutex_destroy(&dinner->philos[i].last_meal_time_mutex);
  }
  pthread_mutex_destroy(&dinner->stop_mutex);
  free(dinner->philos);
  free(dinner->rules);
}
