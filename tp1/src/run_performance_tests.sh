#!/bin/bash
#iterar sobre la carpeta de resultados, 
#	los que no tengan stdout no => ignoro, o anoto que hubo error...
#	los que tengan salida piola por stdout => grafico y comparo sol vs exacta

red='\e[0;31m'
green='\e[0;32m'
blue='\e[0;34m'
purple='\e[0;35m'
NC='\e[0m' # No Color
echo ""
TESTS_INPUT="tests"
TESTS_OUTPUT="test-results"
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

echo -e "${purple}Corriendo tests de performance usando ${1}...${NC}"

if ls tests/*.in &> /dev/null; then
	pushd tests
	for file in *.in; do
		
		file="${file%.*}" #extraigo el nombre sin la extension

		echo -n "Corriendo [${1}] con archivo de input $file.in..."
		"../../bin/tp1" "$file.in" "../$TESTS_OUTPUT/${1}_$file.out" "$method_number" "../$TIMING_OUTPUT/${1}_$file.timingout"
		process_exit_status=$?
		
		#verificar que el proceso haya terminado con return 0
		if [ $process_exit_status -eq 0 ] 
		then
			echo -e "${green}[Ok]${NC}"
			
			# curve fit para heuristica
			# fitType=2
			# echo "$heuristica -> fitType=$fitType"
 
			# ./plot.sh "$heuristica".tmpplot 0 "$fitType"
			# ./plot.sh "$heuristica".tmpplot 1
			# ./plot.sh "$heuristica".tmpplot 2
			# ./plot.sh "$heuristica".tmpplot 3
			# ./plot.sh "$heuristica".tmpplot 4	
		else
			echo -e "${red}[Fail]${NC}"			
		fi

		#usar el script de la catedra para validar resultados porque usa almostequal y no == en comparacion de salida.
	done
	popd
else
    echo "[WARN] NO existen archivos de testing"
fi