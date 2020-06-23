#ifndef LISTTASKSLOG_H
#define LISTTASKSLOG_H

#include "digestogl.h"
#include <QMessageBox>
#include <QWidget>

class ListTasksLog
{
public:
    ListTasksLog();
    DigestOgl nextVariant(); //следующий вариант решения
    DigestOgl previousVariant(); //предыдущий вариант решения
    DigestOgl lastVariant(); //последний вариант решения
    DigestOgl firstVariant(); //первый вариант решения

    DigestOgl openTask(int i); // возвращает дайджест с номером i в последнем варианте

    QList<QString> getListConditionTasks(); // возвращает список задач (текст)
    int getLenghtConditionTasks();
    QString getStrForLabel(); //возвращает строку в формате: 'текущий вариант'/'всего вариантов'
    QList< QList<DigestOgl> > getListTasks();
    QList<DigestOgl> getListVariants(int i);

    void appendTask(DigestOgl d, QString textTask);
    void updateTask(DigestOgl d);

    void setLogin(QString s);
    void setTest(QString s);
    void setVariant(QString s);

    QString getLogin();
    QString getTest();
    QString getVariant();

    int getI();

    void appendCondition(QString s);

    void removeAllTasks();
    void removeTask(int i);
    void removeVariant(int i, int j);

    void updateDigest(int iTask, int iVariant, DigestOgl digest);

private:
    QWidget *wid;

    QString login;
    QString test;
    QString variant;

    QList< QList<DigestOgl> > listTasks; //список задач
    QList<QString> listConditionTask;    //список условий задач

    int currentTask, currentVariant;
    QString curVarForLabel;

    friend QDataStream& operator <<( QDataStream &stream, const ListTasksLog &ltl );
    friend QDataStream& operator >>( QDataStream &stream, ListTasksLog &ltl );
};

#endif // LISTTASKSLOG_H
