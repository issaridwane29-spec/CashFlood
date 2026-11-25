#ifndef TRANSACTIONWIDGET_H
#define TRANSACTIONWIDGET_H

#include <QWidget>
#include <QHash>
#include <QString>
#include "operation.h"
#include "operationdao.h"
#include "comptedao.h"
#include "categoriedao.h"
#include "compteoperationservice.h"

namespace Ui {
class TransactionWidget;
}

class TransactionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionWidget(QWidget *parent = nullptr);
    ~TransactionWidget();

    void setDaos(OperationDao *operationDao, CompteDao *compteDao, CategorieDao *categorieDao);

public slots:
    void rafraichirTransactions();

signals:
    void transactionsModifiees();

private slots:
    void onAjouterTransaction();
    void onReinitialiserFiltres();

private:
    void rechargerFiltresCombos();
    void modifierTransaction(int idope);
    void supprimerTransaction(int idope);

    Ui::TransactionWidget *ui;
    OperationDao *m_operationDao = nullptr;
    CompteDao *m_compteDao = nullptr;
    CategorieDao *m_categorieDao = nullptr;
    CompteOperationService m_service;

    QHash<int, QString> m_nomsComptes;
    QHash<int, QString> m_nomsCategories;
    bool m_chargementFiltres = false;
    bool m_nePlusDemanderSuppression = false;
};

#endif // TRANSACTIONWIDGET_H
