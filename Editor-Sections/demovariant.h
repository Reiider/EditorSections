#ifndef DEMOVARIANT_H
#define DEMOVARIANT_H

#include <QWidget>

namespace Ui {
class DemoVariant;
}

class DemoVariant : public QWidget
{
    Q_OBJECT

public:
    explicit DemoVariant(QWidget *parent = 0);
    ~DemoVariant();

    void setText(QString s);
    void setIndex(int i);
    void setPBRedact();
    void setPBSave();
    bool isSave();

signals:
    void viewVariant(int i);
    void deleteVariant(int i);
    void redactVariant(int i);
    void saveVariant(int i, QString s);

private slots:
    void on_pushButton_view_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_readact_clicked();

private:
    Ui::DemoVariant *ui;
    int index;
};

#endif // DEMOVARIANT_H
