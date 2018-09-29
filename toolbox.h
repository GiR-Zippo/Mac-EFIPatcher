#ifndef TOOLBOX_H
#define TOOLBOX_H

#include <QToolBox>
#include <map>

namespace Ui {
class ToolBox;
}

class ToolBox : public QToolBox
{
    Q_OBJECT

public:
    explicit ToolBox(QWidget *parent = 0);
    ~ToolBox();

    int GetMultiplier();
    int GetOffset();

    void SetVendor(QString arg);
    void SetDeviceID(QString arg);

    void SetEFIDeviceName(QString args);
    void SetEFIDeviceId(QString args);
    void SetEFIFramebuffer(QString args);
    void SetEFIPublisher(QString args);
    void SetEFIURL(QString args);
    void SetEFICompat(QString id, QString name, QString descr);

    void Test(QString arg);

private slots:
    void on_efi_compatcards_currentIndexChanged(const QString /*&arg1*/);

private:
    Ui::ToolBox *ui;

    std::map<uint8_t, QString> _efi_compatlist;
};

#endif // TOOLBOX_H
