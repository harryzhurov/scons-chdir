
from SCons.Script import *
from site_scons.site_tools.mamont.slon    import *

#-------------------------------------------------------------------------------
#
#    Set up tool construction environment
#
def generate(env):

    Slon = Builder(action = slon)

    Builders = {'Slon' : Slon }

    env.Append(BUILDERS = Builders)
    
    #-----------------------------------------------------------------
    #
    #   IP core processing pseudo-builders
    #
    env.AddMethod(launch_slon, 'LaunchSlon')


#-------------------------------------------------------------------------------
def exists(env):
    print('tool: exists')
#-------------------------------------------------------------------------------

