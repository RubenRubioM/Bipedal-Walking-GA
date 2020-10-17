# LaTeX Template cheat sheet

## Apartados entre capitulos
    \section{nombre} 1.1 Nombre
    \subsection{nombre} 1.1.1 Nombre
    \subsubsection{nombre} 1.1.1.1 Nombre
    \paragraph{nombre} 1.1.1.1.1 Nombre

## Citar bibliografia
Importante añadirlos al archivo *bibliografia/bibliografia.bib*
    
    \citet{Nombre} - Cita estandar
    \citep{Nombre} - Cita entre parentesis
    \citep[ver][Cap. 1]{Nombre} - Citar la parte exacta
        \citep[Cap. 1]{Nombre}
        \citep[ver][]{Nombre}
    \citet*{Nombre} - Citar todos los nombres a la vez
    
## Notas a pie de página
    \footnote{Mensaje}

## Estilos de texto
    \textit{Texto} - Cursiva
    \emph{Texto} - Cursiva 2
    \textbf{Texto} - Negrita
    \texttt{Texto} - Monoespacio

## Acrónimos
Importante añadirlos al fichero *anexos/acronimos.tex*

    \gls{MD} - MarkDown

## Miscelanea

    \todo{Mensaje} - Tareas por hacer
    \missingfigure{Mensaje} - Imagen con el mensaje para introducir mas tarde