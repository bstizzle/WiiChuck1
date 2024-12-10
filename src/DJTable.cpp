#include "Accessory1.h"

int Accessory1::getCrossfadeSlider() {
	return decodeInt(crossfadeSliderBytes); 
}


int Accessory1::getEffectDial() {
	return decodeInt(effectDialBytes); 
}


int Accessory1::getStickX() {
	return decodeInt(stickXBytes); 
}

int Accessory1::getStickY() {
	return decodeInt(stickYBytes); 
}
int Accessory1::getRightDJTable() {
	return decodeInt(rightDJTableBytes);
}

int Accessory1::getLeftDJTable() {
	return decodeInt(leftDJTableBytes);
}

int Accessory1::getEuphoriaButton() {
	return decodeBit(euphoriaButtonBytes); 
}
int Accessory1::getPlusButton() {
	return decodeBit(plusButtonBytes); 
}
int Accessory1::getMinusButton() {
	return decodeBit(minusButtonBytes); 
}
int Accessory1::getLeftGreenButton() {
	return decodeBit(leftGreenButtonBytes); 
}

int Accessory1::getLeftRedButton() {
	return decodeBit(leftRedButtonBytes); 
}

int Accessory1::getLeftBlueButton() {
	return decodeBit(leftBlueButtonBytes); 
}

int Accessory1::getRightGreenButton() {
	return decodeBit(rightGreenButtonBytes); 
}

int Accessory1::getRightRedButton() {
	return decodeBit(rightRedButtonBytes); 
}


int Accessory1::getRightBlueButton() {
	return decodeBit(rightBlueButtonBytes); 
}

void  Accessory1::getValuesDj( uint8_t * values){
	values[0]=map(getCrossfadeSlider(),0,255,0,256);
	values[1]=map(getEffectDial(),0,255,0,256);
	values[2]=map(getStickX(),0,255,0,256);
	values[3]=map(getStickY(),0,255,0,256);
	values[4]=map(getRightDJTable(),0,255,0,256);
	values[5]=map(getLeftDJTable(),0,255,0,256);

	values[6]=getPlusButton()?255:(getMinusButton()?0:128);
	values[7]=getEuphoriaButton()?255:0;
	values[8]=0;
	values[9]=0;
	values[10]=getRightBlueButton()?255:0;
	values[11]=getRightRedButton()?255:0;
	values[12]=getRightGreenButton()?255:0;
	values[13]=getLeftBlueButton()?255:0;

	values[14]=getLeftRedButton()?255:0;
	values[15]=getLeftGreenButton()?255:0;
	values[16]=getButtonPlus()?255:0;

	values[17]=getButtonZLeft()?255:0;
	values[18]=getButtonZRight()?255:0;
	for(int i=0;i<WII_VALUES_ARRAY_SIZE;i++){
		if(values[i]>247){
			values[i]=255;
		}
	}
}

void Accessory1::printInputsDj(Stream& stream) {
	char st[100];
	sprintf(st," crossfade slider: %4d | effect dial: %4d | stick x: %4d | stick y: %4d | right DJTable: %4d | left DJTable: %4d |",getCrossfadeSlider(),getEffectDial(),getStickX(),getStickY(),getRightDJTable(),getLeftDJTable());
	stream.print(st);

	if (getEuphoriaButton())
		stream.print("euphoria button");

	if (getPlusButton())
		stream.print("plus button");

	if (getMinusButton())
		stream.print("minus button");

	if (getLeftGreenButton())
		stream.print("left green button");

	if (getLeftRedButton())
		stream.print("left red button");

	if (getLeftBlueButton())
		stream.print("left blue button");

	if (getRightGreenButton())
		stream.print("right green button");

	if (getRightRedButton())
		stream.print("right red button");

	if (getRightBlueButton())
		stream.print("right blue button");

	stream.println("");
}
