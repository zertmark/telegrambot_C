#include "xlsxwriter.h"
#include "database.h"
void writeColumnNamesToWorksheet(char **columnNames, size_t numberOfColumns, 
                                                lxw_worksheet *worksheet)
{
    for(size_t columnCount=0;columnCount<numberOfColumns;columnCount++)
    {
        worksheet_write_string(worksheet, 0, columnCount, columnNames[columnCount], NULL);    
        free(columnNames[columnCount]);
    }
}
int generateExcelFile(char *fileName, char *columnNames, 
                                size_t headerColumnsNumber) 
{
    if (!executeReadCommand("SELECT * FROM STACK;"))
    {
        return 0;
    }
    int numberOfColumnsInRow = 0;
    lxw_workbook  *workbook  = workbook_new(fileName);
    lxw_worksheet *worksheet = workbook_add_worksheet(workbook, NULL);
    // Using numberOfColumnsInRow just to not make another variable
    char **columns = splitString(columnNames, &numberOfColumnsInRow, "\t", headerColumnsNumber);
    writeColumnNamesToWorksheet(columns, headerColumnsNumber, worksheet);

    char **databaseBuffer = fetchall();
    size_t databaseRowsCount = getBufferRowsCount();
    for(size_t row = 0;row<databaseRowsCount;row++)
    {
        columns = splitString(databaseBuffer[row], &numberOfColumnsInRow, "\t", headerColumnsNumber);
        for(int col = 0;col<numberOfColumnsInRow;col++)
        {
            worksheet_write_string(worksheet, row+1, col, columns[col], NULL);
        }
    }
    workbook_close(workbook);
}
//I love @enormouscube ))))))))
