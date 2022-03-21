#include "affichage.h"

void affichage(oxy myOxy){
    if( access( ".verrouData", F_OK ) != -1 )
    {
        // Fichier existe, pas d'action à faire
    }else{
        // Fichier n'existe pas
        //On veut écrire nos valeurs spo2 et pouls
        //On crée le fichier verrou pour que l'interface java n'y ait pas acces pendant l'ecriture
        FILE *verrouData;
        //On va maintenant pouvoir ecrire dans Data car le verrou est créé
        FILE *Data = NULL;
        Data = fopen("Data.txt", "w+");
        int spo2=myOxy.spo2;
        int pouls=myOxy.pouls;
        fprintf(Data, "%d\n%d",spo2,pouls);

        //On ferme le fichier Data.txt
        fclose(Data);

        //On supprime le verrou pour permettre à l'interface de lire la valeur
        remove(".verrouData"); //On supprime le verrou à la fin de l'écriture.
    }
}

