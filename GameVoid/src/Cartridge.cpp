#include "Cartridge.h"


Cartridge::Cartridge(const string &gamePath)
{

	_ROMsize = 0;

	// With ios::ate, the initial position will be the end of the file
	ifstream rom(gamePath, ios::binary | ios::ate);

	// tellg() returns the actual position, since we are in the end of file, it returns the size
	_ROMsize = rom.tellg();

	if (_ROMsize != -1)
	{
		// Points to the beginning of the ifle
		rom.seekg(0, ios::beg);

		// Array of BYTEs containing the ROM
		ROMdata = new BYTE[_ROMsize];

		rom.read((char *) ROMdata, _ROMsize);

		// Read info from ROM
		init();
	}
	else cout << "Error reading rom" << endl;

	rom.close();
}


Cartridge::~Cartridge()
{
}

void Cartridge::init()
{

	// Get game title
	memcpy(_title, &ROMdata[HEADER_TITLE], 17);
	cout << "GAME TITLE: " << _title << endl;

}
