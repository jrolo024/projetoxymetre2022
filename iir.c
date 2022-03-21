#include "iir.h"


absorp iirTest(char* filename){
    //Initialisation des structures
    //Structure de la sortie de IIR actuelle
	absorp	myAbsorp;
    //initialisation de toutes les valeurs car elles sont necessaires au calcul de la premiere sortie
    myAbsorp.acr=0;
    myAbsorp.acir=0;
    myAbsorp.dcr=0;
    myAbsorp.dcir=0;
    //Structure de l'ancienne valeur d'entrée
    absorp entreeOld;
    //Structure de la nouvelle valeur d'entrée
    absorp entreeNew;
    //initialisation de toutes les valeurs car elles sont necessaires au calcul de la premiere sortie
    entreeNew.acr=0;
    entreeNew.acir=0;
    entreeNew.dcr=0;
    entreeNew.dcir=0;
    //Structure de l'ancienne valeur de sortie
    absorp sortieOld;

    //Ouverture du fichier filename
    FILE* projet = initFichier(filename);
    //state parametre de lirefichier pour connaitre l'etat d'avancement dans le fichier
    int state = 0;
    //Boucle qui s'arrete quand on est à la fin du fichier

    while(state != EOF){
        //Nous allons avoir une nouvelle entree donc l'ancienne entree prend la valeur de la nouvelle entree
        entreeOld=entreeNew;
        //l'ancienne sortie prend la valeur de la nouvelle
        sortieOld=myAbsorp;
        //lecture d'entreeNew qui est la nouvelle entree
        entreeNew = lireFichier(projet, &state);
        if(state!=EOF) {
            //calcul de la sortie par rapport aux entrees et à la derniere sortie
            myAbsorp = iir(entreeOld, entreeNew, sortieOld);
        }
    }
    //On est à la fin du fichier donc on le ferme
    finFichier(projet);

	return myAbsorp;
}

//Filtrage IIR
absorp iir(absorp entreeOld, absorp entreeNew, absorp sortieOld){
    //a est le coefficient alpha, on le prend égal à 0,992
    float a=0.992;

    //Création de la structure absorp qui sera renvoyé en sortie
    absorp sortieNew;

    //Les composantes DCIR et DCR sont identiques
    sortieNew.dcir=entreeNew.dcir;
    sortieNew.dcr=entreeNew.dcr;

    //Application du filtrage y(n)=x[n]-x[n-1]+a*y[n-1] avec y les structures de sortie et x celles d'entree
    sortieNew.acir=entreeNew.acir-entreeOld.acir+a*sortieOld.acir;
    sortieNew.acr=entreeNew.acr-entreeOld.acr+a*sortieOld.acr;

    //Renvoie de la structure absorp de sortie (modification des composantes AC par rapport à l'entree)
    return sortieNew;
}