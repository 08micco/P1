#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structs.h"
#include "prototyper.h"

/* Open file and create json type for user */
json_t const *open_json_file_user(void)
{
    /* Read file into a string */
    char *str_user = 0;
    long length_user;

    FILE *fp_user = fopen("data/user_data.json", "r");
    if (fp_user)
    {
        /* Length of file */
        fseek(fp_user, 0, SEEK_END);
        length_user = ftell(fp_user);
        fseek(fp_user, 0, SEEK_SET);
        /* Allocates memory to str_user */
        str_user = malloc(length_user);
        /* Reads file */
        if (str_user)
            fread(str_user, sizeof(char), length_user, fp_user);
    }
    fclose(fp_user);
    /* Create json */
    json_t mem[9000];
    json_t const *json_user = json_create(str_user, mem, sizeof mem / sizeof *mem);
    if (!json_user)
    {
        printf("\n\nError: Json create user.\n\n");
    }
    return json_user;
}

/* Open file and create json type for average */
json_t const *open_json_file_average(void)
{
    /* Read file into a string */
    char *str_average = 0;
    long length_average;

    FILE *fp_average = fopen("data/average_data.json", "r");
    if (fp_average)
    {
        /* Length of file */
        fseek(fp_average, 0, SEEK_END);
        length_average = ftell(fp_average);
        fseek(fp_average, 0, SEEK_SET);
        /* Allocates memory to str_average */
        str_average = malloc(length_average);
        /* Reads file */
        if (str_average)
            fread(str_average, sizeof(char), length_average, fp_average);
    }
    fclose(fp_average);
    /* Create json */
    json_t mem[9000];
    json_t const *json_average = json_create(str_average, mem, sizeof mem / sizeof *mem);
    if (!json_average)
    {
        printf("\n\nError: Json create average.\n\n");
    }
    return json_average;
}

/* Parse days simulated */
int parse_json_days_simulated(json_t const *json)
{
    /* Gets the value of key "days_simulated" */
    json_t const *days = json_getProperty(json, "days_simulated");
    int const days_sim_value = (int)json_getInteger(days);

    return days_sim_value;
}

/* Parse data for user */
user_profile parse_json_user_data(user_profile user, user_profile *user_prev_avg, json_t const *json_user, int days_simulated, int amount_of_plugs, int *time)
{
    int microwave_total = 0, kettle_total = 0, oven_total = 0, refrigerator_total = 0, coffee_total = 0;
    int i, j = 0;

    /* Find "date" in JSON file */
    json_t const *data_user = json_getProperty(json_user, "date");
    if (!data_user || JSON_ARRAY != json_getType(data_user))
    {
        printf("Error (Parse Json User Data): No array found for \"date\"\n");
    }

    /* Loop that parse data for every date */
    json_t const *appliance;
    for (appliance = json_getChild(data_user); j < days_simulated; appliance = json_getSibling(appliance))
    {
        /* Parse time */
        int const time_int = (int)json_getInteger(appliance);
        *time = time_int;

        /* Loop that parse data for every appliance */
        appliance = json_getSibling(appliance);
        if (JSON_OBJ == json_getType(appliance))
        {
            for (i = 0; i < amount_of_plugs; i++)
            {
                switch (user.plug[i].id)
                {
                case microwave:
                {
                    /* Parse microwave data */
                    json_t const *json_microwave = json_getProperty(appliance, "microwave");                            /* Search for "microwave" object */
                    json_t const *microwave_power_consumption = json_getProperty(json_microwave, "power_consumption");  /* Search for "power_consumption" object */
                    double const microwave_power_consumption_value = (double)json_getReal(microwave_power_consumption); /* Gets value for "power_consumption" */

                    /* If last day then assigns data to user_profile, else user_prev_avg */
                    if (j != days_simulated - 1)
                    {
                        user_prev_avg->plug[microwave].power_consumption += microwave_power_consumption_value;
                        microwave_total++;
                    }
                    else
                        user.plug[i].power_consumption = microwave_power_consumption_value;

                    break;
                }
                case kettle:
                {
                    /* Parse kettle data */
                    json_t const *json_kettle = json_getProperty(appliance, "kettle");
                    json_t const *kettle_power_consumption = json_getProperty(json_kettle, "power_consumption");
                    double const kettle_power_consumption_value = (double)json_getReal(kettle_power_consumption);
                    if (j != days_simulated - 1)
                    {
                        user_prev_avg->plug[kettle].power_consumption += kettle_power_consumption_value;
                        kettle_total++;
                    }
                    else
                        user.plug[i].power_consumption = kettle_power_consumption_value;

                    break;
                }
                case oven:
                {
                    /* Parse oven data */
                    json_t const *json_oven = json_getProperty(appliance, "oven");
                    json_t const *oven_power_consumption = json_getProperty(json_oven, "power_consumption");
                    double const oven_power_consumption_value = (double)json_getReal(oven_power_consumption);
                    if (j != days_simulated - 1)
                    {
                        user_prev_avg->plug[oven].power_consumption += oven_power_consumption_value;
                        oven_total++;
                    }
                    else
                        user.plug[i].power_consumption = oven_power_consumption_value;

                    break;
                }
                case refrigerator:
                {
                    /* Parse refrigerator data */
                    json_t const *json_refrigerator = json_getProperty(appliance, "refrigerator");
                    json_t const *refrigerator_power_consumption = json_getProperty(json_refrigerator, "power_consumption");
                    double const refrigerator_power_consumption_value = (double)json_getReal(refrigerator_power_consumption);
                    if (j != days_simulated - 1)
                    {
                        user_prev_avg->plug[refrigerator].power_consumption += refrigerator_power_consumption_value;
                        refrigerator_total++;
                    }
                    else
                        user.plug[i].power_consumption = refrigerator_power_consumption_value;

                    break;
                }
                case coffee:
                {
                    /* Parse coffee machine data */
                    json_t const *json_coffee_machine = json_getProperty(appliance, "coffee_machine");
                    json_t const *coffee_machine_power_consumption = json_getProperty(json_coffee_machine, "power_consumption");
                    double const coffee_machine_power_consumption_value = (double)json_getReal(coffee_machine_power_consumption);
                    if (j != days_simulated - 1)
                    {
                        user_prev_avg->plug[coffee].power_consumption += coffee_machine_power_consumption_value;
                        coffee_total++;
                    }
                    else
                        user.plug[i].power_consumption = coffee_machine_power_consumption_value;

                    break;
                }
                default:
                    printf("ERROR (pare_json_user_data switch): ID not found (ID: %d)\n", user.plug[i].id);
                    break;
                }
            }
        }
        j++;
    }
    /* Calculates the average */
    if (days_simulated > 1)
    {
        user_prev_avg->plug[microwave].power_consumption /= microwave_total;
        user_prev_avg->plug[kettle].power_consumption /= kettle_total;
        user_prev_avg->plug[oven].power_consumption /= oven_total;
        user_prev_avg->plug[refrigerator].power_consumption /= refrigerator_total;
        user_prev_avg->plug[coffee].power_consumption /= coffee_total;
    }
    return user;
}

