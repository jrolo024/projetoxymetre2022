#include "integration.h"

void integrationTest(char* filename)
{
    //Declaration variables generales
    //Ouverture du fichier filename
    FILE* projet = initFichier(filename);
    //state parametre de lirefichier pour connaitre l'etat d'avancement dans le fichier
    int state = 0;
    absorp myAbsorp;
    int i;
    testtab need;//valeur qui permet de faire les mesures dans la fonction mesure
    need.number=0;
    need.numbers=0;
    for(i=0;i<4;i++){
        need.tabs[i]=0; //initialisation des valeurs du tableau du spo2
    }
    for(i=0;i<8;i++){
        need.tabp[i]=0; //initialisation des valeurs du tableau du pouls
    }
    i=0;
    //Declaration variables pour FIR
    float FIR_TAPS[51]={1.4774946e-019,1.6465231e-004,3.8503956e-004,7.0848037e-004,1.1840522e-003,1.8598621e-003,2.7802151e-003,3.9828263e-003,5.4962252e-003,7.3374938e-003,9.5104679e-003,1.2004510e-002,1.4793934e-002,1.7838135e-002,2.1082435e-002,2.4459630e-002,2.7892178e-002,3.1294938e-002,3.4578348e-002,3.7651889e-002,4.0427695e-002,4.2824111e-002,4.4769071e-002,4.6203098e-002,4.7081811e-002,4.7377805e-002,4.7081811e-002,4.6203098e-002,4.4769071e-002,4.2824111e-002,4.0427695e-002,3.7651889e-002,3.4578348e-002,3.1294938e-002,2.7892178e-002,2.4459630e-002,2.1082435e-002,1.7838135e-002,1.4793934e-002,1.2004510e-002,9.5104679e-003,7.3374938e-003,5.4962252e-003,3.9828263e-003,2.7802151e-003,1.8598621e-003,1.1840522e-003,7.0848037e-004,3.8503956e-004,1.6465231e-004,1.4774946e-019}; //tableau des valeurs des coefficients fir
    int ope=0;
    absorp valinit[51]; //stockage valeurs filename
    //Declaration variables pour iir
    //Structure de l'ancienne valeur d'entrée
    absorp entreeOld;
    //Structure de la nouvelle valeur d'entrée
    absorp entreeNew;
    //initialisation de toutes les valeurs car elles sont necessaires au calcul de la premiere sortie
    entreeOld.acr=0;
    entreeOld.acir=0;
    entreeOld.dcr=0;
    entreeOld.dcir=0;
    entreeNew.acr=0;
    entreeNew.acir=0;
    entreeNew.dcr=0;
    entreeNew.dcir=0;
    //Structure de l'ancienne valeur de sortie
    absorp sortieOld;
    //Structure de la nouvelle valeur de sortie
    absorp sortieNew;
    //initialisation de toutes les valeurs car elles sont necessaires au calcul de la premiere sortie
    sortieNew.acr=0;
    sortieNew.acir=0;
    sortieNew.dcr=0;
    sortieNew.dcir=0;
    sortieOld.acr=0;
    sortieOld.acir=0;
    sortieOld.dcr=0;
    sortieOld.dcir=0;
    //Declaration variables pour mesure
    oxy myOxy;
    myOxy.pouls=0;
    myOxy.spo2=0;
    absorp val;
    val.acr=0;
    val.acir=0;
    val.dcr=0;
    val.dcir=0;
    int k=0;
    //fin de la zone de déclarations des variables
    //Calcul pour FIR
    for (ope=0;ope<51;ope++){  //on initie toutes les valeurs à 0
        valinit[ope].acr=0;
        valinit[ope].dcr=0;
        valinit[ope].acir=0;
        valinit[ope].dcir=0;
    }
    //Boucle qui s'arrete quand on est à la fin du fichier
    while(state != EOF){
        myAbsorp = lireFichier(projet,&state);
        if(state!=EOF) {
            //FIR
            myAbsorp = FIR(myAbsorp, FIR_TAPS, valinit);
            i++;
            //printf("%d) acr:%f dcr:%f acir:%f dcir:%f\npouls:%d spo2:%d\n",i,myAbsorp.acr,myAbsorp.dcr,myAbsorp.acir,myAbsorp.dcir,myOxy.pouls,myOxy.spo2);
            //IIR
            //lecture d'entreeNew qui est la nouvelle entree
            entreeNew = myAbsorp;
            //calcul de la sortie par rapport aux entrees et à la derniere sortie
            sortieNew = iir(entreeOld, entreeNew, sortieOld);
            //Nous allons avoir une nouvelle entree donc l'ancienne entree prend la valeur de la nouvelle entree
            entreeOld = entreeNew;
            //l'ancienne sortie prend la valeur de la nouvelle
            sortieOld = sortieNew;
            //printf("%d) acr:%f dcr:%f acir:%f dcir:%f\n",i,sortieNew.acr,sortieNew.dcr,sortieNew.acir,sortieNew.dcir);

            //MESURE
            if ((sortieNew.acr > 0 && val.acr <= 0)||(sortieNew.acr < 0 && val.acr >= 0)) { //calcul du pouls en front montant et en front descendant -> calcul par demi période quand on passe par 0
                if(k>6){ //pour éviter erreur des premières valeurs
                    if(need.number==8){ //on remet à 0 si on dépasse la taille du tableau
                        need.number=0;
                    }
                    need.tabp[need.number]=15000/k;//15000=(500*60)/2 car demi période
                    need.number++; //incrémentation de là où on est dans le tableau
                }
                k=0;
            }
            myOxy = mesure(sortieNew, val, need); //on appelle la fonction de mesure
            k++;
            val = sortieNew;//val=valeur de current qui a permit de faire le calcul de spo2 et pouls qu'on se sert pour comparer avec la prochaine valeur "actuelle"
            printf("%d) acr:%f dcr:%f acir:%f dcir:%f\npouls:%d spo2:%d\n\n",i,sortieNew.acr,sortieNew.dcr,sortieNew.acir,sortieNew.dcir,myOxy.pouls,myOxy.spo2);

            //AFFICHAGE
            affichage(myOxy);
        }
    }
    finFichier(projet); //fermeture du fichier
}