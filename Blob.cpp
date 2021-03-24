#include <iostream>
#include "Blob.h"


#define MAX_ANGLE	360

using namespace std;

//Constructor de blob definido mediante argumentos por defecto.
Blob::Blob(int Xsize, int Ysize, bool mode, int speedMax, float speedProb) {

	position = {random(Xsize), random(Ysize)};
	direction = random(MAX_ANGLE); //direccion de movimiento
	etaryGroup = BABY_BLOB;
	foodCount = 0;
	adjustSpeed(mode, speedMax, speedProb);
}

//Si la distancia entre el blob y la comida es menor o igual que SmellRadius entonces se dirige hacia la comida.
void Blob::adjustMovement (Point& food, double SmellRadius) {
	if (position.getDistance(food) <= SmellRadius){
		direction = position.getAngle(food);
	}
	return;
}

//Ajusta velocidad de blob segun modo de simulacion.
void Blob::adjustSpeed(bool mode, int speedMax, float speedProb) {
	if (!mode) {	//Si es modo 1
		speed = (float) speedMax;
	}
	else {
		speed = random(speedMax);
	}
	speed *= speedProb;
}

//Cambia la posicion del blob.
void Blob::move (Point& food, double SmellRadius, float movement) {
	adjustMovement(food, SmellRadius);
	position.translate(movement, direction);
	if (position.x > SCREEN_W){
		position.x = 0;
	}
	else if (position.x < 0) {
		position.x = SCREEN_W;
	}
	if (position.y > SCREEN_H){
		position.y = 0;
	}
	else if (position.y < 0){
		position.y = SCREEN_H;
	}
}	

//Alimenta blob y devuelve indicador de babyBirth.
bool Blob::feed(void) {
	++foodCount;	//Incrementa contador interno de alimento.
	bool birth = false;
	switch (etaryGroup) {	//Con una cierta cantidad de alimento, se da indicacion de nacimiento.
	case BABY_BLOB:
		if (foodCount == 3) {	
			foodCount = 0;
			birth = true;
		}
		break;
	case GROWN_BLOB:
		if (foodCount == 4) {
			foodCount = 0;
			birth = true;
		}
		break;
	case GOOD_OLD_BLOB:
		if (foodCount == 5) {
			foodCount = 0;
			birth = true;
		}
		break;
	}
	return birth;
}

//Se mergean blobs en uno. Para ello se reciben datos de nuevas posiciones, direcciones y probabilidades de muerte segun evolucion de blob.
void Blob::merge(float averageX, float averageY, float averageDirection, float averageSpeed) {
	switch (etaryGroup) {	//Se evoluciona el blob y probabilidad de muerte.
	case BABY_BLOB:
		etaryGroup = GROWN_BLOB;
		break;
	case GROWN_BLOB:
		etaryGroup = GOOD_OLD_BLOB;
		break;
	case GOOD_OLD_BLOB:
		break;
	}
	position.x = averageX;	//Se cargan nuevas posiciones, direcciones y velocidad.
	position.y = averageY;
	direction = averageDirection;
	speed = averageSpeed;
}

//Se produce el milagro del nacimiento de un babyBlob y se lo carga en un estado etario transitorio
void Blob::birth(Blob& blobMom, int mode, int speedMax, float speedProb) {
	position.x = blobMom.position.x;	
	position.y = blobMom.position.y;
	direction = -blobMom.direction; 
	etaryGroup = BIRTH;
	foodCount = 0;
	adjustSpeed(mode, speedMax, speedProb);
}

//Actualiza probabilidad de muerte segun grupo etario y decide si matar a un blob.
void Blob::death(float deathProbBabyBlob, float deathProbGrownBlob, float deathProbOldBlob) {
	float deathProb = 0;
	switch (etaryGroup) {
	case BABY_BLOB:
		deathProb = deathProbBabyBlob;
		break;
	case GROWN_BLOB:
		deathProb = deathProbGrownBlob;
		break;
	case GOOD_OLD_BLOB:
		deathProb = deathProbOldBlob;
		break;
	}
	double death = random(1);	//Calcula probabilidad de muerte 
	if (death < deathProb) {	//Y decide si lo mata o no.
		etaryGroup = DEATH;
	}
}



