#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "prototyper.h"

/* Prints data on DKK and CO2 */
void charts(user_profile user, int amount_of_plugs, average_profile average)
{
    double your_total_consumption;
    your_total_consumption = total_consumption(amount_of_plugs, user.plug);

    printf("You have used approximately %.2f DKK on power today.\n", convert_power_to_cash(your_total_consumption));
    printf("And have emitted %.2f kg of CO2.\n\n", convert_power_to_CO2(your_total_consumption));

    print_section("Chart Displays Distribution of Appliances Total Power Consumption");
    bar_chart(user, your_total_consumption, amount_of_plugs);
}

/* Bar chart over the user consumption */
void bar_chart(user_profile user, double consumption, int plug_amount)
{
    int i, j;
    for (i = 0; i < plug_amount; i++)
    {
        printf("\n%s on plug %d \n", appliances_string[(appliances)user.plug[i].id], (i + 1));

        for (j = 0; j < percent(user.plug[i].power_consumption, consumption); j++)
            printf("|");

        printf(" [%.2f%%]\n", percent(user.plug[i].power_consumption, consumption));
    }
}
