#ifndef PROTOTYPE_H_
#define PROTOTYPE_H_
#include <string.h>
#include <math.h>
#include "structs.h"
#include "tiny-json/tiny-json.h"

double average_consumption(int, appliance[], user_profile);
double total_consumption(int, appliance[]);
void bar_chart(user_profile, double, int);
void charts(user_profile, int, average_profile);
double percent(double, double);
double convert_power_to_cash(double);
double convert_power_to_CO2(double);
void print_break(void);
void print_title(char[]);
void print_section(char[]);
json_t const *open_json_file_user(void);
json_t const *open_json_file_average(void);
int parse_json_days_simulated(json_t const *);
user_profile parse_json_user_data(user_profile, user_profile *, json_t const *, int, int, int *);
average_profile parse_json_average_data(average_profile, json_t const *);
user_profile initialize_user_profile(user_profile, int *);
user_profile add_plug(user_profile, int);
void compare_plugs(user_profile, user_profile, average_profile, appliance *, appliance *, int, int *, int *, int);
int place_in_correct_array(int, double, double, appliance *, appliance *, int **, int **);
void print_percentage_of_average(int, int, double, double, int, int);
void print_tips(appliance[], appliance[], int, int);
void print_switch(appliance[], int);
double calc_prev_avg(user_profile, user_profile, int, int *);

#endif