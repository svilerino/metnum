#!/bin/bash

red='\e[0;31m'
green='\e[0;32m'
blue='\e[0;34m'
purple='\e[0;35m'
NC='\e[0m' # No Color
echo ""

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

echo -e "${red}Corriendo tests de correctitud de la catedra usando ${1}...${NC}"
python metnum.py test "${method_number}"
