#include "difftests.h"
#include "ui_difftests.h"
#include "QTime"
#include "bitChanger.h"
#include "steganography.h"
#include "stringiterator.h"
#include "iostream"
#include "QImage"
#include "crypt.h"
#include "problemdialog.h"
#include "intermediary.h"
#include "noisewarning.h"
#include "addpicdialog.h"

#include <QtGui>
#define UNICODE 1
#define ASCII 0
#define ENCRYPT 1
#define DECRYPT 0

#define CANCEL 0
#define DENSITY 1
#define PICS 2

int format;
QString ascii = " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";


DiffTests::DiffTests(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiffTests)
{
    ui->setupUi(this);
    setWindowTitle("Stego-saur");

    //personal helps
    //ui->picPathTextField->setText("C:/Users/Ben/Desktop/erdbeere.png");
    //ui->picPathTextField_2->setText("C:/Users/Ben/Desktop/erdbeere2.png");
    ui->picPathTextField->setText("C:/Users/Ben/Desktop/2x2pix.png");
    ui->textEdit->setText("Ich mag Erdbeeren!");

    //hide-Buttons
    connect( ui->picBrowseButton, SIGNAL( clicked() ), this, SLOT( chosePicture() ) );
    connect( ui->textBrowseButton, SIGNAL( clicked() ), this, SLOT( choseText() ) );
    connect( ui->hideButton, SIGNAL( clicked() ), this, SLOT( hide() ) );
    connect( ui->keyBrowseButton, SIGNAL( clicked() ), this, SLOT( browseOneTimePad() ) );
    ui->hideButton->setEnabled(false);
    ui->encryptFrame->hide();
    connect( ui->encryptCheckBox, SIGNAL(toggled(bool)), this, SLOT(showEncryptFrame(bool)) );
    connect( ui->picPathTextField, SIGNAL(textChanged()), this, SLOT(showHideButton()) );
    connect( ui->textPathTextField, SIGNAL(textChanged()), this, SLOT(showHideButton()) );
    connect( ui->textEdit, SIGNAL(textChanged()), this, SLOT(showHideButton()) );
    connect( ui->textFromDocRadio, SIGNAL(toggled(bool)), this, SLOT(showHideButton()) );
    connect( ui->textFromFieldRadio, SIGNAL(toggled(bool)), this, SLOT(showHideButton()) );
    connect( ui->keyTextField, SIGNAL(textChanged()), this, SLOT(showHideButton()) );
    connect( ui->techniqueComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(showHideButton()));

    //find-Buttons
    connect( ui->picBrowseButton_2, SIGNAL( clicked() ), this, SLOT( chosePicture_2() ) );
    connect( ui->findButton, SIGNAL( clicked() ), this, SLOT( find() ) );
    connect( ui->keyBrowseButton_2, SIGNAL( clicked() ), this, SLOT( browseOneTimePad() ) );
    ui->findButton->setEnabled(false);
    ui->decryptFrame->hide();
    ui->textEdit_2->setEnabled(false);
    connect( ui->decryptCheckBox, SIGNAL(toggled(bool)), this, SLOT(showDecryptFrame(bool)) );
    connect( ui->picPathTextField_2, SIGNAL(textChanged()), this, SLOT(showFindButton()) );
    connect( ui->textToDocRadio, SIGNAL(toggled(bool)), this, SLOT(showFindButton()) );
    connect( ui->textToFieldRadio, SIGNAL(toggled(bool)), this, SLOT(showFindButton()) );
    connect( ui->keyTextField_2, SIGNAL(textChanged()), this, SLOT(showFindButton()) );
    connect( ui->techniqueComboBox_2, SIGNAL(currentIndexChanged(int)), this, SLOT(showFindButton()));
}

DiffTests::~DiffTests()
{
    delete ui;
}

int DiffTests::getFormat(QString text)
{
    for(int i = 0; i<text.size();i++)
    {
        if(!(ascii.contains(text.at(i)))){
            return UNICODE;
        }
    }
    return ASCII;

}

void DiffTests::showEncryptFrame(bool show){
    if(show)ui->encryptFrame->show();
    else ui->encryptFrame->hide();
    showHideButton();
}

void DiffTests::showDecryptFrame(bool show){
    if(show)ui->decryptFrame->show();
    else ui->decryptFrame->hide();
    showFindButton();
}

bool DiffTests::isPath(QString path){
    QFileInfo file= path;
    return file.exists();
}

void DiffTests::chosePicture()
{
    QString path;
    path = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                QDir::homePath(),
                "PNG Files(*.png)");  //JPEG files (*.jpg *.png);; Gif files (*.gif)
    ui->picPathTextField->setText( path );
}

