#include "listtaskslog.h"

ListTasksLog::ListTasksLog()
{
    currentTask = -1;
}

DigestOgl ListTasksLog::nextVariant()
{
    if(currentVariant < listTasks[currentTask].length()-1){
        currentVariant++;
    }
    curVarForLabel = QString::number(currentVariant+1) + "/" + QString::number(listTasks[currentTask].length());
    return listTasks[currentTask].at(currentVariant);
}

DigestOgl ListTasksLog::previousVariant()
{
    if(currentVariant > 0){
        currentVariant--;
    }
    curVarForLabel = QString::number(currentVariant+1) + "/" + QString::number(listTasks[currentTask].length());
    return listTasks[currentTask].at(currentVariant);
}

DigestOgl ListTasksLog::lastVariant()
{
    currentVariant = listTasks[currentTask].length()-1;
    curVarForLabel = QString::number(currentVariant+1) + "/" + QString::number(listTasks[currentTask].length());
    return listTasks[currentTask].at(currentVariant);
}

DigestOgl ListTasksLog::firstVariant()
{
    currentVariant = 0;
    curVarForLabel = QString::number(currentVariant+1) + "/" + QString::number(listTasks[currentTask].length());
    return listTasks[currentTask].at(currentVariant);
}

DigestOgl ListTasksLog::openTask(int i)
{
    curVarForLabel = QString::number(listTasks[i].length()) + "/" + QString::number(listTasks[i].length());
    currentVariant = listTasks[i].length()-1;
    currentTask = i;
    return listTasks[i].last();
}

QList<QString> ListTasksLog::getListConditionTasks()
{
    return listConditionTask;
}

int ListTasksLog::getLenghtConditionTasks()
{
    return listConditionTask.length();
}

QString ListTasksLog::getStrForLabel()
{
    return curVarForLabel;
}

QList<QList<DigestOgl> > ListTasksLog::getListTasks()
{
    return listTasks;
}

QList<DigestOgl> ListTasksLog::getListVariants(int i)
{
    return listTasks[i];
}

void ListTasksLog::appendTask(DigestOgl d, QString textTask)
{
    int i;
    for(i = 0; i < listConditionTask.length(); i++){
        if(listConditionTask[i] == textTask) break;
    }
    listTasks[i].append(d);
    currentTask = i;
}

void ListTasksLog::updateTask(DigestOgl d)
{
    if(listTasks.length() > currentTask && currentTask >= 0){
        if(listTasks[currentTask].length() > 0) {
            listTasks[currentTask].removeLast();
        }
        listTasks[currentTask].append(d);
    }
}

void ListTasksLog::setLogin(QString s)
{
    login = s;
}

void ListTasksLog::setTest(QString s)
{
    test = s;
}

void ListTasksLog::setVariant(QString s)
{
    variant = s;
}

QString ListTasksLog::getLogin()
{
    return login;
}

QString ListTasksLog::getTest()
{
    return test;
}

QString ListTasksLog::getVariant()
{
    return variant;
}

int ListTasksLog::getI()
{
    return currentTask;
}

void ListTasksLog::appendCondition(QString s)
{
    listConditionTask.append(s);
    QList<DigestOgl> list;
    listTasks.append(list);
}

void ListTasksLog::removeAllTasks()
{
    listTasks.clear();
    listConditionTask.clear();
}

void ListTasksLog::removeTask(int i)
{
    listTasks.removeAt(i);
    listConditionTask.removeAt(i);
}

void ListTasksLog::removeVariant(int i, int j)
{
    listTasks[i].removeAt(j);
}

void ListTasksLog::updateDigest(int iTask, int iVariant, DigestOgl digest)
{
    listTasks[iTask].removeAt(iVariant);
    listTasks[iTask].insert(iVariant, digest);
}
