/********************************************************************************************************
Title:     Group Project - "Data Mining"

Authors: George Ebeling
         Brendon Dalm
         Tyler Redding
         Tyler Freeman
         Corey Moura

Class:   CIS241
Date:     7/31/2020
Prof:     Dr. Vijay Bhuse

Description:     Write a C program to read in the market data from the text file and draw some
                statistical conclusions from the data. There are no obvious right or wrong answers.
                Your grade depends on:

                    Following items are to be included in your report:
                    •    Source code
                    •    Output supporting conclusions drawn
                    •    Video demo
                    •    Report citing contributions of team members


Notes:
        PROGRAM NOTES:
            DECLARE THE STRUCTURE BEFORE THE FUNCTIONS
            Line count using terminal: wc -l marketData.txt -> determines MACRO NUMENTRIES

            Function Flow:
                Main > importFile >    advanceCursor >  REPEATS: extractLine > formatLine > storeLine

            Data Flow:
                .txt File > imported one line at a time as strings > extract nums from strings...
                and convert to doubles > Store each line of data into the structure array

                    -> Structure array (dataArr) conatins all of the data

        PUT / CALL NOTES:
            The data is a weekly representation of total puts and calls.

            Putt/Call Ratio = (putVolume /  callVolume)
                A falling put-call ratio below .7 and approaching .5, is considered a bullish indicator
                A rising put-call ratio greater than .7 or exceeding 1, is considered a bearish indicator

********************************************************************************************************/

/* C libraries */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/* MARCOS */
#define DECIMAL 46              // ASCii value for '.'
#define NUMENTRIES 2331            // (2332 Total Lines) - (fisrt line) = 2331 Lines of data

/* Variable order: Large -> Small for memory optimization */
struct dataEntry{

    double putVol;
    double callVol;
    double optVol;
    float ratio;
    unsigned int year  : 5;
    unsigned int day   : 5;
    unsigned int month : 4;
};

/* Used for queries over a specific period of time
 *
 * January is 1
 * First of the month is 1
 * */
struct date {
    unsigned int day : 5;
    unsigned int month : 4;
    unsigned int year : 5;
};

/* Functions */
void importFile(struct dataEntry dataArr[NUMENTRIES]);
void advanceCurserToData(char *buff, FILE *fp);
void parseLineToDouble(char *buff, double *numberArr);
void formatLine(char *tempArr);
void storeLine(double *lineOfData, struct dataEntry dataArr[NUMENTRIES]);

int CompareDataToMonth(struct dataEntry data, struct date checkDate);
int FindStartIndex(struct date start, struct dataEntry const dataArr[NUMENTRIES]);
void GetMarketType(struct date start, struct date end, int types[], int monthIncrement, struct dataEntry const dataArr[NUMENTRIES]);
int CompareDate(struct date A, struct date B);
void IncreaseMonth(struct date *dateToChange, int months);

/* Global variables */
int i = 0;
int logNum = 0;


int main(){

    struct dataEntry dataArr[NUMENTRIES];       // Array of type "struct dataEntry"

    importFile(dataArr);

    unsigned int dataArrSize = NUMENTRIES - 1;  // Location in the structure array of the last line of data

    // CONFIRM LAST LINE OF DATA  -> 10/4/19,2.32,1015727,437756,1453483
//    printf("\nFinal line check:");
//    printf("\n%d\t", dataArr[NUMENTRIES - 2].month);
//    printf("%d\t",   dataArr[NUMENTRIES - 2].day);
//    printf("%d\t",   dataArr[NUMENTRIES - 2].year);
//    printf("%.2f\t", dataArr[NUMENTRIES - 2].ratio);
//    printf("%.2f\t", dataArr[NUMENTRIES - 2].putVol);
//    printf("%.2f\t", dataArr[NUMENTRIES - 2].callVol);
//    printf("%.2f\t", dataArr[NUMENTRIES - 2].optVol);


    /* This is an example of how to use the functions I added. I'll be doing more with this later, but I have the core of it done and wanted you
     * guys to be able to use the helper methods I've created.*/
    int types[NUMENTRIES / 4];
    struct date start = {6, 7, 10};
    struct date end = {4, 10, 19};
    GetMarketType(start, end, types, 3, dataArr);

    for(int index = 0; index < NUMENTRIES / 4; index++)
    {
        printf("For %d quarter of year %d, the market was %s\n", ((index)%4 + 1), 0000, (types[index] == -1 ? "bear" : "bull"));
    }

    return 0;
}


/*************************************************************************************************************************
* IMPORT THE FILE:  Calls to  the functions to extract each line of data from the file and store it in the structures
* variables.  Each line of data is its own structure.  The structures of data are stored in an array of structures named
* "dataArr".  fscanf reads up to next whitespace (up to the end of line in this case).
*************************************************************************************************************************/
void importFile(struct dataEntry dataArr[NUMENTRIES]){

    double lineOfData[7];
    char buff[40];

    FILE *fp;
    fp = fopen("./marketData.txt", "r");

    rewind(fp);

    advanceCurserToData(buff, fp);

    while(!feof(fp)){

        fscanf(fp, "%s", buff);
        formatLine(buff);
        parseLineToDouble(buff, lineOfData);
        storeLine(lineOfData, dataArr);

    }

    fclose(fp);

    return;
}


