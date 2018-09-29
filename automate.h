#ifndef AUTOMATE_H
#define AUTOMATE_H


class EfiPatcher;
class Automate
{
public:
    Automate(EfiPatcher* m);

private:
    EfiPatcher* _main;

};

#endif // AUTOMATE_H
