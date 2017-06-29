#pragma once
#include "Header.h"
#include "Cartridge.h"
#include "Controller.h"

#define MEM_SIZE 65536

class Memory
{
public:
	Memory() {}
	~Memory() {}
	void init(Cartridge *cartridge, Controller *controller);
	void reset();
	BYTE read(WORD address);
	void write(WORD address, BYTE value);
	void directModification(WORD address, BYTE value);
	void DMATransfer(BYTE data);
	bool timerTriger();
	void resetTimerTriger();
	BYTE getTimerData();
	bool _resetDiv;
	BYTE *getMemoryData();

private:
	BYTE _MEMORY[MEM_SIZE];
	Cartridge *_cartridge;
	bool _timerTriger;
	BYTE _timerData;
	Controller *_controller;
};
