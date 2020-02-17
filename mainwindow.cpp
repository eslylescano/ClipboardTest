#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTextEdit>
#ifdef Q_OS_WASM
#include "emscripten.h"
#endif

#ifdef Q_OS_WASM



EM_JS(void, copy_to_clipboard, (const char* data), {
          var dataString = UTF8ToString(data);
  alert("Data is: "+dataString);
          navigator.clipboard.writeText(dataString)
            .then(() => {
              alert('Text copied.');
            })
            .catch((e) => {
              alert('Failed to copy text. Error: '+e);
            });
});

EM_JS(void, paste_from_clipboard, (int* address ), {
          alert("Address is: "+address);
          navigator.clipboard.readText()
            .then(text => {
              console.log('Text pasted.');
              var lengthBytes = lengthBytesUTF8(text)+1;
              var stringOnWasmHeap = _malloc(lengthBytes);
              stringToUTF8(text, stringOnWasmHeap, lengthBytes);
              Module._setPasteText(stringOnWasmHeap,address);
            })
            .catch(() => {
              console.log('Failed to read from clipboard.');
            });
});

extern "C" {

void EMSCRIPTEN_KEEPALIVE setPasteText(const char* data,int* address) {
QTextEdit *textEdit = (QTextEdit*)address;
textEdit->setText(QString(data));

}
}


#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

QString dataToCopy = ui->textEdit->toPlainText();
const char *chardataToCopyChar =dataToCopy.toUtf8().constData();
int *address=(int*)ui->textEdit;
#ifdef Q_OS_WASM
copy_to_clipboard(chardataToCopyChar);
#endif

//qDebug() << chardataToCopyChar;

}



void MainWindow::on_pushButton_2_clicked()
{
  //qDebug() << ui->textEdit->toPlainText();
    QString dataToCopy = ui->textEdit->toPlainText();
    const char *chardataToCopyChar =dataToCopy.toUtf8().constData();
    int *address=(int*)ui->textEdit;
    #ifdef Q_OS_WASM
    paste_from_clipboard(address);
    #endif
}
