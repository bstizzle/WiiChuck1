#include "AccessoryOne.h"

int AccessoryOne::getJoyX() {
	return decodeInt(joyXBytes);
}
int AccessoryOne::getJoyY() {
	return decodeInt(joyYBytes);
}

float AccessoryOne::getRollAngle() {
	return atan2((double) getAccelX() - 511, (double) getAccelZ() - 511) * 180.0 / PI;
}
float AccessoryOne::getPitchAngle() {
	return -atan2((double) getAccelY() - 511, (double) getAccelZ() - 511) * 180.0 / PI;
}
int AccessoryOne::getAccelX() {
	return decodeInt(accelXBytes);
}
int AccessoryOne::getAccelY() {
	return decodeInt(accelYBytes);
}
int AccessoryOne::getAccelZ() {
	return decodeInt(accelZBytes);
}
boolean AccessoryOne::getButtonC() {
	return decodeBit(buttonCBits);
}
boolean AccessoryOne::getButtonZ() {
	return decodeBit(buttonZBits);
}
void  AccessoryOne::getValuesNunchuck( uint8_t * values){
	values[0]=map(getJoyX(),0,255,0,255);
	values[1]=map(getJoyY(),0,255,0,255);
	values[2]=map(getRollAngle(),0,1024,0,256);
	values[3]=map(getPitchAngle(),0,1024,0,256);
	values[4]=map(getAccelX(),0,1024,0,256);
	values[5]=map(getAccelY(),0,1024,0,256);

	values[6]=map(getAccelZ(),0,1024,0,256);
	values[7]=0;
	values[8]=0;
	values[9]=0;
	values[10]=getButtonZ()?255:0;
	values[11]=getButtonC()?255:0;
	values[12]=0;
	values[13]=0;

	values[14]=0;
	values[15]=0;
	values[16]=0;

	values[17]=0;
	values[18]=0;
	for(int i=0;i<WII_VALUES_ARRAY_SIZE;i++){
		if(values[i]>247){
			values[i]=255;
		}
	}
}
void AccessoryOne::printInputsNunchuck(Stream& stream) {
	char st[100];

	stream.print("NUNCHUCK ");
	sprintf(st,
	        "  JoyX: %4d  | JoyY: %4d | Ax: %4d | Ay: %4d | Az: %4d | Buttons: ",
	        getJoyX(), getJoyY(), getAccelX(), getAccelY(),getAccelZ());

	stream.print(st);

	if (getButtonC())
		stream.print("C");
	else
		stream.print("-");
	if (getButtonZ())
		stream.print("Z");
	else
		stream.print("-");
	stream.println();
}

