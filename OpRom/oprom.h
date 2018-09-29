#ifndef OPROM_H
#define OPROM_H
#include "efipatcher.h"
#include "QFile"
#include <set>

struct Roms
{
    int  id;
    uint64_t offset;
    uint64_t romlen;
    uint64_t indicator;
};

class EfiPatcher;
class OpRom
{
    public:
        OpRom(EfiPatcher* m);
        bool BiosIsValid();

        bool LoadBios(QString fileName, bool bios);
        bool FixRom();

        QByteArray GetBiosRom() { return _bios; }
        QByteArray GetEfiRom() { return _efi; }
        void SetEfiRom(QByteArray data) { _efi.replace(0,data.length(), data);}
        QByteArray GetCombinedRom() { return _combined; }

    private:
        void readEFIData();

        EfiPatcher* _main;
        QByteArray _bios;
        QByteArray _efi;
        QByteArray _combined;
};

#endif // OPROM_H
