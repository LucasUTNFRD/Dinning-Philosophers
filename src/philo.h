#ifndef PHILO_H
#define PHILO_H

/*
 * This file contains Structure for the simulation
 */

#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>

#define MIN_MEAL_NOT_SET 3

// ERROR MESAGES
#define MSG_NUM_ARGS "Incorrect number of arguments."
#define MSG_NOT_ONLY_DIGITS "Not only digits."
#define MSG_ATOI "Atoi error."
#define MSG_NUM_PHILOS "Wrong number of philosophers."
#define MSG_NUM "Argument must be positive"
#define MSG_MUTEX_INIT "Error initializing mutex."
#define MSG_THREAD_CREATE "Error creating thread."
#define MSG_THREAD_JOIN "Error joining thread."

// MESAGES
#define MSG_DEAD "died"
#define MSG_EATING "is eating"
#define MSG_SLEEPING "is sleeping"
#define MSG_THINKING "is thinking"
#define MSG_TAKING_FORK "has taken a fork"

// ERROR CODE
enum e_exit_status {
  SUCCESS,
  ERR_NUM_ARGS,
  ERR_NOT_ONLY_DIGITS,
  ERR_ATOI,
  ERR_NUM_PHILOS,
  ERR_NUM,
  ERR_MUTEX_INIT,
  ERR_THREAD_CREATE,
  ERR_THREAD_JOIN
};

// Status
enum e_philo_status { DEAD, EATING, SLEEPING, THINKING, TAKING_FORK };

typedef struct s_dinner
    t_dinner; // re-declaration to avoid conlfict insider t_philosopher

typedef long time_t;

typedef struct s_rules {
  // time_t start_time;
  int philo_count;
  time_t lifespan;
  time_t dining_duration;
  time_t rest_duration;
  unsigned int min_meals;
  time_t start_time;
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

  bool full; //?

  pthread_mutex_t meal_counter_mutex;
  unsigned int meal_counter;

  time_t last_meal_time;
  pthread_mutex_t last_meal_time_mutex;

} t_philosopher;

struct s_dinner {
  t_rules *rules;
  t_philosopher *philo;
  t_fork *forks;
  pthread_mutex_t print_mutex; // avoid race conitons in printing statuses
  int exit_status;

  pthread_t supervisor;

  bool stop;
  pthread_mutex_t stop_mutex;
};

bool can_prepare_dinner(t_dinner *dinner, int argc, char **argv);

void dinner_init(t_dinner *dinner, int argc, char **argv);

time_t get_time_in_ms(void);

void dinner_start_simulation(t_dinner *dinner);

void dinner_end_simulation(t_dinner *dinner);

void print_philo_status(t_philosopher *philo, int status);

#endif // !PHILO_H
