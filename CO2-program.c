#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define APPLIANCE_MAX 6
#define PLUGS_MAX 10

enum appliances
{
    microwave = 1,
    kettle,
    oven,
    refrigerator,
    freezer
};
typedef enum appliances appliances;

char *appliances_string[APPLIANCE_MAX] = {"empty", "Microwave", "Kettle", "Oven", "Refrigerator", "Freezer"};

struct appliance
{
    appliances id;
    double power_consumption;
};
typedef struct appliance appliance;

struct user_profile
{
    int household_size;
    appliance plug[PLUGS_MAX];
};
typedef struct user_profile user_profile;

/* prototypes */
user_profile initialize_user_profile(user_profile user, int *);
void add_plug(user_profile, int);
void compareFunction(user_profile, user_profile, appliance *);
void printTips(appliance *);
void print_break(void);
void write_appliance_data_to_file(FILE *, user_profile);

/* Main program */
int main(void)
{
    FILE *data_file;

    appliance higher_consumption[APPLIANCE_MAX];
    user_profile user;
    user_profile general;
    int amount_of_plugs;

    user = initialize_user_profile(user, &amount_of_plugs);

    //compareFunction(user, general, higher_consumption);
    //printTips(higher_consumption);

    write_appliance_data_to_file(data_file, user);

    return EXIT_SUCCESS;
}

/* Assigns appliance to plug */
void add_plug(user_profile user, int plug_index)
{
    printf("Add appliances from the list:\n%d | %s\n%d | %s\n%d | %s\n%d | %s\n%d | %s\nSelect appliance: ",
           microwave, appliances_string[microwave],
           kettle, appliances_string[kettle],
           oven, appliances_string[oven],
           refrigerator, appliances_string[refrigerator],
           freezer, appliances_string[freezer]);

    scanf(" %d", &user.plug[plug_index].id); /* Assigns appliance id to the related plug.*/
}

/* User profile informations is set here */
user_profile initialize_user_profile(user_profile user, int *plug_index)
{
    printf("What is the size of the household?\n"); /* Scan for household size to the user*/
    printf("Number of peoples: ");
    scanf("%d", &user.household_size);
    print_break();

    int run = 1;
    int scan_input;
    *plug_index = 1;

    while (run && *plug_index < PLUGS_MAX) /* All the kitchen appliances is added to user profile here */
    {
        add_plug(user, *plug_index);
        print_break();

        printf("Add one more appliance?:\n1 | Yes\n2 | No\nSelect option: ");
        scanf("%d", &scan_input);
        print_break();
        if (scan_input == 1)
        {
            *plug_index += 1;
            run = 1;
        }
        else
            run = 0;
        }
  
    printf("\nplugs: %d  size %d.\n", *plug_index, user.household_size); /* Skal slettes senere */
    return user;
}

/* Print a breakline */
void print_break(void)
{
    printf("----------------------------------------------------\n");
}


/* Thin function writes household size, appliance number and their power consumption to a file */
void write_appliance_data_to_file(FILE *file, user_profile user)
{
    int i;
    file = fopen("data/test.txt", "w+");

    if (file == NULL)
    {
        printf("\nError while creating file\n");
        exit(1);
    }

    fprintf(file, "Household size: %d\n", user.household_size);
    for (i = 0; i <= APPLIANCE_MAX; i++)
    {
        fprintf(file, "Appliance: %d, Power Consumption: %f\n", user.appliances[i].id, user.appliances[i].power_consumption);
    }

    fclose(file);
}
