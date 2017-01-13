/*
	Jeroen Klock
    You should have received a copy of the GNU General Public License
    along with pilight. If not, see	<http://www.gnu.org/licenses/>
*/

#ifndef piEnergy_h
#define piEnergy_h


#define	DELAY_KWH	100
#define	DELAY_WATER	2000
#define	DELAY_GAS	100

#define PINGAS 	3 // wiringpi GPIO 3 used for GAS meter
#define PINKWH 	1 // wiringpi GPIO 1 used for KWH meter
#define PINWATER 4 //wiringpi GPIO 4 used for water meter

void initMeter(void);
void isrGas(void);
void isrKwh(void);
void isrWater(void);
void setupDatabase(void);

#endif
