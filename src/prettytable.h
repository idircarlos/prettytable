#ifndef PRETTYTABLE_H_
#define PRETTYTABLE_H_

#ifndef PRETTYTABLEAPI
    #define PRETTYTABLEAPI
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define COLOR_BLACK     "\e[0;30m"
#define COLOR_RED       "\e[0;31m"
#define COLOR_GREEN     "\e[0;32m"
#define COLOR_YELLOW    "\e[0;33m"
#define COLOR_BLUE      "\e[0;34m"
#define COLOR_MAGENTA   "\e[0;35m"
#define COLOR_CYAN      "\e[0;36m"
#define COLOR_WHITE     "\e[0;37m"
#define COLOR_RESET     "\e[0m"

#define TRUE  "true"
#define FALSE "false"

#define HEADERS_INIT_CAP 10
#define ROWS_INIT_CAP 20

#define TEMP_BUFF_SIZE 8192

#define MAX_COLOR_FORMATS 10

#define TABLE_EDGE "+"
#define TABLE_HORZ "-"
#define TABLE_VERT "|"

#define TYPE_FORMAT_BOOL "%s"
#define TYPE_FORMAT_UNSIGNED_CHAR "%c"
#define TYPE_FORMAT_CHAR "%c"
#define TYPE_FORMAT_SIGNED_CHAR "%c"
#define TYPE_FORMAT_SHORT_INT "%hi"
#define TYPE_FORMAT_UNSIGNED_SHORT_INT "%hu"
#define TYPE_FORMAT_INT "%d"
#define TYPE_FORMAT_UNSIGNED_INT "%u"
#define TYPE_FORMAT_LONG_INT "%ld"
#define TYPE_FORMAT_UNSIGNED_LONG_INT "%lu"
#define TYPE_FORMAT_LONG_LONG_INT "%lld"
#define TYPE_FORMAT_UNSIGNED_LONG_LONG_INT "%llu"
#define TYPE_FORMAT_FLOAT "%f"
#define TYPE_FORMAT_DOUBLE "%lf"
#define TYPE_FORMAT_LONG_DOUBLE "%Lf"
#define TYPE_FORMAT_CHAR_POINTER "%s"
#define TYPE_FORMAT_VOID_POINTER "%p"
#define TYPE_FORMAT_INT_POINTER "%p"
#define TYPE_FORMAT_OTHER "%p"

#define false (bool)0
#define true (bool)!false

#define TYPE(x) _Generic((x),                                                                      \
            bool: TYPE_BOOL,                   unsigned char: TYPE_UNSIGNED_CHAR,          \
             char: TYPE_CHAR,                     signed char: TYPE_SIGNED_CHAR,            \
        short int: TYPE_SHORT_INT,         unsigned short int: TYPE_UNSIGNED_SHORT_INT,     \
              int: TYPE_INT,                     unsigned int: TYPE_UNSIGNED_INT,           \
         long int: TYPE_LONG_INT,           unsigned long int: TYPE_UNSIGNED_LONG_INT,      \
    long long int: TYPE_LONG_LONG_INT, unsigned long long int: TYPE_UNSIGNED_LONG_LONG_INT, \
            float: TYPE_FLOAT,                         double: TYPE_DOUBLE,                 \
      long double: TYPE_LONG_DOUBLE,                   char *: TYPE_CHAR_POINTER,           \
           void *: TYPE_VOID_POINTER,                   int *: TYPE_INT_POINTER,            \
            Array: TYPE_ARRAY,                        default: TYPE_OTHER                                                                      \
        )

#define FORMAT(t) switch(t) { \
                        case TYPE_UNSIGNED_CHAR:                TYPE_FORMAT_UNSIGNED_CHAR; break;            \
                        case TYPE_CHAR:                         TYPE_FORMAT_CHAR; break;                \
                        case TYPE_SIGNED_CHAR:                  TYPE_FORMAT_SIGNED_CHAR; break;                \
                        case TYPE_SHORT_INT:                    TYPE_FORMAT_SHORT_INT; break;                 \
                        case TYPE_UNSIGNED_SHORT_INT:           TYPE_FORMAT_UNSIGNED_SHORT_INT; break;         \
                        case TYPE_INT:                          TYPE_FORMAT_INT; break;                        \
                        case TYPE_UNSIGNED_INT:                 TYPE_FORMAT_UNSIGNED_INT; break;               \
                        case TYPE_LONG_INT:                     TYPE_FORMAT_LONG_INT; break;                   \
                        case TYPE_UNSIGNED_LONG_INT:            TYPE_FORMAT_UNSIGNED_LONG_INT; break;          \
                        case TYPE_LONG_LONG_INT:                TYPE_FORMAT_LONG_LONG_INT; break;              \
                        case TYPE_UNSIGNED_LONG_LONG_INT:       TYPE_FORMAT_UNSIGNED_LONG_LONG_INT; break;     \
                        case TYPE_FLOAT:                        TYPE_FORMAT_FLOAT; break;                      \
                        case TYPE_DOUBLE:                       TYPE_FORMAT_DOUBLE; break;                     \
                        case TYPE_LONG_DOUBLE:                  TYPE_FORMAT_LONG_DOUBLE; break;              \
                        case TYPE_CHAR_POINTER:                 TYPE_FORMAT_CHAR_POINTER; break;               \
                        case TYPE_VOID_POINTER:                 TYPE_FORMAT_VOID_POINTER; break;               \
                        case TYPE_INT_POINTER:                  TYPE_FORMAT_INT_POINTER; break;                \
                        case TYPE_OTHER:                        TYPE_FORMAT_OTHER; break;                      \
                    }

