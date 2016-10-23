#include <math.h>
#include <stdio.h>

#define __DEBUG__
#define __INTERP_2D__
//#define __GEODESIAN__

#define ARMED_THRESHOLD 1800
#define MIN_SPEED 2.0
#define MAX_SPEED 12.0
#define MIN_ALTITUDE 10.0
#define MAX_ALTITUDE 40.0

#define SPEED_VALUES 11
#define ALTITUDE_VALUES 31
#define DISTANCE_VALUES SPEED_VALUES * ALTITUDE_VALUES
float spd_vals[SPEED_VALUES] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
float alt_vals[ALTITUDE_VALUES] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40};
float dst_vals[ALTITUDE_VALUES][SPEED_VALUES] = {
	{2.8,	4.2,	5.6,	7,		8.4,	9.7,	11.1,	12.5,	13.8,	15.1,	16.5},
	{2.95,	4.45,	5.85,	7.3,	8.8,	10.2,	11.6,	13,		14.4,	15.8,	17.3},
	{3.1,	4.6,	6.2,	7.6,	9.1,	10.7,	12.2,	13.7,	15.1,	16.6,	18	},
	{3.2,	4.8,	6.4,	8,		9.5,	11.1,	12.6,	14.2,	15.7,	17.2,	18.6},
	{3.3,	5,		6.65,	8.3,	9.9,	11.5,	13.1,	14.6,	16.2,	17.7,	19.3},
	{3.4,	5.15,	6.8,	8.5,	10.2,	11.8,	13.5,	15.1,	16.8,	18.4,	20.1},
	{3.5,	5.3,	7,		8.8,	10.5,	12.2,	13.9,	15.6,	17.3,	19.1,	20.7},
	{3.6,	5.4,	7.2,	9.1,	10.8,	12.6,	14.3,	16,		17.8,	19.6,	21.3},
	{3.7,	5.6,	7.5,	9.3,	11.1,	13,		14.7,	16.5,	18.3,	20.1,	21.9},
	{3.8,	5.8,	7.7,	9.6,	11.5,	13.3,	15.1,	17,		18.8,	20.5,	22.3},
	{3.9,	5.9,	7.9,	9.8,	11.75,	13.65,	15.6,	17.4,	19.3,	21.1,	22.9},
	{4,		6,		8.05,	10,		12,		13.9,	15.8,	17.8,	19.8,	21.6,	23.5},
	{4.1,	6.2,	8.2,	10.25,	12.3,	14.3,	16.2,	18.2,	20.1,	22,		24	},
	{4.2,	6.3,	8.4,	10.4,	12.5,	14.5,	16.55,	18.6,	20.6,	22.5,	24.5},
	{4.3,	6.4,	8.6,	10.7,	12.8,	14.9,	16.9,	18.9,	20.9,	23,		25	},
	{4.4,	6.55,	8.7,	10.9,	13,		15.1,	17.2,	19.4,	21.4,	23.4,	25.4},
	{4.5,	6.7,	8.9,	11.05, 	13.3,	15.4,	17.6,	19.7,	21.7,	23.9,	26	},
	{4.6,	6.8,	9.05,	11.3,	13.5,	15.7,	17.8,	19.9,	22.2,	24.3,	26.4},
	{4.7,	6.9,	9.2,	11.5,	13.7,	15.95,	18.2,	20.4,	22.5,	24.6,	26.7},
	{4.8,	7,		9.4,	11.65,	14,		16.2,	18.5,	20.7,	22.8,	25.1,	27.3},
	{4.9,	7.1,	9.5,	11.9,	14.2,	16.5,	18.7,	21,		23.3,	25.5,	27.7},
	{5,		7.2,	9.7,	12,		14.4,	16.7,	19.05,	21.4,	23.6,	25.8,	28	},
	{5.1,	7.3,	9.8,	12.2,	14.65,	17,		19.4,	21.7,	23.9,	26.2,	28.5},
	{5.15,	7.4,	9.95,	12.4,	14.8,	17.2,	19.6,	21.9,	24.2,	26.6,	29	},
	{5.2,	7.5,	10.1,	12.6,	15,		17.5,	19.9,	22.2,	24.65,	27,		29.3},
	{5.25,	7.6,	10.25,	12.7,	15.2,	17.7,	20.1,	22.5,	25,		27.4,	29.7},
	{5.3,	7.7,	10.4,	12.9,	15.4,	17.9,	20.3,	22.9,	25.3,	27.7,	30	},
	{5.35,	7.8,	10.55,	13.1,	15.6,	18.1,	20.6,	23.2,	25.6,	28,		30.4},
	{5.4,	7.9,	10.65,	13.2,	15.8,	18.3,	20.9,	23.4,	25.9,	28.3,	30.7},
	{5.45,	8,		10.75,	13.4,	16,		18.5,	21.15,	23.7,	26.2,	28.7,	31.2},
	{5.5,	8.1,	10.8,	13.5,	16.2,	18.85,	21.4,	24,		26.5,	29,		31.5} };

