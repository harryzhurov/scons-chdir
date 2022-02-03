

import os
import sys

sys.dont_write_bytecode = True

#-------------------------------------------------------------------------------
#
#    Help info
#
help_info ="""
********************************************************************************     
    Available variants:
    ~~~~~~~~~~~~~~~~~~~
        sw (default)
     
    Usage:
    ~~~~~  
    scons [variant=<[path/]name>] [targets]
"""

Help(help_info)

#-------------------------------------------------------------------------------
#
#    General Settings
#

#-------------------------------------------------------------------------------
#
#    Variant management
#
variant = ARGUMENTS.get('variant', 'sw')

variant_name = variant.split(os.sep)[-1]

print('*'*80)
print(' '*27 + 'build variant: ' + variant_name)
print('*'*80 + '\n')

variant_path = os.path.join('src', 'cfg', variant, variant_name + '.scons')

if not os.path.exists(variant_path):
    print('\nError: unsupported variant: ' + variant)
    print(help_info)
    sys.exit(-3)

#-------------------------------------------------------------------------------
#
#    Environment
#
envx = Environment()

SConscript(variant_path, exports='envx')

#-------------------------------------------------------------------------------

if 'dump' in ARGUMENTS:
    env_key = ARGUMENTS[ 'dump' ]
    if env_key == 'env':
        print( envx.Dump() )
    else:
        print( envx.Dump(key = env_key) )

#-------------------------------------------------------------------------------