#define AppendItem(row, item)   do {                                                                            \
                                    char *temp_buff = (char*)calloc(TEMP_BUFF_SIZE, sizeof(char));                                                                                                                                      \
                                    Table *from_table = row.parent_table;                                 \
                                    Type temp_type = TableGetHeaderType(from_table, row.length);              \
                                    if (temp_type == TYPE_ANY) {                                                \
                                        temp_type = TYPE(item);                                         \
                                    }\
                                    char *temp_type_format;                                     \
                                    switch(temp_type) {                                                                                           \
                                        case TYPE_BOOL:                                            \
                                            _BuildBoolToString(temp_buff, 1, item);    \
                                            row.values[row.length++] = (Entry){strdup(temp_buff), row.length, TYPE_BOOL};      \
                                            break; \
                                        \
                                        case TYPE_ARRAY:    \
                                            _BuildArrayToString(temp_buff, 1, item);\
                                            row.values[row.length++] = (Entry){strdup(temp_buff), row.length, temp_type};   \
                                            break;          \
                                        case TYPE_UNSIGNED_CHAR:                temp_type_format = TYPE_FORMAT_UNSIGNED_CHAR; break;              \
                                        case TYPE_CHAR:                         temp_type_format = TYPE_FORMAT_CHAR; break;                 \
                                        case TYPE_SIGNED_CHAR:                  temp_type_format = TYPE_FORMAT_SIGNED_CHAR; break;                \
                                        case TYPE_SHORT_INT:                    temp_type_format = TYPE_FORMAT_SHORT_INT; break;                  \
                                        case TYPE_UNSIGNED_SHORT_INT:           temp_type_format = TYPE_FORMAT_UNSIGNED_SHORT_INT; break;         \
                                        case TYPE_INT:                          temp_type_format = TYPE_FORMAT_INT; break;                        \
                                        case TYPE_UNSIGNED_INT:                 temp_type_format = TYPE_FORMAT_UNSIGNED_INT; break;               \
                                        case TYPE_LONG_INT:                     temp_type_format = TYPE_FORMAT_LONG_INT; break;                   \
                                        case TYPE_UNSIGNED_LONG_INT:            temp_type_format = TYPE_FORMAT_UNSIGNED_LONG_INT; break;          \
                                        case TYPE_LONG_LONG_INT:                temp_type_format = TYPE_FORMAT_LONG_LONG_INT; break;              \
                                        case TYPE_UNSIGNED_LONG_LONG_INT:       temp_type_format = TYPE_FORMAT_UNSIGNED_LONG_LONG_INT; break;     \
                                        case TYPE_FLOAT:                        temp_type_format = TYPE_FORMAT_FLOAT; break;                      \
                                        case TYPE_DOUBLE:                       temp_type_format = TYPE_FORMAT_DOUBLE; break;                     \
                                        case TYPE_LONG_DOUBLE:                  temp_type_format = TYPE_FORMAT_LONG_DOUBLE; break;                \
                                        case TYPE_CHAR_POINTER:                 temp_type_format = TYPE_FORMAT_CHAR_POINTER; break;               \
                                        case TYPE_VOID_POINTER:                 temp_type_format = TYPE_FORMAT_VOID_POINTER; break;               \
                                        case TYPE_INT_POINTER:                  temp_type_format = TYPE_FORMAT_INT_POINTER; break;                \
                                        case TYPE_OTHER:                        temp_type_format = TYPE_FORMAT_OTHER; break;                      \
                                        case TYPE_BOOL_TRUE:                    temp_type_format = TRUE; break;                                \
                                        case TYPE_BOOL_FALSE:                   temp_type_format = FALSE; break;                                   \
                                        default:                                temp_type_format = TYPE_FORMAT_OTHER; break;                      \
                                    }\
                                    if (temp_type != TYPE_BOOL && temp_type != TYPE_ARRAY) {\
                                        sprintf(temp_buff, temp_type_format, item);   \
                                        row.values[row.length++] = (Entry){strdup(temp_buff), row.length, temp_type};\
                                    }\
                                    free(temp_buff); \
                                } while(0); \

