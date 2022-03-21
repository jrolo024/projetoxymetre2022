#include "fir.h"
#include "define.h"
#include "fichiers.h"

absorp firTest(char* filename){
    //zone de déclaration des variables
    int ope;
	absorp	myAbsorp;
    FILE* firfile=initFichier(filename);
    int state=0;
    absorp valinit[51]; //stockage valeurs filename
    absorp endfir; //fichier post traitement
    float FIR_TAPS[51]={1.4774946e-019,1.6465231e-004,3.8503956e-004,7.0848037e-004,1.1840522e-003,1.8598621e-003,2.7802151e-003,3.9828263e-003,5.4962252e-003,7.3374938e-003,9.5104679e-003,1.2004510e-002,1.4793934e-002,1.7838135e-002,2.1082435e-002,2.4459630e-002,2.7892178e-002,3.1294938e-002,3.4578348e-002,3.7651889e-002,4.0427695e-002,4.2824111e-002,4.4769071e-002,4.6203098e-002,4.7081811e-002,4.7377805e-002,4.7081811e-002,4.6203098e-002,4.4769071e-002,4.2824111e-002,4.0427695e-002,3.7651889e-002,3.4578348e-002,3.1294938e-002,2.7892178e-002,2.4459630e-002,2.1082435e-002,1.7838135e-002,1.4793934e-002,1.2004510e-002,9.5104679e-003,7.3374938e-003,5.4962252e-003,3.9828263e-003,2.7802151e-003,1.8598621e-003,1.1840522e-003,7.0848037e-004,3.8503956e-004,1.6465231e-004,1.4774946e-019}; //tableau des valeurs des coefficients fir
    //fin de zone de déclaration des variables
    for (ope=0;ope<51;ope++){  //on initie toutes les valeurs à 0
        valinit[ope].acr=0;
        valinit[ope].dcr=0;
        valinit[ope].acir=0;
        valinit[ope].dcir=0;
    }
    while(state!=EOF){
        myAbsorp= lireFichier(firfile,&state);  //lecture du fichier
        if(state!=EOF){
        endfir=FIR(myAbsorp,FIR_TAPS,valinit);  //traitement du signal
        }
    }
    finFichier(firfile); //fermeture du fichier

    return endfir;

}
absorp FIR(absorp myAbsorp,float* FIR_TAPS, absorp* valinit){
    //déclaration var
    int variable=0;
    //fin déclaration var
    while(variable<50){ //tableau circulaire de 50 valeurs
        valinit[variable]=valinit[variable+1]; // création du buffer
        if(variable==49){
            valinit[variable+1]=myAbsorp; //dernière valeurs qui vaut valeur actuelle
        }
        variable++; //incrémentation pour le while
    }
    myAbsorp.acr=0; //pour éviter erreurs avec valeurs random pour la somme
    myAbsorp.acir=0;
    for (variable=51;variable>0;variable--){
         //somme pour filtrage
         myAbsorp.acr+=FIR_TAPS[variable]*valinit[50-variable].acr;         //somme pour acr où on récupère valeur qui nous manque dans fir_tabs
         myAbsorp.acir+=FIR_TAPS[variable]*valinit[50-variable].acir;       //somme pour acir où on récupère valeur qui nous manque dans fir_tabs

    }

    return myAbsorp; //on return myAbsorp avec valeurs de acr et de acir modifiées
}