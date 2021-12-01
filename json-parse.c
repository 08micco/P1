#include <stdio.h>
#include <stdlib.h>
#include "json-parser-master/json.h"
#include <sys/stat.h>

/* COMPILE (Static link)

    "   gcc -o json_parse -I.. json-parse.c json-parser-master/json.c -lm  "



RUN:

  " json_parse.exe data/user_data.json  "

 */

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

struct average_profile
{
    unsigned int household_size;
    appliance appliances[APPLIANCE_MAX];
};
typedef struct average_profile average_profile;

user_profile parse_json_data(user_profile);
static void process_value(json_value *value, int depth);

static void print_depth_shift(int depth)
{
    int j;
    for (j = 0; j < depth; j++)
    {
        printf(" ");
    }
}

static void process_value(json_value *value, int depth);

static void process_object(json_value *value, int depth)
{
    int length, x;
    if (value == NULL)
    {
        return;
    }
    length = value->u.object.length;
    for (x = 0; x < length; x++)
    {
        print_depth_shift(depth);
        printf("object[%d].name = %s\n", x, value->u.object.values[x].name);
        process_value(value->u.object.values[x].value, depth + 1);
    }
}

static void process_array(json_value *value, int depth)
{
    int length, x;
    if (value == NULL)
    {
        return;
    }
    length = value->u.array.length;
    printf("array\n");
    for (x = 0; x < length; x++)
    {
        process_value(value->u.array.values[x], depth);
    }
}

static void process_value(json_value *value, int depth)
{
    if (value == NULL)
    {
        return;
    }
    if (value->type != json_object)
    {
        print_depth_shift(depth);
    }
    switch (value->type)
    {
    case json_none:
        printf("none\n");
        break;
    case json_null:
        printf("null\n");
        break;
    case json_object:
        process_object(value, depth + 1);
        break;
    case json_array:
        process_array(value, depth + 1);
        break;
    case json_integer:
        printf("int: %10ld\n", (long)value->u.integer);
        break;
    case json_double:
        printf("double: %f\n", value->u.dbl);
        break;
    case json_string:
        printf("string: %s\n", value->u.string.ptr);
        break;
    case json_boolean:
        printf("bool: %d\n", value->u.boolean);
        break;
    }
}

int main(int argc, char **argv)
{
    char *filename;
    FILE *fp;
    struct stat filestatus;
    int file_size;
    char *file_contents;
    json_char *json;
    json_value *value;
    user_profile user;

    if (argc != 2)
    {
        fprintf(stderr, "%s <file_json>\n", argv[0]);
        return 1;
    }
    filename = argv[1];

    if (stat(filename, &filestatus) != 0)
    {
        fprintf(stderr, "File %s not found\n", filename);
        return 1;
    }
    file_size = filestatus.st_size;
    file_contents = (char *)malloc(filestatus.st_size);
    if (file_contents == NULL)
    {
        fprintf(stderr, "Memory error: unable to allocate %d bytes\n", file_size);
        return 1;
    }

    fp = fopen(filename, "rb");
    if (fp == NULL)
    {
        fprintf(stderr, "Unable to open %s\n", filename);
        fclose(fp);
        free(file_contents);
        return 1;
    }
    int nr;
    nr = fread(file_contents, file_size, 1, fp);
    printf("%d\n\n", nr);

    if (nr != 1)
    {
        fprintf(stderr, "Unable to read content of %s | %d | %d\n", filename, errno, file_size);
        fclose(fp);
        free(file_contents);
        return 1;
    }
    fclose(fp);

    printf("%s\n", file_contents);

    printf("--------------------------------\n\n");

    json = (json_char *)file_contents;

    value = json_parse(json, file_size);

    if (value == NULL)
    {
        fprintf(stderr, "Unable to parse data\n");
        free(file_contents);
        exit(1);
    }

    process_value(value, 0);

    json_value_free(value);
    free(file_contents);
    return 0;
}
