#include "addpicdialog.h"
#include "ui_addpicdialog.h"
#include "difftests.h"
#include "intermediary.h"

#include <QtGui>

AddPicDialog::AddPicDialog(Intermediary *im,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddPicDialog),
    im(im)
{
    ui->setupUi(this);
    setWindowTitle("Add pictures");

    ui->okButton->setEnabled(false);
    ui->delButton->setEnabled(false);
    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(add()) );
    connect(ui->delButton, SIGNAL(clicked()), this, SLOT(del()) );
    connect(ui->okButton, SIGNAL(clicked()), this, SLOT(ok()) );
    connect(ui->cancleButton, SIGNAL(clicked()), this, SLOT(reject()) );
    connect(ui->bpPComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showOK()) );
    connect(ui->picList, SIGNAL(clicked(QModelIndex)), this, SLOT(showDEL()) );
    actualizeList();
}

AddPicDialog::~AddPicDialog()
{
    delete ui;
}

void AddPicDialog::actualizeList()
{
    ui->picList->clear();
    QMap<QString, QImage>::const_iterator it = im->images->constBegin();
    while (it != im->images->constEnd()) {
        ui->picList->addItem(it.key());
        it++;
    }
}

void AddPicDialog::add()
{
    QString newPath = QFileDialog::getOpenFileName(this, tr("Add picture"), QDir::homePath(), tr("*.png"));
    im->addImage(newPath);
    ui->picList->addItem(new QListWidgetItem(newPath));
    actualizeList();
    showOK();
}

void AddPicDialog::del()
{
    QString path = ui->picList->item(ui->picList->currentRow())->text();
    im->images->remove(path);
    ui->delButton->setEnabled(false);
    actualizeList();
    showOK();
}

void AddPicDialog::ok()
{
    QString newPath = QFileDialog::getSaveFileName(this, tr("Save as"), QDir::homePath(), tr("*.png"));
    switch(ui->bpPComboBox->currentIndex())
    {
    case 0: im->hide_1Bit(newPath);
        break;
    case 1: im->hide_3Bit(newPath);
        break;
    case 2: im->hide_6Bit(newPath);
        break;
    default:;
    }
    setResult(1);
    close();
}

void AddPicDialog::showDEL(){ ui->delButton->setEnabled(true);}

void AddPicDialog::showOK()
{
    bool show = false;
    switch(ui->bpPComboBox->currentIndex())
    {
    case 0: if(im->isReady_1Bit()){show=true;}
        break;
    case 1: if(im->isReady_3Bit()) {show=true;}
        break;
    case 2: if(im->isReady_6Bit()) {show=true;}
        break;
    default:;
    }
    ui->okButton->setEnabled(show);
}
