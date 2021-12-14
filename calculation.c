#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "Prototyper.h"

/* Function that calculate total consumption. The function is called with different arrays of the type appliance*/
double average_consumption(int amount, appliance array[], user_profile user)
{
    double consumption = 0;
    int i, micro_yes = 0, kettle_yes = 0, oven_yes = 0, refrigerator_yes = 0, coffee_yes = 0;
    /* This loop will calculate the average power consumption in the kitchen, based of off the appliances the user has selected.
       Each appliance can only be added once to the average, no matter the users amount. */
    for (i = 0; i < amount; i++)
        switch (user.plug[i].id)
        {
        case microwave:
            if (micro_yes != 1)
            {
                consumption += array[microwave].power_consumption;
                micro_yes++;
            }
            break;

        case kettle:
            if (kettle_yes != 1)
            {
                consumption += array[kettle].power_consumption;
                kettle_yes++;
            }
            break;

        case oven:
            if (oven_yes != 1)
            {
                consumption += array[oven].power_consumption;
                oven_yes++;
            }
            break;

        case refrigerator:
            if (refrigerator_yes != 1)
            {
                consumption += array[refrigerator].power_consumption;
                refrigerator_yes++;
            }
            break;

        case coffee:
            if (coffee_yes != 1)
            {
                consumption += array[coffee].power_consumption;
                coffee_yes++;
            }
            break;

        default:
            printf("Error (average_consumption): ID not found (ID: %d)\n", user.plug[i].id);
            break;
        }

    return consumption;
}

double total_consumption(int amount, appliance array[])
{
    /* This function calculates the users total power consumption, and assembles the data in one variable. */
    double tot_con = 0;
    int i;
    for (i = 0; i < amount; i++)
        tot_con += array[i].power_consumption;

    return tot_con;
}

/* A simple function for calculating percentage. */
double percent(double a, double b)
{
    return (a / b) * 100;
}

/* A simple function for calculating cost of power. */
double convert_power_to_cash(double power)
{
    return power * KWH_TO_DKK;
}

/* A simple function for calculating emission in kg, caused by power usage. */
double convert_power_to_CO2(double power)
{
    return power * KWH_TO_CO2_KG;
}

/* Prints percentage of average consumption.*/
void print_percentage_of_average(int i, int app, double user_cons, double average_cons, int min_field_width_appliance, int min_field_width_plug)
{
    printf("Consumption of your %-*s on plug %*d is %6.4f kWh. This is %6.2lf%% of the average Dane.\n", min_field_width_appliance,
           appliances_string_lwr[app], min_field_width_plug, i + 1, user_cons, percent(user_cons, average_cons));
}