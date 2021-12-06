#include <stdio.h>
#include <stdlib.h>
#include "./tiny-json/tiny-json.h"

#define APPLIANCE_MAX 6
#define PLUGS_MAX 10

enum appliances
{
    microwave = 1,
    kettle,
    oven,
    refrigerator,
    coffee
};
typedef enum appliances appliances;

char *appliances_string[APPLIANCE_MAX] = {"empty", "Microwave", "Kettle", "Oven", "Refrigerator", "Coffee machine"};
//
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
};
typedef struct user_profile user_profile;

struct average_profile
{
    unsigned int household_size;
    appliance appliances[APPLIANCE_MAX];
};
typedef struct average_profile average_profile;

user_profile parse_json_data(user_profile);
void print_user_struct(user_profile user);
user_profile parse_json_user_data(user_profile user, json_value *value);

typedef struct date
{
    int day;
    int month;
    int year;
} date;

int main(void)
{

    char *str = 0;
    long length;

    FILE *fp = fopen("user_data.json", "r");

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

    json_t mem[320];
    json_t const *json = json_create(str, mem, sizeof mem / sizeof *mem);
    if (!json)
    {
        puts("Error json create.");
        return EXIT_FAILURE;
    }

    json_t const *date = json_getProperty(json, "01-12-2021");
    json_t const *dateChild;
    for (dateChild = json_getChild(date); dateChild != 0; dateChild = json_getSibling(dateChild))
    {
        json_t const *appliance_id = json_getProperty(dateChild, "appliance_id");
        int const appliance_id_value = (int)json_getInteger(appliance_id);

        json_t const *power_consumption = json_getProperty(dateChild, "power_consumption");
        double const power_consumption_value = (double)json_getReal(power_consumption);

        printf("ID: %d | Power Consumption: %f\n", appliance_id_value, power_consumption_value);
    }

    return EXIT_SUCCESS;
}