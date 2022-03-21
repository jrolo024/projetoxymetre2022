#ifndef DEFINE_H
#define DEFINE_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

/*! ------------------------------------------------------------
  
  \struct absorp
 
  \brief measures
  
  ------------------------------------------------------------*/

typedef struct{
    float acr;  /*!< AC R */
    float dcr;  /*!< DC R */
    float acir;  /*!< AC IR */
    float dcir;  /*!< DC IR */
} absorp;

typedef struct{
    float tabp[8];  /*!< tableau qui stocke les valeurs du pouls pour faire une moyenne à la fin  */
    float tabs[4];  /*!< tableau qui stocke les valeurs du spo2 pour faire une moyenne à la fin */
    int number;  /*!< pour savoir où on est dans le tableau du pouls */
    int numbers; /*!< pour savoir où on est dans le tableau du spo2 */
    int count; /*!< pour moyenne du spo2 */
    int countp; /*!< pour moyenne du pouls */
}testtab;

/*! ------------------------------------------------------------

  \struct oxy

  \brief

  ------------------------------------------------------------*/

typedef struct{
    int spo2;  /*!< SPO2 */
    int pouls;  /*!< Pouls */
} oxy;
#endif // DEFINE_H
