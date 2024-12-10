#include "AccessoryOne.h"
#include <Wire.h>

AccessoryOne::AccessoryOne() {
	type = NUNCHUCKONE;
}
/**
 * Reads the device type from the controller
 */
ControllerTypeOne AccessoryOne::getControllerTypeOne() {
	return type;
}

ControllerTypeOne AccessoryOne::identifyController() {
	//Serial.println("Reading periph bytes");
	_burstRead(0xfa);
	//printInputs(Serial);

	if (_dataarray[4] == 0x00)
		if (_dataarray[5] == 0x00)
			return NUNCHUCKONE; // nunchuck

	if (_dataarray[4] == 0x01)
		if (_dataarray[5] == 0x01)
			return WIICLASSICONE; // Classic Controller

	if (_dataarray[0] == 0x00)
		if (_dataarray[1] == 0x00)
			if (_dataarray[2] == 0xa4)
				if (_dataarray[3] == 0x20)
					if (_dataarray[4] == 0x01)
						if (_dataarray[5] == 0x03)
							return GuitarHeroControllerOne; // Guitar Hero Controller

	if (_dataarray[0] == 0x01)
		if (_dataarray[1] == 0x00)
			if (_dataarray[2] == 0xa4)
				if (_dataarray[3] == 0x20)
					if (_dataarray[4] == 0x01)
						if (_dataarray[5] == 0x03)
							return GuitarHeroWorldTourDrumsOne; // Guitar Hero World Tour Drums

	if (_dataarray[0] == 0x03)
		if (_dataarray[1] == 0x00)
			if (_dataarray[2] == 0xa4)
				if (_dataarray[3] == 0x20)
					if (_dataarray[4] == 0x01)
						if (_dataarray[5] == 0x03)
							return TurntableOne; // Guitar Hero World Tour Drums

	if (_dataarray[0] == 0x00)
		if (_dataarray[1] == 0x00)
			if (_dataarray[2] == 0xa4)
				if (_dataarray[3] == 0x20)
					if (_dataarray[4] == 0x01)
						if (_dataarray[5] == 0x11)
							return DrumControllerOne; // Taiko no Tatsujin TaTaCon (Drum controller)

	if (_dataarray[0] == 0xFF)
		if (_dataarray[1] == 0x00)
			if (_dataarray[2] == 0xa4)
				if (_dataarray[3] == 0x20)
					if (_dataarray[4] == 0x00)
						if (_dataarray[5] == 0x13)
							return DrawsomeTabletOne; // Drawsome Tablet

	return UnknownOne;
}

void AccessoryOne::sendMultiSwitch(uint8_t iic, uint8_t sw) {
	uint8_t err = 0;
	int i = 0;
	for (; i < 10; i++) {
		Wire1.beginTransmission(iic);
		Wire1.write(1 << sw);
		Wire1.endTransmission();
		err = Wire1.endTransmission();
		if (err != 0) {
//			Serial.println(
//					"sendMultiSwitch Resetting because of " + String(err));
//			reset();
		} else
			return;
	}

}

void AccessoryOne::addMultiplexer(uint8_t iic, uint8_t sw) {
	if (sw >= 8)
		return;

	_multiplexI2C = iic;
	_multiplexSwitch = sw;
}

void AccessoryOne::switchMultiplexer() {
	if (_multiplexI2C == 0)
		return; // No multiplexer set
	sendMultiSwitch(_multiplexI2C, _multiplexSwitch);
}

void AccessoryOne::switchMultiplexer(uint8_t iic, uint8_t sw) {
	if (sw >= 8)
		return;
#if defined(TWCR)
	if (TWCR == 0)
#endif
		Wire1.begin();
	// Start I2C if it's not running
	sendMultiSwitch(iic, sw);
}

/*
 * public function to read data
 */
boolean AccessoryOne::readData() {
	switchMultiplexer();

	if (_burstRead()) {
		return true;
	}
	return false;
}

uint8_t* AccessoryOne::getDataArray() {
	return _dataarray;
}

