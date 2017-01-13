/* gasmeter.c
 * 
 * Jeroen@Klock.nl
 * Send message to pushover app
 * License: "Free BSD license"
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>
#include <wiringPi.h>

#include "piEnergy.h"
#include "setupmysql.h"

int main(void){
    char query[200];
    time_t epoch, last_epoch;
    char* c_time_string;

	initMeter();
	initDatabase();

    for(;;)
    {
        epoch = time(NULL);
        // every exact clock minute
        if(epoch%60 == 0){
            // write totals to pulspermin table
            epoch = time(NULL);
            c_time_string = ctime(&epoch);
            MYSQL *con = mysql_init(NULL);
	        if (con == NULL) 
	        {
		        fprintf(stderr, "%s\n", mysql_error(con));
		        exit(1);
	        }
	        if (mysql_real_connect(con, "localhost", USER, PASSWD,"piDomotica", 0, NULL, 0) == NULL)
	        {
		        fprintf(stderr, "%s\n", mysql_error(con));
		        exit(1);
	        }
            sprintf(query,"INSERT INTO pulspermin_gas(datetime,pulses)VALUES(default,(SELECT COUNT(epoch)FROM pulslog_gas WHERE epoch BETWEEN %d AND %d))",last_epoch,epoch);
	        if (mysql_query(con, query))
	        {      
		        fprintf(stderr, "%s\n", mysql_error(con));
		    }
            sprintf(query,"INSERT INTO pulspermin_water(datetime,pulses)VALUES(default,(SELECT COUNT(epoch)FROM pulslog_water WHERE epoch BETWEEN %d AND %d))",last_epoch,epoch);
	        if (mysql_query(con, query))
	        {      
		        fprintf(stderr, "%s\n", mysql_error(con));

		    }
            sprintf(query,"INSERT INTO pulspermin_kwh(datetime,pulses)VALUES(default,(SELECT COUNT(epoch)FROM pulslog_kwh WHERE epoch BETWEEN %d AND %d))",last_epoch,epoch);
	        if (mysql_query(con, query))
	        {      
		        fprintf(stderr, "%s\n", mysql_error(con));

		    }



            last_epoch = epoch;
	        mysql_close(con);

        }
		delay(1000);
    }
	return 0;
}

void initMeter(void)
{
   	// load wiringPi
	if(wiringPiSetup() < 0){
		printf("WiringPi setup failed. Maybe you haven't installed it yet?");
	}

	pullUpDnControl (PINGAS, PUD_DOWN);
	pinMode(PINKWH,INPUT);
	if(wiringPiISR (PINGAS, INT_EDGE_FALLING, &isrGas) < 0)
	{
		fprintf (stderr, "Unable to setup isrGas\n") ;
	    exit(1);
	}

	pinMode(PINGAS,INPUT);
	pullUpDnControl (PINKWH, PUD_UP);
	if(wiringPiISR (PINKWH, INT_EDGE_RISING, &isrKwh) < 0)
	{
		fprintf (stderr, "Unable to setup isrGas\n") ;
	    exit(1);
	}

	pinMode(PINWATER,INPUT);
	pullUpDnControl (PINWATER, PUD_UP);
	if(wiringPiISR (PINWATER, INT_EDGE_RISING, &isrWater) < 0)
	{
		fprintf (stderr, "Unable to setup isrGas\n") ;
	    exit(1);
	}

}

void isrGas()
{
    time_t epoch;
    MYSQL *con;
    char query[200];
    static int lasttime;
//    printf("=== isr Gas ================\n");
//    printf("lasttime: %d\t millis: %d\n",lasttime,millis());

    // Debounce
    if ((millis() - lasttime) > DELAY_GAS)
    {
        epoch = time(NULL);
        printf("%d logging gas puls\n",epoch);

    	con = mysql_init(NULL);

	    if (mysql_real_connect(con, "localhost", USER, PASSWD,
		      "piDomotica", 0, NULL, 0) == NULL) 
	    {
		    fprintf(stderr, "error mysql:%s\n", mysql_error(con));
		    exit(1);
	    }
        sprintf(query,"INSERT INTO pulslog_gas(epoch) VALUES('%d')",epoch); 
        if (mysql_query(con, query))
	    { 
		    fprintf(stderr, "error mysql:%s\n", mysql_error(con));
		    exit(1);
	    }
	    mysql_close(con);
    }
    lasttime = millis(); 
}

void isrWater()
{
    time_t epoch;
    MYSQL *con;
    char query[200];
    static int lasttime;
//    printf("=== isr Water ================\n");
//    printf("lasttime: %d\t millis: %d\n",lasttime,millis());

    // Debounce
    if ((millis() - lasttime) > DELAY_WATER)
    {
        epoch = time(NULL);
        
        printf("%d logging water puls\n",epoch);
	    con = mysql_init(NULL);

	    if (mysql_real_connect(con, "localhost", USER, PASSWD,
		      "piDomotica", 0, NULL, 0) == NULL) 
	    {
		    fprintf(stderr, "error mysql:%s\n", mysql_error(con));
		    exit(1);
	    }
        sprintf(query,"INSERT INTO pulslog_water(epoch) VALUES('%d')",epoch); 
        if (mysql_query(con, query))
	    { 
		    fprintf(stderr, "error mysql:%s\n", mysql_error(con));
		    exit(1);
	    }
	    mysql_close(con);
    }
    lasttime = millis(); 
}

void isrKwh()
{
    time_t epoch;
    MYSQL *con;
    char query[200];
    static int lasttime;
//    printf("=== isr KWH ================\n");
//    printf("lasttime: %d\t millis: %d\n",lasttime,millis());

    // Debounce
    if ((millis() - lasttime) > DELAY_KWH)
    {
        epoch = time(NULL);
        
        printf("%d logging kwh puls\n",epoch);
	    con = mysql_init(NULL);

        if (mysql_real_connect(con, "localhost", USER, PASSWD, "piDomotica", 0, NULL, 0) == NULL) 
        {
	        fprintf(stderr, "error mysql:%s\n", mysql_error(con));
	        exit(1);
        }
        sprintf(query,"INSERT INTO pulslog_kwh(epoch) VALUES('%d')",epoch); 
        if (mysql_query(con, query))
        { 
	        fprintf(stderr, "error mysql:%s\n", mysql_error(con));
	        exit(1);
        }
        mysql_close(con);
    }
    lasttime = millis(); 
}
