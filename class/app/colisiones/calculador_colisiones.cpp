#include "calculador_colisiones.h"

using namespace App_Colisiones;
using namespace App_Interfaces;
using namespace App_Niveles;
using namespace App_Juego;
using namespace App_Definiciones;

std::vector<const Celda *> Calculador_colisiones::celdas_en_caja(const Espaciable::t_caja& c, const Sala& sala) const
{
	auto calcular_indice_celda=[](unsigned int px, unsigned int py, unsigned int w_en_celdas)
	{
		return (py * w_en_celdas) + px;
	};

	auto mundo_a_indice_celdas=[](float val)
	{
		int v=floor(val);
		return floor(v / tipos::DIM_CELDA); //Si W y H tuvieran valores distintos sería un tema.
	};

	float punto_inicio_x=c.origen.x;
	float punto_fin_x=c.origen.x+c.w;
	float punto_inicio_y=c.origen.y;
	float punto_fin_y=c.origen.y+c.h;

	float punto_fin_rejilla_x=sala.acc_w()*tipos::DIM_CELDA;
	float punto_fin_rejilla_y=sala.acc_h()*tipos::DIM_CELDA;

	std::vector<const Celda *> resultado;

	/**
	* Lo primerísimo: si toda la caja está "fuera" de la rejilla por algún
	* lado no vamos a hacer nada. La forma más sencilla de comprobar esto
	* es ver que al menos alguno de los puntos queda cerca.
	**/

	if(! (punto_fin_x < 0.0 
		|| punto_fin_y < 0.0 
		|| punto_inicio_x > punto_fin_rejilla_x 
		|| punto_inicio_y > punto_fin_rejilla_y))
	{
		/*Acerca de esos ceil y -1... Imaginemos que el mundo es una rejilla
		de 16x16. El objeto del jugador mide 16 de ancho. Si está en la posición
		0 cabe justo en la columna 0, pero el código te dirá que también
		está dentro de la columna 1 puesto que 0+16 (x+w) son 16, que corresponde
		a la columna 1. El -1 se encarga de eso.
		El ceil... Cuando le hemos restado 1, 16 se queda en 15. Si intentamos
		avanzar un poco, 16.2 se queda en 15.2, que no hace colisión. Al darle
		un poco más de caña si que lo hace... Es una mierda, pero funciona.*/

		int ini_x=mundo_a_indice_celdas(punto_inicio_x);
		int fin_x=mundo_a_indice_celdas(ceil(punto_fin_x-1.0));
		int ini_y=0;
		int fin_y=mundo_a_indice_celdas(ceil(punto_fin_y-1.0));
		

		/* Las últimas comprobaciones serán ver que no se sale del "mundo".
		Alguno de los puntos está fuera del mundo (por ejemplo, se sale por la
		derecha) vamos a tener un problema al calcular el índice de la misma
		y nos va a dar una celda que probablemente esté en el otro extremo. 
		La idea es que si hay algo "fuera de la rejilla" no tenga colisiones.
		Como ya hemos comprobado que no esté fuera, lo que nos queda es ajustar
		los máximos y mínimos.
		*/

		int max_x=sala.acc_w()-1;
		int max_y=sala.acc_h()-1;

		if(ini_x < 0) ini_x=0;
		if(ini_y < 0) ini_y=0;
		if(fin_x > max_x) fin_x=max_x;
		if(fin_y > max_y) fin_y=max_y;

		auto& rejilla=sala.acc_matriz().acc_matriz();	//Si no hacemos esta referencia se va todo a la mierda XD!.
		auto w=sala.acc_w();

		do
		{
			ini_y=mundo_a_indice_celdas(punto_inicio_y);
			do
			{
				if(ini_x >= 0 && ini_y >= 0) 
				{			
					unsigned int indice=calcular_indice_celda(ini_x, ini_y, w);

					if(rejilla.count(indice)) 
					{
						resultado.push_back(& (rejilla.at(indice)));
					}
				}
				++ini_y;

			}while(ini_y <= fin_y);

			++ini_x;
		}while(ini_x <= fin_x);
	}

	return resultado;
}

void Calculador_colisiones::ajustar_colisiones_actor_movil_y_con_celdas(Actor_movil& actor, const std::vector<const Celda * >& celdas) const
{
	auto vy=actor.acc_vector_y();

	if(vy < 0.0)
	{
		int fy=-1;
		for(const Celda * e: celdas)
		{
			if(e->acc_espaciable_fy() >= fy)
			{
				fy=e->acc_espaciable_fy();
			}
		}
		if(fy != -1)
		{
			actor.ajustar(fy, Actor_movil::posiciones_ajuste::arriba);
		}
	}
	else if(vy > 0.0)
	{
		int fy=-1;
		for(const Celda * e: celdas)
		{
			if(e->acc_espaciable_y() >= fy)
			{
				fy=e->acc_espaciable_y();
			}
		}
		if(fy != -1)
		{
			actor.ajustar(fy, Actor_movil::posiciones_ajuste::abajo);
		}
	}
}

void Calculador_colisiones::ajustar_colisiones_actor_movil_x_con_celdas(Actor_movil& actor, const std::vector<const Celda * >& celdas) const
{
	auto vx=actor.acc_vector_x();

	if(vx < 0.0)
	{
		int fx=-1;
		for(const Celda * e: celdas)
		{
			if(e->acc_espaciable_fx() >= fx)
			{
				fx=e->acc_espaciable_fx();
			}
		}
		if(fx != -1)
		{
			actor.ajustar(fx, Actor_movil::posiciones_ajuste::izquierda);
		}
	}
	else if(vx > 0.0)
	{
		int fx=-1;
		for(const Celda * e: celdas)
		{
			if(e->acc_espaciable_x() >= fx)
			{
				fx=e->acc_espaciable_x();
			}
		}
		if(fx != -1)
		{
			actor.ajustar(fx, Actor_movil::posiciones_ajuste::derecha);
		}
	}
}

/**
* @return bool : Devuelve verdadero cuando no hay colisión entre las cajas.
* @param t_caja : Caja espaciable.
* @param Sala : Sala.
*
* Comprueba si el objeto está fuera de la sala. Devuelve true si es así.
*/

bool Calculador_colisiones::es_fuera_de_sala(const App_Interfaces::Espaciable::t_caja& caja, const App_Niveles::Sala& sala) const
{
	using namespace App_Definiciones;
	Espaciable::t_caja caja_sala(0, 0, sala.acc_w()*tipos::DIM_CELDA, sala.acc_h()*tipos::DIM_CELDA);

	return !caja_sala.es_en_colision_con(caja);
}
