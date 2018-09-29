#ifndef EFIPATCHER_H
#define EFIPATCHER_H

#include <QMainWindow>

namespace Ui {
class EfiPatcher;
}

class OpRom;
class ToolBox;
class Automate;
class QXmlStreamReader;

class EfiPatcher : public QMainWindow
{
    Q_OBJECT

public:
    explicit EfiPatcher(QWidget *parent = 0);
    ~EfiPatcher();

    ToolBox *GetTooBox() { return _box; }

private slots:
    void on_actionOpen_Rom_triggered();

    void on_actionQuit_triggered();

    void on_actionOpen_Efi_triggered();

    void on_actionShow_Bios_triggered();

    void on_actionShow_Efi_triggered();

    void on_actionShow_combined_triggered();

    void on_actionSave_Combined_triggered();

    void on_actionCombine_triggered();

    void on_actionOpen_XML_triggered();

private:
    Ui::EfiPatcher *ui;

    ToolBox* _box;
    OpRom* _oprom;
    Automate* _automate;

    void readCompatList(QXmlStreamReader& xml);
};

#endif // EFIPATCHER_H
