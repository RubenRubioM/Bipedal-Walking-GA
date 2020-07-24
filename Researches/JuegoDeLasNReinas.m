%% Juego de las N reinas

% Se basa en encontrar posicion para 8 reinas en un tablero donde ninguna
% se ataquen entre ellas, es decir, que ninguna reina compartan fila,
% columa ni diagonal.

% Dudas:
% 1)Los tramos de probabilidades en la seleccion por ruleta los he escogido
% con el algoritmo implementado en RouletteProbability. ¿Podría haber
% escogido cualquier sesgo para esas probabilidades siempre que se cumplan
% que los mejores tengan mayor probabilidad?
%
% 2)Los cruces entre cromosomas he escogido un crossover especializado
% donde cogiendo padres 2 a 2 voy buscando de 1:N alternando entre cada
% padre e insertando el valor encontrado en la posicion donde la tenia el
% padre. En caso de colisión lo inserto en la posicion mas cercana, EJ:
%       Padre1: 2, 4 ,1, 3
%       Padre2: 3, 2, 1, 4
%      ---------------------
%       It 1:   0, 0, 1, 0  -> Buscamos 1 en Padre1 = (Padre1[3])
%       It 2:   2, 0, 1, 0  -> Buscamos 2 en Padre2 = (Padre2[1])
%       It 3:   2, 0, 1, 3  -> Buscamos 3 en Padre3 = (Padre1[4])
%       It 4:   2, 4, 1, 3  -> Buscamos 4 en Padre4 = (Padre2[4]) -> Se inserta en (2) porque el 4 ya esta ocupado
%
% 

%% INPUT:
% nReinas: número de reinas
% NIND: número de individuos de cada generación
% MAXGEN: número máximo de generaciones
% procNewInd: porcentaje de nuevos individuos en la siguiente generación
% porcMutacion: porcentaje de mutación
% funSeleccion: operador de selección (Ruleta o Torneo)

%% OUTPUT:
% fitnessMedio: vector con el fitness medio de cada generación
% mejorFitness: vector con el mejor fitness de cada generación
% mejorIndividuo: matriz de contiene la representación del mejor individuo
%                 de cada generación

%% Representación:
% Se utiliza un vector con tantas posiciones como columnas tiene el
% tablero. Cada valor del vector indica la fila en la que se encuentra la
% reina en esa columna. No se permiten valores repetidos en el vector.

%% Función principal
function [fitnessMedio, mejorFitness, mejorIndividuo] = JuegoDeLasNReinas(nReinas, NIND, MAXGEN, ...
                                                        porcNewInd, pMutacion, funSeleccion)

    valorObjetivo = sum(1:nReinas-1);
    fprintf('Valor objetivo: %d\n', valorObjetivo);
    
    %Poblacion inicial
    poblacion = GeneraPoblacionInicial(NIND,nReinas,'GeneraPosicion');
    objV = EvaluaPoblacion(poblacion,'EvaluaPosicion');
    
    %Para el gráfico
    close;
    grid;
    hold on;
    axis([1 MAXGEN 0 valorObjetivo*2]);
    
    %Inicializaciones
    fitnessMedio = zeros(MAXGEN,1);
    mejorFitness = ones(1,1);
    mejorIndividuo = zeros(MAXGEN,nReinas);
    gen = 0;
    
    while (gen < MAXGEN && min(mejorFitness)~=0)
        poblacion = FormaNuevaPoblacion(poblacion,objV,porcNewInd,funSeleccion,'CruzaPosicion','MutaPosicion',pMutacion);
        objV = EvaluaPoblacion(poblacion,'EvaluaPosition');
        %Almacenar media del fitness y mejor individuo
        % TODO: Debugear esto porque creo que es min en vez de max
        mejor = find(objV == min(objV));
        mejorFitness(gen+1) = objV(mejor(1));
        mejorIndividuo(gen+1,:) = poblacion(mejor(1),:);
        fitnessMedio(gen+1) = mean(objV);
        
        %Incrementar el numero de generaciones
        gen = gen+1;
        
        %Actualizar gráfico
        plot(mejorFitness,'-b');
        plot(fitnessMedio,'-r');
        xlabel('Número de generaciones');
        ylabel('Número medio de errores');
        if gen==1
            %legend('Mejor Fitness','Media de la población');
        end
        drawnow;
    end
    disp("La solucion para el problema de " + nReinas+ " reinas es: ");
    poblacion(mejor(1),:)
    hold off
end


%% Funcion para generar población inicial
function poblacion = GeneraPoblacionInicial(NIND,nReinas,modo)
    %Cada fila de valores representa los cromosomas de una posible solucion
    %y cada valor es la posicion de la reina en la columna i
    poblacion = zeros(NIND,nReinas);
    %Rellenamos con valores aleatorios sin repeticiones
    for i = 1:NIND
        poblacion(i,:) = randperm(nReinas);
    end
end

%% Calcula el fitness de los individuos de la población
%  Es decir, de cada fila cuenta los posibles ataques entre las reinas
function objV = EvaluaPoblacion(poblacion,modo)
    %Rellenamos de ceros una matriz (cromosomas,1)
    objV = zeros(numel(poblacion(:,1)),1);
    %Recorremos toda la poblacion fila a fila calculando el fitness de cada
    %individuo
    for i = 1:numel(poblacion(:,1))
        for j = 1:numel(poblacion(1,:))
            for k = 1:numel(poblacion(1,:))
                % abs(poblacion(i,j)-poblacion(i,k)) == abs(j-k) then
                % comparten diagonal
                if abs(poblacion(i,j)-poblacion(i,k)) == abs(j-k) && j~=k
                    objV(i,1) = objV(i,1) + 1;                 
                end
            end
        end
    end
    

