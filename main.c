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

char *GetTypeString(double PCR);
int CompareDataToMonth(struct dataEntry data, struct date checkDate);
int FindStartIndex(struct date start, struct dataEntry const dataArr[NUMENTRIES]);
void GetPCRForSubPeriods(struct date start, struct date end, double *PCRs, int monthIncrement, const struct dataEntry *dataArr);
int CompareDate(struct date A, struct date B);
void IncreaseMonth(struct date *dateToChange, int months);
char* toString_Date(struct date date);
void DataToMonth(struct dataEntry data, struct date *date);
void MarketType_SixMonthPeriod(const struct dataEntry *dataArr, const struct date *start, const struct date *end);
void MarketType_Quarterly(const struct dataEntry *dataArr, const struct date *start, const struct date *end);
void MarketType_Monthly(const struct dataEntry *dataArr, const struct date *start, const struct date *end);

/* Global variables */
int i = 0;
int logNum = 0;


int main(){

    struct dataEntry dataArr[NUMENTRIES];       // Array of type "struct dataEntry"

    importFile(dataArr);

    unsigned int dataArrSize = NUMENTRIES - 1;  // Location in the structure array of the last line of data

    /* Calculate the market type for different periods of time. The printf calls require different strings, so I made a couple different functions
     * for periods of time that would be interesting. -- George*/
    struct date start = {6, 7, 10};
    struct date end = {4, 10, 19};

//    MarketType_SixMonthPeriod(dataArr, &start, &end);
    MarketType_Quarterly(dataArr, &start, &end);
//    MarketType_Monthly(dataArr, &start, &end);

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
* ADVANCE THE CURSOR:  The beginning of the text file has column titles which are not part of the data.  This will move
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


/*************************************************************************************************************************
* Get the market type for over a given period.
 * Requires an update if it is desired for the end date to be something other than the end date of the data.
* @author George Ebeling
*************************************************************************************************************************/
void GetPCRForSubPeriods(struct date start, struct date end, double *PCRs, int monthIncrement, const struct dataEntry *dataArr)
{
    //Start must be less than end
    if(CompareDate(start, end) >= 0) return;


    // loop through the entire time period given
    for(int PCRIndex = 0; -1 == CompareDate(start, end); IncreaseMonth(&start, monthIncrement), PCRIndex++)
    {
        // Find the starting index based on the start date
        int index = FindStartIndex(start, dataArr);

        // Get the end date for the sub period.
        struct date subPeriodEnd = start;
        IncreaseMonth(&subPeriodEnd, monthIncrement);

        long int numPut = 0, numCall = 0;

        // loop through the sub period until the end of the sub period has been reached.
        /* TODO If this function is going to be used for an End that is not the end of the data, then the exit condition of this loop will nee to be
         * updated.*/
        for(; CompareDataToMonth(dataArr[index], subPeriodEnd) < 0 && index < NUMENTRIES - 1; index++)
        {
            numCall += dataArr[index].callVol;
            numPut += dataArr[index].putVol;
        }
        double PCR = (double)numPut / (double)numCall;

        PCRs[PCRIndex] = PCR;
    }
}

/*************************************************************************************************************************
* Returns a string for the market type given a put call ratio
*************************************************************************************************************************/
char *GetTypeString(double PCR)
{
    if(PCR > 1)
        return "bear";
    else if(PCR < 0.75)
        return "bull";
    else
        return "neutral";
}

/*************************************************************************************************************************
* Print the market type (bull, bear, or neutral) in six month periods based on the PCR.
* @author George Ebeling
*************************************************************************************************************************/
void MarketType_SixMonthPeriod(const struct dataEntry *dataArr, const struct date *start, const struct date *end) {
    double PCRs[19]; //found experimentally
    int periodLength = 6;
    int modPeriod = 12 / periodLength;
    struct date period = *start;

    GetPCRForSubPeriods((*start), (*end), PCRs, periodLength, dataArr);

    for( int index = 0; CompareDate((period), (*end)) < 0; index++)
    {

        printf("For the %d half of year 20%s, the market was %s (PCR: %f)\n", ((index + 1) % modPeriod + 1), period.year,
               GetTypeString(PCRs[index]), PCRs[index]);
        fflush(stdout);

        IncreaseMonth(&period, periodLength);
    }
}

/*************************************************************************************************************************
* Print the market type (bull, bear, or neutral) in quarterly periods based on the PCR.
* @author George Ebeling
*************************************************************************************************************************/
void MarketType_Quarterly(const struct dataEntry *dataArr, const struct date *start, const struct date *end) {
    double PCRs[37]; //found experimentally
    int periodLength = 3;
    int modPeriod = 12 / periodLength;
    struct date period = *start;

    GetPCRForSubPeriods((*start), (*end), PCRs, periodLength, dataArr);

    for(int index = 0; CompareDate((period), (*end)) < 0; index++)
    {

        printf("For the %d quarter of year 20%d, the market was %s (PCR: %.3f)\n", ((index+2) % modPeriod + 1), period.year,
               GetTypeString(PCRs[index]), PCRs[index]);
        fflush(stdout);

        IncreaseMonth(&period, periodLength);
    }
}

/*************************************************************************************************************************
* Print the market type (bull, bear, or neutral) in monthly periods based on the PCR.
* @author George Ebeling
*************************************************************************************************************************/
void MarketType_Monthly(const struct dataEntry *dataArr, const struct date *start, const struct date *end) {
    double PCRs[120]; //
    int periodLength = 1;
    int modPeriod = 12 / periodLength;
    struct date period = *start;

    GetPCRForSubPeriods((*start), (*end), PCRs, periodLength, dataArr);

    for( int index = 0; CompareDate((period), (*end)) < 0; index++)
    {

        printf("For the %2d month of year 20%s, the market was %s (PCR: %.3f)\n", ((index+6) % modPeriod + 1), period.year,
               GetTypeString(PCRs[index]), PCRs[index]);
        fflush(stdout);

        IncreaseMonth(&period, periodLength);
    }
}

/*************************************************************************************************************************
* Compare two dates
* @return 1 if A>B, -1 if A<B, 0 if A==B
* @author George Ebeling
*************************************************************************************************************************/
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

/*************************************************************************************************************************
* Safely increase the month. If the month was increase passed december, increase the year and decrease the months until a
* valid date has been found.
* @param dateToChange
* @param months
* @author George Ebeling
*************************************************************************************************************************/
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

/*************************************************************************************************************************
* Returns a string for printing the date in the format mm-dd-yy
* @author George Ebeling
*************************************************************************************************************************/
char* toString_Date(struct date date)
{
    char monthString[3];
    _itoa_s(date.month, monthString, 3, 10);
    char dayString[3];
    _itoa_s(date.day, dayString, 3, 10);
    char yearString[3];
    _itoa_s(date.year, yearString, 3, 10);
    char dateString[9];
    sprintf(dateString, "%s-%s-%s", monthString, dayString, yearString);
    return dateString;


}

/*************************************************************************************************************************
* Compares the given dataEntry struct to the given checkData using CompareDate().
* @param data Of type struct dataEntry
* @param checkDate Of type struct date
* @return CompareDate(dataEntry, checkDate)
* @author George Ebeling
*************************************************************************************************************************/
int CompareDataToMonth(struct dataEntry data, struct date checkDate)
{
    struct date dataDate;
    DataToMonth(data, &dataDate);
    return CompareDate(dataDate, checkDate);
}

/*************************************************************************************************************************
* Convert a data entry to a date struct.
* @author George Ebeling
*************************************************************************************************************************/
void DataToMonth(struct dataEntry data, struct date *date)
{
    date->month = data.month;
    date->day = data.day;
    date->year = data.year;
}

/*************************************************************************************************************************
* Finds which index of dataArr contains where to begin to match the start date.
* @author George Ebeling
*************************************************************************************************************************/
int FindStartIndex(struct date start, struct dataEntry const dataArr[NUMENTRIES])
{
    int index = 0;

    // Loop until the starting location has been found.
    for(;CompareDataToMonth(dataArr[index], start) < 0; index++);

    return index;
}




