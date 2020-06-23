#ifndef STATISTIC_H
#define STATISTIC_H

#include <QWidget>
#include "database.h"
#include <QTableWidget>
#include <QMessageBox>
#include "filemanager.h"
#include <QDateTime>
#include <QString>
#include <QList>
#include "viewtasks.h"

namespace Ui {
class Statistic;
}

class Statistic : public QWidget
{
    Q_OBJECT

public:
    explicit Statistic(QWidget *parent = 0, DataBase *db = new DataBase(), ViewTasks *vt = new ViewTasks());
    ~Statistic();

    void deleteSelectedItems();

public slots:
    void showThisWindow();

private slots:
    void doubleClicked(int row, int col);

signals:
    void viewTaskInRedact();

private:
    Ui::Statistic *ui;
    DataBase *db;
    ViewTasks *vt;
    int colLastClick;

    QList<int> listId;
    void update();
    int previousRow;

    void deleteId(int id);
};

#endif // STATISTIC_H
