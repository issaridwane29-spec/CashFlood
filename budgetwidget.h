#ifndef BUDGETWIDGET_H
#define BUDGETWIDGET_H

#include <QMap>
#include <QString>
#include <QWidget>
#include "budget.h"
#include "budgetdao.h"
#include "budgetservice.h"
#include "categoriedao.h"

namespace Ui {
class BudgetWidget;
}

class BudgetWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BudgetWidget(QWidget *parent = nullptr);
    ~BudgetWidget();

    void setDaos(BudgetDao *budgetDao, CategorieDao *categorieDao);

signals:
    void budgetsModifies();

public slots:
    void rafraichirBudgets();

private slots:
    void onNouveauBudget();
    void onModifierBudget();
    void onSupprimerBudget();
    void onFiltreChange(int index);

private:
    int idBudgetSelectionne(int *ligneCourante = nullptr) const;
    QString nomsCategoriesPourBudget(const Budget &budget, const QMap<int, QString> &mapNoms) const;

    Ui::BudgetWidget *ui;
    BudgetDao *m_budgetDao = nullptr;
    CategorieDao *m_categorieDao = nullptr;
    BudgetService m_budgetService;
};

#endif // BUDGETWIDGET_H
