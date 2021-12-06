#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "tiny-json/tiny-json.h"

#define APPLIANCE_MAX 6
#define PLUGS_MAX 10
#define DAYS_MAX 100

enum appliances
{
    microwave = 1,
    kettle,
    oven,
    refrigerator,
    coffee_machine
};
typedef enum appliances appliances;

char *appliances_string[APPLIANCE_MAX] = {"empty", "Microwave", "Kettle", "Oven", "Refrigerator", "Coffee machine"};

struct appliance
{
    int id;
    double power_consumption;
};
typedef struct appliance appliance;

struct user_profile
{
    int household_size;
    appliance plug[PLUGS_MAX];
    time_t date;
};
typedef struct user_profile user_profile;

struct average_profile
{
    unsigned int household_size;
    appliance appliances[APPLIANCE_MAX];
};
typedef struct average_profile average_profile;

user_profile parse_json_data(user_profile, int *, int *);
void print_profile_struct(user_profile, int, int);

int main(void)
{

    user_profile user;
    int days_simulated, time;

    user = parse_json_data(user, &days_simulated, &time);

    //print_profile_struct(user, days_simulated, time);

    return EXIT_SUCCESS;
}

user_profile parse_json_data(user_profile user, int *days_simulated, int *time)
{
    /* Read file into a string */
    char *str = 0;
    long length;

    FILE *fp = fopen("data/user_data.json", "r");
    if (fp)
    {
        fseek(fp, 0, SEEK_END);
        length = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        str = malloc(length);
        if (str)
            fread(str, 1, length, fp);
    }
    fclose(fp);

    /* Create json */
    json_t mem[320];
    json_t const *json = json_create(str, mem, sizeof mem / sizeof *mem);
    if (!json)
    {
        puts("Error: Json create.");
    }

    /* Parse days simulated */
    json_t const *days = json_getProperty(json, "days_simulated");
    int const days_sim_value = (int)json_getInteger(days);
    *days_simulated = days_sim_value;

    //printf("data TYPE = %u\n", json_getType(days));

    printf("\n---------------\n\n\nDays simulated: %d \n\n", *days_simulated);

    /* Parse time (date) and data*/

    json_t const *data = json_getProperty(json, "date");
    if (!data || JSON_ARRAY != json_getType(data))
    {
        puts("Error: No array found for DATE");
    }

    //  printf("test0\n");
    //printf("DATA TYPE: %u\n", json_getType(data));

    int j = 0;

    json_t const *appliance;
    for (appliance = json_getChild(data); j <= *days_simulated; appliance = json_getSibling(appliance))
    {
        int i = 0;
        //     printf("Appliance VALUE: %s\n", json_getValue(appliance));
        //    printf("APP TYPE: %u\n", json_getType(appliance));
        int const date_time = (int)json_getInteger(appliance);
        *time = date_time;
        //  printf("Time (date): %d:\n", *time);
        time_t date_time_t = *time;
        struct tm *date_time_struct = localtime(&date_time_t);
        char time_string[20];
        strftime(time_string, 20, "%d-%m-%Y", date_time_struct);
        printf("Date: %s:\n", time_string);

        //printf("TIME: %d\n", time);/

        /*json_t const *date = json_getSibling(appliance);
        printf("Date TYPE: %u\n", json_getType(date)); */

        /*json_t const *dateChild = json_getChild(date);
        printf("DateChild TYPE: %u\n", json_getType(dateChild)); */
        appliance = json_getSibling(appliance);
        if (JSON_OBJ == json_getType(appliance))
        {
            /*   json_t const *date = json_getChild(appliance);
            char const *date_value = json_getValue(date); */

            /*             printf("DATE TYPE = %u\n", json_getType(date));
            printf("DATE = %p\n", date);
            printf("DATE : %s\n", date_value); */

            /* Parse microwave data */
            json_t const *microwave = json_getProperty(appliance, "microwave");
            json_t const *microwave_appliance_id = json_getProperty(microwave, "appliance_id");
            int const microwave_appliance_id_value = (int)json_getInteger(microwave_appliance_id);
            user.plug[i].id = microwave_appliance_id_value;

            json_t const *microwave_power_consumption = json_getProperty(microwave, "power_consumption");
            double const microwave_power_consumption_value = (double)json_getReal(microwave_power_consumption);
            user.plug[i].power_consumption = microwave_power_consumption_value;
            printf("Index: %d | Plug ID: %-15s | Power Consumption: %f kWh\n", i, appliances_string[user.plug[i].id], user.plug[i].power_consumption);
            i++;

            /* Parse kettle data */
            json_t const *kettle = json_getProperty(appliance, "kettle");
            json_t const *kettle_appliance_id = json_getProperty(kettle, "appliance_id");
            int const kettle_appliance_id_value = (int)json_getInteger(kettle_appliance_id);
            user.plug[i].id = kettle_appliance_id_value;

            json_t const *kettle_power_consumption = json_getProperty(kettle, "power_consumption");
            double const kettle_power_consumption_value = (double)json_getReal(kettle_power_consumption);
            user.plug[i].power_consumption = kettle_power_consumption_value;
            printf("Index: %d | Plug ID: %-15s | Power Consumption: %f kWh\n", i, appliances_string[user.plug[i].id], user.plug[i].power_consumption);
            i++;

            /* Parse oven data */
            json_t const *oven = json_getProperty(appliance, "oven");
            json_t const *oven_appliance_id = json_getProperty(oven, "appliance_id");
            int const oven_appliance_id_value = (int)json_getInteger(oven_appliance_id);
            user.plug[i].id = oven_appliance_id_value;

            json_t const *oven_power_consumption = json_getProperty(oven, "power_consumption");
            double const oven_power_consumption_value = (double)json_getReal(oven_power_consumption);
            user.plug[i].power_consumption = oven_power_consumption_value;
            printf("Index: %d | Plug ID: %-15s | Power Consumption: %f kWh\n", i, appliances_string[user.plug[i].id], user.plug[i].power_consumption);
            i++;

            /* Parse refrigerator data */
            json_t const *refrigerator = json_getProperty(appliance, "refrigerator");
            json_t const *refrigerator_appliance_id = json_getProperty(refrigerator, "appliance_id");
            int const refrigerator_appliance_id_value = (int)json_getInteger(refrigerator_appliance_id);
            user.plug[i].id = refrigerator_appliance_id_value;

            json_t const *refrigerator_power_consumption = json_getProperty(refrigerator, "power_consumption");
            double const refrigerator_power_consumption_value = (double)json_getReal(refrigerator_power_consumption);
            user.plug[i].power_consumption = refrigerator_power_consumption_value;
            printf("Index: %d | Plug ID: %-15s | Power Consumption: %f kWh\n", i, appliances_string[user.plug[i].id], user.plug[i].power_consumption);
            i++;

            /* Parse coffee machine data */
            json_t const *coffee_machine = json_getProperty(appliance, "coffee_machine");
            json_t const *coffee_machine_appliance_id = json_getProperty(coffee_machine, "appliance_id");
            int const coffee_machine_appliance_id_value = (int)json_getInteger(coffee_machine_appliance_id);
            user.plug[i].id = coffee_machine_appliance_id_value;

            json_t const *coffee_machine_power_consumption = json_getProperty(coffee_machine, "power_consumption");
            double const coffee_machine_power_consumption_value = (double)json_getReal(coffee_machine_power_consumption);
            user.plug[i].power_consumption = coffee_machine_power_consumption_value;
            printf("Index: %d | Plug ID: %-15s | Power Consumption: %f kWh\n\n", i, appliances_string[user.plug[i].id], user.plug[i].power_consumption);
        }
        j++;
    }

    /*------------------------------------*/

    /* 
    json_t const *jsonSibling = json_getSibling(data);
    json_t const *jsonSiblingChild = json_getSibling(jsonSibling);

    printf("jsonSibling TYPE = %u\n", json_getType(jsonSiblingChild));

    int i = 0;

    json_t const *date;
    for (date = json_getChild(jsonSiblingChild); date != 0; date = json_getSibling(jsonSibling))
    {
        printf("Test START\n");
        json_t const *appliance_id = json_getProperty(date, "appliance_id");
        int const appliance_id_value = (int)json_getInteger(appliance_id);
        user.plug[i].id = appliance_id_value;
        printf("AP ID: %d\n", appliance_id_value);

        json_t const *power_consumption = json_getProperty(date, "power_consumption");
        double const power_consumption_value = (double)json_getReal(power_consumption);
        user.plug[i].power_consumption = power_consumption_value;
        printf("POW CON: %f\n", power_consumption_value);

        printf("Test END\n");
        i++;
    }
 */
    /*-------------------------------------*/

    return user;
}

void print_profile_struct(user_profile user, int days_simulated, int time)
{

    int i, j;

    printf("\n---------------\n\n\nDays simulated: %d \n\n", days_simulated);

    for (j = 0; j <= days_simulated; j++)
    {
        printf("Tim3e (date): %d:\n", time);
        for (i = 0; i < PLUGS_MAX; i++)
        {
            printf("Index: %d | Plug ID: %-15s | Power Consumption: %f kWh\n", i, appliances_string[user.plug[i].id], user.plug[i].power_consumption);
        }
    }
}