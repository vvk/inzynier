//
// Created by vka on 26.04.15.
//


#ifndef INZYNIER_ALGORITHM_H
#define INZYNIER_ALGORITHM_H

#include "structures.h"

/*
 * glówna funkcja odpowiadająca za detekcję ilości osi pojazdu,
 * którego profil przekazany jest przy pomocy argumentu.
 *
 * Kolejność operacji:
 * 1) usunięcie offsetu ze wszystkich składowych poza czasem
 */
vehicle_class algorithm2(data_vector_t *);

/*
 * usuwa offset ze składowych wektora
 */
void remove_offset(data_vector_t *, unsigned);

/*
 * funkcja służąca do określenia parametrów ruchu pojazdu
 */
void find_velocity_distance(data_vector_t *, double *, double *);

/*
 * funkcja ogarniczająca zakres danych do istotnych
 */
void trim_data(data_vector_t *, double, double);

/*
 * funkcja pomocnicza dla trim_data, usuwa zbędne próbki danych dla zadanych parametrów
 */
void trim_values(data_vector_t *, data_field_t, unsigned, unsigned);

#endif //INZYNIER_ALGORITHM_H

