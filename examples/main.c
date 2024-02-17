#include "../src/prettytable.h"

// My variables
int int_array[] = {1,2,3,4,5,6,7,8,9,10};
int empty_array[0] = {};
float float_array[] = {1.9234f,269.1f,2.454f};
bool bool_array[] = {true,false,true,false};

int a = 1345;
int *pointer = &a;
char *string = "This is a constant string";
bool boolean = true;
double my_double = 12340.92 - 3034.0223;

int main(int argc, char *argv[]) {
    // Create table
    Table *table = InitTable();
    SetAlignmentFormat(table, ALIGN_CENTER);
    // SetColorFormat(table, CF_INTEGER, COLOR_CYAN);
    // SetTheme(table, COOL_THEME);

    int *dynamic_array = malloc(3*sizeof(int));
    dynamic_array[0] = 3;
    dynamic_array[1] = 2;
    dynamic_array[2] = 1;

    // Append headers
    // AppendHeaders(table, "Char", "Integer", "Float", "String", "Boolean", "Array");
    AppendHeader(table, "Char", TYPE_ANY);
    AppendHeader(table, "Integer", TYPE_INT);
    AppendHeader(table, "Float", TYPE_FLOAT);
    AppendHeader(table, "String", TYPE_CHAR_POINTER);
    AppendHeader(table, "Boolean", TYPE_BOOL);
    AppendHeader(table, "Array", TYPE_ARRAY);
    AppendItems(table, (char)'A', a, 3.14159, "This is a string", true,  CreateArray(float_array));
    AppendItems(table, 'B', 87532, 3.0001f, "This is another string", boolean, CreateArray(empty_array));
    AppendItems(table, 67, 4444, my_double, "STRING IN CAPS", false, CreateArrayM(dynamic_array, 3));
    AppendItems(table, (char)'D', 1111111, 123, "AaBbCcDdEe", !true, CreateArray(bool_array));

    // Print the table
    PrintTable(table);

    // Be a good boy and free the table :)
    FreeTable(table);
    
    return 0;
}