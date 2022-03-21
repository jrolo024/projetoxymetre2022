#include "lecture.h"


void lecusb(){
    int numDevs=0;//Initialisation des variables : numDevs correspond au numero de la liaison USB
    absorp myAbsorp;    //Initialisation de la structure myAbsorp
    int i;      //i sert pour les boucles for
    testtab need;   //initialisation de la structure testtab pour mesure
    need.number=0;  //initialisation des composantes à 0
    need.numbers=0;
    for(i=0;i<4;i++){   //On initialise les valeurs du tableau pour le SPO2 à 0
        need.tabs[i]=0;
    }
    for(i=0;i<8;i++){   //On initialise les valeurs du tableau pour le poul à 0
        need.tabp[i]=0;
    }
    i=0;
    //Declaration variables pour FIR
    float FIR_TAPS[51]={1.4774946e-019,1.6465231e-004,3.8503956e-004,7.0848037e-004,1.1840522e-003,1.8598621e-003,2.7802151e-003,3.9828263e-003,5.4962252e-003,7.3374938e-003,9.5104679e-003,1.2004510e-002,1.4793934e-002,1.7838135e-002,2.1082435e-002,2.4459630e-002,2.7892178e-002,3.1294938e-002,3.4578348e-002,3.7651889e-002,4.0427695e-002,4.2824111e-002,4.4769071e-002,4.6203098e-002,4.7081811e-002,4.7377805e-002,4.7081811e-002,4.6203098e-002,4.4769071e-002,4.2824111e-002,4.0427695e-002,3.7651889e-002,3.4578348e-002,3.1294938e-002,2.7892178e-002,2.4459630e-002,2.1082435e-002,1.7838135e-002,1.4793934e-002,1.2004510e-002,9.5104679e-003,7.3374938e-003,5.4962252e-003,3.9828263e-003,2.7802151e-003,1.8598621e-003,1.1840522e-003,7.0848037e-004,3.8503956e-004,1.6465231e-004,1.4774946e-019}; //tableau des valeurs des coefficients fir
    int ope=0; //ope pour le tableau qui sert au calcul du FIR
    absorp valinit[51]; //stockage valeurs filename
    //Declaration variables pour iir
    absorp entreeOld; //Structure de l'ancienne valeur d'entrée pour IIR
    absorp entreeNew;//Structure de la nouvelle valeur d'entrée pour IIR
    //initialisation de toutes les composantes car elles sont necessaires au calcul de la premiere sortie
    entreeOld.acr=0;entreeOld.acir=0;entreeOld.dcr=0;entreeOld.dcir=0;
    entreeNew.acr=0;entreeNew.acir=0;entreeNew.dcr=0;entreeNew.dcir=0;
    //Structure de l'ancienne valeur de sortie
    absorp sortieOld;
    //Structure de la nouvelle valeur de sortie
    absorp sortieNew;
    //initialisation de toutes les composantes car elles sont necessaires au calcul de la premiere sortie
    sortieNew.acr=0;sortieNew.acir=0;sortieNew.dcr=0;sortieNew.dcir=0;
    sortieOld.acr=0;sortieOld.acir=0;sortieOld.dcr=0;sortieOld.dcir=0;
    //Declaration variables pour mesure
    oxy myOxy;//Declaration structure Oxy pour mesure
    myOxy.pouls=0;myOxy.spo2=0;//Initialisation de la structure Oxy avec ces composantes à 0
    absorp val;//Initialisation structure val
    val.acr=0;val.acir=0;val.dcr=0;val.dcir=0;//Initialisation composante à 0
    int k=0;//variable pour incrementation pour la mesure du poul
    //Calcul pour FIR
    for (ope=0;ope<51;ope++){  //on initie toutes les valeurs à 0
        valinit[ope].acr=0;valinit[ope].dcr=0;valinit[ope].acir=0;valinit[ope].dcir=0;
    }
    FT_HANDLE ftHandle;FT_STATUS state; //statut
    DWORD nbOctets = 21;DWORD octetsRecus; // nbOctets : nombre d'octet attendu pour la lecture, octetsRecus : nombre d'octet recu lors de la lecture
    char tabRecu[21];//tableau de char qui contient les valeurs des octets recus
    state = FT_Open(numDevs, &ftHandle); //On ouvre le port pour la lecture de l'USB
    while(state == FT_OK) {//tant que le statut n'indique pas d'erreur
        FT_SetTimeouts(ftHandle, 300, 0);//on met le timeout à 300 milliseconds car le pc a du mal si on met moins
        state = FT_Read(ftHandle, tabRecu, nbOctets, &octetsRecus);//on lit les octets recu
        if (state == FT_OK && octetsRecus == nbOctets && tabRecu[4] == ',' && tabRecu[9] == ',' && tabRecu[14] == ',' &&
            tabRecu[19] == 0x0A && tabRecu[20] == 0x0D) {// on fait les verifications pour s'assurer que la trame est bonne (, bien placée et fin de la trame respectée
            myAbsorp.acr = ((int) (tabRecu[0]) - 48) * 1000 + ((int) (tabRecu[1]) - 48) * 100 +
                           ((int) (tabRecu[2]) - 48) * 10 + ((int) (tabRecu[3]) - 48);//on fait -48 pour passer d'un char en int (ascii), les multiplications permettent de placer les chiffres au bon endroit et former le bon nombre
            myAbsorp.dcr = ((int) (tabRecu[5]) - 48) * 1000 + ((int) (tabRecu[6]) - 48) * 100 +
                           ((int) (tabRecu[7]) - 48) * 10 + ((int) (tabRecu[8]) - 48);//on fait -48 pour passer d'un char en int (ascii), les multiplications permettent de placer les chiffres au bon endroit et former le bon nombre
            myAbsorp.acir = ((int) (tabRecu[10]) - 48) * 1000 + ((int) (tabRecu[11]) - 48) * 100 +
                            ((int) (tabRecu[12]) - 48) * 10 + ((int) (tabRecu[13]) - 48);//on fait -48 pour passer d'un char en int (ascii), les multiplications permettent de placer les chiffres au bon endroit et former le bon nombre
            myAbsorp.dcir = ((int) (tabRecu[15]) - 48) * 1000 + ((int) (tabRecu[16]) - 48) * 100 +
                            ((int) (tabRecu[17]) - 48) * 10 + ((int) (tabRecu[18]) - 48);//on fait -48 pour passer d'un char en int (ascii), les multiplications permettent de placer les chiffres au bon endroit et former le bon nombre
            //FIR
            if (0 <= myAbsorp.acr && myAbsorp.acr < 4096 && 0 <= myAbsorp.dcr && myAbsorp.dcr < 4096 &&
                0 <= myAbsorp.acir && myAbsorp.acir < 4096 && 0 <= myAbsorp.dcir && myAbsorp.dcir < 4096) {//On verifie que les valeurs dans les composantes sont bien comprises entre 0 et 4096
                myAbsorp.acr-=2048;//on recentre la valeur de acr en 0
                myAbsorp.acir-=2048;//on recentre la valeur de acir en 0
//FIR
                myAbsorp = FIR(myAbsorp, FIR_TAPS, valinit);//On applique le filtre FIR qui modifie les composantes
                i++;    //on incremente i
//IIR
//lecture d'entreeNew qui est la nouvelle entree
                entreeNew = myAbsorp;
//calcul de la sortie par rapport aux entrees et � la derniere sortie
                sortieNew = iir(entreeOld, entreeNew, sortieOld);
//Nous allons avoir une nouvelle entree donc l'ancienne entree prend la valeur de la nouvelle entree
                entreeOld = entreeNew;
//l'ancienne sortie prend la valeur de la nouvelle
                sortieOld = sortieNew;
                if ((sortieNew.acr > 0 && val.acr <= 0) || (sortieNew.acr < 0 && val.acr >= 0)) {//calcul du poul en front montant et descendant pour obtenir la valeur d'une demi période et la stocker dans un tableau de 8 afin de faire la moyenne de 4 périodes totales à la fin de la fonction mesure
                    if (k > 6) {
                        if (need.number == 8) {
                            need.number = 0;//on remet à 0 quand on dépasse la taille maximale du tableau
                        }
                        need.tabp[need.number] = 15000 / k;//1500=(500*60)/2 pour valeur d'une demi période
                        need.number++;
                    }k = 0;//on incrémente la position dans le tableau
                }
                myOxy = mesure(sortieNew, val, need);//on utilise la fonction mesure pour recevoir le myOxy et la valeur du poul et de SPO2
                k++;//On ncremente k
                val = sortieNew; //les composantes de val prennent les valeurs de sortieNew
                printf("%d) acr:%f dcr:%f acir:%f dcir:%f\npouls:%d spo2:%d\n\n", i, sortieNew.acr, sortieNew.dcr,
                       sortieNew.acir, sortieNew.dcir, myOxy.pouls, myOxy.spo2);
                affichage(myOxy);//AFFICHAGE les valeurs de myOxy sont ecrites dans Data
            }}}FT_Close(ftHandle);//On ferme la connexion
}

