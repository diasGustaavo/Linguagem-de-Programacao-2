#include <iostream>
#include "random.hpp"

using Random = effolkronium::random_static;

int main(){
	std::cout << "gerando numero alatorio: " << Random(2.3, 5.7);
}	