#define AppendHeaders(table, ...) _AppendHeaders(table, sizeof((const char* []) {__VA_ARGS__}) / sizeof(const char*), __VA_ARGS__)

#define __AppendItems_typeid(a) \
	__builtin_choose_expr(__AppendItems_is_type(a, double), TYPE_DOUBLE, \
	__builtin_choose_expr(__AppendItems_is_type(a, float), TYPE_FLOAT, \
	__builtin_choose_expr(__AppendItems_is_type(a, char), TYPE_CHAR, \
	__builtin_choose_expr(__AppendItems_is_type(a, unsigned char), TYPE_UNSIGNED_CHAR, \
	__builtin_choose_expr(__AppendItems_is_type(a, int), TYPE_INT, \
	__builtin_choose_expr(__AppendItems_is_type(a, unsigned int), TYPE_UNSIGNED_INT, \
	__builtin_choose_expr(__AppendItems_is_type(a, long), TYPE_LONG_INT, \
	__builtin_choose_expr(__AppendItems_is_type(a, unsigned long), TYPE_UNSIGNED_LONG_INT, \
	__builtin_choose_expr(__AppendItems_is_type(a, short), TYPE_SHORT_INT, \
	__builtin_choose_expr(__AppendItems_is_type(a, char*), TYPE_CHAR_POINTER, \
	__builtin_choose_expr(__AppendItems_is_type(a, char[]), TYPE_CHAR_POINTER, \
	__builtin_choose_expr(__AppendItems_is_type(a, void*), TYPE_VOID_POINTER, \
	__builtin_choose_expr(__AppendItems_is_type(a, int[]), TYPE_INT_POINTER, \
    __builtin_choose_expr(__AppendItems_is_type(a, int*), TYPE_INT_POINTER, \
    __builtin_choose_expr(__AppendItems_is_type(a, bool), TYPE_BOOL, \
    __builtin_choose_expr(__AppendItems_is_type(a, Array), TYPE_ARRAY, \
	__builtin_choose_expr(sizeof(a) == 1, 2, \
	__builtin_choose_expr(sizeof(a) == 2, 4, \
	(0)  ))))))))))))))))))

#define __AppendItems_push(c,size,cont) ((void)cont, *--_p = c | (size << 5))
#define __builtin_choose_expr __builtin_choose_expr
#define __AppendItems_is_type(a, t) __builtin_types_compatible_p(typeof(a), t)
#define __AppendItems_count_int(q,w,e,r,t,y,u,i,o,p,a,s,d,f,g,h,j,k,l,z,x,c,v,b,n,m,...) m
#define __AppendItems_count(a...)__AppendItems_count_int(a,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0)

#define __AppendItems_code(a, cont) __builtin_choose_expr(__AppendItems_is_type(a, void), 0, __AppendItems_push(__AppendItems_typeid(a), (sizeof(a)<(1<<16>>5)?sizeof(a):(1<<16>>5)-1), cont))

#define __AppendItems_types_int(q,w,e,r,t,y,u,i,o,p,a,s,d,f,g,h,j,k,l,z,x,c,v,b,n,m,...)__AppendItems_code(q,__AppendItems_code(w,__AppendItems_code(e,__AppendItems_code(r,__AppendItems_code(t,__AppendItems_code(y,__AppendItems_code(u,__AppendItems_code(i,__AppendItems_code(o,__AppendItems_code(p,__AppendItems_code(a,__AppendItems_code(s,__AppendItems_code(d,__AppendItems_code(f,__AppendItems_code(g,__AppendItems_code(h,__AppendItems_code(j,__AppendItems_code(k,__AppendItems_code(l,__AppendItems_code(z,__AppendItems_code(x,__AppendItems_code(c,__AppendItems_code(v,__AppendItems_code(b,__AppendItems_code(n,__AppendItems_code(m, 0))))))))))))))))))))))))))

#define __AppendItems_types(a...) __AppendItems_types_int(a, (void)0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1)

// Append multiple items to a table
#define AppendItems(table, a...) ({ \
    Row row = InitRow(table);   \
    int count = __AppendItems_count(a); \
	unsigned short stack[count], *_p = stack + count; \
	__AppendItems_types(a); \
	__AppendItems_func(row, count, _p, a); \
    AppendRow2(table, row);         \
})

// Create an array structure from a fixed size array
#define CreateArray(array) _CreateArray(array, sizeof(array)/sizeof(array[0]), TYPE(array[0]))
// Create an array structure from a dynamic size array
#define CreateArrayM(array, size) _CreateArray(array, size, TYPE(array[0]))

#define IS_ODD(n) n % 2 != 0

