# --------------------------------------------------------------------
# Makefile Genérico :: Módulo 2. Curso Experto Desarrollo Videojuegos
# Carlos González Morcillo     Escuela Superior de Informática (UCLM)
# --------------------------------------------------------------------
EXEC := HundirLaFlota2

DIRSRC := src/
DIROBJ := obj/
DIRHEA := include/
#Añadido por mi para compilar en Fedora
DIRHEACEGUI1 := /usr/include/cegui-0/CEGUI
DIRHEACEGUI2 := /usr/include/cegui-0
LIBCEGUIBASE := CEGUIBase-0
LIBCEGUIOGRERENDERER := CEGUIOgreRenderer-0    #-lCEGUIBase-0 -lCEGUIOgreRenderer-0

CXX := g++

# Flags de compilación -----------------------------------------------
CXXFLAGS := -I$(DIRHEA) -I$(DIRHEACEGUI1) -I$(DIRHEACEGUI2) -Wall `pkg-config --cflags OIS OGRE` 

# Flags del linker ---------------------------------------------------
LDFLAGS := `pkg-config --libs-only-L OGRE`
LDLIBS := `pkg-config --libs-only-l gl OIS OGRE xerces-c` -lstdc++ -lboost_system -l$(LIBCEGUIBASE) -l$(LIBCEGUIOGRERENDERER)

# Modo de compilación (-mode=release -mode=debug) --------------------
ifeq ($(mode), release) 
	CXXFLAGS += -O2 -D_RELEASE
else 
	CXXFLAGS += -g -D_DEBUG
	mode := debug
endif

CXXFLAGS += -Wno-deprecated -Wno-deprecated-declarations

# Obtención automática de la lista de objetos a compilar -------------
OBJS := $(subst $(DIRSRC), $(DIROBJ), \
	$(patsubst %.cpp, %.o, $(wildcard $(DIRSRC)*.cpp)))

.PHONY: all clean

all: info dirs $(EXEC)

info:
	@echo '------------------------------------------------------'
	@echo '>>> Using mode $(mode)'
	@echo '    (Please, call "make" with [mode=debug|release])  '
	@echo '------------------------------------------------------'

dirs:
	mkdir -p $(DIROBJ)

# Enlazado -----------------------------------------------------------
$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compilación --------------------------------------------------------
$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza de temporales ---------------------------------------------
clean:
	rm -f *.log $(EXEC) *~ $(DIRSRC)*~ $(DIRHEA)*~ 
	rm -rf $(DIROBJ)