void DiffTests::chosePicture_2()
{
    if(ui->SeveralPicsCheckBox->isChecked()){
        //TODO: chose sev pics dialog o.�.
    }else{
        QString path;
        path = QFileDialog::getOpenFileName(
                    this,
                    "Choose a file",
                    QDir::homePath(),
                    "PNG Files(*.png)");
        ui->picPathTextField_2->setText( path );
    }
}

void DiffTests::choseText()
{
    //TODO: Bildauswahl m�glich -> kein caesar/vigenere
    QString path;
    path = QFileDialog::getOpenFileName(
                this,
                "Choose a file",
                QString::null,
                "Text Files(*.txt *.png)");
    if(path.endsWith(".png")){
        ui->encryptCheckBox->setEnabled(false);
    }
    ui->textPathTextField->setText( path );
}

void DiffTests::showHideButton()
{
    if(ui->textFromDocRadio->isChecked() && isPath(ui->textPathTextField->toPlainText()) && isPath(ui->picPathTextField->toPlainText())){
            ui->hideButton->setEnabled(true);
            QFile file(ui->textPathTextField->toPlainText());
            file.open(QIODevice::ReadOnly | QIODevice::Text);
            QTextStream in(&file);
            QString plain = in.readAll();
          format = getFormat(plain);
            file.close();
            if(format == UNICODE) ui->asciiUnicodeLabel->setText("Unicode format");
            else ui->asciiUnicodeLabel->setText("Ascii format");
    }else if(ui->textFromFieldRadio->isChecked() && !(ui->textEdit->toPlainText().isEmpty())&& isPath(ui->picPathTextField->toPlainText())){
            ui->hideButton->setEnabled(true);
            format = getFormat(ui->textEdit->toPlainText());
            if(format == UNICODE) ui->asciiUnicodeLabel->setText("Unicode format");
            else ui->asciiUnicodeLabel->setText("Ascii format");
    }else{
            ui->hideButton->setEnabled(false);
            ui->asciiUnicodeLabel->setText("");
    }

    bool show = ui->hideButton->isEnabled();

    if(ui->encryptCheckBox->isChecked()){
        int keyLength = ui->keyTextField->toPlainText().size();
        int oldFormat = format;
        if(getFormat(ui->keyTextField->toPlainText()) == ASCII)
        {
            switch (ui->techniqueComboBox->currentIndex())
            {
                case 0:
                    //Caesar
                    if(keyLength == 1){
                        ui->hideButton->setEnabled(true);
                        ui->keyTipLabel->setText("ok");
                    }else{
                        ui->hideButton->setEnabled(false);
                        ui->keyTipLabel->setText("only one letter key");
                    };
                    break;
                case 1:
                    //Vigen�re
                    if(keyLength > 0){
                        ui->keyTipLabel->setText("ok");
                        ui->hideButton->setEnabled(true);
                    }else{
                        ui->keyTipLabel->setText("one letter minimum");
                        ui->hideButton->setEnabled(false);
                    }
                    break;
                case 2:
                    //AES
                    if(keyLength <= 32 && keyLength >= 16){
                        ui->hideButton->setEnabled(true);
                    }else{
                        ui->hideButton->setEnabled(false);
                    }
                    break;
                default:
                    ui->hideButton->setEnabled(false);
                    ui->keyTipLabel->setText("");
            }
        }else{
            ui->keyTipLabel->setText("key is not Ascii format");
            ui->hideButton->setEnabled(false);
        }
        format = oldFormat;
        if(show != ui->hideButton->isEnabled()) ui->hideButton->setEnabled(false);
    }
}

