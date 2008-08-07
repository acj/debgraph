-- Basic setup: load the shared library and the contents of "cache"
libdg = package.loadlib('libdebgraph.so', 'luaopen_libdebgraph')
libdg()
print(ShowVersion())
LoadPackages('cache')
