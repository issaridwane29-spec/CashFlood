#ifndef COMPTEWIDGET_H
#define COMPTEWIDGET_H

#include <QWidget>
#include "databasemanager.h"
class DatabaseManager;

namespace Ui {
class CompteWidget;
}

class CompteWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CompteWidget(QWidget *parent = nullptr);
    void setDatabaseManager(DatabaseManager *dbManager);
    ~CompteWidget();


private:
    Ui::CompteWidget *ui;
    DatabaseManager *m_dbManager = nullptr;

private slots:
    void rafraichirComptes();

};

#endif // COMPTEWIDGET_H
