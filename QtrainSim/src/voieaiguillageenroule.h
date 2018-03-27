#ifndef VOIEAIGUILLAGEENROULE_H
#define VOIEAIGUILLAGEENROULE_H

#include <voievariable.h>
#include <loco.h>

class VoieAiguillageEnroule : public VoieVariable
{
    Q_OBJECT
public:
    explicit VoieAiguillageEnroule(qreal angle, qreal rayon, qreal longueur, qreal direction);
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
    qreal rayonInterieur, rayonExterieur, angle, longueur, direction;
    QPointF* centreInterieur;
    QPointF* centreExterieur;
    qreal lastDistDel;

};

#endif // VOIEAIGUILLAGEENROULE_H
