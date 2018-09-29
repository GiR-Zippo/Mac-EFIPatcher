#include "efipatcher.h"
#include "ui_efipatcher.h"
#include "QFileDialog"
#include "QMessageBox"
#include "QXmlStreamReader"

#include "toolbox.h"
#include "hexview/QHexView.h"

#include "OpRom/oprom.h"
#include "automate.h"

#include "QDebug"

EfiPatcher::EfiPatcher(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EfiPatcher)
{
    ui->setupUi(this);

    QHexView *pwgt = new QHexView;
    setCentralWidget(pwgt);

    _box = new ToolBox;
    _box->setVisible(true);
    _oprom = new OpRom(this);
    _automate = new Automate(this);

}

EfiPatcher::~EfiPatcher()
{
    delete ui;
}

void EfiPatcher::on_actionOpen_Rom_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Rom"), "/home/", tr("Rom Files (*.bin *.rom *.img)"));
    _oprom->LoadBios(fileName, true);
    QHexView *pcntwgt = dynamic_cast<QHexView *>(centralWidget());
    pcntwgt -> clear();
    pcntwgt -> setData(new QHexView::DataStorageArray(_oprom->GetBiosRom()));
    this->update();
}

void EfiPatcher::on_actionQuit_triggered()
{
    exit(1);
}

void EfiPatcher::on_actionOpen_Efi_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Efi"), "/home/", tr("Efi Files (*.efi)"));
    _oprom->LoadBios(fileName, false);
    QHexView *pcntwgt = dynamic_cast<QHexView *>(centralWidget());
    pcntwgt -> clear();
    pcntwgt -> setData(new QHexView::DataStorageArray(_oprom->GetEfiRom()));
    this->update();
}

void EfiPatcher::on_actionShow_Bios_triggered()
{
    QHexView *pcntwgt = dynamic_cast<QHexView *>(centralWidget());
    pcntwgt -> clear();
    pcntwgt -> setData(new QHexView::DataStorageArray(_oprom->GetBiosRom()));
    pcntwgt->update();
}

void EfiPatcher::on_actionShow_Efi_triggered()
{
    QHexView *pcntwgt = dynamic_cast<QHexView *>(centralWidget());
    pcntwgt -> clear();
    pcntwgt -> setData(new QHexView::DataStorageArray(_oprom->GetEfiRom()));
    pcntwgt->update();
}

void EfiPatcher::on_actionShow_combined_triggered()
{
    QHexView *pcntwgt = dynamic_cast<QHexView *>(centralWidget());
    pcntwgt -> clear();
    pcntwgt -> setData(new QHexView::DataStorageArray(_oprom->GetCombinedRom() ));
    pcntwgt->update();
}

void EfiPatcher::on_actionSave_Combined_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),"/home/", tr("Rom File (*.bin *.rom)"));
    QFile file(fileName);
    if ( file.open(QIODevice::ReadWrite) )
    {
        file.write(_oprom->GetCombinedRom() );
    }
        file.close();
}

void EfiPatcher::on_actionCombine_triggered()
{
    _oprom->FixRom();
}

void EfiPatcher::on_actionOpen_XML_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML"), "/home/", tr("XML Files (*.xml)"));
    QFile xmlFile(fileName);
    if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this,"Load XML File Problem",
        "Couldn't open xmlfile",
        QMessageBox::Ok);
        return;
    }

    QXmlStreamReader xmlReader(&xmlFile);
    while(!xmlReader.atEnd() && !xmlReader.hasError())
    {
        QXmlStreamReader::TokenType token = xmlReader.readNext();
        if(token == QXmlStreamReader::StartDocument)
            continue;

        if(token == QXmlStreamReader::StartElement)
        {
            if(xmlReader.name() == "Description")
            {
                foreach(const QXmlStreamAttribute &attr, xmlReader.attributes())
                {
                    if (attr.name().toString() == QLatin1String("DeviceID"))
                        _box->SetEFIDeviceId(attr.value().toString());
                    if (attr.name().toString() == QLatin1String("DeviceName"))
                        _box->SetEFIDeviceName(attr.value().toString());
                    if (attr.name().toString() == QLatin1String("Framebuffer"))
                        _box->SetEFIFramebuffer(attr.value().toString());
                    if (attr.name().toString() == QLatin1String("Publisher"))
                        _box->SetEFIPublisher(attr.value().toString());
                    if (attr.name().toString() == QLatin1String("URL"))
                        _box->SetEFIURL(attr.value().toString());
                }
                continue;
            }
            if(xmlReader.name() == "compat")
            {
                readCompatList(xmlReader);
            }
            if(xmlReader.name() == "EFI")
            {
                QByteArray b;
                b = xmlReader.readElementText().toStdString().c_str();
                _oprom->SetEfiRom(QByteArray::fromBase64(b));
            }
        }
    }

    if(xmlReader.hasError())
    {
        QMessageBox::critical(this,"xmlFile Parse Error",xmlReader.errorString(),QMessageBox::Ok);
        return;
    }
    xmlReader.clear();
    xmlFile.close();
}

void EfiPatcher::readCompatList(QXmlStreamReader &xml)
{
    if(xml.tokenType() != QXmlStreamReader::StartElement && xml.name() != "ENTRY")
        return;
    xml.readNext();
    while( xml.tokenType() != QXmlStreamReader::EndElement)
    {
        if(xml.name() == "ENTRY")
        {
            QString name, id, descr;
            foreach(const QXmlStreamAttribute &attr, xml.attributes())
            {
                if (attr.name().toString() == QLatin1String("ID"))
                    id = attr.value().toString();
                if (attr.name().toString() == QLatin1String("Name"))
                    name = attr.value().toString();
            }

            descr = xml.readElementText().toStdString().c_str();
            _box->SetEFICompat(id, name, descr);
        }
        xml.readNext();
    }
    return;

}
