#this is for determinating what parts to build

defineTest(ModuleBuild) {

aix-g++:                 PLATFORM=aix-g++
aix-g++-64:              PLATFORM=aix-g++-64
aix-xlc:                 PLATFORM=aix-xlc
aix-xlc-64:              PLATFORM=aix-xlc-64
common:                  PLATFORM=common
darwin-g++:              PLATFORM=darwin-g++
default:                 PLATFORM=default
features:                PLATFORM=features
freebsd-g++:             PLATFORM=freebsd-g++
freebsd-g++34:           PLATFORM=freebsd-g++34
freebsd-g++40:           PLATFORM=freebsd-g++40
freebsd-icc:             PLATFORM=freebsd-icc
hpux-acc:                PLATFORM=freebsd-icc
hpux-acc-64:             PLATFORM=hpux-acc
hpux-acc-o64:            PLATFORM=hpux-acc-64
hpux-g++:                PLATFORM=hpux-acc-o64
hpux-g++-64:             PLATFORM=hpux-g++-64
hpuxi-acc:               PLATFORM=hpuxi-acc
hpuxi-acc-64:            PLATFORM=hpuxi-acc-64
hurd-g++:                PLATFORM=hurd-g++
irix-cc:                 PLATFORM=irix-cc
irix-cc-64:              PLATFORM=irix-cc-64
irix-g++:                PLATFORM=irix-g++
irix-g++-64:             PLATFORM=irix-g++-64
linux-cxx:               PLATFORM=linux-cxx
linux-ecc-64:            PLATFORM=linux-ecc-64
linux-g++:               PLATFORM=linux-g++
linux-g++-32:            PLATFORM=linux-g++-32
linux-g++-64:            PLATFORM=linux-g++-64
linux-icc:               PLATFORM=linux-icc
linux-kcc:               PLATFORM=linux-kcc
linux-lsb:               PLATFORM=linux-lsb
linux-pgcc:              PLATFORM=linux-pgcc
lynxos-g++:              PLATFORM=lynxos-g++
macx-g++:                PLATFORM=macx-g++
macx-icc:                PLATFORM=macx-icc
macx-pbuilder:           PLATFORM=macx-pbuilder
macx-xcode:              PLATFORM=macx-xcode
macx-xlc:                PLATFORM=macx-xlc
netbsd-g++:              PLATFORM=netbsd-g++
openbsd-g++:             PLATFORM=openbsd-g++
qconfig.pri:             PLATFORM=qconfig.pri
qws:             	 PLATFORM=qws
sco-cc:                  PLATFORM=sco-cc
sco-g++:                 PLATFORM=sco-g++
solaris-cc:              PLATFORM=solaris-cc
solaris-cc-64:           PLATFORM=solaris-cc-64
solaris-g++:             PLATFORM=solaris-g++
solaris-g++-64:          PLATFORM=solaris-g++-64
tru64-cxx:               PLATFORM=tru64-cxx
tru64-g++:               PLATFORM=tru64-g++
unixware-cc:             PLATFORM=unixware-cc
unixware-g++:            PLATFORM=unixware-g++
win32-g++:               PLATFORM=win32-g++



for( part,1 ){
	QXT_MODULE-=$$part
        contains(QXT_stability,stable):  contains($${part}.stability,+$$PLATFORM):$$MODULE+=$$part 
        contains(QXT_stability,broken):  contains($${part}.stability,-$$PLATFORM):$$MODULE+=$$part
        contains(QXT_stability,unknown):!contains($${part}.stability,-$$PLATFORM):!contains($${part}.stability,+$$PLATFORM):$$MODULE+=$$part
	}

$$MODULE += $$QXT_ignore_stability




HEADERS=
for(part, $$MODULE){
	header=
	for(header, $${part}.headers){
		HEADERS += $$header
		}
	}	 



SOURCES=
for(part, $$MODULE){
	source=
	for(source, $${part}.sources){
		SOURCES += $$source
		}
	}	 




#this is just for debug and overview.  not essential

partlist=
for(part, $$MODULE):  partlist+= $$part

message()
message(building $$MODULE : $$partlist)
message($$HEADERS)
message($$SOURCES)
message()






#QXT_LIBS

}