end

%% Generamos una nueva población a partir de la anterior
function nuevaPoblacion = FormaNuevaPoblacion(poblacion,objV,porcNewInd,funSeleccion,modo,modo2,pMutacion)
    %Calculamos el número de nuevo individuos que van a haber
    [NIND,nReinas] = size(poblacion);
    nuevosIndividuos = round(NIND * porcNewInd);
    maximoValor = max(poblacion(1,:));
    padre1Indx = 1;
    padre2Indx = 1;
    nuevaPoblacion = zeros(NIND,nReinas);

    % Hacemos el crossover entre los individuos
    for n=1:nuevosIndividuos
        padre1Indx = randi(NIND);
        padre2Indx = randi(NIND);
        while padre1Indx == padre2Indx
            padre1Indx = randi(NIND);
            padre2Indx = randi(NIND);
        end
        
        nuevoIndividuo = CrossOverIndividual(poblacion(padre1Indx,:),poblacion(padre2Indx,:),maximoValor);
        nuevaPoblacion(n,:) = nuevoIndividuo;
    end
    
    %Finalmente añadimos los progenitores que si sobreviven a la selección
    for i=nuevosIndividuos:NIND
            
        %Ahora realizamos la seleccion mediante 'Ruleta' o 'Torneo'
        if strcmp(funSeleccion,'Ruleta')
            %Cuanto menos optimo sea el cromosoma menos probabilidad le
            %asignamos de cara a la seleccion.
            valores = flip(unique(objV)); %Los invertimos para que el mas importante es el menor
            %El numero de rangos serán los diferentes valores de objV
            probabilidades = RouletteProbability(numel(valores));
            n = rand();
            %Vemos que tramo de porcentajes ha tocado
            indx = 1; % Que tramo de probabilidades ha salido
            for j=1:numel(probabilidades)
                if j == 1
                    if n>=0 && n<= probabilidades(j)
                        indx = j;
                    end
                elseif n>=probabilidades(j-1) && n<=probabilidades(j)
                    indx = j;
                end
            end
            % Ahora buscar un elemento en la población que tenga el valor de valores(indx)
            posicionCromosoma = find(objV==valores(indx),1,'First');
            nuevaPoblacion(i,:) = poblacion(posicionCromosoma,:);
            poblacion(posicionCromosoma,:) = []; %Eliminamos la fila ya insertada
            objV(posicionCromosoma) = [];
            
            

        elseif strcmp(funSeleccion,'Torneo')

        end
        
    end % for i=1:nIndividuos
    
    
    % Mutacion de la nueva poblacion
    for i=1:numel(poblacion(:,1))
        nuevaPoblacion(i,:) = Mutacion(nuevaPoblacion(i,:),pMutacion);
    end

end

% Hace el crossover con dos padres y devuelve el hijo resultante
% El mecanismo es buscar de 1:maxValor la posicion de esos valores
% alternando entre el primer y el segundo padre. En caso de colisión se
% insertará en el valor 0 mas cercano.
% INPUT: 
% padre1: El primer padre
% padre2: El segundo padre
% maxValor: Valor maximo del cromosoma
function hijo = CrossOverIndividual(padre1,padre2,maxValor)
    hijo = zeros(1,size(padre1,2));
    
    for i=1:maxValor
        if mod(i,2) == 0
            pos = find(padre1 == i);
            if hijo(pos) == 0
                hijo(pos) = i;
            else
                pos = find(hijo == 0,1, 'First');
                hijo(pos) = i;
            end
        else % mod(i,2) ~= 0
            pos = find(padre2 == i);
            if hijo(pos) == 0
                hijo(pos) = i;
            else
                pos = find(hijo == 0,1, 'First');
                hijo(pos) = i;
            end
        end 
    end
end

% Mutamos los cromosomas
function nuevoIndividuo = Mutacion(individuo,pMutacion)
    nuevoIndividuo = individuo;
    n = rand();
    valoresParaDesplazar = randi(max(individuo));
    if n<pMutacion
        % la mutacion es hacer un shift aleatorio a los valores del cromosoma
        if valoresParaDesplazar > 0 
            nuevoIndividuo = circshift(individuo,valoresParaDesplazar);
        end
    end
end

%{ 
%DEPRECATED pero por si acaso no la borro que puede ser útil
% Hace el crossover a la poblacion
function nuevaPoblacion = CrossOver(poblacion)
    [R,C] = size(poblacion);
    nuevaPoblacion = zeros(R,C);
    hijo1 = zeros(1,C);
    hijo2 = zeros(1,C);
    maximoValor = max(poblacion(1,:));
    
    for i=1:2:R
        % El if por si es impar el numero de pares de padres pues ese
        % elemento sobrante no se junta con ningun otro
        if i==R
            if i~=1
                hijo1 = CrossOverIndividual(poblacion(i,:),poblacion(i-1,:),maximoValor);
                nuevaPoblacion(i,:) = hijo1;
            else %Solo queda 1 individuo en la poblacion
                hijo1 = poblacion(i,:);
            end
            
        elseif i~=R
            hijo1 = CrossOverIndividual(poblacion(i,:),poblacion(i+1,:),maximoValor);
            hijo2 = CrossOverIndividual(poblacion(i+1,:),poblacion(i,:),maximoValor);
            nuevaPoblacion(i,:) = hijo1;
            nuevaPoblacion(i+1,:) = hijo2;
        end
        
    end
    
end
%}
