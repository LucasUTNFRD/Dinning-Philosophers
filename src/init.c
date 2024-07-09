#include "philo.h"
#include <stdlib.h>

void set_dinner_rules(t_dinner *dinner, int argc, char **argv) {
  // set_dinner_start_time(dinner);
  dinner->rules->philo_count = atoi(argv[1]);
  dinner->rules->lifespan = atoi(argv[2]);
  dinner->rules->dining_duration = atoi(argv[3]);
  dinner->rules->rest_duration = atoi(argv[4]);
  if (argc == 6) {
    dinner->rules->min_meals = atoi(argv[5]);
  } else {
    dinner->rules->min_meals = MIN_MEAL_NOT_SET;
  }
}

static void fork_init(t_dinner *dinner) {
  int i = -1;
  while (++i < dinner->rules->philo_count) {
    pthread_mutex_init(&dinner->forks[i].fork, NULL);
    dinner->forks[i].fork_id = i;
  }
}

static void assing_fork(t_philosopher *philo, t_fork *forks, int i) {
  // here is were we implement our approach to avoid deadlock
}

static void philo_init(t_dinner *dinner) {
  int i;
  t_philosopher *philo;

  i = -1;
  while (++i < dinner->rules->philo_count) {
    philo = dinner->philo + i;
    philo->id = i + 1;
    philo->full = false;
    philo->dinner = dinner;
    philo->meal_counter = 0;

    assing_fork(philo, dinner->forks, i);
  }
}

void dinner_init(t_dinner *dinner, int argc, char **argv) {
  set_dinner_rules(dinner, argc, argv);
  dinner->philo = malloc(sizeof(t_philosopher) * dinner->rules->philo_count);
  dinner->forks = malloc(sizeof(t_fork) * dinner->rules->philo_count);
  fork_init(dinner);
  philo_init(dinner); // TODO
}
