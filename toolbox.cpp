#include "toolbox.h"
#include "ui_toolbox.h"

#include "QDebug"
ToolBox::ToolBox(QWidget *parent) :
    QToolBox(parent),
    ui(new Ui::ToolBox)
{
    ui->setupUi(this);
    ui->offsetbox->setPrefix("0x");
    ui->offsetbox->setDisplayIntegerBase(16);
    ui->offsetbox->setRange(INT_MIN, INT_MAX);
    ui->offsetbox->setValue(0x10000);
}

ToolBox::~ToolBox()
{
    delete ui;
}

int ToolBox::GetMultiplier()
{
    return ui->Multiplier->value();
}

int ToolBox::GetOffset()
{
    return ui->offsetbox->value();
}

void ToolBox::SetVendor(QString arg)
{
    ui->vendorName->setText(arg);
}

void ToolBox::SetDeviceID(QString arg)
{
    ui->deviceId->setText(arg);
}

void ToolBox::Test(QString arg)
{
    ui->textBrowser->setText(arg);
}

//The efi page
void ToolBox::SetEFIDeviceName(QString args)
{
    ui->efi_devicename->setText(args);
}

void ToolBox::SetEFIDeviceId(QString args)
{
    ui->efi_deviceId->setText(args);
}

void ToolBox::SetEFIFramebuffer(QString args)
{
    ui->efi_framebuffer->setText(args);
}

void ToolBox::SetEFIPublisher(QString args)
{
    ui->efi_publisher->setText(args);
}

void ToolBox::SetEFIURL(QString args)
{
    ui->efi_url->setText(args);
}

void ToolBox::SetEFICompat(QString id, QString name, QString descr)
{
    _efi_compatlist[id.toInt()] = descr;
    ui->efi_compatcards->addItem(name);
}

void ToolBox::on_efi_compatcards_currentIndexChanged(const QString /*&arg1*/)
{
    ui->efi_compattext->clear();
    ui->efi_compattext->appendPlainText(_efi_compatlist[ui->efi_compatcards->currentIndex()]);
}