void DiffTests::showFindButton()
{
    if((ui->textToDocRadio->isChecked() || ui->textToFieldRadio->isChecked()) && isPath(ui->picPathTextField_2->toPlainText())){
        ui->findButton->setEnabled(true);
    }else{
        ui->findButton->setEnabled(false);
    }

    bool show = ui->findButton->isEnabled();

    if(ui->decryptCheckBox->isChecked()){
        int keyLength = ui->keyTextField_2->toPlainText().size();
        int oldFormat = format;
        if(getFormat(ui->keyTextField_2->toPlainText()) == ASCII)
        {
            switch (ui->techniqueComboBox_2->currentIndex())
            {
                case 0:
                    //Caesar
                    if(keyLength == 1){
                        ui->findButton->setEnabled(true);
                        ui->keyTipLabel_2->setText("ok");
                    }else{
                        ui->findButton->setEnabled(false);
                        ui->keyTipLabel_2->setText("only one letter key");
                    };
                    break;
                case 1:
                    //Vigen�re
                    if(keyLength > 0){
                        ui->keyTipLabel_2->setText("ok");
                        ui->findButton->setEnabled(true);
                    }else{
                        ui->keyTipLabel_2->setText("one letter minimum");
                        ui->findButton->setEnabled(false);
                    }
                    break;
                case 2:
                    //AES
                    if(keyLength <= 32 && keyLength >= 16){
                        ui->findButton->setEnabled(true);
                    }else{
                        ui->findButton->setEnabled(false);
                    }
                    break;
                default:
                    ui->findButton->setEnabled(false);
                    ui->keyTipLabel->setText("");
            }
        }else{
            ui->keyTipLabel_2->setText("key is not Ascii format");
            ui->findButton->setEnabled(false);
        }
        format = oldFormat;
    }
    if(show != ui->findButton->isEnabled()) ui->findButton->setEnabled(false);
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

QString DiffTests::encrypt(QString plain)
{
    Crypt c (plain,ui->keyTextField->toPlainText(),format);
    QString cipher;
    switch (ui->techniqueComboBox->currentIndex())
    {
        case 0: //Caesar
            cipher = c.caesar(ENCRYPT);
            break;
        case 1: //Vigen�re
            cipher = c.vigenere(ENCRYPT);
            break;
        default:
            ui->keyTipLabel->setText("Encryption failed");
            return plain;
    }
    return cipher;
}

QString DiffTests::decrypt(QString cipher)
{
    Crypt c (cipher,ui->keyTextField_2->toPlainText(),format);
    QString plain;
    switch (ui->techniqueComboBox_2->currentIndex())
    {
        case 0: //Caesar
            plain = c.caesar(DECRYPT);
            break;
        case 1: //Vigen�re
            plain = c.vigenere(DECRYPT);
            break;
        default:
            ui->keyTipLabel_2->setText("Decryption failed");
            return cipher;
    }
    return plain;
}

int DiffTests::popupProblemDialog()
{
    pd = new ProblemDialog();
    pd->exec();
    return pd->result();
}

int DiffTests::noiseWarningDialog()
{
    nw = new NoiseWarning();
    nw->exec();
    return nw->result();
}

Intermediary* DiffTests::addPicDialog(Intermediary* im)
{
    apd = new AddPicDialog(im);
    apd->exec();
    return im;
}

void DiffTests::hide()
{
    QString plain;
    if(ui->textFromDocRadio->isChecked()){
        QString plainPath = ui->textPathTextField->toPlainText();
        QFile file(plainPath);
        file.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(&file);
        plain = in.readAll();
        file.close();
    }else if(ui->textFromFieldRadio->isChecked()) plain = ui->textEdit->toPlainText(); //if(textFromFieldRadio)

    //encrypt
    if(ui->encryptCheckBox->isChecked()){
        plain = encrypt(plain);
    }

    QString oldPath = ui->picPathTextField->toPlainText();
    im = new Intermediary(&plain, format, oldPath);
    QString newPath;

    if(im->isReady_1Bit()){
        newPath = QFileDialog::getSaveFileName(this, tr("Save File"), ui->picPathTextField->toPlainText(), tr("*.png *.jpg"));
        im->hide_1Bit(newPath);
    }else{
        int action = popupProblemDialog();
        while(action != CANCEL){

            if( action == DENSITY){
                int w = noiseWarningDialog();
                if(im->isReady_3Bit() && w == 1){
                    newPath = QFileDialog::getSaveFileName(this, tr("Save File"), ui->picPathTextField->toPlainText(), tr("*.png *.jpg"));
                    im->hide_3Bit(newPath);
                    action = CANCEL;
                }else{
                    action = popupProblemDialog();
                    if( action == DENSITY){
                        w = noiseWarningDialog();
                        if(im->isReady_3Bit() && w == 1){
                            newPath = QFileDialog::getSaveFileName(this, tr("Save File"), ui->picPathTextField->toPlainText(), tr("*.png *.jpg"));
                            im->hide_6Bit(newPath);
                            action = CANCEL;
                        }else{/*verstecken nicht m�glich*/}
                    }
                }
            }
            else if(action == PICS){
                apd = new AddPicDialog(im);
                apd->exec();
                if(apd->result() == 1){
                    action = CANCEL;
                }else{
                    action = 3;
                }
            }else{action=CANCEL;}
            qDebug("looprun");
        }
    }
    ui->picPathTextField_2->setText(newPath);
}

void DiffTests::find()
{
    im = new Intermediary(ui->picPathTextField_2->toPlainText());
    QString* plain = im->getHiddenText();//oder hiddenimage

    //decrypt
    if(ui->decryptCheckBox->isChecked()){
        plain = &(decrypt(*plain));
    }

    if(ui->textToFieldRadio->isChecked()){
        ui->textEdit_2->setText(*plain);
    }else if(ui->textToDocRadio->isChecked()){
        QString newPath = QFileDialog::getSaveFileName(
                    this,
                    "Save Textfile",
                    QString::null,
                    "Text Files(*.txt *.png)");
        //TODO auslesen, ob bild oder text
        QFile file(newPath);
        file.open(QIODevice::WriteOnly | QIODevice::Text);
        QTextStream out(&file);
        out << plain;
        file.close();
    }
}
