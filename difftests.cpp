#include "difftests.h"
#include "ui_difftests.h"
#include "QTime"
#include "bitChanger.h"
#include "steganography.h"
#include "stringiterator.h"
#include "iostream"
#include "QImage"

#include <QtGui>

DiffTests::DiffTests(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiffTests)
{
    ui->setupUi(this);
    
    //hide-Buttons
    connect( ui->picBrowseButton, SIGNAL( clicked() ), this, SLOT( chosePicture() ) );
    connect( ui->textBrowseButton, SIGNAL( clicked() ), this, SLOT( choseText() ) );
    connect( ui->hideButton, SIGNAL( clicked() ), this, SLOT( hide() ) );
    connect( ui->keyBrowseButton, SIGNAL( clicked() ), this, SLOT( browseOneTimePad() ) );
    ui->hideButton->setEnabled(false);
    ui->cryptFrame->hide();
    connect( ui->encryptCheckBox, SIGNAL(toggled(bool)), this, SLOT(showCryptFrame(bool)) );
    connect( ui->picPathTextField, SIGNAL(textChanged()), this, SLOT(showButtonHide()) );
    connect( ui->textPathTextField, SIGNAL(textChanged()), this, SLOT(showButtonHide()) );
    connect( ui->textEdit, SIGNAL(textChanged()), this, SLOT(showButtonHide()) );
    connect( ui->textFromDocRadio, SIGNAL(toggled(bool)), this, SLOT(showButtonHide()) );
    connect( ui->textFromFieldRadio, SIGNAL(toggled(bool)), this, SLOT(showButtonHide()) );

    //find-Buttons
    connect( ui->picBrowseButton_2, SIGNAL( clicked() ), this, SLOT( chosePicture() ) );
    connect( ui->findButton, SIGNAL( clicked() ), this, SLOT( find() ) );
    connect( ui->keyBrowseButton_2, SIGNAL( clicked() ), this, SLOT( browseOneTimePad() ) );
   // ui->findButton->setEnabled(false); //TODO: run clicked RadioButtons
    ui->cryptFrame_2->hide();
    ui->knowledgeKeyFrame->hide();
    ui->attackFrame->hide();
    connect( ui->encryptCheckBox_2, SIGNAL(toggled(bool)), this, SLOT(showCryptCheckFrame_2(bool)) );
    connect( ui->knownKeyRadio, SIGNAL(clicked()), this, SLOT(showCryptFrame_2(bool)) );
    connect( ui->unknownKeyRadio, SIGNAL(clicked()), this, SLOT(showAttackFrame(bool)) );
    connect( ui->picPathTextField_2, SIGNAL(textChanged()), this, SLOT(showButtonFind()) );
    connect( ui->textToDocRadio, SIGNAL(clicked()), this, SLOT(showButtonFind()) );
    connect( ui->textToFieldRadio, SIGNAL(clicked()), this, SLOT(showButtonFind()) );
}

DiffTests::~DiffTests()
{
    delete ui;
}

void DiffTests::showCryptCheckFrame_2(bool show){
    if(show){
        ui->knowledgeKeyFrame->show();
    }else{
        ui->knowledgeKeyFrame->hide();
    }
}

void DiffTests::showCryptFrame_2(bool show){
    if(show){
        ui->cryptFrame_2->show();
    }else{
        ui->cryptFrame_2->hide();
    }
}

void DiffTests::showAttackFrame(bool show){
    if(show){
        ui->attackFrame->show();
    }else{
        ui->attackFrame->hide();
    }
}

void DiffTests::showCryptFrame(bool show){
    if(show){
        ui->cryptFrame->show();
    }else{
        ui->cryptFrame->hide();
    }
}

void DiffTests::showButtonHide(){
    if(ui->textFromDocRadio && isPath(ui->textPathTextField->toPlainText()) && isPath(ui->picPathTextField->toPlainText())){
            ui->hideButton->setEnabled(true);
    }else if(ui->textFromFieldRadio && !(ui->textEdit->toPlainText().isEmpty())&& isPath(ui->picPathTextField->toPlainText())){
            ui->hideButton->setEnabled(true);
    }else{
            ui->hideButton->setEnabled(false);
        }
}

void DiffTests::showFindButton(){
    if(isPath(ui->picPathTextField_2->toPlainText()) && (ui->textToDocRadio->isChecked() || ui->textToFieldRadio->isChecked())){
        ui->findButton->setEnabled(true);
    }else{
        ui->findButton->setEnabled(false);
    }
}

bool DiffTests::isPath(QString path){
    //TODO: implement this method
    return !(path.isEmpty());
}

void DiffTests::chosePicture()
{
    QString path;
    path = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                QDir::current().path(),
                "PNG Files(*.png)");  //JPEG files (*.jpg *.png);; Gif files (*.gif)
    ui->picPathTextField->setText( path );
    ui->picPathTextField_2->setText( path );
}

void DiffTests::choseText()
{
    QString path;
    path = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                QString::null,
                "Text Files(*.txt)");
    ui->textPathTextField->setText( path );
}

void DiffTests::hide()
{
    QString picPath = ui->picPathTextField->toPlainText();
    Steganography stego(picPath);

    QString plain;
    if(ui->textFromDocRadio->isChecked()) plain = ui->textPathTextField->toPlainText();
    else plain = ui->textEdit->toPlainText();  //if(textFromFieldRadio)

    stego.insertText(plain);

    QString newPath = QFileDialog::getSaveFileName(this, tr("Save File"), picPath, tr("*.png *.jpg"));

    stego.saveImage(newPath);
    qDebug("Fertig!");
}

void DiffTests::find()
{
    QString picPath = ui->picPathTextField_2->toPlainText();
    Steganography stego(picPath);

    QString plain = stego.getText();
    ui->textEdit_2->setText(plain);
}

void DiffTests::browseOneTimePad()
{
    QString path;
    path = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                QString::null,
                "Text Files(*.txt)");
    ui->keyTextField->setText( path );
    ui->keyTextField_2->setText( path );
}

void DiffTests::decrypt()
{

}

void DiffTests::encrypt()
{

}