void AccessoryOne::initBytes() {
	//Serial.println("Init Periph..");
	_writeRegister(0xF0, 0x55);
	_writeRegister(0xFB, 0x00);
	delay(100);

	type = identifyController();
	delay(100);

	if (_encrypted) {
		//Serial.println("Beginning Encrypted Comms");

		delay(100);

		_writeRegister(0xF0, 0xAA); // enable enc mode?
		delay(90);

		AccessoryOne::_burstWriteWithAddress(0x40, _key_table_1, 8);
		AccessoryOne::_burstWriteWithAddress(0x48, _key_table_1 + 0x8, 8);
		delay(100);

		//_writeRegister(0x40, 0x00);
	}
}

void AccessoryOne::setDataArray(uint8_t data[6]) {
	for (int i = 0; i < 6; i++)
		_dataarray[i] = data[i];
}

int AccessoryOne::decodeInt(uint8_t mmsbbyte, uint8_t mmsbstart, uint8_t mmsbend,
		uint8_t msbbyte, uint8_t msbstart, uint8_t msbend, uint8_t csbbyte,
		uint8_t csbstart, uint8_t csbend, uint8_t lsbbyte, uint8_t lsbstart,
		uint8_t lsbend) {
// 6 bit int split across 3 bytes in 4 parts.... :(
	bool msbflag = false, csbflag = false, lsbflag = false, mmsbflag = false;
	if (msbbyte > 5)
		msbflag = true;
	if (csbbyte > 5)
		csbflag = true;
	if (lsbbyte > 5)
		lsbflag = true;
	if (mmsbbyte > 5)
		mmsbflag = true;

	uint32_t analog = 0;
	uint32_t lpart = 0;
	lpart = (lsbflag) ? 0 : _dataarray[lsbbyte];
	lpart = lpart >> lsbstart;
	lpart = lpart & (0xFF >> (7 - (lsbend - lsbstart)));

	uint32_t cpart = 0;
	cpart = (csbflag) ? 0 : _dataarray[csbbyte];
	cpart = cpart >> csbstart;
	cpart = cpart & (0xFF >> (7 - (csbend - csbstart)));

	cpart = cpart << ((lsbend - lsbstart) + 1);

	uint32_t mpart = 0;
	mpart = (msbflag) ? 0 : _dataarray[msbbyte];
	mpart = mpart >> msbstart;
	mpart = mpart & (0xFF >> (7 - (msbend - msbstart)));

	mpart = mpart << (((lsbend - lsbstart) + 1) + ((csbend - csbstart) + 1));

	uint32_t mmpart = 0;
	mmpart = (mmsbflag) ? 0 : _dataarray[mmsbbyte];
	mmpart = mmpart >> mmsbstart;
	mmpart = mmpart & (0xFF >> (7 - (mmsbend - mmsbstart)));

	mmpart = mmpart
			<< (((lsbend - lsbstart) + 1) + ((csbend - csbstart) + 1)
					+ ((msbend - msbstart) + 1));

	analog = lpart | cpart | mpart | mmpart;
	//analog = analog + offset;
	//analog = (analog*scale);

	return analog;
}

int AccessoryOne::decodeInt(uint8_t msbbyte, uint8_t msbstart, uint8_t msbend,
		uint8_t csbbyte, uint8_t csbstart, uint8_t csbend, uint8_t lsbbyte,
		uint8_t lsbstart, uint8_t lsbend) {
// 5 bit int split across 3 bytes. what... the... fuck... nintendo...
	bool msbflag = false, csbflag = false, lsbflag = false;
	if (msbbyte > 5)
		msbflag = true;
	if (csbbyte > 5)
		csbflag = true;
	if (lsbbyte > 5)
		lsbflag = true;

	uint32_t analog = 0;
	uint16_t lpart = 0;
	lpart = (lsbflag) ? 0 : _dataarray[lsbbyte];
	lpart = lpart >> lsbstart;
	lpart = lpart & (0xFF >> (7 - (lsbend - lsbstart)));

	uint16_t cpart = 0;
	cpart = (csbflag) ? 0 : _dataarray[csbbyte];
	cpart = cpart >> csbstart;
	cpart = cpart & (0xFF >> (7 - (csbend - csbstart)));

	cpart = cpart << ((lsbend - lsbstart) + 1);

	uint16_t mpart = 0;
	mpart = (msbflag) ? 0 : _dataarray[msbbyte];
	mpart = mpart >> msbstart;
	mpart = mpart & (0xFF >> (7 - (msbend - msbstart)));

	mpart = mpart << (((lsbend - lsbstart) + 1) + ((csbend - csbstart) + 1));

	analog = lpart | cpart | mpart;
	//analog = analog + offset;
	//analog = (analog*scale);

	return analog;
}

