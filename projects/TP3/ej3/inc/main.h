#include "sapi.h"


typedef enum{
   PULSADOR_ON,
   PULSADOR_OFF,
   PULSADOR_PUSLANDO,
   PULSADOR_LIBERANDO
} estado_pulsador_t;

typedef struct {
	gpioMap_t id_pulsador;
	estado_pulsador_t estado_pulsador;
	char * nombre;
}pulsador_t;

void iniciar_pulsador(pulsador_t *, gpioMap_t, char *);
void actualizar(pulsador_t *);
void pulsado(pulsador_t *);
void liberado(pulsador_t *);
void recibir(void *);
