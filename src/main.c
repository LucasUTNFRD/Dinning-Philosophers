#include "philo.h"
#include <stdio.h>
#include <unistd.h>

/* MAKE
 *  ./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep]
 * [number_of_times_each_philosopher_must_eat (optional)]
 */

int main(int argc, char **argv) {
  t_dinner dinner;
  if (can_prepare_dinner(&dinner, argc, argv)) {

    dinner_init(&dinner, argc, argv);

    dinner_start_simulation(&dinner); // TODO

    dinner_end_simulation(&dinner);
  } else {
    // exit(1);
    printf("error\n");
  }
  return 0;
}