bool AccessoryOne::decodeBit(uint8_t byte, uint8_t bit, bool activeLow) {
	if (byte > 5)
		return false;
	uint8_t swb = _dataarray[byte];
	uint8_t sw = (swb >> bit) & 0x01;
	return activeLow ? (!sw) : (sw);
}

void AccessoryOne::begin() {
#if defined(TWCR)
	if (TWCR == 0)
#endif
#if defined(ARDUINO_ARCH_ESP32)
		Wire1.begin(SDA,SCL,10000);
#else
	Wire1.begin();

#endif
	// Start I2C if it's not running

	switchMultiplexer();

	initBytes();
	identifyController();
	if (getControllerTypeOne() == DrawsomeTabletOne) {
		initBytesDrawsome();
	}
	delay(100);
	_burstRead();
	delay(100);
	_burstRead();
}

boolean AccessoryOne::_burstRead(uint8_t addr) {
	//int readAmnt = dataArraySize;
	uint8_t err = 0;
	bool dataBad = true;
	int b = 0;
	//bool consecCheck = true;
	uint8_t readBytes=0;
	for (; b < 5; b++) {
		Wire1.beginTransmission(WII_I2C_ADDR);
		Wire1.write(addr);
		err = Wire1.endTransmission();
		if (err == 0) {			// wait for data to be converted

			delayMicroseconds(275);
			int requested = Wire1.requestFrom(WII_I2C_ADDR, dataArraySize);
			delayMicroseconds(100);
			// read data
			 readBytes = Wire1.readBytes(_dataarrayTMP,requested);
			dataBad = true;
			//consecCheck=true;
			// If all bytes are 255, this is likely an error packet, reject
			for (int i = 0; i < dataArraySize && dataBad; i++){
				if(_dataarrayTMP[i]!=(uint8_t)255){
					dataBad=false;
				}
			}
			// check to see we read enough bytes and that they are valid
			if(readBytes == dataArraySize && dataBad==false){
				// decrypt bytes
				if (_encrypted) {
					for (int i = 0; i < dataArraySize; i++)
						_dataarray[i] = decryptByte(_dataarrayTMP[i], addr + i);

				}else{
					//Serial.print(" DATA= ");
					for (int i = 0; i < dataArraySize; i++){
						_dataarray[i] = _dataarrayTMP[i];
						//Serial.print(" , "+String( (uint8_t)_dataarray[i]));
					}
				}
				// Check the read in data aganst the last read date,
				// a valid burst read is 2 reads that produce the same data
				dataBad=false;
//				for (int x = 0; x< dataArraySize && dataBad==false; x++){
//					if(_dataarray[x]!=_dataarrayReadConsec[x]){
//						dataBad=true;
////						if(b>2){
////							Serial.print("\nBad Data Packet repeted: _burstRead Resetting " + String(b+1)+"\n\tExpected: ");
////							for (int i = 0; i < dataArraySize; i++){
////
////								Serial.print(" "+String( (uint8_t)_dataarrayReadConsec[i]));
////							}
////							Serial.print("\n\tgot:      ");
////							for (int i = 0; i < dataArraySize; i++){
////
////								Serial.print(" "+String( (uint8_t)_dataarray[i]));
////							}
////						}
//						//consecCheck=false;
//					}
//				}
				// copy current frame to compare to next frame
				for (int i = 0; i < dataArraySize; i++){
					_dataarrayReadConsec[i]=_dataarray[i];
				}
				// after 2 identical reads, process the data
				if(!dataBad){
					getValues();			//parse the data into readable data
					return true; // fast return once the success case is reached
				}else{
					//delay(3);
				}

			}else{

				dataBad=true;
			}
		}
		if(dataBad || (err != 0) ){
			if((err != 0)){
				Serial.println(	"\nI2C error code _burstRead error: " + String(err)
												+ " repeted: " + String(b+1));
				if(err==5){
					begin();
				}

			}else if(readBytes != dataArraySize){
				Serial.println("\nI2C Read length failure _burstRead Resetting " + String(readBytes)
												+ " repeted: " + String(dataArraySize));
			}else if(dataBad){

			}else
				Serial.println(
						"\nOther I2C error, packet all 255 _burstRead Resetting " + String(err)
								+ " repeted: " + String(b+1));
			reset();
		}

	}

	return !dataBad && (err == 0);
}

