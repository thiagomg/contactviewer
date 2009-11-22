#include "dlgcontact.h"
#include "ui_dlgcontact.h"

#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDomDocument>

DlgContact::DlgContact(QWidget *parent, QString fName)
    : QDialog(parent), ui(new Ui::DlgContact)
{
    ui->setupUi(this);

    ui->txtDetails->setAcceptRichText(true);

    _fName = fName;
    _isTitled = false;

    //connect(this, SIGNAL(accepted()), this, SLOT(on_show()));
    on_show();
}

DlgContact::~DlgContact()
{
    delete ui;
}

void DlgContact::on_show()
{

    QFile f(_fName);
    if( !f.open(QIODevice::ReadOnly) ) {
        QString text = "Arquivo " + _fName + QString::fromUtf8(" não encontrado");
        QMessageBox::warning(this, "Erro", text);
        qApp->quit();
        return;
    }

    QDomDocument doc;
    if( !doc.setContent(&f) ) {
        f.close();
        return;
    }

    QDomElement root = doc.documentElement();

    const QDomNodeList &rootChilds = root.childNodes();
    for(int i=0; i < rootChilds.count(); i++) {
        const QDomNode &node = rootChilds.at(i);
        if( node.hasChildNodes() && node.nodeName().contains("Collection", Qt::CaseInsensitive) && node.nodeName().compare("c:ContactIDCollection") != 0 ) {

            //We found a collection. Let's get the childs.
            const QDomNodeList &childs = node.childNodes();
            int ret=0;
            for(int c=0; c < childs.count(); c++) {
                const QDomNode &n = childs.at(c);
                ret += _printItem(n);
            }
            if( ret > 0 )
                ui->txtDetails->append( "" );

        }
    }

    QTextCursor cursor = ui->txtDetails->textCursor();
    cursor.setPosition(0, QTextCursor::MoveAnchor);
    ui->txtDetails->setTextCursor(cursor);

    f.close();

}

int DlgContact::_printItem(const QDomNode &itemNode)
{
    int ret=0;
    bool headerPrinted=false;
    if( !itemNode.hasChildNodes() ) return 0;

    const QDomNodeList &list = itemNode.childNodes();

    for(int i=0; i < list.count(); i++) {
        const QDomNode &node = list.at(i);
        if( node.nodeName().contains("Collection") ) continue;
        if( node.hasChildNodes() ) {
            if( !headerPrinted ) {
                ui->txtDetails->setFontUnderline(true);
                int weight = ui->txtDetails->fontWeight();
                ui->txtDetails->setFontWeight(QFont::Bold);
                ui->txtDetails->append( _formatKey(itemNode.nodeName()) );
                ui->txtDetails->setFontWeight(weight);
                ui->txtDetails->setFontUnderline(false);
            }
            headerPrinted=true;
            const QDomText &text = node.firstChild().toText();
            QString name = node.nodeName();
            ui->txtDetails->append( _formatKey(node.nodeName()) + ": " + text.nodeValue());
            ret++;
        }
    }
    return ret;
}

QString DlgContact::_formatKey(const QString &key)
{
    if( key.compare("c:Notes", Qt::CaseInsensitive) == 0 ) {
        return "Nota";
    } else if( key.compare("c:CreationDate", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Criação");
    } else if( key.compare("c:FormattedName", Qt::CaseInsensitive) == 0 ) {
        return "Nome Completo";
    } else if( key.compare("c:Name", Qt::CaseInsensitive) == 0 ) {
        return "Nome";
    } else if( key.compare("c:GivenName", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Nome Título");
    } else if( key.compare("c:PostalCode", Qt::CaseInsensitive) == 0 ) {
        return "CEP";
    } else if( key.compare("c:Street", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Logradouro");
    } else if( key.compare("c:Number", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Número");
    } else if( key.compare("c:MiddleName", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Nome meio");
    } else if( key.compare("c:PhysicalAddress", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Endereço");
    } else if( key.compare("c:PhoneNumber", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Telefone");
    } else if( key.compare("c:EmailAddress", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Endereço de e-mail");
    } else if( key.compare("c:Position", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Cargo");
    } else if( key.compare("c:Type", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Tipo");
    } else if( key.compare("c:Address", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Endereço");
    } else if( key.compare("c:Company", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Empresa");
    } else if( key.compare("c:Url", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Site");
    } else if( key.compare("c:Value", Qt::CaseInsensitive) == 0 ) {
        return QString::fromUtf8("Valor");
    } else {
        return key;
    }
}
