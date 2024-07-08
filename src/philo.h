#ifndef PHILO_H
#define PHILO_H

/*
 *
 * This file contains Structure for the simulation
 *
 */

#include <pthread.h>
#include <stdbool.h>

#define PHILO_MAX 5

typedef struct s_dinner
    t_dinner; // re-declaration to avoid conlfict insider t_philosopher

typedef long time_t;

typedef struct s_rules {
  time_t start_time;
  unsigned philo_count;
  time_t lifespan;
  time_t dining_duration;
  time_t rest_duration;
  unsigned min_meals;
} t_rules;

typedef struct s_fork {
  pthread_mutex_t fork;
  int fork_id;
} t_fork;

typedef struct s_philosopher {
  int id;
  pthread_t thread; // philosopher are represented as threads
  t_fork *left_fork;
  t_fork *right_fork;
  t_dinner *dinner;
  bool full;
  int meal_counter;
  time_t last_meal_time;

} t_philosopher;

struct s_dinner {
  t_rules *rules;
  t_philosopher philo[PHILO_MAX];
};

bool can_prepare_dinner(t_dinner *dinner, int argc, char **argv);

void dinner_init(t_dinner *dinner);

#endif // !PHILO_H