// Types of data available
typedef enum {
    TYPE_BOOL = 0,
    TYPE_BOOL_TRUE,
    TYPE_BOOL_FALSE,
    TYPE_UNSIGNED_CHAR,
    TYPE_CHAR,
    TYPE_SIGNED_CHAR,
    TYPE_SHORT_INT,
    TYPE_UNSIGNED_SHORT_INT,
    TYPE_INT,
    TYPE_UNSIGNED_INT,
    TYPE_LONG_INT,
    TYPE_UNSIGNED_LONG_INT,
    TYPE_LONG_LONG_INT,
    TYPE_UNSIGNED_LONG_LONG_INT,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_LONG_DOUBLE,
    TYPE_CHAR_POINTER ,
    TYPE_VOID_POINTER,
    TYPE_INT_POINTER,
    TYPE_ARRAY,
    TYPE_OTHER,
    TYPE_ANY,
} Type;

// Alignments formats
typedef enum {
    ALIGN_LEFT = 0,
    ALIGN_RIGHT = 1,
    ALIGN_CENTER = 2,
} AlignmentFormat;

// Color formats
typedef enum {
    CF_HEADER = 0,
    CF_CHAR,
    CF_INTEGER,
    CF_FLOAT,
    CF_STRING,
    CF_FALSE,
    CF_TRUE,
    CF_POINTER,
    CF_TABLE,
    CF_DEFAULT,
} ColorFormatKey;

// Array definition
typedef struct Array {
    void *data;
    Type type;
    int length;
} Array;

typedef _Bool bool;
typedef char* ANSIColorCode;
typedef struct Table Table;

// Theme: Array color that define how the values are printed in the table
typedef struct Theme {
    ANSIColorCode *color_formats;
} Theme;

// Format options on how the table is printed
typedef struct {
    AlignmentFormat alignment;
    Theme theme;
} FormatOptions;

// Define an entry of the table
typedef struct Entry {
    char *value;
    size_t index;
    Type type;
} Entry;

// Define a header of the table
typedef struct Header {
    Entry entry;
    Type header_type;
} Header;

// Define a dynamic array of headers of the table
typedef struct Headers {
    Header *headers;
    size_t size;
    size_t capacity;
} Headers;

// Define a row of the table
typedef struct Row {
    Table *parent_table;
    Entry *values;
    size_t length;
    size_t index;
} Row;

// Define a dynamic array of rows of the table
typedef struct Rows {
    Row *rows;
    size_t size;
    size_t capacity;
} Rows;

// Define a table
struct Table {
    Headers *headers;
    Rows *rows;
    FormatOptions format;
};

PRETTYTABLEAPI Table *InitTable();                                                              // Init an empty table
PRETTYTABLEAPI Row InitRow(Table *table);                                                       // Init an empty row for a table
PRETTYTABLEAPI size_t TableSize(Table *table);                                                  // Returns the number of rows of the table
PRETTYTABLEAPI size_t TableCols(Table *table);                                                  // Returns the number of cols of the table
PRETTYTABLEAPI Entry TableGetHeader(Table *table, size_t header_index);                         // Returns the header given an index
PRETTYTABLEAPI Type TableGetHeaderType(Table *table, size_t header_index);                      // Returns the header type given an index
PRETTYTABLEAPI Entry TableGetEntry(Table *table, size_t header_index, int value_index);         // Returns the entry given the header index and the row index 
PRETTYTABLEAPI int AppendHeader(Table *table, const char* header, Type columnType);             // Appends a header given with a default type  
PRETTYTABLEAPI int AppendRow(Table *table, char **rows);                                        // Appends a row which is an array of strings
PRETTYTABLEAPI void SetAlignmentFormat(Table *table, AlignmentFormat alignment);                // Sets the alignment format to a table
PRETTYTABLEAPI void SetColorFormat(Table *table, ColorFormatKey key, ANSIColorCode color);      // Sets the color format to a table given the key
PRETTYTABLEAPI void SetTheme(Table *table, Theme theme);                                        // Sets a theme to a table, which is a set of defined formated colors
PRETTYTABLEAPI void PrintTable(Table *table);                                                   // Prints a table with its format options
PRETTYTABLEAPI void PrintHeaders(Table *table);                                                 // Prints the headers of a table with its format options
PRETTYTABLEAPI void PrintRows(Table *table);                                                    // Prints the rows of a table with its format options
PRETTYTABLEAPI void FreeTable(Table *table);                                                    // Frees a table

#endif // PRETTYTABLE_H_

/***********************************************************************************
*
*   PRETTYTABLE IMPLEMENTATION
*
************************************************************************************/