/* Parse data for average */
average_profile parse_json_average_data(average_profile average, json_t const *json)
{
    int days_simulated = 1;
    int j = 0;

    /* Find "data" in JSON file */
    json_t const *data = json_getProperty(json, "data");
    if (!data || JSON_ARRAY != json_getType(data))
    {
        puts("Error (Parse Json Data): No array found for \"data\"");
    }

    json_t const *appliance;
    for (appliance = json_getChild(data); j < days_simulated; appliance = json_getSibling(appliance))
    {
        /* Parse data for every appliance  */
        appliance = json_getSibling(appliance);
        if (JSON_OBJ == json_getType(appliance))
        {
            /* Parse microwave data */
            json_t const *json_microwave = json_getProperty(appliance, "microwave");                 /* Search for "microwave" object */
            json_t const *microwave_appliance_id = json_getProperty(json_microwave, "appliance_id"); /* Search for "appliance_id" object */
            int const microwave_appliance_id_value = (int)json_getInteger(microwave_appliance_id);   /* Gets value for "appliance_id" */
            average.appliances[microwave].id = microwave_appliance_id_value;

            json_t const *microwave_power_consumption = json_getProperty(json_microwave, "power_consumption");  /* Search for "power_consumption" object */
            double const microwave_power_consumption_value = (double)json_getReal(microwave_power_consumption); /* Gets value for "power_consumption" */
            average.appliances[microwave].power_consumption = microwave_power_consumption_value;

            /* Parse kettle data */
            json_t const *json_kettle = json_getProperty(appliance, "kettle");
            json_t const *kettle_appliance_id = json_getProperty(json_kettle, "appliance_id");
            int const kettle_appliance_id_value = (int)json_getInteger(kettle_appliance_id);
            average.appliances[kettle].id = kettle_appliance_id_value;

            json_t const *kettle_power_consumption = json_getProperty(json_kettle, "power_consumption");
            double const kettle_power_consumption_value = (double)json_getReal(kettle_power_consumption);
            average.appliances[kettle].power_consumption = kettle_power_consumption_value;

            /* Parse oven data */
            json_t const *json_oven = json_getProperty(appliance, "oven");
            json_t const *oven_appliance_id = json_getProperty(json_oven, "appliance_id");
            int const oven_appliance_id_value = (int)json_getInteger(oven_appliance_id);
            average.appliances[oven].id = oven_appliance_id_value;

            json_t const *oven_power_consumption = json_getProperty(json_oven, "power_consumption");
            double const oven_power_consumption_value = (double)json_getReal(oven_power_consumption);
            average.appliances[oven].power_consumption = oven_power_consumption_value;

            /* Parse refrigerator data */
            json_t const *json_refrigerator = json_getProperty(appliance, "refrigerator");
            json_t const *refrigerator_appliance_id = json_getProperty(json_refrigerator, "appliance_id");
            int const refrigerator_appliance_id_value = (int)json_getInteger(refrigerator_appliance_id);
            average.appliances[refrigerator].id = refrigerator_appliance_id_value;

            json_t const *refrigerator_power_consumption = json_getProperty(json_refrigerator, "power_consumption");
            double const refrigerator_power_consumption_value = (double)json_getReal(refrigerator_power_consumption);
            average.appliances[refrigerator].power_consumption = refrigerator_power_consumption_value;

            /* Parse coffee machine data */
            json_t const *json_coffee_machine = json_getProperty(appliance, "coffee_machine");
            json_t const *coffee_machine_appliance_id = json_getProperty(json_coffee_machine, "appliance_id");
            int const coffee_machine_appliance_id_value = (int)json_getInteger(coffee_machine_appliance_id);
            average.appliances[coffee].id = coffee_machine_appliance_id_value;

            json_t const *coffee_machine_power_consumption = json_getProperty(json_coffee_machine, "power_consumption");
            double const coffee_machine_power_consumption_value = (double)json_getReal(coffee_machine_power_consumption);
            average.appliances[coffee].power_consumption = coffee_machine_power_consumption_value;
        }
        j++;
    }
    return average;
}
