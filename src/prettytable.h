#ifndef PRETTYTABLE_H_
#define PRETTYTABLE_H_

#ifndef PRETTYTABLEAPI
    #define PRETTYTABLEAPI       // Functions defined as 'extern' by default (implicit specifiers)
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HEADERS_INIT_CAP 5
#define ROWS_INIT_CAP 20

#define TABLE_EDGE '+'
#define TABLE_HORZ '-'
#define TABLE_VERT '|'
#define CARRIAGE_RETURN_UP "\x1B[1A"

typedef struct Entry {
    char *value;
    size_t index;
} Entry;

typedef struct Headers {
    Entry *headers;
    size_t size;
    size_t capacity;
} Headers;

typedef struct Row {
    Entry *values;
    size_t index;
} Row;

typedef struct Rows {
    Row *rows;
    size_t size;
    size_t capacity;
} Rows;

typedef struct Table {
    Headers *headers;
    Rows *rows;
} Table;

PRETTYTABLEAPI Table *InitTable();
PRETTYTABLEAPI void FreeTable(Table *table);
PRETTYTABLEAPI size_t TableSize(Table *table);
PRETTYTABLEAPI size_t TableCols(Table *table);
PRETTYTABLEAPI Entry TableGetHeader(Table *table, size_t header_index);
PRETTYTABLEAPI Entry TableGetEntry(Table *table, size_t header_index, int value_index);
PRETTYTABLEAPI int AppendHeader(Table *table, const char* header);
PRETTYTABLEAPI int AppendRow(Table *table, char **rows);
PRETTYTABLEAPI void PrintTable(Table *table);
PRETTYTABLEAPI void PrintHeaders(Table *table);
PRETTYTABLEAPI void PrintRows(Table *table);

#endif // PRETTYTABLE_H_

/***********************************************************************************
*
*   PRETTYTABLE IMPLEMENTATION
*
************************************************************************************/

static Headers *_InitHeaders();
static Rows *_InitRows();
static void _PrintHorizontalLine(Table *table, size_t *row_lengths);
static void _PrintHeadersLine(Table *table, size_t *row_lengths);
static void _PrintEntriesLine(Table *table, size_t *row_lengths, int row);

Table *InitTable() {
    Table *table = (Table*)malloc(sizeof(Table));
    table->headers = _InitHeaders();
    table->rows = _InitRows();
    return table;
}