float deg_to_rad(float degrees) {
	return (degrees * M_PI / 180.0);
}

#ifdef __GEODESIAN__
	double calc_distance(double lat1, double lon1, double lat2, double lon2)
	{
		double a = 6378137, b = 6356752.314245, f = 1 / 298.257223563;
		double L = deg_to_rad(lon2 - lon1);

		double U1 = atan((1 - f) * tan(deg_to_rad(lat1)));
		double U2 = atan((1 - f) * tan(deg_to_rad(lat2)));
		double sinU1 = sin(U1), cosU1 = cos(U1);
		double sinU2 = sin(U2), cosU2 = cos(U2);
		double cosSqAlpha;
		double sinSigma;
		double cos2SigmaM;
		double cosSigma;
		double sigma;

		double lambda = L, lambdaP, iterLimit = 100;
		do {
			double sinLambda = sin(lambda), cosLambda = cos(lambda);
			sinSigma = sqrt(	(cosU2 * sinLambda)
							* (cosU2 * sinLambda)
								+ (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda)
									* (cosU1 * sinU2 - sinU1 * cosU2 * cosLambda)
								);
			if (sinSigma == 0) {
				return 0;
			}

			cosSigma = sinU1 * sinU2 + cosU1 * cosU2 * cosLambda;
			sigma = atan2(sinSigma, cosSigma);
			double sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
			cosSqAlpha = 1 - sinAlpha * sinAlpha;
			cos2SigmaM = cosSigma - 2 * sinU1 * sinU2 / cosSqAlpha;

			double C = f / 16 * cosSqAlpha * (4 + f * (4 - 3 * cosSqAlpha));
			lambdaP = lambda;
			lambda = 	L + (1 - C) * f * sinAlpha
						* 	(sigma + C * sinSigma
							* 	(cos2SigmaM + C * cosSigma
								* 	(-1 + 2 * cos2SigmaM * cos2SigmaM)
								)
							);

		} while (fabs(lambda - lambdaP) > 1e-12 && --iterLimit > 0);

		if (iterLimit == 0) {
			return 0;
		}

		double uSq = cosSqAlpha * (a * a - b * b) / (b * b);
		double A = 1 + uSq / 16384
				* (4096 + uSq * (-768 + uSq * (320 - 175 * uSq)));
		double B = uSq / 1024 * (256 + uSq * (-128 + uSq * (74 - 47 * uSq)));
		double deltaSigma =
					B * sinSigma
						* (cos2SigmaM + B / 4
							* (cosSigma
								* (-1 + 2 * cos2SigmaM * cos2SigmaM) - B / 6 * cos2SigmaM
									* (-3 + 4 * sinSigma * sinSigma)
										* (-3 + 4 * cos2SigmaM * cos2SigmaM)));

		double s = b * A * (sigma - deltaSigma);

		return s;
	}
