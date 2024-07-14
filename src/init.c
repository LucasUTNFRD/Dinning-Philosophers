#include "philo.h"
#include <stdlib.h>
#include <time.h>

void set_dinner_rules(t_dinner *dinner, int argc, char **argv) {
  dinner->rules = malloc(sizeof(t_rules));
  dinner->rules->start_time = get_time_in_ms();
  dinner->rules->philo_count = atoi(argv[1]);
  dinner->rules->lifespan = atoi(argv[2]);
  dinner->rules->dining_duration = atoi(argv[3]);
  dinner->rules->rest_duration = atoi(argv[4]);
  if (argc == 6) {
    dinner->rules->min_meals = atoi(argv[5]);
  } else {
    dinner->rules->min_meals = MIN_MEAL_NOT_SET;
  }

  pthread_mutex_init(&dinner->stop_mutex, NULL);
  dinner->stop = false;
}

static void fork_init(t_dinner *dinner) {
  int i = -1;
  while (++i < dinner->rules->philo_count) {
    pthread_mutex_init(&dinner->forks[i], NULL);
  }
}

static bool philosopher_is_left_handed(t_philosopher *philo) {
  return (philo->id % 2);
}

static void assign_left_fork_first(t_philosopher *philo) {
  philo->forks[0] = philo->id;
  philo->forks[1] = (philo->id + 1) % philo->dinner->rules->philo_count;
}

static void assign_right_fork_first(t_philosopher *philo) {
  philo->forks[1] = philo->id;
  philo->forks[0] = (philo->id + 1) % philo->dinner->rules->philo_count;
}

static void assing_fork(t_philosopher *philo) {
  if (philosopher_is_left_handed(philo))
    assign_left_fork_first(philo);
  else
    assign_right_fork_first(philo);
}

static void philo_init(t_dinner *dinner) {
  int i;
  t_philosopher *philo;

  i = -1;
  while (++i < dinner->rules->philo_count) {
    philo = dinner->philos + i;
    philo->id = i + 1;
    philo->full = false;
    philo->dinner = dinner;
    philo->meal_counter = 0;
    philo->last_meal_time = get_time_in_ms();
    pthread_mutex_init(&philo->last_meal_time_mutex, NULL);
    pthread_mutex_init(&philo->meal_counter_mutex, NULL);
    assing_fork(philo);
  }
}

void dinner_init(t_dinner *dinner, int argc, char **argv) {
  dinner->exit_status = SUCCESS;
  set_dinner_rules(dinner, argc, argv);
  dinner->philos = malloc(sizeof(t_philosopher) * dinner->rules->philo_count);
  // dinner->forks = malloc(sizeof(t_fork) * dinner->rules->philo_count);
  fork_init(dinner);
  philo_init(dinner); // TODO
}
