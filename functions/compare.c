#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "prototyper.h"

/* This function perform comparisons on the power consumption of plugs. They will be broken down as they are called */
void compare_plugs(user_profile user, user_profile user_prev_avg, average_profile average, appliance *above_average_consumption,
                   appliance *below_average_consumption, int amount_of_plugs, int *index_above, int *index_below, int days_simulated)
{
    int current_appliance;
    double appliance_difference[APPLIANCE_MAX + 1];
    double your_total_consumption, average_power_consumption;
    int i;
    int min_field_width_appliance = 0;
    int min_field_width_plug = 0;

    /* if there are more than 10 plugs - set minimum field with to 2 */
    if (amount_of_plugs >= 10)
        min_field_width_plug = 2;
    else
        min_field_width_plug = 1;

    /* Adjusts minimum field with according to the amount of characters in each appliance_string */
    for (i = 0; i < amount_of_plugs; i++)
    {
        switch (user.plug[i].id)
        {
        case microwave:
            if (min_field_width_appliance < strlen(appliances_string[microwave]))
                min_field_width_appliance = strlen(appliances_string[microwave]);
            break;

        case kettle:
            if (min_field_width_appliance < strlen(appliances_string[kettle]))
                min_field_width_appliance = strlen(appliances_string[kettle]);
            break;

        case oven:
            if (min_field_width_appliance < strlen(appliances_string[oven]))
                min_field_width_appliance = strlen(appliances_string[oven]);
            break;

        case refrigerator:
            if (min_field_width_appliance < strlen(appliances_string[refrigerator]))
                min_field_width_appliance = strlen(appliances_string[refrigerator]);
            break;

        case coffee:
            if (min_field_width_appliance < strlen(appliances_string[coffee]))
                min_field_width_appliance = strlen(appliances_string[coffee]);
            break;
        }
    }

    print_section("Your Consumption Today Compared To The Average Dane");
    printf("\n");
    for (i = 0; i < amount_of_plugs; i++)
    {
        /* this switch-case is sorted after the id of the users plugs.
        current_appliances is signed the int value of the current case. E.g. in case microwave: current_appliances is assigned microwave

        appliance_difference is used to print percentage later*/
        switch (user.plug[i].id)
        {
        case microwave:
            current_appliance = place_in_correct_array(microwave, user.plug[i].power_consumption, average.appliances[microwave].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);
            appliance_difference[microwave] = percent(user.plug[i].power_consumption, user_prev_avg.plug[microwave].power_consumption);

            break;

        case kettle:
            current_appliance = place_in_correct_array(kettle, user.plug[i].power_consumption, average.appliances[kettle].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);

            appliance_difference[kettle] = percent(user.plug[i].power_consumption, user_prev_avg.plug[kettle].power_consumption);

            break;

        case oven:
            current_appliance = place_in_correct_array(oven, user.plug[i].power_consumption, average.appliances[oven].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);

            appliance_difference[oven] = percent(user.plug[i].power_consumption, user_prev_avg.plug[oven].power_consumption);

            break;

        case refrigerator:
            current_appliance = place_in_correct_array(refrigerator, user.plug[i].power_consumption, average.appliances[refrigerator].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);
            appliance_difference[refrigerator] = percent(user.plug[i].power_consumption, user_prev_avg.plug[refrigerator].power_consumption);
            break;

        case coffee:
            current_appliance = place_in_correct_array(coffee, user.plug[i].power_consumption, average.appliances[coffee].power_consumption,
                                                       above_average_consumption, below_average_consumption, &index_above, &index_below);

            appliance_difference[coffee] = percent(user.plug[i].power_consumption, user_prev_avg.plug[coffee].power_consumption);

            break;
        }

        /* prints percentage of average on current appliance once for iteration of the for-loop. */
        print_percentage_of_average(i, current_appliance, user.plug[i].power_consumption, average.appliances[current_appliance].power_consumption, min_field_width_appliance, min_field_width_plug);
    }

    /* Prints the percent-comparison between user current consumption and the average of their previous*/

    /* Average power consumption, calculates from the appliances plugged in*/
    average_power_consumption = average_consumption(amount_of_plugs, average.appliances, user);
    printf("\nThe power consumption of the average Dane is %.4f kWh pr. day for the same appliances as you.\n", average_power_consumption);

    /* Total user consumption calculates from the amount of plug.*/
    your_total_consumption = total_consumption(amount_of_plugs, user.plug);
    printf("Your total power consumption is %.4f kWh today.\n", your_total_consumption);

    /* This if/else will print the percentage of user-power consumption compared to the average power consumption.
       And will tell you how much money it cost and Co2 it emitted, and will display charts of the data. */
    if (your_total_consumption > average_power_consumption)
        printf("You use %.2f%% more power then the average Dane.\n",
               (percent(your_total_consumption - average_power_consumption, average_power_consumption)));
    else
        printf("You use %.2f%% less power then the average Dane.\n",
               (percent(average_power_consumption - your_total_consumption, average_power_consumption)));

    printf("\n");

    print_section("Your Consumption Today Compared To Your Previous Days");
    printf("\n");
    if (days_simulated > 1)
    {
        for (i = 0; i < amount_of_plugs; i++)
        {
            if (user.plug[i].id != 0)
                printf("Consumption of your %-*s on plug %*d today is %6.2f%% compared to average of previous days.\n",
                       min_field_width_appliance, appliances_string_lwr[user.plug[i].id], min_field_width_plug, i + 1, appliance_difference[user.plug[i].id]);
        }
    }
    int is_smaller;

    /* prints whether your overall consumption is higher or lower than previously */
    double differnce = calculate_previous_average(user, user_prev_avg, amount_of_plugs, &is_smaller);
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

/* finds average of all power consumptions in user_prev_avg. returns percentage of the difference between previous consumption and current consumption*/

double calculate_previous_average(user_profile user, user_profile user_prev_avg, int amount_of_plugs, int *is_smaller)
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

    if (avg_of_prev_avg < avg_of_today)
    {
        *is_smaller = 1;
        return percent(avg_of_today - avg_of_prev_avg, avg_of_today);
    }
    else if (avg_of_prev_avg > avg_of_today)
    {
        *is_smaller = 0;
        return percent(avg_of_prev_avg - avg_of_today, avg_of_prev_avg);
    }
    else
    {
        *is_smaller = -1;
        return 0;
    }
}
