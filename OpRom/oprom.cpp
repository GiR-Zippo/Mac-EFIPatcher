#include "oprom.h"
#include "QDebug"
#include "QMessageBox"
#include "toolbox.h"

OpRom::OpRom(EfiPatcher* m) : _main(m)
{

}

bool OpRom::LoadBios(QString fileName, bool bios)
{
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(_main, "File opening problem", "Problem with open file `" + fileName + "`for reading");
        return false;
    }

    if (bios)
    {
        _bios = (file.readAll());
        if (!BiosIsValid())
            return false;
    }
    else
    {
        _efi = (file.readAll());
        readEFIData();
    }
    file.close();
    return true;
}

bool OpRom::BiosIsValid()
{
    if (!((uint8_t(_bios.at(0)) == 0x55) && (uint8_t(_bios.at(1)) == 0xAA)))
    {
        QMessageBox::information(_main, "ROM problem", "ROM isn't valid! Loading anyway but patched rom could be unusable.");
        return false;
    }

    return true;
}

bool OpRom::FixRom()
{
    //Combine
    _combined = _bios;
    _combined.replace(_main->GetTooBox()->GetOffset(), _efi.length(), _efi);


    std::vector<Roms*> roms;
    uint64_t offset = 0;
    int id = 0;
    bool run = true;
    while (run)
    {
        Roms *rom = new Roms();
        rom->id = id;
        rom->offset = offset;

        if (!((uint8_t(_combined.at(offset)) == 0x55) && (uint8_t(_combined.at(offset+1)) == 0xAA)))
        {
            id--;
            run = false;
        }

        rom->romlen = uint64_t(uint8_t(_combined.at(offset+2)) * _main->GetTooBox()->GetMultiplier());
        if (_combined.length() < rom->romlen)
            QMessageBox::information(_main, "ROM problem", "Rom is too short!");

        rom->indicator = char(_combined.at(offset+0x18)) + char(_combined.at(offset+0x19)) * 256 + 0x15;
        roms.push_back(rom);

        offset = offset + rom->romlen;
        id++;
    }

    for (std::vector<Roms*>::const_iterator itr = roms.begin(); itr != roms.end(); itr++)
    {
        uint8_t indicator = _combined.at((*itr)->offset+(*itr)->indicator);

        //Check if we hit the EFI
        if ((*itr)->id == id)
            return true;

        //Check if we are at the last rom
        if ((*itr)->id+1 != id)
         indicator &= 0x7F; //force msb to 0
        else
          indicator |= 0x80; //force msb to 1

        _combined.data()[((*itr)->offset+(*itr)->indicator)] = indicator;
        uint8_t sum = 0;
        for (uint64_t counter = (*itr)->offset; counter != ((*itr)->romlen + (*itr)->offset)-1; counter++)
            sum = (sum + _combined.data()[counter]) % 0x100;
        _combined.data()[((*itr)->romlen + (*itr)->offset)-1] = (0x100 - sum);
    }
    return true;
}

void OpRom::readEFIData()
{
    QString str_value;
    int j = 0;
    while ((j = _efi.indexOf("PCIR", j)) != -1)
    {
        break;
        ++j;
    }

    uint16_t vendor = ((uint16_t)_efi.data()[j+5] << 8) | uint8_t(_efi.data()[j+4]);
    str_value= QString("0x%1").arg(vendor , 0, 16);
    _main->GetTooBox()->SetEFIDeviceName(str_value);

    uint16_t device = ((uint16_t)_efi.data()[j+7] << 8) | uint8_t(_efi.data()[j+6]);
    str_value= QString("0x%1").arg(device , 0, 16);
    _main->GetTooBox()->SetEFIDeviceId(str_value);

    j = 0;
    while ((j = _efi.indexOf("ATY,", j)) != -1)
    {
        break;
        ++j;
    }

    str_value.clear();
    int t = 0;
    while (true)
    {
        if ((uint8_t)_efi.data()[j+t] == 0)
            break;

        str_value.append(_efi.data()[j+t]);
        ++t;
    }
    _main->GetTooBox()->SetEFIFramebuffer(str_value);
}