absorp lecture(FILE* file_pf, int* file_state){
    //Initialisation de la structure de sortie
    absorp myAbsorp;
    //On initialise les valeurs à 0 pour entrer dans le while et eviter toutes valeurs aleatoires
    myAbsorp.dcir = 0;
    myAbsorp.acir = 0;
    myAbsorp.dcr = 0;
    myAbsorp.acr = 0;

    //tant que nos valeurs lues dans le documents ne sont pas égale à 0 (donc si on a bien une tram d'info complete)
    //et qu'on est pas à la fin du fichier
    while(((myAbsorp.acr == 0 || myAbsorp.dcr == 0) || (myAbsorp.acir ==0 || myAbsorp.dcir ==0)) && *file_state != EOF)
    {
        myAbsorp = lireFichier(file_pf, file_state);//on lit le fichier d'entrée et on rentre les valeurs dans la structure myAbsorp
        if((myAbsorp.acr !=0 && myAbsorp.dcr !=0) && (myAbsorp.acir !=0 && myAbsorp.dcir !=0)) { //si les valeurs de myAbsorp sont différentes de 0 alors c'est correct
            //Il faut recentrer les composantes acr et acir en 0, actuellement elles sont centrées en 2048 donc il suffit de faire -2048
            myAbsorp.acr -= 2048;
            myAbsorp.acir -= 2048;
        }
    }
    //On retourne le myAbsorp qui a été créé avec les valeurs du fichiers et le recentrage des composantes
    return myAbsorp;
}