#else
	#define EARTH_RADIUS 6378140

	float calc_distance(float lat1, float lon1, float lat2, float lon2) {
		float dlon, dlat, a, c;
		float dist = 0.0;

		// calculate distance difference in radians
		dlon = deg_to_rad(lon2 - lon1);
		dlat = deg_to_rad(lat2 - lat1);

		// haversine
		a = pow(sin(dlat/2),2) + cos(deg_to_rad(lat1)) * cos(deg_to_rad(lat2)) * pow(sin(dlon/2),2);
		c = 2 * atan2(sqrt(a), sqrt(1-a));
		dist = EARTH_RADIUS * c;

		return dist;
	}
#endif /* __GEODESIAN__ */

#ifdef __INTERP_2D__
	double interp_distance(float fSpeed, float fHeight) {
		double x = (double) fHeight, y = (double) fSpeed;
		double x_min = MIN_ALTITUDE, x_max = MAX_ALTITUDE;
		double y_min = MIN_SPEED, y_max = MAX_SPEED;
		double interp = 0.0;
		double mf, nf;
		int m, n;

		// Find integer and fractional part of column index
		nf = (SPEED_VALUES-1) * (x - x_min) / (x_max - x_min);
		n = (int)nf;
		nf = nf - n;

		// Find integer and fractional part of row index
		mf = (ALTITUDE_VALUES-1) * (y - y_min) / (y_max - y_min);
		m = (int)mf;
		mf = mf - m;

		// Calculate interpolated estimated
		interp = (1-nf)*(1-mf)*dst_vals[m][n] + nf*(1-mf)*dst_vals[m][n+1] + (1-nf)*mf*dst_vals[m+1][n] + nf*mf*dst_vals[m+1][n+1];

		return interp;
	}
#else
	double interp_distance(float fSpeed, float fHeight) {
		int i, j;
		int speed = (int) fSpeed;
		int altitude = (int) fHeight;
		double interp = 0.0;

		for (i = 0; i < ALTITUDE_VALUES; i++) {
			if (altitude == alt_vals[i]) {
				for (j = 0; j < SPEED_VALUES; j++) {
					if (speed == spd_vals[j]) {
						interp = floor(dst_vals[i][j]);
						break;
					}
				}
				break;
			}
		}

		return interp;
	}
#endif /* __INTERP_2D__ */

// *** release_calc ***
// Create: 01/10/2016
// Author: Sergio Carretero - Airbus Defence & Space
// Last Modification: 01/10/2016
// Description: This function shall implement the release logic for the dron payload. Taking into account armed signals, current dron position and target position.
// Inputs:
//   - dTargetLat - double. Target Latitude position.
//   - dTargetLong - double. Target Longitude position.
//   - dCurrLat - double. Dron Latitude position.
//   - dCurrLong - double. Dron Longitude position.
//   - ffSpeed - float. Dron flight fSpeed.
//   - dfHeight - float. Dron flight fHeight.
//   - iArmedSystemSignal - int. Weapon System armed signal.
//   - iArmedWeaponSignal - int. Payload (Weapon) armed signal.
// Return:
//   - bool. True if the payload must be released. False if the payload must not be released.
bool release_calc(double dTargetLat, double dTargetLong, double dCurrLat, double dCurrLong, float fSpeed, float fHeight, int iArmedSystemSignal, int iArmedWeaponSignal, float fOffset){
	double distance = 0.0;
	double interp = 0.0;

	// Check armed status
	if (iArmedSystemSignal < ARMED_THRESHOLD || iArmedWeaponSignal < ARMED_THRESHOLD) {
		return false;
	}

	// Check speed
	if (fSpeed < MIN_SPEED || fSpeed > MAX_SPEED) {
		return false;
	}

	// Check altitude
	if (fHeight < MIN_ALTITUDE || fHeight > MAX_ALTITUDE) {
		return false;
	}

	// Calculate distance to target
	distance = calc_distance(dCurrLat, dCurrLong, dTargetLat, dTargetLong);

	// Interpolate value using the ballistic algorithm
	interp = interp_distance(fSpeed, fHeight);

	// Apply fix to interpolation
	interp += fOffset;

#ifdef __DEBUG__
	printf("interp=%f ; distance=%f\n", interp, distance);
#endif

	// Check release threshold
	if (distance <= interp) {
		return true;
	} else {
		return false;
	}
}
