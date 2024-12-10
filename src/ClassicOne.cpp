#include "AccessoryOne.h"

int AccessoryOne::getJoyXLeft() {
	return decodeInt(joyXLeftBytes);
}


int AccessoryOne::getJoyXRight() {
	return decodeInt(joyXRightBytes);
}

int AccessoryOne::getJoyYLeft() {
	return decodeInt(joyYLeftBytes);
}

int AccessoryOne::getJoyYRight() {
	return decodeInt(joyYRightBytes);
}

int AccessoryOne::getTriggerLeft() {
	return decodeInt(triggerLeftBytes);
}

int AccessoryOne::getTriggerRight() {
	return decodeInt(triggerRightBytes);
}
int AccessoryOne::getPadRight() {
	return decodeBit(padRightBytes);
}

int AccessoryOne::getPadDown() {
	return decodeBit(padDownBytes);
}

int AccessoryOne::getPadUp() {
	return decodeBit(padUpBytes);
}

int AccessoryOne::getPadLeft() {
	return decodeBit(padLeftBytes);
}
int AccessoryOne::getButtonX() {
	return decodeBit(buttonXBytes);
}

int AccessoryOne::getButtonY() {
	return decodeBit(buttonYBytes);
}

int AccessoryOne::getButtonA() {
	return decodeBit(buttonABytes);
}

int AccessoryOne::getButtonB() {
	return decodeBit(buttonBBytes);
}


int AccessoryOne::getButtonMinus() {
	return decodeBit(buttonMinusBytes);
}

int AccessoryOne::getButtonHome() {
	return decodeBit(buttonHomeBytes);
}

int AccessoryOne::getButtonPlus() {
	return decodeBit(buttonPlusBytes);
}


int AccessoryOne::getButtonZLeft() {
	return decodeBit(buttonZLBytes);
}

int AccessoryOne::getButtonZRight() {
	return decodeBit(buttonZRBytes);
}

void  AccessoryOne::getValuesClassic( uint8_t * values){
	values[0]=map(getJoyXLeft(),0,64,0,256);
	values[1]=map(getJoyYLeft(),0,64,0,256);
	values[2]=map(getJoyXRight(),0,32,0,256);
	values[3]=map(getJoyYRight(),0,32,0,256);
	values[4]=0;
	values[5]=0;

	values[6]=getPadRight()?255:(getPadLeft()?0:128);
	values[7]=getPadDown()?0:(getPadUp()?255:128);
	values[8]=getButtonX()?255:0;
	values[9]=getButtonY()?255:0;
	values[10]=getButtonZLeft()?255:0;
	values[11]=map(getTriggerLeft(),0,32,0,256);
	values[12]=getButtonA()?255:0;
	values[13]=getButtonB()?255:0;

	values[14]=getButtonMinus()?0:(getButtonPlus()?255:128);
	values[15]=getButtonHome()?255:0;
	values[16]=0;

	values[17]=map(getTriggerRight(),0,32,0,256);
	values[18]=getButtonZRight()?255:0;
	for(int i=0;i<WII_VALUES_ARRAY_SIZE;i++){
		if(values[i]>247){
			values[i]=255;
		}
	}
}


void AccessoryOne::printInputsClassic(Stream& stream) {
	char st[100];
	sprintf(st,"Classic jxL: %4d | jxR: %4d | jyL: %4d | jyR: %4d | trigL: %4d | trigR: %4d |",getJoyXLeft(),getJoyXRight(),getJoyYLeft(),getJoyYRight(),getTriggerLeft(),getTriggerRight());
	stream.print(st);

	if (getPadRight())
		stream.print("pad right");

	if (getPadDown())
		stream.print("pad down");

	if (getPadUp())
		stream.print("pad up");

	if (getPadLeft())
		stream.print("pad left");

	if (getButtonX())
		stream.print("button x");

	if (getButtonY())
		stream.print("button y");

	if (getButtonA())
		stream.print("button a");

	if (getButtonB())
		stream.print("button b");

	if (getButtonMinus())
		stream.print("button minus");

	if (getButtonHome())
		stream.print("button home");

	if (getButtonPlus())
		stream.print("button plus");

	if (getButtonZLeft())
		stream.print("button ZL");

	if (getButtonZRight())
		stream.print("button ZR");

	stream.println("");
}
