require "qxt-support"

solution "LibQxt"
    newoption {
        trigger = "with-bonjour",
        description = "Build QxtZeroconf with bonjour backend"
    }
    newoption {
        trigger = "with-avahi",
        description = "Build QxtZeroconf with avahi backend"
    }
    newoption {
        trigger = "with-bdb",
        description = "Build QxtBdb"
    }

    local projectbase = path.getabsolute(_PREMAKE_COMMAND:gsub("/[^/]*$","/../"))
    local buildbase   = path.getabsolute(_OPTIONS["to"]);

    configurations { "DebugShared", "ReleaseShared"}

    defines { "BUILD_QXT" }
    libdirs { buildbase .. "/lib"}
    if not os.is("windows") and not os.is("macosx") then
        linkoptions { "-Wl,-rpath," .. buildbase .. "/bin" }
    end

    project "QxtCore"
        targetdir (buildbase .. "/bin")
        language "C++"
        files { projectbase .. "/src/core/*.h", projectbase .. "/src/core/*.cpp" }

        if (os.get() == "windows") then
            files { projectbase .. "/src/core/windows/*.h"}
            files { projectbase .. "/src/core/windows/*.cpp"}
        elseif (os.get() == "macosx") then
            files { projectbase .. "/src/core/unix/*.h"}
            files { projectbase .. "/src/core/unix/*.cpp"}
        elseif (os.get() == "linux") then
            files { projectbase .. "/src/core/unix/*.h"}
            files { projectbase .. "/src/core/unix/*.cpp"}
        else
            error("unsupported target os" .. OS)
        end

        includedirs {
            projectbase .. "/src/core",
            projectbase .. "/include/QxtCore"
        }

        uses "QtCore"

        configuration "DebugShared"
            kind "SharedLib"
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "ReleaseShared"
            kind "SharedLib"
            defines { "NDEBUG" }

    project "QxtWidgets"
    targetdir (buildbase .. "/bin")
        language "C++"
        files { projectbase .. "/src/widgets/*.h", projectbase .. "/src/widgets/*.cpp" }

        if (os.get() == "windows") then
            files { projectbase .. "/src/widgets/windows/*.cpp"}
        elseif (os.get() == "macosx") then
            files { projectbase .. "/src/widgets/mac/*.cpp"}
        elseif (os.get() == "linux") then
            files { projectbase .. "/src/widgets/linux/*.cpp"}
        else
            error("unsupported target os" .. OS)
        end


        includedirs {
            projectbase .. "/src/core",
            projectbase .. "/include/QxtCore",
            projectbase .. "/src/widgets",
            projectbase .. "/include/QxtWidgets"
        }

        uses "QtCore"
        uses "QtGui"
        if (premake.qt.qtversionMajor >= 5) then
            uses "QtWidgets"
        end

        configuration "DebugShared"
            kind "SharedLib"
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "ReleaseShared"
            kind "SharedLib"
            defines { "NDEBUG" }
            flags { "Optimize" }
            flags { "Optimize" }


    project "QxtNetwork"
        targetdir (buildbase .. "/bin")
        language "C++"
        files {
            projectbase .. "/src/network/*.h",
            projectbase .. "/src/network/*.cpp",
            projectbase .. "/src/3rdparty/libssh2/libssh2/src/*.c"
        }

        includedirs {
                projectbase .. "/src/core",
                projectbase .. "/include/QxtCore",
                projectbase .. "/src/network",
                projectbase .. "/include/QxtNetwork",
                projectbase .. "/src/3rdparty/libssh2/libssh2/src/",
                projectbase .. "/src/3rdparty/libssh2/libssh2/"
        }
        if (os.get() == "windows") then
            includedirs { projectbase .. "/src/3rdparty/libssh2/libssh2/win32/"}
        else
            includedirs { projectbase .. "/src/3rdparty/libssh2/linux/"}
        end

        uses "QtCore"
        uses "QtNetwork"

        configuration "DebugShared"
            kind "SharedLib"
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "ReleaseShared"
            kind "SharedLib"
            defines { "NDEBUG" }
            flags { "Optimize" }
            flags { "Optimize" }

    project "QxtSql"
        targetdir (buildbase .. "/bin")
        language "C++"
        files { projectbase .. "/src/sql/*.h", projectbase .. "/src/sql/*.cpp" }

        includedirs {
                projectbase .. "/src/core",
                projectbase .. "/include/QxtCore",
                projectbase .. "/src/sql",
                projectbase .. "/include/QxtSql"
            }

        uses "QtCore"
        uses "QxtCore"
        uses "QtSql"

        configuration "DebugShared"
            kind "SharedLib"
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "ReleaseShared"
            kind "SharedLib"
            defines { "NDEBUG" }
            flags { "Optimize" }
            flags { "Optimize" }

    project "QxtWeb"
        targetdir (buildbase .. "/bin")
        language "C++"
        files { projectbase .. "/src/web/*.h", projectbase .. "/src/web/*.cpp" }

        includedirs {
                projectbase .. "/src/core",
                projectbase .. "/include/QxtCore",
                projectbase .. "/src/web",
                projectbase .. "/include/QxtWeb",
                projectbase .. "/src/network",
                projectbase .. "/include/QxtNetwork"
            }

        uses "QtCore"
        uses "QtNetwork"
        uses "QxtNetwork"

        configuration "DebugShared"
            kind "SharedLib"
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "ReleaseShared"
            kind "SharedLib"
            defines { "NDEBUG" }
            flags { "Optimize" }
            flags { "Optimize" }

    if (_OPTIONS["with-bonjour"] or _OPTIONS["with-avahi"]) then
    project "QxtZeroconf"

        targetdir (buildbase .. "/bin")
        language "C++"
        files { projectbase .. "/src/zeroconf/*.h", projectbase .. "/src/zeroconf/*.cpp" }

        includedirs {
                projectbase .. "/src/core",
                projectbase .. "/include/QxtCore",
                projectbase .. "/src/zeroconf",
                projectbase .. "/include/QxtZeroconf",
            }

        uses "QtCore"
        uses "QtNetwork"
        uses "QxtCore"


        if (_OPTIONS["with-bonjour"]) then
            files { projectbase .. "/src/zeroconf/bonjour/*.cpp"}
            includedirs { projectbase .. "/src/zeroconf/bonjour" }
        elseif (_OPTIONS["with-avahi"]) then
            files { projectbase .. "/src/zeroconf/avahi/*.cpp"}
            includedirs { projectbase .. "/src/zeroconf/avahi" }
            defines { "USE_AVAHI" }
        else

        end

        configuration "DebugShared"
            kind "SharedLib"
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "ReleaseShared"
            kind "SharedLib"
            defines { "NDEBUG" }
            flags { "Optimize" }
            flags { "Optimize" }
    end

    if (_OPTIONS["with-bdb"]) then
    project "QxtBdb"
        targetdir (buildbase .. "/bin")
        language "C++"
        files { projectbase .. "/src/bdb/*.h", projectbase .. "/src/bdb/*.cpp" }

        includedirs {
                projectbase .. "/src/core",
                projectbase .. "/include/QxtCore",
                projectbase .. "/src/bdb",
                projectbase .. "/include/Qxtbdb",
            }

        uses "QtCore"
        uses "QxtCore"

        configuration "DebugShared"
            kind "SharedLib"
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "ReleaseShared"
            kind "SharedLib"
            defines { "NDEBUG" }
            flags { "Optimize" }
            flags { "Optimize" }
    end

    project "qxtjsonrpc"
        targetdir (buildbase .. "/bin")
        language "C++"
        files { projectbase .. "/tools/jsonrpcclient/main.cpp" }

        -- -reduce-relocations is part of the qt5 default flags :(
        buildoptions { "-fPIE" }

        includedirs {
                projectbase .. "/src/core",
                projectbase .. "/include/QxtCore",
                projectbase .. "/src/network",
                projectbase .. "/include/QxtNetwork"
        }
        uses "QtCore"
        uses "QtNetwork"
        uses "QxtCore"
        uses "QxtNetwork"

        configuration "DebugShared"
            kind "ConsoleApp"
            defines { "DEBUG" }
            flags { "Symbols" }

        configuration "ReleaseShared"
            kind "ConsoleApp"
            defines { "NDEBUG" }
            flags { "Optimize" }
            flags { "Optimize" }






    function registerQxtModule(qxtpath)
            local gitQ = io.popen("git describe", "r");
            local version = assert(gitQ:read('*a')):gsub("\n",""):gsub("v","");
            gitQ:close();


            local qmakeQV = io.popen((_OPTIONS["qt-qmake"] or "qmake") .. " -query QMAKE_MKSPECS", 'r' );
            local mkspecs =  assert(qmakeQV:read('*a')):gsub("\n","");
            qmakeQV:close();

            local fd,e  = io.open(mkspecs .. "/modules/qxtvars.prf", "w");
            if (fd == nil) then error(e) end;
            fd:write("QXT_INSTALL_PREFIX = " .. qxtpath .. "\n");
            fd:write("QXT_INSTALL_LIBS = " .. qxtpath .. "/bin\n" );
            fd:write("QXT_INSTALL_BINS = " .. qxtpath .. "/bin\n" );
            fd:write("QXT_INSTALL_HEADERS = " .. qxtpath .. "/include\n" );
            fd:write("QXT_VERSION = " .. version .. "\n" );
            fd:close();

            os.copyfile(qxtpath .. "/features/qt_qxt.pri", mkspecs .. "/modules/");

    end



    newaction {
        trigger     = "inject",
        description = "Configure qmake to recognize this local build of libqxt",
        execute = function ()
            registerQxtModule(projectbase);
        end
    }

