#include "AccessoryOne.h"



int AccessoryOne::getStickXDrums() {
	return decodeInt(stickXBytes); 
}


int AccessoryOne::getStickYDrums() {
	return decodeInt(stickYBytes); 
}

int AccessoryOne::getSoftnessDataFor() {
	return decodeInt(softnessDataForBytes); 
}

int AccessoryOne::getSoftness() {
	return decodeInt(softnessBytes); 
}

int AccessoryOne::getHighHatDataFlag() {
	return decodeBit(highHatDataFlagBytes); 
}

int AccessoryOne::getSoftnessDataFlag() {
	return decodeBit(softnessDataFlagBytes); 
}


int AccessoryOne::getMinusButtonDrums() {
	return decodeBit(minusButtonBytes); 
}


int AccessoryOne::getPlusButtonDrums() {
	return decodeBit(plusButtonBytes); 
}


int AccessoryOne::getOrangeDrum() {
	return decodeBit(orangeDrumBytes); 
}


int AccessoryOne::getRedDrum() {
	return decodeBit(redDrumBytes); 
}


int AccessoryOne::getYellowDrum() {
	return decodeBit(yellowDrumBytes); 
}


int AccessoryOne::getGreenDrum() {
	return decodeBit(greenDrumBytes); 
}


int AccessoryOne::getBlueDrumm() {
	return decodeBit(blueDrummBytes); 
}

int AccessoryOne::getBassPedal() {
	return decodeBit(bassPedalBytes); 
}

void AccessoryOne::getValuesDrums(uint8_t * values){
	values[0]=map(getCrossfadeSlider(),0,255,0,256);
	values[1]=map(getEffectDial(),0,255,0,256);
	values[2]=map(getStickXGuitar(),0,255,0,256);
	values[3]=map(getStickYGuitar(),0,255,0,256);
	values[4]=map(getRightDJTable(),0,255,0,256);
	values[5]=map(getLeftDJTable(),0,255,0,256);

	values[6]=getPlusButtonGuitar()?255:(getMinusButtonGuitar()?0:128);
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


void AccessoryOne::printInputsDrums(Stream& stream) {
	char st[100];
	sprintf(st," stick x: %4d | stick y: %4d | ",getStickXDrums(),getStickYDrums());

	stream.print(st);

	if (getSoftnessDataFlag()){
		switch(getSoftnessDataFor()){
			case 27:
				stream.print("bass pedal:\t");
				stream.print(getSoftness());
				break;
			case 25:
				stream.print("red drum:\t");
				stream.print(getSoftness());
				break;
			case 17:
				stream.print("yellow drum:\t");
				stream.print(getSoftness());
				break;
			case 15:
				stream.print("blue drumm:\t");
				stream.print(getSoftness());
				break;
			case 14:
				stream.print("orange drum:\t");
				stream.print(getSoftness());
				break;
			case 18:
				stream.print("green drum:\t");
				stream.print(getSoftness());
				break;
		}
		if (getHighHatDataFlag()){
			stream.print("high hat:\t");
			stream.print(getSoftness());
		}
	}

	if (getMinusButtonDrums())
		stream.print("minus button");

	if (getPlusButtonDrums())
		stream.print("plus button");

	stream.println("");
}
