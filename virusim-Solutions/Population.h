#ifndef __POPULATION_H
#define __POPULATION_H

#include "Random.h"

//
// Classe que representa uma popula��o contamin�vel.
//
class Population
{
   public:

      Population(int s);
      ~Population();

      struct PersonPosn
      {
            int i;
            int j;
      };
      struct PersonPosn centralPerson();


      int propagateUntilOut(PersonPosn start_person, double prob_spread, Random& r);
      double getPercentInfected();


   private:

      void reset();
      void exposePerson(PersonPosn p);
      void propagate(double prob_spread, Random& r);


      bool isPropagating();
      bool virusSpreads(double prob_spread, Random& r);

   private:

      // A popula��o � armazenada em uma matriz.
      // Cada elemento � um indiv�duo que pode estar em 4 estados.

      enum Person {
         Uninfected,    // pessoa n�o infectada
         Exposed,       // pessoa exposta
         Infected       // pessoa infectada
      };

      Person** pop;
      int size;

};

#endif