static FormatOptions _InitFormatOptions();                                                      // Inits the format options
static Headers *_InitHeaders();                                                                 // Inits headers
static Rows *_InitRows();                                                                       // Inits rows
static ANSIColorCode _GetColorFromType(Table *table, Type type);                                // Gets the color of a data type
static char *_GenerateBlankString(int strlen);                                                  // Generate a blank string of a size
static void _PrintHorizontalLine(Table *table, size_t *col_lengths);                            // Prints an horizontal line with given the col sizes
static void _PrintHeadersLine(Table *table, size_t *col_lengths);                               // Prints the headers line
static void _PrintEntriesLine(Table *table, size_t *col_lengths, int row);                      // Prints a row line
static void _BuildBoolToString(char *temp_buff, int size, ...);                                 // Builds the boolean string. VA_ARGS is a trick, to pass any variable type here from a macro, so we dont get errors.
static void _BuildArrayToString(char *temp_buff, int size, ...);                                // Builds the array string. VA_ARGS is a trick, to pass any variable type here from a macro, so we dont get errors.
static Array _CreateArray(void *array, int length, Type type);                                  // Creates an array
static void _AppendArrayElementByType(Array *array, int index, char *temp_buff);                // Appends the item string into the buffer to build the array string
static char* _GetFormatFromType(Type type);                                                     // Gets the format from a type

ANSIColorCode blank_color_formats[] = {COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE, COLOR_WHITE};
ANSIColorCode cool_color_formats[]  = {COLOR_CYAN, COLOR_BLUE, COLOR_MAGENTA, COLOR_YELLOW, COLOR_WHITE, COLOR_RED, COLOR_GREEN, COLOR_CYAN, COLOR_WHITE, COLOR_WHITE};

PRETTYTABLEAPI Theme BLANK_THEME = { .color_formats = blank_color_formats };
PRETTYTABLEAPI Theme COOL_THEME  = { .color_formats = cool_color_formats };

PRETTYTABLEAPI Table *InitTable() {
    Table *table = (Table*)malloc(sizeof(Table));
    table->headers = _InitHeaders();
    table->rows = _InitRows();
    table->format = _InitFormatOptions();
    return table;
}

PRETTYTABLEAPI Row InitRow(Table *table) {
    Row row = { 0 };
    row.parent_table = table;
    row.values = (Entry*)malloc(TableCols(table)*sizeof(Entry));
    row.length = 0;
    return row;
}

PRETTYTABLEAPI size_t TableSize(Table *table) {
    return table->rows->size;
}

PRETTYTABLEAPI size_t TableCols(Table *table) {
    return table->headers->size;
}

PRETTYTABLEAPI int AppendHeader(Table *table, const char* header, Type columnType) {
    if (table->headers->size == table->headers->capacity) {
        table->headers->headers = (Header*)realloc(table->headers->headers, table->headers->capacity*2*sizeof(Header));
        if (!table->headers->headers) {
            fprintf(stderr, "Error reallocating for new headers");
            return 1;
        }
        table->headers->capacity *= 2;
    }
    table->headers->headers[table->headers->size].entry = (Entry){strdup(header), table->headers->size, TYPE_CHAR_POINTER};
    table->headers->headers[table->headers->size].header_type = columnType;
    table->headers->size++;
    return 0;
}

PRETTYTABLEAPI int _AppendHeaders(Table *table, int size, ...) {
    va_list ap;
    va_start(ap, size);
    for (int i = 0; i < size; i++) {
        const char *header = va_arg(ap, const char*);
        AppendHeader(table, header, TYPE_ANY);
    }
    va_end(ap);
    return 0;
}

void __AppendItems_func(Row row, int size, unsigned short types[], ...) {
    va_list v;
	va_start(v, types);
    for (int i = 0; i < size; i++) {
        Type type = (Type)(types[i] & 0x1F);        
        switch(type) {
            case TYPE_DOUBLE: AppendItem(row, (double)va_arg(v, double)); break;
            case TYPE_FLOAT: AppendItem(row, (float)va_arg(v, double)); break;
            case TYPE_CHAR: AppendItem(row, (char)va_arg(v, int)); break;
            case TYPE_INT: AppendItem(row, (int)va_arg(v, int)); break;
            case TYPE_LONG_INT: AppendItem(row, (long)va_arg(v, long)); break;
            case TYPE_SHORT_INT: AppendItem(row, (short)va_arg(v, int)); break;
            case TYPE_CHAR_POINTER: AppendItem(row, (char*)va_arg(v, char*)); break;
            case TYPE_INT_POINTER: AppendItem(row, (int*)va_arg(v, int*)); break;
            case TYPE_VOID_POINTER: AppendItem(row, (void*)va_arg(v, void*)); break;
            case TYPE_ARRAY: AppendItem(row, (Array)va_arg(v, Array)); break;
            case TYPE_BOOL: AppendItem(row, (bool)va_arg(v, int)); break;
        }
    }
}   

