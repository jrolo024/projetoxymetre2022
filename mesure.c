#include "mesure.h"
oxy mesureTest(char* filename){
    //déclaration variable
    oxy myOxy;
    absorp current,val;
    val.acr=0; //initialisation à 0 pour éviter valeurs random
    val.acir=0;
    val.dcr=0;
    val.dcir=0;
    testtab need;
    need.number=0;
    need.numbers=0;
    int i;
    //fin de la déclaration des variables
    for(i=0;i<8;i++){
        need.tabs[i]=0;//initialisation des tableaux
        need.tabp[i]=0;
    }
    //Ouverture du fichier filename
    FILE* projet = initFichier(filename);
    //state parametre de lirefichier pour connaitre l'etat d'avancement dans le fichier
    int state = 0;
    int k=0;
    int pouls=0;
    //Boucle qui s'arrete quand on est à la fin du fichier
    while(state != EOF){ //Boucle qui s'arrete quand on est à la fin du fichier
        current= lireFichier(projet, &state);
        if(state!=EOF) { //on revérifie sinon erreur pour la dernière ligne du fichier
            if(current.acr>0 && val.acr<=0){
                if(k>6){
                    if(need.number==8){
                        need.number=0; //remise à 0 du compteur si on dépasse la taille du tableau
                    }
                    need.tabp[need.number]=30000/k; //calcul du pouls et stockage dans un tableau
                    need.number++;
                }
                k=0;
            }
            myOxy = mesure(current,val,need);//appel de la fonction mesure
            k++;
            val=current;
        }
    }
    //On est à la fin du fichier donc on le ferme
    finFichier(projet);
    return myOxy;
}

oxy mesure(absorp myAbsorp, absorp val, testtab need){     //on cherche à mesurer le pouls et le spo2
    //déclaration var
    oxy oxyNew;
    float testpositifacr=0,testnegatifacr=0,testpositifacir=0,testnegatifacir=0;//valeurs qui vont nous permettent de calculer le ptp
    //fin déclaration var
    /*myAbsorp.acr=0;
    myAbsorp.acir=0;*/
    oxyNew.spo2=0;
    if ((myAbsorp.acr>val.acr)||(myAbsorp.acr<val.acr)) {//recherche des min/max acr et acir
        if (val.acr > testpositifacr) {
            testpositifacr = val.acr; //max acr
        }
        if (val.acr < testnegatifacr) {
            testnegatifacr = val.acr; //min acr
        }
        if (val.acir > testpositifacir) {
            testpositifacir = val.acir; //max acir
        }
        if (val.acir < testnegatifacir) {
            testnegatifacir = val.acir; //min acir
        }
    }
    if ((testpositifacir - testnegatifacir != 0) && (testpositifacr - testnegatifacr) != 0) {
            float rsir = ((testpositifacr - testnegatifacr) / myAbsorp.dcr) /
                         ((testpositifacir - testnegatifacir) / myAbsorp.dcir);//calcul de la valeur numérique du rsir
            /*printf("%f,%f\n",testpositifacr,testnegatifacr);
            printf("%f,%f\n",testpositifacir,testnegatifacir);*/
            if (need.numbers == 4) { need.numbers = 0; } //remise à 0 si on dépasse la taille du tableau
            if (rsir < 1 && rsir >= 0.4) { //affectation de valeur de spo2 suivant la valeur du rsir (courbe donnée par le prof)
                need.tabs[need.numbers] = (int) (110 - rsir * 25);
                need.numbers++;
            } else if (rsir >= 1 && rsir <= 3.4) {
                need.tabs[need.numbers] = (int) (120 - rsir * 32);
                need.numbers++;
            } else if (rsir < 0.4) {
                need.tabs[need.numbers] = 100;
                need.numbers++;
            }

        }

    int t;
    float poul=0,spo=0;
    need.count=0;need.countp=0;
    for (t=0;t<=3;t++){ //calcul de la somme des valeurs stockées de spo2 grâce au tableau de valeurs
        if(need.tabs[t]!=0){ //on n'ajoute pas certaines valeurs si elles valent 0
            spo+=need.tabs[t];
            need.count++;//counter du spo2 pour calcul de la moyenne à la fin
        }

    }
    for (t=0;t<=7;t++){ //calcul de la somme des valeurs stockées du pouls grâce au tableau de valeurs
        if(need.tabp[t]!=0){//on n'ajoute pas certaines valeurs si elles valent 0
            poul+=need.tabp[t];
            need.countp++;//counter du pouls pour calcul de la moyenne à la fin
        }
    }

    if(need.count!=0){
        spo=spo/need.count;//calcul de la moyenne de spo2
    }


    if(need.countp!=0){
        poul=poul/need.countp; //calcul de la moyenne du pouls
    }

    oxyNew.spo2=(int) spo;//affectation de la valeur finale
    oxyNew.pouls=(int) poul;//affectation de la valeur finale

    return oxyNew; //return du oxy modifié avec les nouvelles valeurs de pouls et de spo2
}
