#!/bin/bash
red='\e[0;31m'
green='\e[0;32m'
blue='\e[0;34m'
purple='\e[0;35m'
NC='\e[0m' # No Color

file=${1}
tipoGrafico=${2}
if [ -e $file ]; then
	echo "Procesando $file para tipo de grafico $tipoGrafico..."	
	if [ $tipoGrafico -eq 0 ]
	then	
		echo -n "    * Graficando $file""_time_consumed_med.png..."
		python plotter.py "$file"_time_consumed_med.png "$file" 0 "${3}"
		echo -e "${green}Ok!${NC}"
	fi

	if [ $tipoGrafico -eq 1 ]
	then	
		echo -n "    * Graficando $file""_time_consumed_med_over_n.png..."
		python plotter.py "$file"_time_consumed_med_over_n.png "$file" 1
		echo -e "${green}Ok!${NC}"
	fi

	if [ $tipoGrafico -eq 2 ]
	then	
		echo -n "    * Graficando $file""_time_consumed_med_over_n_square.png..."
		python plotter.py "$file"_time_consumed_med_over_n_square.png "$file" 2
		echo -e "${green}Ok!${NC}"
	fi

	if [ $tipoGrafico -eq 3 ]
	then	
		echo -n "    * Graficando $file""_time_consumed_med_over_n_cube.png..."
		python plotter.py "$file"_time_consumed_med_over_n_cube.png "$file" 3
		echo -e "${green}Ok!${NC}"
	fi

	if [ $tipoGrafico -eq 4 ]
	then	
		echo -n "    * Graficando $file""_time_consumed_med_over_n_fourth.png..."
		python plotter.py "$file"_time_consumed_med_over_n_fourth.png "$file" 4
		echo -e "${green}Ok!${NC}"
	fi

	# rm -f "$file"
else
	echo "[WARN] NO existe el archivo de datos para graficacion $file"
fi