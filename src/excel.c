#include "excel.h"
#include "database.h"
void writeColumnNamesToWorksheet(char **columnNames, size_t numberOfColumns, 
                                                lxw_worksheet *worksheet)
{
    for(int columnCount=0;columnCount<numberOfColumns;columnCount++)
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
    char **columns = splitString(columnNames, &numberOfColumnsInRow, "\t", headerColumnsNumber); // using numberOfColumnsInRow just to not make another variable
    writeColumnNamesToWorksheet(columns, headerColumnsNumber, worksheet);

    char **databaseBuffer = fetchall();
    int databaseRowsCount = getBufferRowsCount(); 
    for(size_t row = 0;row<databaseRowsCount;row++)
    {
        columns = splitString(databaseBuffer[row], &numberOfColumnsInRow, "\t", headerColumnsNumber);
        for(size_t col = 0;col<numberOfColumnsInRow;col++)
        {
            worksheet_write_string(worksheet, row+1, col, columns[col], NULL);
        }
    }
    workbook_close(workbook);
}
//I love @enormouscube ))))))))
