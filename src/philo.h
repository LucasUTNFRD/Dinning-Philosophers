#ifndef PHILO_H
#define PHILO_H

/*
 * This file contains Structure for the simulation
 */

#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#define MIN_MEAL_NOT_SET 3
#define PHILO_MAX 5

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

typedef struct s_philosopher {
  int id;
  pthread_t thread; // philosopher are represented as threads
                    // t_fork *left_fork;
                    // t_fork *right_fork;
  unsigned int forks[2];
  t_dinner *dinner;

  bool full; //?

  pthread_mutex_t meal_counter_mutex;
  unsigned int meal_counter;

  time_t last_meal_time;
  pthread_mutex_t last_meal_time_mutex;

} t_philosopher;

struct s_dinner {
  t_rules *rules;
  t_philosopher *philos;
  pthread_mutex_t forks[PHILO_MAX];
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

void report_and_set_error(t_dinner *dinner, int code, char *msg);

bool check_stop_condition_safely(pthread_mutex_t *mutex, bool *stop_flag);

unsigned int check_times_eaten_safely(pthread_mutex_t *mutex,
                                      unsigned int *times_eaten);

time_t check_last_meal_time_safely(pthread_mutex_t *mutex,
                                   time_t *last_meal_time);

void update_stop_condition_safely(pthread_mutex_t *mutex, bool *end_flag,
                                  bool new_value);

void update_times_eaten_safely(pthread_mutex_t *mutex,
                               unsigned int *times_eaten,
                               unsigned int new_value);

void update_last_meal_time_safely(pthread_mutex_t *mutex,
                                  time_t *last_meal_time, time_t new_value);

#endif // !PHILO_H
