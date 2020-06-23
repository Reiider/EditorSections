#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

// Xq3FgVN8Hfdcz2Fe

#include <QWidget>
#include "database.h"
#include <QMessageBox>
#include <QRegExp>

namespace Ui {
class Authorization;
}

class Authorization : public QWidget
{
    Q_OBJECT

public:
    explicit Authorization(QWidget *parent = 0, DataBase *db = new DataBase());
    ~Authorization();

    void changeMainLable();

signals:
    void setAccess(QString login, QString acc);

public slots:
    void createCube();
    void exitLogin();


private slots:
    void disableEntryForLoad();

    void rotate();

    void on_checkBox_clicked(bool checked);

    void on_checkBox_reg_password_clicked(bool checked);

    void on_checkBox_reg_repeat_password_clicked(bool checked);

    void on_entry_clicked();

    void on_entry_login_textChanged();

    void on_entry_password_textChanged();

    void on_reg_clicked();

    void on_reg_login_textChanged();

    void on_reg_password_textChanged();

    void on_reg_repeat_password_textChanged();

private:
    Ui::Authorization *ui;

    DataBase *db;

    bool firstReg;
    QTimer *timer;
};

#endif // AUTHORIZATION_H
