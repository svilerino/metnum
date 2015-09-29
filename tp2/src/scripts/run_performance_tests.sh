#!/bin/bash
red='\e[0;31m'
green='\e[0;32m'
blue='\e[0;34m'
purple='\e[0;35m'
NC='\e[0m' # No Color
TESTFOLDER="tests"

echo ""
echo -e "${purple}Corriendo tests de performance...(time is un microseconds)${NC}"
echo -e "${red}Multiples tiempos por test indican tiempo por instancia de test${NC}"

if ls tests/*.in &> /dev/null; then
	rm -rf "../$TESTFOLDER/${1}.tmpplot"
	for file in tests/*.in; do
		
		file="${file%.*}" #extraigo el nombre sin la extension

		echo -e -n "Corriendo ${purple}${NC} archivo de input $file.in..."
		"../bin/tp2" "$file.in" "$file.out"
		process_exit_status=$?

		#verificar que el proceso haya terminado con return 0
		if [ $process_exit_status -eq 0 ] 
		then

			#extraigo la salida del archivo de timings
			timeconsumed=$(cat $file.timing | grep 'microsegundos power_method: ' | awk -F' ' '{print $5}' | tr '\n' ' ')
			probabilidad_c=$(cat $file.timing | grep 'Probabilidad de Teletransportación: ' | awk -F' ' '{print $4}' | tr '\n' ' ')

			echo -e "Teletransportación: ${probabilidad_c} ${green}[Returned EXIT_SUCCESS in ${timeconsumed}]${NC}"

			#python tools/plotter.py "${file}".convergence "${file}".convergence.png

		else
			echo -e "${red}[Fail]${NC}"
		fi
	done

#paste test1.convergence test2.convergence test3.convergence | column -s $'\t' -t

#	sort "$TESTFOLDER/${1}.tmpplot" -k1 --numeric-sort > "$TESTFOLDER/${1}.tmpplot.tmp"
#	mv "$TESTFOLDER/${1}.tmpplot.tmp" "$TESTFOLDER/${1}.tmpplot"
#
#	#llamo al plotter de python
#	./plot.sh "$TESTFOLDER/${1}.tmpplot" 0 -1
else
    echo "[WARN] NO existen archivos de testing"
fi