PRETTYTABLEAPI int AppendRow(Table *table, char **rows) {
    size_t table_size = table->headers->size;
    if (table->rows->size == table->rows->capacity) {
        table->rows->rows = (Row*)realloc(table->rows->rows, table->rows->capacity*2*sizeof(Row));
        if (!table->rows->rows) {
            fprintf(stderr, "Error reallocating for new rows");
            return 1;
        }
        table->rows->capacity *= 2;
    }
    Row row = {0};
    row.values = (Entry*)malloc(table_size*sizeof(Entry));
    row.index = table->rows->size;
    for (size_t i = 0; i < table_size; i++) {
        row.values[i] = (Entry){strdup(rows[i]), table->rows->size, TYPE_CHAR_POINTER};
    }
    table->rows->rows[table->rows->size] = row;
    table->rows->size++;
    return 0;
}

PRETTYTABLEAPI int AppendRow2(Table *table, Row row) {
    if (table->rows->size == table->rows->capacity) {
        table->rows->rows = (Row*)realloc(table->rows->rows, table->rows->capacity*2*sizeof(Row));
        if (!table->rows->rows) {
            fprintf(stderr, "Error reallocating for new rows");
            return 1;
        }
        table->rows->capacity *= 2;
    }    
    row.index = table->rows->size;
    table->rows->rows[table->rows->size] = row;
    table->rows->size++;
    return 0;
}

PRETTYTABLEAPI Entry TableGetEntry(Table *table, size_t header_index, int value_index) {
    if (value_index < 0) {
        return TableGetHeader(table, header_index);
    }
    return table->rows->rows[value_index].values[header_index];
}

PRETTYTABLEAPI Entry TableGetHeader(Table *table, size_t header_index) {
    return table->headers->headers[header_index].entry;
}

PRETTYTABLEAPI Type TableGetHeaderType(Table *table, size_t header_index) {
    return table->headers->headers[header_index].header_type;
}

PRETTYTABLEAPI void SetAlignmentFormat(Table *table, AlignmentFormat alignment) {
    table->format.alignment = alignment;
}

PRETTYTABLEAPI void SetColorFormat(Table *table, ColorFormatKey key, ANSIColorCode color) {
    table->format.theme.color_formats[key] = color;
}

PRETTYTABLEAPI void SetTheme(Table *table, Theme theme) {
    table->format.theme = theme;
}

PRETTYTABLEAPI void PrintTable(Table *table) {
    // Calculate max lengths of every row and header
    size_t table_size = TableSize(table);
    size_t table_cols = TableCols(table);
    size_t col_lengths[table_cols]; // Headers
    for (size_t i = 0; i < table_cols; i++) {
        size_t max_length = 0;
        for (size_t j = 0; j < table_size; j++) {
           Entry e = TableGetEntry(table, i, j);
           size_t len = strlen(e.value);
           max_length = len > max_length ? len : max_length;
        }
        size_t header_len = strlen(TableGetHeader(table, i).value); // If header is bigger than some row, put it as max len
        col_lengths[i] = header_len > max_length ? header_len : max_length;
    }

    // ---------------------DRAW HEADERS---------------------
    _PrintHorizontalLine(table, col_lengths);
    _PrintHeadersLine(table, col_lengths);
    _PrintHorizontalLine(table, col_lengths);
    
    // ---------------------DRAW ROWS---------------------
    for (size_t r = 0; r < TableSize(table); r++) {
        _PrintEntriesLine(table, col_lengths, r);
    }
    _PrintHorizontalLine(table, col_lengths);
}

PRETTYTABLEAPI void PrintHeaders(Table *table) {
    size_t table_size = TableSize(table);
    size_t col_lengths[table_size + 1]; // Header + Rows
    for (size_t i = 0; i < TableCols(table); i++) {
        col_lengths[i] = strlen(TableGetHeader(table, i).value); // If header is bigger than some row, put it as max len
    }
    _PrintHorizontalLine(table, col_lengths);
    _PrintHeadersLine(table, col_lengths);
    // Draw second horizontal line with edges (bottom horizontal line of headers)
    _PrintHorizontalLine(table, col_lengths);
}

PRETTYTABLEAPI void PrintRows(Table *table) {
    // Calculate max lengths of every row and header
    size_t table_size = TableSize(table);
    size_t col_lengths[table_size + 1]; // Header + Rows
    for (size_t i = 0; i < TableCols(table); i++) {
        size_t max_length = 0;
        for (size_t j = 0; j < TableSize(table); j++) {
           Entry e = TableGetEntry(table, i, j);
           size_t len = strlen(e.value);
           max_length = len > max_length ? len : max_length;
        }
        col_lengths[i] = max_length;
    }
    _PrintHorizontalLine(table, col_lengths);
    // Draw rows with blank string to accomodate them
    for (size_t r = 0; r < TableSize(table); r++) {
         _PrintEntriesLine(table, col_lengths, r);
    }
    _PrintHorizontalLine(table, col_lengths);
}

