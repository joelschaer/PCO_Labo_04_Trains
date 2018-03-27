#ifndef VOIEVARIABLE_H
#define VOIEVARIABLE_H

#include <voie.h>
#include <loco.h>

class VoieVariable : public Voie
{
    Q_OBJECT

public:
    VoieVariable();
    virtual void calculerAnglesEtCoordonnees(Voie *v=0)=0;
    virtual QList<QList<Voie*>*> explorationContactAContact(Voie* voieAppelante)=0;
    virtual void avanceLoco(qreal &dist, qreal &angle, qreal &rayon, qreal angleCumule, QPointF posActuelle, Voie *voieSuivante)=0;
    void setEtat(int nouvelEtat);
    /** permet d'indiquer à la voie variable quel est son numéro.
      * \param numVoieVariable le numéro de la voie variable.
      */
    virtual void setNumVoieVariable(int numVoieVariable)=0;
    virtual void correctionPosition(qreal deltaX, qreal deltaY, Voie* v)=0;
    virtual void correctionPositionLoco(qreal &x, qreal &y)=0;
signals:
    /** signale que la voie variable a été modifiée.
      * \param v la voie modifiée.
      */
    void etatModifie(Voie* v);
public slots:

protected:
    int etat;
    int numVoieVariable;
};

#endif // VOIEVARIABLE_H
