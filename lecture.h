#include "define.h"
#include <stdbool.h>
#include "fichiers.h"
#include <stdio.h>
#include "affichage.h"
#include "iir.h"
#include "fir.h"
#include "mesure.h"

#include "CDM/ftd2xx.h"

void lecusb();
absorp lecture(FILE* file_pf, int* file_state);
