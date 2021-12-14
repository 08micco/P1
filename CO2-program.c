#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "tiny-json/tiny-json.h"
#include "Prototyper.h"
#include "structs.h"

/* Main program */
int main(void)
{
    appliance above_average_consumption[PLUGS_MAX];
    appliance below_average_consumption[PLUGS_MAX];
    int index_above = 0, index_below = 0;
    user_profile user;
    user_profile user_prev_avg;

    /* user_profile user_data; */
    average_profile average;

    memset(&user, 0, sizeof(user));
    memset(&user_prev_avg, 0, sizeof(user_prev_avg));
    memset(&average, 0, sizeof(average));
    int amount_of_plugs = 0;
    int time;

    /* Line 30 collects the manual user input of appliances and plugs. */
    print_title("Create Your Home Profile");
    user = initialize_user_profile(user, &amount_of_plugs);

    /* Json file */
    json_t const *json_user = open_json_file_user();
    int days_simulated = parse_json_days_simulated(json_user);
    printf("Registered data for %d day(s).\n", days_simulated);
    print_break();
    user = parse_json_user_data(user, &user_prev_avg, json_user, days_simulated, amount_of_plugs, &time);
    json_t const *json_average = open_json_file_average();
    average = parse_json_average_data(average, json_average);

    print_title("Comparison of Appliances");
    /* Here, compare_plugs is called to calculate and compare user data to dataset of average use. */
    compare_plugs(user, user_prev_avg, average, above_average_consumption, below_average_consumption, amount_of_plugs, &index_above, &index_below, days_simulated);

    print_break();

    /* Here, the function used to display power usage in charts is called. */
    print_title("Overview of Your Power Usage");
    charts(user, amount_of_plugs, average);
    print_break();

    /* Here, the function used to display tips for the user is called. */
    print_title("Tips To Improve Your CO2 Footprint");
    initialize_tips(above_average_consumption, below_average_consumption, index_above, index_below);
    print_break();

    return EXIT_SUCCESS;
}