#include "compte.h"

#include <QString>

Compte::Compte() : m_id(0), m_soldeInitial(0), m_ordre(0) {}

int Compte::id() const { return m_id; }
void Compte::setId(int id) { m_id = id; }

QString Compte::nom() const {
    return m_nom;
}
void Compte::setNom(const QString &nom){ m_nom = nom; }

QString Compte::type() const { return m_type; }
void Compte::setType(const QString &type) { m_type = type; }

double Compte::soldeInitial() const { return m_soldeInitial; }
void Compte::setSoldeInitial(double solde) { m_soldeInitial = solde; }

int Compte::ordre() const { return m_ordre; }
void Compte::setOrdre(int ordre) { m_ordre = ordre; }