PRETTYTABLEAPI void FreeTable(Table *table) {
    // Free headers
    size_t cols = TableCols(table);
    for (size_t i = 0; i < cols; i++) {
        free(table->headers->headers[i].entry.value);
    }
    free(table->headers->headers);
    free(table->headers);
    // Free rows
    size_t rows = TableSize(table);
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            free(table->rows->rows[i].values[j].value);
        }
        free(table->rows->rows[i].values);
    }
    free(table->rows->rows);
    free(table->rows);
    free(table);
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------

static FormatOptions _InitFormatOptions() {
    FormatOptions format = {0};
    format.alignment = ALIGN_LEFT;
    format.theme = BLANK_THEME;
    return format;
}

static Headers *_InitHeaders() {
    Headers *headers = (Headers*)malloc(sizeof(Headers));
    headers->headers = (Header*)malloc(HEADERS_INIT_CAP*sizeof(Header));
    headers->size = 0;
    headers->capacity = HEADERS_INIT_CAP;
    return headers;
}

static Rows *_InitRows() {
    Rows *rows = (Rows*)malloc(sizeof(Rows));
    rows->rows = (Row*)malloc(ROWS_INIT_CAP*sizeof(Row));
    rows->size = 0;
    rows->capacity = ROWS_INIT_CAP;
    return rows;
}

static ANSIColorCode _GetColorFromType(Table *table, Type type) {
    switch (type) {
        case TYPE_BOOL_TRUE:
            return table->format.theme.color_formats[CF_TRUE];
        case TYPE_BOOL_FALSE:
            return table->format.theme.color_formats[CF_FALSE];
        case TYPE_UNSIGNED_CHAR:
        case TYPE_CHAR:
        case TYPE_SIGNED_CHAR:
            return table->format.theme.color_formats[CF_CHAR];
        case TYPE_SHORT_INT:
        case TYPE_UNSIGNED_SHORT_INT:
        case TYPE_INT:
        case TYPE_UNSIGNED_INT:
        case TYPE_LONG_INT:
        case TYPE_UNSIGNED_LONG_INT:
        case TYPE_LONG_LONG_INT:
        case TYPE_UNSIGNED_LONG_LONG_INT:
            return table->format.theme.color_formats[CF_INTEGER];
        case TYPE_FLOAT:
        case TYPE_DOUBLE:
        case TYPE_LONG_DOUBLE:
            return table->format.theme.color_formats[CF_FLOAT];
        case TYPE_CHAR_POINTER:
            return table->format.theme.color_formats[CF_STRING];
        case TYPE_VOID_POINTER:
        case TYPE_INT_POINTER:
            return table->format.theme.color_formats[CF_POINTER];
        case TYPE_OTHER:
        default:
            return table->format.theme.color_formats[CF_DEFAULT];
    }
}

static char *_GenerateBlankString(int strlen) {
    char *str = (char*)malloc(strlen + 1);
    memset(str, ' ', strlen);
    str[strlen] = '\0';
    return str;
}

static void _PrintHorizontalLine(Table *table, size_t *col_lengths) {
    // Draw first horizontal line with edges (top horizontal line of table/headers)
    for (size_t i = 0; i < TableCols(table); i++) {
        printf("%s%s", TABLE_EDGE, TABLE_HORZ);
        for (size_t j = 0; j < col_lengths[i]; j++) {
            printf("%s", TABLE_HORZ);    
        }
        printf("%s", TABLE_HORZ);
    }
    printf("%s\n", TABLE_EDGE);
}

static void _PrintHeadersLine(Table *table, size_t *col_lengths) {
    _PrintEntriesLine(table, col_lengths, -1);
}

static void _PrintEntriesLine(Table *table, size_t *col_lengths, int row) {
    printf("%s", TABLE_VERT);
    ANSIColorCode color;
    // Draw headers with blank string to accomodate them
    for (size_t i = 0; i < TableCols(table); i++) {
        Entry entry = TableGetEntry(table, i, row);
        color = _GetColorFromType(table, entry.type);
        char *value = entry.value;
        size_t strlen_value = strlen(value);
        char *blanks_string_1;
        char *blanks_string_2;
        if (table->format.alignment == ALIGN_LEFT) {
            blanks_string_1 = _GenerateBlankString(col_lengths[i] - strlen_value);
            printf(" %s%s%s%s %s", color, value, COLOR_RESET, blanks_string_1, TABLE_VERT);
        }
        else if (table->format.alignment == ALIGN_RIGHT) {
            blanks_string_1 = _GenerateBlankString(col_lengths[i] - strlen_value);
            printf(" %s%s%s%s %s", blanks_string_1, color, value, COLOR_RESET, TABLE_VERT);
        }
        else if (table->format.alignment == ALIGN_CENTER) {
            int blanks = (col_lengths[i] - strlen_value);
            int left_blanks = blanks/2;
            int right_blanks = left_blanks;
            if (IS_ODD(blanks)) left_blanks++;
            blanks_string_1 = _GenerateBlankString(left_blanks);
            blanks_string_2 = _GenerateBlankString(right_blanks);
            printf(" %s%s%s%s%s %s", blanks_string_1, color, value, COLOR_RESET, blanks_string_2, TABLE_VERT);
            free(blanks_string_2);
        }
        free(blanks_string_1);
    }
    printf("\n");
}

