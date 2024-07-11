#include "philo.h"
#include <stdlib.h>
#include <time.h>

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
/*
 * EXPLANATION
 *
 */
static void assing_fork(t_philosopher *philo, t_fork *forks, int philo_pos) {

  int philo_nbr = philo->dinner->rules->philo_count;

  philo->left_fork = &forks[(philo_pos + 1) % philo_nbr];
  philo->right_fork = &forks[philo_pos];
  if (philo->id % 2 == 0) {
    philo->left_fork = &forks[philo_pos];
    philo->right_fork = &forks[(philo_pos + 1) % philo_nbr];
  }
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
    philo->last_meal_time = get_time_in_ms();
    pthread_mutex_init(&philo->last_meal_time_mutex, NULL);
    pthread_mutex_init(&philo->meal_counter_mutex, NULL);
    assing_fork(philo, dinner->forks, i);
  }
}

void dinner_init(t_dinner *dinner, int argc, char **argv) {
  dinner->exit_status = SUCCESS;
  set_dinner_rules(dinner, argc, argv);
  dinner->philo = malloc(sizeof(t_philosopher) * dinner->rules->philo_count);
  dinner->forks = malloc(sizeof(t_fork) * dinner->rules->philo_count);
  fork_init(dinner);
  philo_init(dinner); // TODO
}
