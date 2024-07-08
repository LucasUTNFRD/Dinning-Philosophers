#include "src/philo.h"
#include <stdio.h>
#include <unistd.h>

/* MAKE
 *  ./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep]
 * [number_of_times_each_philosopher_must_eat (optional)]
 */
int main(int argc, char **argv) {
  t_dinner dinner;
  if (can_prepare_dinner(&dinner, argc, argv)) {
    dinner_init(&dinner);

    dinner_start_simulation(&dinner);

    dinner_end(&dinner);
  } else {
    exit(1);
  }
  return 0;
}