void AccessoryOne::_writeRegister(uint8_t reg, uint8_t value) {
	//Serial.print("W ");
	//Serial.print(reg,HEX);
	//Serial.print(": ");
	//Serial.println(value,HEX);
	uint8_t err = 0;
	int i = 0;
	for (; i < 10; i++) {
		Wire1.beginTransmission(WII_I2C_ADDR);
		Wire1.write(reg);
		Wire1.write(value);
		err = Wire1.endTransmission();
		if (err != 0) {
//			Serial.println(
//					"_writeRegister Resetting because of " + String(err)
//							+ " repeted: " + String(i));
			reset();
		} else
			return;
	}

}

void AccessoryOne::_burstWriteWithAddress(uint8_t addr, uint8_t* arr,
		uint8_t size) {
	//Serial.print("W ");
	//Serial.print(addr,HEX);
	//Serial.print(": ");
	//for (int i=0; i<size; i++) {//Serial.print(arr[i],HEX);//Serial.print(" ");
	//}
	//Serial.println("");
	uint8_t err = 0;
	int i = 0;
	for (; i < 3; i++) {
		Wire1.beginTransmission(WII_I2C_ADDR);
		Wire1.write(addr);
		for (int i = 0; i < size; i++)
			Wire1.write(arr[i]);
		err = Wire1.endTransmission();
		if (err != 0) {
//			Serial.println(
//					"_burstWriteWithAddress Resetting because of " + String(err)
//							+ " repeted: " + String(i));
			reset();
		} else
			return;
	}

}

void AccessoryOne::reset() {
#if defined(ARDUINO_ARCH_ESP32)
		Wire1.begin(SDA,SCL,10000);
#else
	Wire1.begin();

#endif
}

void AccessoryOne::enableEncryption(bool enc) {
	_encrypted = enc;
}

int AccessoryOne::smap(int16_t val, int16_t aMax, int16_t aMid, int16_t aMin,
		int16_t sMax, int16_t sZero, int16_t sMin) {
	int mapv = sZero;
	if (val > aMid) {
		mapv = map(val, aMid, aMax, sZero, sMax);
	} else if (val < aMid) {
		mapv = map(val, aMin, aMid, sMin, sZero);
	}
//Serial.print(val);Serial.print(" ");Serial.println(mapv);

	return mapv;
}

uint8_t AccessoryOne::decryptByte(uint8_t byte, uint8_t address) {
//return (byte ^ _key_table_1[address % 8]) + _key_table_1[(address % 8)+0x08];
	return (byte ^ 0x97) + 0x97;
}

void AccessoryOne::printInputs(Stream& stream) {
	switch (getControllerTypeOne()) {
	case WIICLASSICONE:
		printInputsClassic(stream);
		break;
	case GuitarHeroControllerOne:
		printInputsGuitar(stream);
		break;
	case GuitarHeroWorldTourDrumsOne:
		printInputsDrums(stream);
		break;
	case DrumControllerOne:
		printInputsDrums(stream);
		break;
	case DrawsomeTabletOne:
		printInputsDrawsome(stream);
		break;
	case TurntableOne:
		printInputsDj(stream);
		break;
	case NUNCHUCKONE:
		printInputsNunchuck(stream);
		break;
	default:
		stream.println("Unknown controller!");
		break;

	}
}

uint8_t * AccessoryOne::getValues() {
	switch (getControllerTypeOne()) {
	case WIICLASSICONE:
		getValuesClassic(values);
		break;
	case GuitarHeroControllerOne:
		getValuesGuitar(values);
		break;
	case GuitarHeroWorldTourDrumsOne:
		getValuesDrums(values);
		break;
	case DrumControllerOne:
		getValuesDrums(values);
		break;
	case DrawsomeTabletOne:
		getValuesDrawsome(values);
		break;
	case TurntableOne:
		getValuesDj(values);
		break;
	case NUNCHUCKONE:
	default:
		getValuesNunchuck(values);
		break;

	}
	return values;
}
;