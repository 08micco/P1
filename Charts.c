#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "prototyper.h"

void charts(user_profile user, int amount_of_plugs, average_profile average)
{
    double your_total_consumption, average_power_consumption;

    /* Average power consumption, calculates from the appliances plugged in*/
    average_power_consumption = average_consumption(amount_of_plugs, average.appliances, user);
    printf("The power consumption of the average Dane is %.4f kWh pr. day for the same appliances as you.\n", average_power_consumption);

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

    printf("\nYou have used approximately %.2f DKK on power today.\n", convert_power_to_cash(your_total_consumption));
    printf("And have emitted %.2f kg of CO2.\n\n", convert_power_to_CO2(your_total_consumption));

    print_section("Chart Displays Distribution of Appliances Total Power Consumption");
    bar_chart(user, your_total_consumption, amount_of_plugs);
}

/* Bar chart over the user consumption*/
void bar_chart(user_profile user, double consumption, int plug_amount)
{
    int i, j;
    for (i = 0; i < plug_amount; i++)
    {
        printf("\n %s on plug %d \n", appliances_string[(appliances)user.plug[i].id], (i + 1));

        for (j = 0; j < percent(user.plug[i].power_consumption, consumption); j++)
            printf("|");

        printf(" [%.2f%%]\n", percent(user.plug[i].power_consumption, consumption));
    }
}
