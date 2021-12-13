#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include"Prototyper.h"

/* This function compares the power consumption of plugs with the average power consumption of appliances. */
void compare_plugs(user_profile user, user_profile user_prev_avg, average_profile average, appliance *above_average_consumption,
                   appliance *below_average_consumption, int amount_of_plugs, int *index_above, int *index_below, int days_simulated)
{
    int current_appliance;
    double appliance_differnce[APPLIANCE_MAX + 1];
    int i;
    int min_field_width_appliance = 0;
    int min_field_width_plug = 0;

    for (i = 0; i < amount_of_plugs; i++)
    {
        if (min_field_width_appliance < strlen(appliances_string[microwave]))
            min_field_width_appliance = strlen(appliances_string[microwave]);
        if (min_field_width_appliance < strlen(appliances_string[kettle]))
            min_field_width_appliance = strlen(appliances_string[kettle]);
        if (min_field_width_appliance < strlen(appliances_string[oven]))
            min_field_width_appliance = strlen(appliances_string[oven]);
        if (min_field_width_appliance < strlen(appliances_string[refrigerator]))
            min_field_width_appliance = strlen(appliances_string[refrigerator]);
        if (min_field_width_appliance < strlen(appliances_string[coffee]))
            min_field_width_appliance = strlen(appliances_string[coffee]);
    }

    if (amount_of_plugs >= 10)
        min_field_width_plug = 2;
    else
        min_field_width_plug = 1;

    for (i = 0; i < amount_of_plugs; i++)
    {
        /* this switch-case is sorted after the id of the users plugs.
        current_appliances is signed the int value of the current case. E.g. in case microwave: current_appliances is assigned microwave */
        switch (user.plug[i].id)
        {
        case microwave:
            current_appliance = place_in_correct_array(microwave, user.plug[i].power_consumption, average.appliances[microwave].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);

            appliance_differnce[microwave] = percent(user.plug[i].power_consumption, user_prev_avg.plug[microwave].power_consumption);

            break;

        case kettle:
            current_appliance = place_in_correct_array(kettle, user.plug[i].power_consumption, average.appliances[kettle].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);
            appliance_differnce[kettle] = percent(user.plug[i].power_consumption, user_prev_avg.plug[kettle].power_consumption);

            break;

        case oven:
            current_appliance = place_in_correct_array(oven, user.plug[i].power_consumption, average.appliances[oven].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);
            appliance_differnce[oven] = percent(user.plug[i].power_consumption, user_prev_avg.plug[oven].power_consumption);

            break;

        case refrigerator:
            current_appliance = place_in_correct_array(refrigerator, user.plug[i].power_consumption, average.appliances[refrigerator].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);
            appliance_differnce[refrigerator] = percent(user.plug[i].power_consumption, user_prev_avg.plug[refrigerator].power_consumption);

            break;

        case coffee:
            current_appliance = place_in_correct_array(coffee, user.plug[i].power_consumption, average.appliances[coffee].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);
            appliance_differnce[coffee] = percent(user.plug[i].power_consumption, user_prev_avg.plug[coffee].power_consumption);

            break;
        }

        print_percentage_of_average(i, current_appliance, user.plug[i].power_consumption, average.appliances[current_appliance].power_consumption, min_field_width_appliance, min_field_width_plug);
    }
    printf("\n");
    if (days_simulated > 1)
    {
        for (i = 0; i < amount_of_plugs; i++)
        {
            if (user.plug[i].id != 0)
                printf("Consumption of your %-*s on plug %*d today is %6.2f%% compared to average of previous days.\n", min_field_width_appliance, appliances_string_lwr[user.plug[i].id], min_field_width_plug, i + 1, appliance_differnce[user.plug[i].id]);
        }
    }
    int is_smaller;
    double differnce = calc_prev_avg(user, user_prev_avg, amount_of_plugs, &is_smaller);
    if (days_simulated > 1)
    {
        if (is_smaller == 0)
            printf("\nYou have decreased your power consumption today by %.2f%% compared to previous days.\n", differnce);
        else if (is_smaller == 1)
            printf("\nYou have increased your power consumption today by %.2f%% compared to previous days.\n", differnce);
        else
            printf("\nYou have the same power consumption today as the previous days.\n");
    }
}


/* if the user power consumtion is higher than the average for that appliance,
it is placed into the array above_average_consumption. Otherwise places in below_average_consumption */
int place_in_correct_array(int id, double user_consumption, double average_consumption, appliance *above_average_consumption,
                           appliance *below_average_consumption, int **index_above, int **index_below)
{
    if (user_consumption > average_consumption)
    {
        above_average_consumption[**index_above].power_consumption = user_consumption;
        above_average_consumption[(**index_above)++].id = id;
    }
    else
    {
        below_average_consumption[**index_below].power_consumption = user_consumption;
        below_average_consumption[(**index_below)++].id = id;
    }

    return id;
}

double calc_prev_avg(user_profile user, user_profile user_prev_avg, int amount_of_plugs, int *is_smaller)
{
    double avg_of_prev_avg = 0;
    double avg_of_today = 0;
    int i;
    for (i = 0; i < amount_of_plugs; i++)
    {
        if (user.plug[i].id != 0)
        {
            avg_of_prev_avg += user_prev_avg.plug[user.plug[i].id].power_consumption;
            avg_of_today += user.plug[i].power_consumption;
        }
    }
    avg_of_prev_avg /= amount_of_plugs;
    avg_of_today /= amount_of_plugs;

    /*printf("AVG of PREV: %f   |   AVG of TODAY: %f\n", avg_of_prev_avg, avg_of_today);*/

    if (avg_of_prev_avg < avg_of_today)
        *is_smaller = 1;
    else if (avg_of_prev_avg > avg_of_today)
        *is_smaller = 0;
    else
        *is_smaller = -1;

    return percent(avg_of_prev_avg - avg_of_today, avg_of_today);
}