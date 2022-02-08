
import os
from SCons.Script import *


#------------------------------------------------
#
#   Builder
#
def slon(target, source, env):

    print('>>> Project root directory:   ', os.path.abspath(str(env.Dir('#'))))
    print('>>> Current working directory:', os.getcwd())

    return None

#------------------------------------------------
#
#   Pseudobuilder
#
def launch_slon(env, src):

    builddir = os.path.join( os.path.abspath( str(env.Dir('#')) ), 'build' )
    src      = os.path.abspath( src )
    
    trg = os.path.join(builddir, os.path.basename(src) + '.kot')
    
    print('target:', trg, '   |   source:',src)
      
    return env.Slon(trg, src, env)

#------------------------------------------------

