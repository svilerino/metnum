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
		echo -n "    * Graficando $file""_complexity_variation.png..."
		python plotter.py "$file"_complexity_variation.png "$file" 0 1
		echo -e "${green}Ok!${NC}"
	fi

	if [ $tipoGrafico -eq 1 ]
	then	
		echo -n "    * Graficando $file""_complexity_med_over_n.png..."
		python plotter.py "$file"_complexity_med_over_n.png plot_"$file".out 1
		echo -e "${green}Ok!${NC}"
	fi

	if [ $tipoGrafico -eq 2 ]
	then	
		echo -n "    * Graficando $file""_complexity_med_over_n_square.png..."
		python plotter.py "$file"_complexity_med_over_n_square.png plot_"$file".out 2
		echo -e "${green}Ok!${NC}"
	fi

	if [ $tipoGrafico -eq 3 ]
	then	
		echo -n "    * Graficando $file""_complexity_med_over_n_cube.png..."
		python plotter.py "$file"_complexity_med_over_n_cube.png plot_"$file".out 3
		echo -e "${green}Ok!${NC}"
	fi

	if [ $tipoGrafico -eq 4 ]
	then	
		echo -n "    * Graficando $file""_complexity_med_over_n_fourth.png..."
		python plotter.py "$file"_complexity_med_over_n_fourth.png plot_"$file".out 4
		echo -e "${green}Ok!${NC}"
	fi

	if [ $tipoGrafico -eq 10 ]
	then	
		echo -n "    * Graficando $file""_complexity_med_over_2_poweredto_n_.png..."
		python plotter_factorial.py "$file"_complexity_med_over_2_poweredto_n_.png plot_"$file".out 1
		echo -e "${green}Ok!${NC}"
	fi

	if [ $tipoGrafico -eq 20 ]
	then	
		echo -n "    * Graficando $file""_complexity_med_over_n_20.png..."
		python plotter_factorial.py "$file"_complexity_med_over_n_20.png plot_"$file".out 2
		echo -e "${green}Ok!${NC}"
	fi

	if [ $tipoGrafico -eq 30 ]
	then	
		echo -n "    * Graficando $file""_complexity_med_over_n_30.png..."
		python plotter_factorial.py "$file"_complexity_med_over_n_30.png plot_"$file".out 3
		echo -e "${green}Ok!${NC}"
	fi

	rm -f plot_"$file".out
else
	echo "[WARN] NO existe el archivo de datos para graficacion $file"
fi