void FreeTable(Table *table) {
    // Free headers
    size_t cols = TableCols(table);
    for (size_t i = 0; i < cols; i++) {
        free(table->headers->headers[i].value);
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

size_t TableSize(Table *table) {
    return table->rows->size;
}

size_t TableCols(Table *table) {
    return table->headers->size;
}

int AppendHeader(Table *table, const char* header) {
    if (table->headers->size == table->headers->capacity) {
        table->headers->headers = (Entry*)realloc(table->headers->headers, table->headers->capacity*2*sizeof(Entry));
        if (!table->headers->headers) {
            fprintf(stderr, "Error reallocating for new headers");
            return 1;
        }
        table->headers->capacity *= 2;
    }
    table->headers->headers[table->headers->size] = (Entry){strdup(header), table->headers->size};
    table->headers->size++;
    return 0;
}

int AppendRow(Table *table, char **rows) {
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
        row.values[i] = (Entry){strdup(rows[i]), table->rows->size};
    }
    table->rows->rows[table->rows->size] = row;
    table->rows->size++;
    return 0;
}

Entry TableGetEntry(Table *table, size_t header_index, int value_index) {
    if (value_index < 0) {
        return TableGetHeader(table, header_index);
    }
    return table->rows->rows[value_index].values[header_index];
}

Entry TableGetHeader(Table *table, size_t header_index) {
    return table->headers->headers[header_index];
}

char *GenerateBlankString(int strlen) {
    char *str = (char*)malloc(strlen + 1);
    for (size_t i = 0; i < strlen; i++) {
        str[i] = ' ';
    }
    str[strlen] = '\0';
    return str;
}

void PrintTable(Table *table) {
    // Calculate max lengths of every row and header
    size_t table_size = TableSize(table);
    size_t row_lengths[table_size + 1]; // Header + Rows
    for (size_t i = 0; i < TableCols(table); i++) {
        size_t max_length = 0;
        for (size_t j = 0; j < TableSize(table); j++) {
           Entry e = TableGetEntry(table, i, j);
           size_t len = strlen(e.value);
           max_length = len > max_length ? len : max_length;
        }
        size_t header_len = strlen(TableGetHeader(table, i).value); // If header is bigger than some row, put it as max len
        row_lengths[i] = header_len > max_length ? header_len : max_length;
    }

    // ---------------------DRAW HEADERS---------------------
    _PrintHorizontalLine(table, row_lengths);
    _PrintHeadersLine(table, row_lengths);
    _PrintHorizontalLine(table, row_lengths);
    
    // ---------------------DRAW ROWS---------------------
    for (size_t r = 0; r < TableSize(table); r++) {
        _PrintEntriesLine(table, row_lengths, r);
    }
    _PrintHorizontalLine(table, row_lengths);
}

void PrintHeaders(Table *table) {
    size_t table_size = TableSize(table);
    size_t row_lengths[table_size + 1]; // Header + Rows
    for (size_t i = 0; i < TableCols(table); i++) {
        row_lengths[i] = strlen(TableGetHeader(table, i).value); // If header is bigger than some row, put it as max len
    }
    _PrintHorizontalLine(table, row_lengths);
    _PrintHeadersLine(table, row_lengths);
    // Draw second horizontal line with edges (bottom horizontal line of headers)
    _PrintHorizontalLine(table, row_lengths);
}

void PrintRows(Table *table) {
    // Calculate max lengths of every row and header
    size_t table_size = TableSize(table);
    size_t row_lengths[table_size + 1]; // Header + Rows
    for (size_t i = 0; i < TableCols(table); i++) {
        size_t max_length = 0;
        for (size_t j = 0; j < TableSize(table); j++) {
           Entry e = TableGetEntry(table, i, j);
           size_t len = strlen(e.value);
           max_length = len > max_length ? len : max_length;
        }
        row_lengths[i] = max_length;
    }
    _PrintHorizontalLine(table, row_lengths);
    // Draw rows with blank string to accomodate them
    for (size_t r = 0; r < TableSize(table); r++) {
         _PrintEntriesLine(table, row_lengths, r);
    }
    _PrintHorizontalLine(table, row_lengths);
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------

Headers *_InitHeaders() {
    Headers *headers = (Headers*)malloc(sizeof(Headers));
    headers->headers = (Entry*)malloc(HEADERS_INIT_CAP*sizeof(Entry));
    headers->size = 0;
    headers->capacity = HEADERS_INIT_CAP;
    return headers;
}

Rows *_InitRows() {
    Rows *rows = (Rows*)malloc(sizeof(Rows));
    rows->rows = (Row*)malloc(ROWS_INIT_CAP*sizeof(Row));
    rows->size = 0;
    rows->capacity = ROWS_INIT_CAP;
    return rows;
}

static void _PrintHorizontalLine(Table *table, size_t *row_lengths) {
    // Draw first horizontal line with edges (top horizontal line of table/headers)
    for (size_t i = 0; i < TableCols(table); i++) {
        printf("%c%c", TABLE_EDGE, TABLE_HORZ);
        for (size_t j = 0; j < row_lengths[i]; j++) {
            printf("%c", TABLE_HORZ);    
        }
        printf("%c", TABLE_HORZ);
    }
    printf("%c\n", TABLE_EDGE);
}

static void _PrintHeadersLine(Table *table, size_t *row_lengths) {
    _PrintEntriesLine(table, row_lengths, -1);
}

static void _PrintEntriesLine(Table *table, size_t *row_lengths, int row) {
    printf("%c", TABLE_VERT);
    // Draw headers with blank string to accomodate them
    for (size_t i = 0; i < TableCols(table); i++) {
        char *value = TableGetEntry(table, i, row).value;
        size_t strlen_value = strlen(value);
        char *blanks_string = GenerateBlankString(row_lengths[i] - strlen_value); 
        printf(" %s%s %c", value, blanks_string, TABLE_VERT);
        free(blanks_string);
    }
    printf("\n");
}
