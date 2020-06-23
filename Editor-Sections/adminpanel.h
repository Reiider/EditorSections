#ifndef ADMINPANEL_H
#define ADMINPANEL_H

#include <QWidget>
#include "database.h"
#include <QList>
#include "helpfunction.h"
#include <QTimer>

namespace Ui {
class AdminPanel;
}

class AdminPanel : public QWidget
{
    Q_OBJECT

public:
    explicit AdminPanel(QWidget *parent = 0, DataBase *db = new DataBase());
    ~AdminPanel();

    void startListningNewUsers();
    void stopListningNewUsers();

public slots:
    void showThisWindow();


private slots:
    void loadUsers();

    void on_select_clicked();

    void on_cancel_clicked();

    void on_save_clicked();

    void on_pushButton_del_clicked();

    void on_users_currentTextChanged(const QString &arg1);

    void on_adm_clicked();

private:
    Ui::AdminPanel *ui;
    DataBase *db;

    QList<QString> users;

    QTimer *forLoadUser;
    HelpFunction help;
    void setAcc(QString s, bool ok);

};

#endif // ADMINPANEL_H
