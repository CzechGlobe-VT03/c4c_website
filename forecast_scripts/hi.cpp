#include "hi.h"

double HeatIndex(double temp, double rh)
/* Heat index [�F]
	temp - air temperature [�C]
	rh - air relative humidity [%]
*/
{
	double hi, t2, r2;

	hi = 0.5 * (temp + 61.0 + ((temp - 68.0) * 1.2) + (rh * 0.094));
	if (hi >= 80.0)
	{
		t2 = temp * temp;
		r2 = rh * rh;
		hi = -42.379 + 2.04901523 * temp + 10.14333127 * rh - 0.22475541 * temp * rh - 0.00683783 * t2 - 0.05481717 * r2 + 0.00122874 * t2 * rh + 0.00085282 * temp * r2 - 0.00000199 * t2 * r2;
		if ((rh < 13.0) && (temp >= 80) && (temp < 112))
			hi -= ((13.0 - rh) / 4.0) * sqrt((17 - fabs(temp - 95.0)) / 17.0);
		if ((rh > 85.0) && (temp >= 80) && (temp < 87))
			hi += ((rh - 85.0) / 10.0) * (87 - temp) / 5.0;
	}
	return hi;
}

char HeatIndex_cat(double hi)
/* Klasifikace rizika pro heat index
	1 - No danger
	2 - Caution: fatigue is possible with prolonged exposure and activity. Continuing activity could result in heat cramps
	3 - Extreme caution: heat cramps and heat exhaustion are possible. Continuing activity could result in heat stroke
	4 - Danger: heat cramps and heat exhaustion are likely; heat stroke is probable with continued activity
	5 - Extreme danger: heat stroke is imminent

	hi - heat index [�F]
*/
{
	if (hi < 80.0)
		return 1;
	else if (hi < 90.0)
		return 2;
	else if (hi < 103.0)
		return 3;
	else if (hi < 125.0)
		return 4;
	else
		return 5;
}