/*************************************************************************************************************************
* ADVANCE THE CURSOR:  The begining of the text file has column titles which are not part of the data.  This will move
* the cursor (file pointer fp) to the end of the first line of data.
*************************************************************************************************************************/
void advanceCurserToData(char *buff, FILE *fp){

    for(i = 0; i < 10; i++)
        fscanf(fp, "%s", buff);

    return;
}


/*************************************************************************************************************************
* EXRTRACT NUMBERS FROM STRING:  Convert the numbers from the string (line of data) to double values.  Store the
* numerical values in the array "lineOfData[]".  Will extract all numbers from the string.
*************************************************************************************************************************/
void parseLineToDouble(char *buff, double *lineOfData){

    char *ptrArray[20] = {buff};

    for(i = 0; i <= 6 ; i++)
        lineOfData[i] = strtod(ptrArray[i], &ptrArray[i+1]);

    return;
}


/*************************************************************************************************************************
* FORMAT STRING FOR THE strtod() FUNCTION: The strtod() function is weak and requires specific formating for it to
* successfully process a string therefore all punctuation is replace by whitespace except for '.' for decimal nums
*************************************************************************************************************************/
void formatLine(char *tempArr){

    int size = strlen(tempArr);

    for(i = 0; i <= size; i++)
        if(ispunct(tempArr[i]))
            (tempArr[i] == DECIMAL)  ?  (tempArr[i]) :  (tempArr[i] = ' ');

    return;
}


/*************************************************************************************************************************
* STORE THE LINE OF DATA: Store the parsed data from the line into the strucutre array using it's corresponding index.
*************************************************************************************************************************/
void storeLine(double *lineOfData, struct dataEntry dataArr[NUMENTRIES]){

    dataArr[logNum].month   = lineOfData[0];
    dataArr[logNum].day     = lineOfData[1];
    dataArr[logNum].year    = lineOfData[2];
    dataArr[logNum].ratio   = lineOfData[3];
    dataArr[logNum].putVol  = lineOfData[4];
    dataArr[logNum].callVol = lineOfData[5];
    dataArr[logNum].optVol  = lineOfData[6];

//    printf("\n%d\t", dataArr[logNum].month);
//    printf("%d\t",   dataArr[logNum].day);
//    printf("%d\t",   dataArr[logNum].year);
//    printf("%.2f\t", dataArr[logNum].ratio);
//    printf("%.2f\t", dataArr[logNum].putVol);
//    printf("%.2f\t", dataArr[logNum].callVol);
//    printf("%.2f\t", dataArr[logNum].optVol);

    logNum++;

    return;
}


/**
 * Get the market type for over a given period.
 * Recommended for start.day == 0 and end.day == 31
 *
 * -1 for bear, +1 for bull
 * @author George Ebeling
 */
void GetMarketType(struct date start, struct date end, int types[], int monthIncrement, struct dataEntry const dataArr[NUMENTRIES])
{
    //Start must be less than end
    if(CompareDate(start, end) >= 0) return;


    // loop through the entire time period given
    for(int typeIndex = 0; -1 == CompareDate(start, end); IncreaseMonth(&start, monthIncrement), typeIndex++)
    {
        // Find the starting index based on the start date
        int index = FindStartIndex(start, dataArr);

        // A date for when to stop the sub period.
        struct date subPeriodEnd = start;
        IncreaseMonth(&subPeriodEnd, monthIncrement);

        int bullCount = 0, bearCount = 0;

        // loop through the sub period until the end of the sub period has been reached.
        for(; CompareDataToMonth(dataArr[index], subPeriodEnd) < 0; index++)
        {
            if(dataArr[index].ratio > 1)
                bearCount++;
            else
                bullCount++;
        }

        // Set the market type for this period
        //TODO if there are index out of bound errors, issue a memory check here before adding to the array
        types[typeIndex] = (bearCount >= bullCount ? -1 : 1);
    }
}


/**
 * Compare two dates
 * @return 1 if A>B, -1 if A<B, 0 if A==B
 * @author George Ebeling
 */
int CompareDate(struct date A, struct date B)
{
    if(A.year > B.year)
        return 1;
    else if(B.year > A.year)
        return -1;
    else
        {
        if(A.month > B.month)
            return 1;
        else if(B.month > A.month)
            return -1;
        else
            {
            if(A.day > B.day)
                return 1;
            else if(B.day > A.day)
                return -1;
            else
                return 0;
        }
    }
}

/**
 * Safely incrase the month. If the month was increase passed december, increase the year and decrease the months until a valid date has been found.
 * @param dateToChange
 * @param months
 * @author George Ebeling
 */
void IncreaseMonth(struct date *dateToChange, int months)
{
    dateToChange->month += months;
    //Continue increase the year and decreasing the month until the months are valid
    while(dateToChange->month > 12)
    {
        dateToChange->month -= 12;
        dateToChange->year += 1;
    }
}


/**
 * Compares the given dataEntry struct to the given checkData using CompareDate().
 * @param data Of type struct dataEntry
 * @param checkDate Of type struct date
 * @return CompareDate(dataEntry, checkDate)
 * @author George Ebeling
 */
int CompareDataToMonth(struct dataEntry data, struct date checkDate)
{
    struct date dataDate = {data.day, data.month, data.year};
    return CompareDate(dataDate, checkDate);
}

/**
 * Finds which index of dataArr contains where to begin to match the start date.
 * @author George Ebeling
 */
int FindStartIndex(struct date start, struct dataEntry const dataArr[NUMENTRIES])
{
    int index = 0;

    // Loop until the starting location has been found.
    for(;CompareDataToMonth(dataArr[index], start) < 0; index++);

    return index;
}

