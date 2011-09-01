#
# Makefile for building RPM and Debian packages
#

include config

target = i386
rpmopts = ba
srcroot = $(abspath .)
rpmroot = $(srcroot)/rpm/
debroot = $(srcroot)/deb/
srcsdir=$(rpmroot)/SOURCES
specdir=$(rpmroot)/SPECS
srpmdir=$(rpmroot)/SRPMS
rpmsdir=$(rpmroot)/RPMS/$(target)
builddir=$(rpmroot)/BUILD
specsrc=$(srcroot)/$(PKG_NAME).spec.in
specfile=$(specdir)/$(PKG_NAME).spec
tarball = $(PKG_NAME)-$(PKG_VERSION).tar.bz2
release=$(PKG_RELEASE)
debdir=$(debroot)/$(PKG_NAME)-$(PKG_VERSION)

$(tarball): FORCE
	@echo "Create tarball from source"
	tar -cjf $(tarball) --exclude 'packages' --exclude '.hg*' \
	    --exclude '.svn' ..

rpm: $(tarball) $(specsrc)
	@echo "Creating local RPM build tree"
	mkdir -p $(srcsdir) $(specdir) $(srpmdir) $(rpmsdir) $(builddir)
	@echo "Copying spec and tarred sources into build tree"
	sed "s/\@VERSION\@/$(PKG_VERSION)/g" <$(specsrc) >$(specfile)
	cp $(tarball) $(srcsdir)
	@echo "Building"
	rpmbuild -$(rpmopts) --target $(target) --define \
	'_topdir $(srcroot)' --define '_release $(release)' \
	$(rpmbuildopts) $(specfile)

rpm-clean:
	rm -rf $(rpmroot)

deb: $(tarball)
	@echo "Creating local Debian/Ubuntu build tree"
	mkdir -p $(debdir)
	@echo "Installing source tarball ..."
	tar -xvj -C $(debdir) -f $(srcroot)/$(tarball)
	@echo "Copying debian build-rules ..."
	cp -r debian $(debdir)
	@echo "Building Debian/Ubuntu packages ..."
	(cd $(debdir) ; fakeroot dpkg-buildpackage -D)

deb-bin: $(tarball)
	@echo "Creating local Debian/Ubuntu build tree"
	mkdir -p $(debdir)
	@echo "Installing source tarball ..."
	tar -xvj -C $(debdir) -f $(srcroot)/$(tarball)
	@echo "Copying debian build-rules ..."
	cp -r debian $(debdir)
	@echo "Building Debian/Ubuntu packages (BINARIES ONLY) ..."
	(cd $(debdir) ; fakeroot dpkg-buildpackage -D -B)

deb-clean:
	rm -rf $(debroot)

.PHONY: FORCE
