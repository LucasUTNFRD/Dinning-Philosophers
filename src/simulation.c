#include "philo.h"
#include <unistd.h>

void wait_all_threads(time_t start_time) {
  while (get_time_in_ms() < start_time) {
    continue;
  }
}

void take_left_fork(t_philosopher *philo) {
  pthread_mutex_lock(&philo->left_fork->fork);
}
void take_right_fork(t_philosopher *philo) {
  pthread_mutex_lock(&philo->right_fork->fork);
}

void take_forks(t_philosopher *philo) {
  take_left_fork(philo);
  take_right_fork(philo);
}

void release_forks(t_philosopher *philo) {
  pthread_mutex_unlock(&philo->right_fork->fork);
  pthread_mutex_unlock(&philo->left_fork->fork);
}

void eat(t_philosopher *philo) {
  take_forks(philo);
  // print status

  release_forks(philo);
}
void rest(t_philosopher *philo) {}
void think(t_philosopher *philo) {}

void *dinner_simulation(void *data) {
  t_philosopher *philo = (t_philosopher *)data;
  t_dinner *dinner = philo->dinner;

  wait_all_threads(philo->dinner->rules->start_time);

  // set last meal time
  while (
      !check_stop_condition(&philo->dinner->stop_mtx, &philo->dinner->stop)) {
    eat(philo);
    rest(philo);
    think(philo);
  }

  return NULL;
}

void dinner_start_simulation(t_dinner *dinner) {
  int i;
  dinner->rules->start_time = get_time_in_ms();
  // create philosophers threads
  for (i = 0; i < dinner->rules->philo_count; i++) {
    pthread_create(&dinner->philo[i].thread, NULL, dinner_simulation,
                   &dinner->philo[i]);
  }
  // Join philosopher threads
  for (i = 0; i < dinner->rules->philo_count; i++) {
    pthread_join(dinner->philo[i].thread, NULL);
  }
}
