#include "operation.h"

Operation::Operation()
    : m_id(0), m_montant(0), m_idCompte(0), m_idCategorie(0) {}

Operation::Operation(int id, const QString &type, double montant,
                     const QDate &date, const QString &description,
                     int idCompte, int idCategorie)
    : m_id(id), m_type(type), m_montant(montant), m_date(date),
      m_description(description), m_idCompte(idCompte),
      m_idCategorie(idCategorie) {}

int Operation::id() const { return m_id; }
void Operation::setId(int id) { m_id = id; }

QString Operation::type() const { return m_type; }
void Operation::setType(const QString &type) { m_type = type; }

double Operation::montant() const { return m_montant; }
void Operation::setMontant(double montant) { m_montant =montant; }

QDate Operation::date() const { return m_date; }
void Operation::setDate(const QDate &date) { m_date = date; }

QString Operation::description() const { return m_description; }
void Operation::setDescription(const QString &description) { m_description = description; }

int Operation::idCompte() const { return m_idCompte; }
void Operation::setIdCompte(int idCompte) { m_idCompte = idCompte; }

int Operation::idCategorie() const { return m_idCategorie; }
void Operation::setIdCategorie(int idCategorie) { m_idCategorie = idCategorie; }