static void _BuildBoolToString(char *temp_buff, int size, ...) {
    va_list ap;
    va_start(ap, size);
    bool b = va_arg(ap, int);
    va_end(ap);
    sprintf(temp_buff, TYPE_FORMAT_CHAR_POINTER, b == false ? FALSE : TRUE); 
}

void _BuildArrayToString(char *temp_buff, int size, ...) {
    va_list ap;
    va_start(ap, size);
    Array array = va_arg(ap, Array);
    va_end(ap);
    strcat(temp_buff, "["); 
    char *temp_buff_aux = (char*)calloc(TEMP_BUFF_SIZE, sizeof(char));        
    int i;
    for (i = 0; i < array.length - 1; i++) {
        _AppendArrayElementByType(&array, i, temp_buff_aux);                   
        strcat(temp_buff, temp_buff_aux);               
        strcat(temp_buff, ", ");    
        strcpy(temp_buff_aux, "");                                               
    }
    if (array.length != 0) {
        _AppendArrayElementByType(&array, i, temp_buff_aux);             
        strcat(temp_buff, temp_buff_aux);    
    }       
    strcat(temp_buff, "]");
    free(temp_buff_aux);
}

static Array _CreateArray(void *array, int length, Type type) {
    Array a = {0};
    a.data = array;
    a.type = type;
    a.length = length;
    return a;    
}

void _AppendArrayElementByType(Array *array, int index, char *temp_buff) {
    char* format = _GetFormatFromType(array->type);
    switch(array->type) {
        case TYPE_INT: sprintf(temp_buff, format, ((int*)(array->data))[index]); break;
        case TYPE_FLOAT: sprintf(temp_buff, format, ((float*)(array->data))[index]); break;
        case TYPE_BOOL: sprintf(temp_buff, format, ((bool*)(array->data))[index] == false ? FALSE : TRUE); break;
        default:
    } 
}

static char* _GetFormatFromType(Type type) {
    switch(type) { 
        case TYPE_BOOL:                         return TYPE_FORMAT_BOOL;
        case TYPE_UNSIGNED_CHAR:                return TYPE_FORMAT_UNSIGNED_CHAR;
        case TYPE_CHAR:                         return TYPE_FORMAT_CHAR;
        case TYPE_SIGNED_CHAR:                  return TYPE_FORMAT_SIGNED_CHAR;
        case TYPE_SHORT_INT:                    return TYPE_FORMAT_SHORT_INT;
        case TYPE_UNSIGNED_SHORT_INT:           return TYPE_FORMAT_UNSIGNED_SHORT_INT;
        case TYPE_INT:                          return TYPE_FORMAT_INT;
        case TYPE_UNSIGNED_INT:                 return TYPE_FORMAT_UNSIGNED_INT;
        case TYPE_LONG_INT:                     return TYPE_FORMAT_LONG_INT;
        case TYPE_UNSIGNED_LONG_INT:            return TYPE_FORMAT_UNSIGNED_LONG_INT;
        case TYPE_LONG_LONG_INT:                return TYPE_FORMAT_LONG_LONG_INT;
        case TYPE_UNSIGNED_LONG_LONG_INT:       return TYPE_FORMAT_UNSIGNED_LONG_LONG_INT;
        case TYPE_FLOAT:                        return TYPE_FORMAT_FLOAT;
        case TYPE_DOUBLE:                       return TYPE_FORMAT_DOUBLE;
        case TYPE_LONG_DOUBLE:                  return TYPE_FORMAT_LONG_DOUBLE;
        case TYPE_CHAR_POINTER:                 return TYPE_FORMAT_CHAR_POINTER;
        case TYPE_VOID_POINTER:                 return TYPE_FORMAT_VOID_POINTER;
        case TYPE_INT_POINTER:                  return TYPE_FORMAT_INT_POINTER;
        case TYPE_OTHER:                        return TYPE_FORMAT_OTHER;
        default:                                return TYPE_FORMAT_OTHER;
    }
    return TYPE_FORMAT_OTHER;
}
