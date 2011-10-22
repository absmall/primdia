# SCons build script
import os
import os.path
import string

############################
# Define utility functions #
############################

# This works around what I would consider a bug in scons, it puts double quotes around
# things Append'ed that contain a space, even if the space is wrapped in double quotes.
# Once that's fixed in scons, then we should use quotename instead
def fakequote(name):
	return '\\"' + name + '\\"'

def quotename(name):
	return "'" + '"' + name + '"' + "'"

def safename(name):
	return name.replace(' ', '-').lower()

def CheckPKGConfig(context, version):
	context.Message( 'Checking for pkg-config... ' )
	ret = context.TryAction('pkg-config --atleast-pkgconfig-version=%s' % version)[0]
	context.Message ('Ret = %d' % ret)
	context.Result( ret )
	return ret

def CheckPKG(context, name, version=0):
	context.Message( 'Checking for %s... ' % name )
	ret = context.TryAction('pkg-config --exists \'%s\'' % name)[0]
	if ret == 0: 
		context.Result( ret )
		return ret
	if version == 0:
		# Don't care about version
		context.Result( 1 )
		return 1
	else:
		ret = context.TryAction('pkg-config --atleast-version \'%s\' \'%s\'' % (version, name))[0]
		context.Result( ret )
		return ret

def po_helper(po,pot):
    args = [ 'msgmerge',
             '--update',
             po,
             pot,
             ]
    print 'Updating ' + po
    return os.spawnvp (os.P_WAIT, 'msgmerge', args)

def mo_builder(target,source,env):
    po_helper (source[0].get_path(), source[1].get_path())
    args = [ 'msgfmt',
             '-c',
             '-o',
             target[0].get_path(),
             source[0].get_path()
             ]
    return os.spawnvp (os.P_WAIT, 'msgfmt', args)

def pot_builder(target,source,env):
    args = [ 'xgettext', 
             '--keyword=_',
             '--keyword=N_',
             '--from-code=UTF-8',
             '-o', target[0].get_path(), 
             "--default-domain=" + packageName,
             '--copyright-holder="Aaron Small"' ]
    args += [ src.get_path() for src in source ]

    return os.spawnvp (os.P_WAIT, 'xgettext', args)
 
##################################
# Define configuration variables #
##################################

vars = Variables('custom.py')
vars.Add('name', 'Name of the program', 'Primitive Diagrammer')
vars.Add(EnumVariable('UI', 'User interface to use', 'gtk', allowed_values=('gtk', 'qt', 'console')))
vars.Add(PathVariable(
			'prefix',
			'Root to install to',
			'/usr/local',
			PathVariable.PathIsDir))
vars.Add('gettext', 'Enable native language translations', 1)

configEnv = Environment(variables = vars)

packageName = configEnv.get('name')
packageSafeName = safename(packageName)
localeDir = configEnv.get('prefix')+'/share/locale'
useGettext = configEnv.get('gettext')

########################################
# Check for the necessary dependencies #
########################################
 
if not GetOption('clean'):
	libraries = [ ('libxml++-2.6', 0) ]

	uilibs = {}
	uilibs['console'] = []
	uilibs['qt'] = []
	uilibs['gtk'] = [ ('gtk+-3.0', '3.0.0'),
	                  ('gconf-2.0', 0),
	                  ('pango', 0),
	                  ('pangocairo', 0) ]

	headers = []
	if( useGettext ):
		headers += [ 'libintl.h' ]

	uiheaders = {}
	uiheaders['console'] = ['readline/readline.h']
	uiheaders['qt'] = []
	uiheaders['gtk'] = []

	conf = Configure(configEnv, custom_tests = { 'CheckPKGConfig' : CheckPKGConfig, 'CheckPKG' : CheckPKG })
	 
	if not conf.CheckPKGConfig('0.15.0'):
		print 'pkg-config >= 0.15.0 not found.'
		Exit(1)

	for (lib,version) in libraries + uilibs[ configEnv.get('UI') ]:
		if not conf.CheckPKG(lib, version):
			if version:
				print lib, '>=', version, 'not found.'
			else:
				print lib, 'not found.'
			Exit(1)

	for header in headers + uiheaders[ configEnv.get('UI') ]:
		if not conf.CheckCHeader(header):
			Exit(1)

	configEnv = conf.Finish()

################################
# Set up environment variables #
################################

env = Environment(CPPPATH = 'inc', CCFLAGS='-g3', variables = vars)

Help(vars.GenerateHelpText(env))

env.Append(CPPDEFINES=[['PACKAGE', fakequote(packageName)]])
env.Append(CPPDEFINES=[['PACKAGE_NAME', quotename(packageSafeName)]])
env.Append(CPPDEFINES=[['LOCALEDIR', quotename(localeDir)]])
env.Append(CPPDEFINES=[['ENABLE_NLS', '${gettext}']])
env.ParseConfig('pkg-config --cflags --libs libxml++-2.6')
srcFiles = Glob('src/*.cc')
uiEnv = env.Clone()
if(env.get('UI') == 'gtk'):
	uiEnv.ParseConfig('pkg-config --cflags --libs gtk+-3.0 cairo gconf-2.0 pango pangocairo libxml++-2.6')
if(env.get('UI') == 'console'):
	uiEnv.Append(LIBS=['readline'])
uiEnv.Append(CPPPATH=['${UI}/inc'])
uiSrcFiles = Glob(env.get('UI')+'/src/*.cc')

###################
# Set up builders #
###################
 
# Main program
lib = env.SharedLibrary('primdia', srcFiles)
app = uiEnv.Program(safename(packageName), [lib, uiSrcFiles])
env.Install('$prefix/lib', lib)
env.Install('$prefix/bin', app)
Default(app)

# Tools
env.Install('$prefix/share/'+packageSafeName+'/tools', Glob('tools/*'))
env.Install('$prefix/share/'+packageSafeName+'/images', Glob('images/*'))
gconf_install = env.Command('gconf_install', 'schema', 'gconftool --install-schema-file $SOURCE')

# Schema
env.Alias('install', ['$prefix', gconf_install])

# Gettext stufff
mo_bld = Builder (action = mo_builder)
env.Append(BUILDERS = {'MoBuild' : mo_bld})

pot_bld = Builder (action = pot_builder)
env.Append(BUILDERS = {'PotBuild' : pot_bld})

if(env.get('gettext')):
	env.PotBuild ('po/' + packageSafeName + '.pot', srcFiles)
	p_oze = [ os.path.basename (str(po)) for po in Glob ('po/*.po') ]
	languages = [ po.replace ('.po', '') for po in p_oze ]
	m_oze = [ po.replace (".po", ".mo") for po in p_oze ]

	for mo in m_oze:
	    po = 'po/' + mo.replace (".mo", ".po")
	    mo = 'po/' + mo
	    env.MoBuild (mo, [ po, 'po/'+ packageSafeName + '.pot' ])

	for lang in languages:
	    env.Alias('install', env.InstallAs(os.path.join('$prefix', 
	'share/locale/' + lang + '/LC_MESSAGES/' + packageSafeName + '.mo'), 'po/' + lang 
	+ '.mo'))
