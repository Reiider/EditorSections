#ifndef BUTTONBLOCKACTION_H
#define BUTTONBLOCKACTION_H

#include <QPushButton>

class ButtonBlockAction : public QPushButton
{
    Q_OBJECT
public:
    explicit ButtonBlockAction(QWidget *parent = 0);
    ButtonBlockAction(QString name, int id);
    bool isBlocked();
    int getId();
    void reset();

signals:

public slots:

    void blockAction();

private:
    int id;

};

#endif // BUTTONBLOCKACTION_H
