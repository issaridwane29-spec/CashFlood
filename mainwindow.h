#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTabWidget>
#include "comptedao.h"
#include "operationdao.h"
#include "categoriedao.h"
#include "comptewidget.h"
#include "transactionwidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTabWidget *m_onglets = nullptr;

    CompteDao m_compteDao;
    OperationDao m_operationDao;
    CategorieDao m_categorieDao;

    CompteWidget *m_compteWidget = nullptr;
    TransactionWidget *m_transactionWidget = nullptr;
};

#endif // MAINWINDOW_H
