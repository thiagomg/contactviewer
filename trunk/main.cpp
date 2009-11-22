#include <QtGui/QApplication>
#include "dlgcontact.h"

#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if( argc == 1 ) {
        QMessageBox::warning(NULL, "Aviso", QString::fromUtf8("O primeiro parâmetro deve ser um arquivo de contato"));
        return -1;
    }

    QString fName;
    //fName = QString::fromUtf8("/home/thiago/contatos/Contatos Comerciais/CLÍNICA ORTOPÉDICA - COT Tatuapé.contact");
    fName = QString::fromUtf8(argv[1]);

    DlgContact w(NULL, fName);
    w.show();
    return a.exec();
}
