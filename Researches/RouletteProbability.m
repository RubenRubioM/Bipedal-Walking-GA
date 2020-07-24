% INPUT:  Número de elementos
% OUTPUT: Array con las probabilidades distribuidas de manera no lineal
% Descripción: Con este método conseguimos que los valores finales tengan
% una probabilidad mayor a los valores iniciales

% https://en.wikipedia.org/wiki/Fitness_proportionate_selection

function probabilities = RouletteProbability(nElements)

    sum_of_fitness = 10;
    previous_probability = 0.0;
    probabilities = zeros(nElements,1);
    for i=1:nElements
        previous_probability = previous_probability + (i/sum_of_fitness);
        probabilities(i,1) = previous_probability;
    end
    
    probabilities = probabilities / previous_probability;
end