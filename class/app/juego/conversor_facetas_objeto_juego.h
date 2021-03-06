#ifndef CONVERSOR_FACETAS_OBJETO_JUEGO_H
#define CONVERSOR_FACETAS_OBJETO_JUEGO_H

#include <vector>
#include <memory>
#include "../interfaces/objeto_juego_i.h"
#include "../interfaces/borrable_i.h"
#include "../interfaces/bonus_i.h"
#include "../interfaces/con_turno_i.h"
#include "../graficos/representable.h"

namespace App_Juego
{

/**
* Esta solución tampoco sirve... Al intentar instanciar un template de Bonus_I,
* por ejemplo, te dirá que el método para Enemigo_basico no es compatible.
*/

/*
template <typename T> class VisTemplate:public App_Visitantes::Visitante_objeto_juego
{
	private:
	typedef std::vector<T *> t_res;
	t_res& v;

	public:
	VisTemplate(t_res& r):v(r) {}
	virtual void visitar(App_Juego_ObjetoJuego::Bonus_tiempo& b) {v.push_back(&b);}
	virtual void visitar(App_Juego_ObjetoJuego::Bonus_salud& b) {v.push_back(&b);}
	virtual void visitar(App_Juego_ObjetoJuego::Enemigo_basico& b) {v.push_back(&b);}
};
*/

class Conversor_facetas_objeto_juego
{
	/////////////////////////////////////
	//Interface publica:
	public:

	//TODO: Quizás separar en dos métodos, uno que devuelva y otro 
	static void	extraer_representables(const std::vector<std::shared_ptr<App_Interfaces::Objeto_juego_I> >, 	std::vector<const App_Graficos::Representable *>&);
	static void	extraer_borrables(const std::vector<std::shared_ptr<App_Interfaces::Objeto_juego_I> >, 		std::vector<App_Interfaces::Borrable_I *>&);
	static void	extraer_bonus(const std::vector<std::shared_ptr<App_Interfaces::Objeto_juego_I> >, 		std::vector<App_Interfaces::Bonus_I *>&);
	static void	extraer_con_turno(const std::vector<std::shared_ptr<App_Interfaces::Objeto_juego_I> >, 		std::vector<App_Interfaces::Con_turno_I *>&);

	/////////////////////////////////////
	//Privados:
	private:
			Conversor_facetas_objeto_juego();
};

}

#endif
