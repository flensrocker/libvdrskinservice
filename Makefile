# Makefile for libvdrskinservice

NAME = vdrskinservice
LIBNAME = lib$(NAME)
MAJOR = 0
MINOR = 0.1
VERSION = $(MAJOR).$(MINOR)
 
SONAME = $(LIBNAME).so.$(MAJOR)
TARGET_LIB = $(SONAME).$(MINOR)

PREFIX ?= /usr/local
INCDIR ?= $(PREFIX)/include
LIBDIR ?= $(PREFIX)/lib
PCDIR  ?= $(PREFIX)/lib/pkgconfig
TMPDIR ?= /tmp

### The name of the distribution archive:
ARCHIVE = $(LIBNAME)-$(VERSION)

CXXFLAGS = $(shell pkg-config --variable=cxxflags vdr)
LDFLAGS = -shared -Wl,-soname,$(SONAME)

DEFINES +=
INCLUDES +=
 
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)
 
.PHONY: all
all: ${TARGET_LIB} ${LIBNAME}.pc
 
%.o: %.c
	$(CXX) $(CXXFLAGS) -c $(DEFINES) $(INCLUDES) -o $@ $<

$(TARGET_LIB): $(OBJS)
	$(CXX) ${LDFLAGS} -o $@ $^
 
$(SRCS:.c=.d):%.d:%.c
	$(CXX) $(CXXFLAGS) -MM $< > $@

.PHONY: $(LIBNAME).pc
$(LIBNAME).pc:
	@echo "includedir=$(INCDIR)" > $@
	@echo "libdir=$(LIBDIR)" >> $@
	@echo "" >> $@
	@echo "Name: $(LIBNAME)" >> $@
	@echo "Description: helper lib for vdr-skin-plugins" >> $@
	@echo "Version: $(VERSION)" >> $@
	@echo "Cflags: -I$(INCDIR)" >> $@
	@echo "Libs: -L$(LIBDIR) -l$(NAME)" >> $@
 
-include $(SRCS:.c=.d)

install-lib: $(TARGET_LIB)
	install -D $^ $(DESTDIR)$(LIBDIR)/$^
	if [ -z "$(DESTDIR)" ] ; then ldconfig; fi
	cd $(DESTDIR)$(LIBDIR) ; if [ ! -e $(LIBNAME).so ] ; then ln -s $(TARGET_LIB) $(LIBNAME).so; fi

install-includes:
	@mkdir -p $(DESTDIR)$(INCDIR)/$(LIBNAME)
	@cp -pLR *.h $(DESTDIR)$(INCDIR)/$(LIBNAME)

install-pc: $(LIBNAME).pc
	if [ -n "$(PCDIR)" ] ; then\
	  mkdir -p $(DESTDIR)$(PCDIR) ;\
	  cp $(LIBNAME).pc $(DESTDIR)$(PCDIR) ;\
	fi

install: install-lib install-pc install-includes

.PHONY: clean
clean:
	-rm -f ${TARGET_LIB} ${OBJS} $(SRCS:.c=.d) $(LIBNAME).pc $(LIBNAME).so $(ARCHIVE).tgz

dist: clean
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@mkdir $(TMPDIR)/$(ARCHIVE)
	@cp -a * $(TMPDIR)/$(ARCHIVE)
	@tar czf $(ARCHIVE).tgz --exclude .git* --exclude *.o --exclude *.rej --exclude *.orig -C $(TMPDIR) $(ARCHIVE)
	@-rm -rf $(TMPDIR)/$(ARCHIVE)
	@echo Distribution package created as $(ARCHIVE).tgz
