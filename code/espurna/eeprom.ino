/*

EEPROM MODULE

*/

#include <EEPROM_Rotate.h>

// -----------------------------------------------------------------------------

bool eepromBackup() {
    DEBUG_MSG_P(PSTR("[EEPROM] Backing up data to last sector\n"));
    return EEPROMr.backup();
}

String eepromSectors() {
    String response;
    for (uint32_t i = 0; i < EEPROMr.pool(); i++) {
        if (i > 0) response = response + String(", ");
        response = response + String(EEPROMr.base() - i);
    }
    return response;
}

#if TERMINAL_SUPPORT

void _eepromInitCommands() {

    settingsRegisterCommand(F("EEPROM.DUMP"), [](Embedis* e) {
        EEPROMr.dump(settingsSerial());
        DEBUG_MSG_P(PSTR("\n+OK\n"));
    });

}

#endif

// -----------------------------------------------------------------------------

void eepromSetup() {

    #ifdef EEPROM_ROTATE_SECTORS
        EEPROMr.pool(EEPROM_ROTATE_SECTORS);
    #else
        uint8_t sectors = 0;
        if (EEPROMr.last() > 1000) { // 4Mb boards
            sectors = 4;
        } else if (EEPROMr.last() > 250) { // 1Mb boards
            sectors = 2;
        } else {
            sectors = 1;
        }
        EEPROMr.pool(sectors);
    #endif

    EEPROMr.offset(EEPROM_ROTATE_DATA);
    EEPROMr.begin(EEPROM_SIZE);

    #if TERMINAL_SUPPORT
        _eepromInitCommands();
    #endif

}