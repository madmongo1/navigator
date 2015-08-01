#include "bonus_tiempo.h"
#include "../../recursos.h"

using namespace App_Juego_ObjetoJuego;
using namespace App_Graficos;
using namespace App;

const float Bonus_tiempo::CANTIDAD_TIEMPO_DEFECTO=10.0f;

Bonus_tiempo::Bonus_tiempo():
	Actor(),
	tiempo(CANTIDAD_TIEMPO_DEFECTO)
{

}

Bonus_tiempo::Bonus_tiempo(float x, float y, float t):
	Actor(x, y, W, H),
	tiempo(t)
{

}

unsigned short int Bonus_tiempo::obtener_profundidad_ordenacion()const
{
	//TODO.
	return 10;
}

void Bonus_tiempo::transformar_bloque(App_Graficos::Bloque_transformacion_representable &b)const
{
	//TODO...
	//Se asume que todos los frames van mirando a la derecha.
	b.establecer_tipo(Bloque_transformacion_representable::tipos::TR_BITMAP);
	b.establecer_alpha(255);
	b.establecer_recurso(Recursos_graficos::RT_DEFECTO);
	b.establecer_recorte(32, 0, 27, 16);
	b.establecer_posicion(acc_espaciable_x()-1, acc_espaciable_y()-1, 27, 16);
}