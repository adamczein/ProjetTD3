#pragma once
#include <iostream>
#include <memory>
#include <cassert>
#include "gsl/span"
#include "cppitertools/range.hpp"

using namespace std;
using namespace gsl;
using namespace iter;

template <typename T>
class Liste
{
public:
	//TODO: Constructeurs et surcharges d'op�rateurs

	Liste() : nElements_(0), capacite_(0), elements_(make_unique<shared_ptr<T>[]>(capacite_)) {};

	Liste(Liste<T>& autre)
	{
		*this = autre;
	}

	~Liste() = default; 
	
	shared_ptr<T> operator[](unsigned int index) const { return elements_[index]; }

	Liste& operator= (const Liste<T>& autre)
	{
		if (this != &autre)
		{
			nElements_ = autre.nElements_;
			capacite_ = autre.capacite_;
			elements_ = make_unique<shared_ptr<T>[]>(capacite_);

			for (unsigned i : range(nElements_))
			{
				elements_[i] = autre[i];

			}
		}
		
		return *this;
	}

	//TODO: M�thode pour ajouter un �l�ment � la liste

	void ajouterElement(const shared_ptr<T> pointeurT);

	shared_ptr<T[]> getElements() const { return elements_; }

	span<shared_ptr<T>> enSpan() const { return { elements_.get(), nElements_ }; };

	void afficher(ostream& os) const
	{
		for (unsigned i : range(nElements_)) { os << elements_[i]; }
	}

	// Pour size, on utilise le m�me nom que les accesseurs de la biblioth�que standard, qui permet d'utiliser certaines fonctions de la bibliotheque sur cette classe.
	unsigned size() const         { return nElements_; }
	unsigned getCapacite() const  { return capacite_; }

	//TODO: M�thode pour changer la capacit� de la liste

	void changerCapacite(const unsigned nouvelleCapacite);

	//TODO: M�thode pour trouver une �l�ment selon un crit�re (lambda).

	template <typename PredicatUnaire>
	shared_ptr<T> trouverCritere(const PredicatUnaire& critere);

private:
	unsigned nElements_;
	unsigned capacite_;
	unique_ptr<shared_ptr<T>[]> elements_;
	//TODO: Attribut contenant les �l�ments de la liste.
};

template<typename T>

void Liste<T>::ajouterElement(const shared_ptr<T> pointeurT)
{
	if (nElements_ == capacite_)
		changerCapacite(max(unsigned(1), capacite_ * 2));
	elements_[nElements_++] = pointeurT;
}

template<typename T>

void Liste<T>::changerCapacite(const unsigned nouvelleCapacite)
{
	assert(nouvelleCapacite >= nElements_);
	unique_ptr<shared_ptr<T>[]> nouvelleListe = make_unique<shared_ptr<T>[]>(nouvelleCapacite);

	for (unsigned i : range(nElements_))
		nouvelleListe[i] = elements_[i];

	elements_ = move(nouvelleListe);
	capacite_ = nouvelleCapacite;
}

template <typename T>
template <typename PredicatUnaire>

shared_ptr<T> Liste<T>::trouverCritere(const PredicatUnaire& critere)
{
	int i = 0;
	for (auto&& v : enSpan())
	{
		if (critere(v))
			return v;
		i++;

	}

	return nullptr;
}