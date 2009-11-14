/*
 * MiRegistroFijo.h
 *
 *  Created on: 11/10/2009
 *      Author: ale
 */

#ifndef MIREGISTROFIJO_H_
#define MIREGISTROFIJO_H_

class MiRegistroFijo {

private:
	int miIntID;
	char miCharID;
	int miInt;

public:
	MiRegistroFijo(int miIntID, char miCharID, int miInt){
		this->miIntID = miIntID;
		this->miCharID = miCharID;
		this->miInt = miInt;
	}
	virtual ~MiRegistroFijo(){};
	int getMiIntID(){
		return miIntID;
	}
	char getMiCharID(){
		return miCharID;
	}
	int getMiInt(){
		return miInt;
	}
};

#endif /* MIREGISTROFIJO_H_ */
