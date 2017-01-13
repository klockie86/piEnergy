/* setupmysql.c
 * 
 * Jeroen@Klock.nl
 * License: "Free BSD license"
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mysql/mysql.h>

#include "setupmysql.h"

time_t current_time;
char* c_time_string;

void initDatabase(void)
{
	MYSQL *con = mysql_init(NULL);
	printf("MySQL client version: %s\n", mysql_get_client_info());

	if (con == NULL) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_real_connect(con, "localhost", USER, PASSWD,
		  "piDomotica", 0, NULL, 0) == NULL) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}  

	if (mysql_query(con, "CREATE DATABASE IF NOT EXISTS piDomotica")) 
	{
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_query(con, "CREATE TABLE IF NOT EXISTS pulslog_water(id INT NOT NULL AUTO_INCREMENT, epoch INT NOT NULL, PRIMARY KEY (id));"))
	{      
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}
	if (mysql_query(con, "CREATE TABLE IF NOT EXISTS pulslog_gas(id INT NOT NULL AUTO_INCREMENT, epoch INT NOT NULL, PRIMARY KEY (id));"))
	{      
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}
	if (mysql_query(con, "CREATE TABLE IF NOT EXISTS pulslog_kwh(id INT NOT NULL AUTO_INCREMENT, epoch INT NOT NULL, PRIMARY KEY (id));"))
	{      
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_query(con, "CREATE TABLE IF NOT EXISTS pulspermin_water(id INT NOT NULL AUTO_INCREMENT, datetime TIMESTAMP DEFAULT CURRENT_TIMESTAMP, pulses INT,PRIMARY KEY (id));"))
	{      
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_query(con, "CREATE TABLE IF NOT EXISTS pulspermin_gas(id INT NOT NULL AUTO_INCREMENT, datetime TIMESTAMP DEFAULT CURRENT_TIMESTAMP, pulses INT,PRIMARY KEY (id));"))
	{      
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	if (mysql_query(con, "CREATE TABLE IF NOT EXISTS pulspermin_kwh(id INT NOT NULL AUTO_INCREMENT, datetime TIMESTAMP DEFAULT CURRENT_TIMESTAMP, pulses INT,PRIMARY KEY (id));"))
	{      
		fprintf(stderr, "%s\n", mysql_error(con));
		exit(1);
	}

	mysql_close(con);
}
