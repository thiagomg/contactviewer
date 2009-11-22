#ifndef DLGCONTACT_H
#define DLGCONTACT_H

#include <QtGui/QDialog>

namespace Ui
{
    class DlgContact;
}

class QDomNode;

class DlgContact : public QDialog
{
    Q_OBJECT

public:
    DlgContact(QWidget *parent = 0, QString fName="");
    ~DlgContact();

protected:
    QString _formatKey(const QString &key);

    int _printItem(const QDomNode &itemNode);

protected slots:
    void on_show();

private:
    Ui::DlgContact *ui;
    QString _fName;
    bool _isTitled;
    typedef QPair<QString, QString> ListItem;
    QList<ListItem> _list;
};

#endif // DLGCONTACT_H
