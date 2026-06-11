#ifndef FUNCIONES_H_INCLUDED
#define FUNCIONES_H_INCLUDED

#include "config.h"

void Menu();
int leerConfig(tConfiguracion* config, const char* arch);
int tirarDado();
void jugarPartida();
void ManejoErrores(int codError, const char *arch);
#endif // FUNCIONES_H_INCLUDED
