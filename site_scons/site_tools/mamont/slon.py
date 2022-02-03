
import os
from SCons.Script import *


#------------------------------------------------
#
#   Builder
#
def slon(target, source, env):

    print(os.getcwd())

    return None

#------------------------------------------------
#
#   Pseudobuilder
#
def launch_slon(env, src):

    builddir = str(env.Dir('#'))
    
    trg = os.path.join(builddir, src + '.kot')
    
    print('target:', trg, '   |   source:',src)
      
    return env.Slon(trg, src, env)

#------------------------------------------------

