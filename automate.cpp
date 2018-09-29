#include "automate.h"
#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>

#include "efipatcher.h"
#include "toolbox.h"

std::string exec(const char* cmd) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(cmd, "r");
    if (!pipe) throw std::runtime_error("popen() failed!");
    try {
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != NULL)
                result += buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }
    pclose(pipe);
    return result;
}


Automate::Automate(EfiPatcher* m) : _main(m)
{
    QString out = exec("system_profiler SPDisplaysDataType").c_str();
    _main->GetTooBox()->Test(out);

    QStringList lines = out.split("\n");
    QStringList words; //Temp

    QString vendor = lines.value(10);
    _main->GetTooBox()->SetVendor(vendor);

    QString device_id = lines.value(11);
    _main->GetTooBox()->SetDeviceID(device_id);

    //GetVendorId
    words = vendor.split(" ");
    words.removeDuplicates();
    _main->GetTooBox()->Test(words.value(3));
}

