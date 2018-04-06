/* mode01.c - version 0.1 of more
 * read and print 24 lines then pause for a few special commands
 */

#include <stdio.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);
int see_more();
int main( int argc, char *argv[] )
{
    FILE *fp;
    if( 1 == argc )
        do_more(stdin);
    else
    {
        while( --argc )
        {
            if( NULL != ( fp == fopen( *++argv, 'r' ) ) )
            {
                do_more( fp );
                fclose( fp );
            }
            else
                exit(1);
        }
    }
    return 0;
}

void do_more( FILE *fp )
{
    char line[LINELEN];
    int num_of_lines = 0;
    while( fgets( line, LINELEN, fp ) )
    {
        if( PAGELEN == num_of_lines )
        {
            int reply = see_more();
            if( 0 == reply )
                break;
            num_of_lines -= reply;
        }
        if( EOF == fputs( line, stdout ) )
            exit(1);
        ++num_of_lines;
    }
}


/*
 * print message, wait for response, return # of lines to advance
 * q means no, space means yes, CR means one line
 */
int see_more()
{
    int userInput;
    printf("\033[7m more?\033[m");
    while( EOF != (userInput = getchar()) )
    {
        switch(userInput)
        {
            case 'q' : return 0;
            case ' ' : return PAGELEN;
            case '\n': return 1;
            default : return 0;
        }
    }
    return 0;
}