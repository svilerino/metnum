import os
from utils import listfiles

# Sources listadas manualmente
# sources = ['main', 'utils', 'utils2/utils2']

# Sources listadas automaticamente
sources = [f.rstrip('.cpp') for f in listfiles('.', '*.cpp')]

# Compilador
compiler = 'g++'

# Programa compilado
executable = '../bin/tp1' if os.name == 'posix' else 'tp.exe'

# Parametros extra
extraParams = ['0']

