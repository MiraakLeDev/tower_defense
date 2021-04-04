#
# CONFIGURATION GENERALE
#

EXEC =serveur client_udp client_tcp
OBJETS =cellule_tcp.o liste_tcp.o ncurses.o fenetre.o interface.o jeu.o liste_adj.o cellule_unite.o
NOM_PROJET = socketUDP

#
# SUFFIXES
#

.SUFFIXES: .c .o

#
# OBJETS
#

EXEC_O = $(EXEC:=.o)
OBJETS_O = $(OBJETS) $(EXEC_O)

#
# ARGUMENTS ET COMPILATEUR
#

CC = gcc
CCFLAGS = -Wall -O3 -ansi -pedantic -Werror -D _XOPEN_SOURCE
CCFLAGS_DEBUG = -D _DEBUG_
CCFLAGS = $(CCFLAGS_STD)
CCLIBS = -lncurses -lpthread

#
# REGLES
#

all: $(OBJETS) $(EXEC_O)
	@echo "Creation des executables..."
	@for i in $(EXEC); do \
	$(CC) -o $$i $$i.o $(OBJETS) $(CCLIBS); \
	done
	@echo "Termine."

#
# REGLES PAR DEFAUT
#

.c.o: .h
	@cd $(dir $<) && ${CC} ${CCFLAGS} -c $(notdir $<) -o $(notdir $@)

#
# REGLES GENERALES
#

clean:
	@echo "Suppresion des objets, des fichiers temporaires..."
	@rm -f $(OBJETS) $(EXEC_O)
	@rm -f *~ *#
	@rm -f $(EXEC)
	@rm -f dependances
	@echo "Termine."

depend:
	@echo "Creation des dependances..."
	@sed -e "/^# DEPENDANCES/,$$ d" makefile > dependances
	@echo "# DEPENDANCES" >> dependances
	@for i in $(OBJETS_O); do \
	$(CC) -MM -MT $$i $(CCFLAGS) `echo $$i | sed "s/\(.*\)\\.o$$/\1.c/"` >> dependances; \
	done
	@cat dependances > makefile
	@rm dependances
	@echo "Termine."

#
# CREATION ARCHIVE
#

ARCHIVE_FILES = *

archive: clean
	@echo "Creation de l'archive $(NOM_PROJET)$(shell date '+%y%m%d.tar.gz')..."
	@REP=`basename $$PWD`; cd .. && tar zcf $(NOM_PROJET)$(shell date '+%y%m%d.tar.gz') $(addprefix $$REP/,$(ARCHIVE_FILES))
	@echo "Termine."

# DEPENDANCES
cellule_tcp.o: cellule_tcp.c cellule_tcp.h
liste_tcp.o: liste_tcp.c liste_tcp.h cellule_tcp.h
ncurses.o: ncurses.c ncurses.h
fenetre.o: fenetre.c fenetre.h
interface.o: interface.c fenetre.h ncurses.h jeu.h interface.h
jeu.o: jeu.c jeu.h
liste_adj.o: liste_adj.c liste_adj.h cellule_unite.h jeu.h
cellule_unite.o: cellule_unite.c cellule_unite.h jeu.h
serveur.o: serveur.c include.h liste_tcp.h cellule_tcp.h jeu.h
client_udp.o: client_udp.c include.h
client_tcp.o: client_tcp.c jeu.h ncurses.h fenetre.h interface.h \
 liste_adj.h cellule_unite.h
