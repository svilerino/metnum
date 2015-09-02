#!/bin/bash
#iterar sobre la carpeta de resultados, 

red='\e[0;31m'
green='\e[0;32m'
blue='\e[0;34m'
purple='\e[0;35m'
NC='\e[0m' # No Color
echo ""
TIMING_OUTPUT="timings-out"

if [ "${1}" == "eliminacion_gaussiana" ]
then
	method_number=0
elif [ "${1}" == "factorizacion_lu" ]
then
	method_number=1
elif [ "${1}" == "eliminacion_gaussiana_pivoteo_parcial" ]
then
	method_number=2
fi


echo -e "${purple}Corriendo tests de performance usando ${1}...(time is un microseconds)${NC}"
echo -e "${red}Multiples tiempos por test indican tiempo por instancia de test${NC}"

if ls tests/*.in &> /dev/null; then
	pushd tests
	rm -rf "../$TIMING_OUTPUT/${1}.tmpplot"
	for file in *.in; do
		
		file="${file%.*}" #extraigo el nombre sin la extension

		echo -e -n "Corriendo ${purple}[${1}]${NC} con archivo de input $file.in..."
		"../../bin/tp1" "$file.in" "$file.out" "$method_number" "../$TIMING_OUTPUT/${1}_$file.timingout" "$file.isoout" "$file.seguridadisoout"
		process_exit_status=$?

		#verificar que el proceso haya terminado con return 0
		if [ $process_exit_status -eq 0 ] 
		then
			#extraigo la salida del archivo de timings
			timeconsumed=$(cat "../$TIMING_OUTPUT/${1}_$file.timingout" | awk -F' ' '{print $3}' | tr '\n' ' ')
			seguridad_isoterma_promedio=$(cat "$file.seguridadisoout" | awk -F' ' '{print $1}' | tr '\n' ' ')
			seguridad_isoterma_maximo=$(cat "$file.seguridadisoout" | awk -F' ' '{print $2}' | tr '\n' ' ')

			echo -e -n "${green}[Ok in ${timeconsumed}]${NC}"
			
			echo -e "${purple}[Ratios-isotermas: promedio: ${seguridad_isoterma_promedio} max: ${seguridad_isoterma_maximo}]${NC}"

			#agregar resultado a la lista de timings	

			cat "../$TIMING_OUTPUT/${1}_$file.timingout" >> "../$TIMING_OUTPUT/${1}.tmpplot"
			echo "" >> "../$TIMING_OUTPUT/${1}.tmpplot"

			# Plot de la solucion
			pushd ../tools
			# plotea el heatmap y la isoterma
			octave --eval "horno(\"../tests/$file.in\", \"../tests/$file.out\", \"../tests/\", \"../tests/$file.isoout\")"
			popd
		
			# Crear video del test 6 de la isoterma
			# ffmpeg -framerate 3/1 -start_number 1 -i "$file"_inst_%d_isomap.png -c:v libx264 -r 30 -pix_fmt yuv420p "$file"_video_isomap.mp4
			# Crear video del test 6 del calor
			# ffmpeg -framerate 3/1 -start_number 1 -i "$file"_inst_%d_heatmap.png -c:v libx264 -r 30 -pix_fmt yuv420p "$file"_video_heatmap.mp4
		else
			echo -e "${red}[Fail]${NC}"			
		fi
		

		#usamos el script de la catedra para validar resultados porque usa almostequal y no == en comparacion de salida.
	done


	popd
	#time to plot willies!
	#ordeno plot por la primer columna de numeros 

	#TENER EN CUENTA QUE EN ARCHIVOS MULTIINSTANCIA, SOBRETODO EN LU ORDENAR ESTO 
	#ES UN PROBLEMA PORQUE SE PIERDE EL ORDEN RELATIVO DE RESULTADOS DENTRO INSTANCIAS DE UN MISMO TEST

	sort "$TIMING_OUTPUT/${1}.tmpplot" -k1,1 --numeric-sort > "$TIMING_OUTPUT/${1}.tmpplot.tmp"
	mv "$TIMING_OUTPUT/${1}.tmpplot.tmp" "$TIMING_OUTPUT/${1}.tmpplot"

	#llamo al plotter de python
	./plot.sh "$TIMING_OUTPUT/${1}.tmpplot" 0 -1
	#./plot.sh "$TIMING_OUTPUT/${1}.tmpplot" 0 1
	./plot.sh "$TIMING_OUTPUT/${1}.tmpplot" 1
	./plot.sh "$TIMING_OUTPUT/${1}.tmpplot" 2
	./plot.sh "$TIMING_OUTPUT/${1}.tmpplot" 3
	./plot.sh "$TIMING_OUTPUT/${1}.tmpplot" 4
else
    echo "[WARN] NO existen archivos de testing"
fi