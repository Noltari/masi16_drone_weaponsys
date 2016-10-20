
#include "release.h"

bool payload_loaded = false;

bool weapon_computation (double target_lat, double target_long, double curr_lat, double curr_long, float vel, float height, int armedsystemsignal, int armedweaponsignal, int loadsignal){

	if (loadsignal > 1800){
		payload_loaded = false;
		return true;
	}

	else if (!payload_loaded){
		payload_loaded = true;
		return false;
	}
	else{
		return release_calc(target_lat, target_long, curr_lat, curr_long, vel, height, armedsystemsignal, armedweaponsignal);
		//return release_calc(40.543439,-4.013027,40.543270,-4.012716,5,20);
	}

}
