#ifndef VOIEAIGUILLAGETRIPLE_H
#define VOIEAIGUILLAGETRIPLE_H

#include <voievariable.h>
#include <loco.h>

class VoieAiguillageTriple : public VoieVariable
{
public:
    VoieAiguillageTriple(qreal angle, qreal rayon, qreal longueur);
    void setNumVoieVariable(int numVoieVariable);
    void calculerAnglesEtCoordonnees(Voie *v);
    void calculerPositionContact();
    QList<QList<Voie*>*> explorationContactAContact(Voie* voieAppelante);
    qreal getLongueurAParcourir();
    Voie* getVoieSuivante(Voie* voieArrivee);
    void avanceLoco(qreal &dist, qreal &angle, qreal &rayon, qreal angleCumule, QPointF posActuelle, Voie *voieSuivante);
    void correctionPosition(qreal deltaX, qreal deltaY, Voie *v);
    void correctionPositionLoco(qreal &, qreal &);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);

    virtual void mousePressEvent (QGraphicsSceneMouseEvent *);
private:
    qreal rayonGauche, rayonDroite, angle, longueur;
    QPointF* centreGauche;
    QPointF* centreDroite;
    qreal lastDistDel;
};

#endif // VOIEAIGUILLAGETRIPLE_H
