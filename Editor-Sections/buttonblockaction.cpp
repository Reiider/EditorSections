#include "buttonblockaction.h"

ButtonBlockAction::ButtonBlockAction(QWidget *parent) :
    QPushButton(parent)
{

}

ButtonBlockAction::ButtonBlockAction(QString name, int id)
{
    this->id = id;
    this->setText(name);
    this->setFlat(true);
    connect(this, SIGNAL(clicked()), this, SLOT(blockAction()));
    this->setStyleSheet("font-size:10pt;");
}

bool ButtonBlockAction::isBlocked()
{
    if(this->styleSheet() != "font-size:10pt;") return true;
    else return false;
}

int ButtonBlockAction::getId()
{
    return id;
}

void ButtonBlockAction::reset()
{
    this->setStyleSheet("font-size:10pt;");
}

void ButtonBlockAction::blockAction()
{
    if(this->styleSheet() == "font-size:10pt;"){
        this->setStyleSheet("text-decoration: line-through; color: #a0a0a4; font-size:10pt;");
    }
    else{
        this->setStyleSheet("font-size:10pt;");
    }
}
